<?php
/**
 * @file
 * A database-mediated implementation of a locking mechanism.
 *
 * Supports cross request persistance as well as GAP-LOCK mitigation.
 */

/**
 * Class for handling lock functions.
 *
 * This is a pseudo namespace really. Should probably be refactored...
 */
class UltimateCronLock {
  private static $locks = NULL;

  public static $killable = TRUE;

  /**
   * Shutdown handler for releasing locks.
   */
  static public function shutdown() {
    if (self::$locks) {
      foreach (array_keys(self::$locks) as $lock_id) {
        self::unlock($lock_id);
      }
    }
  }

  /**
   * Dont release lock on shutdown.
   *
   * @param string $lock_id
   *   The lock id to persist.
   */
  static public function persist($lock_id) {
    if (isset(self::$locks)) {
      unset(self::$locks[$lock_id]);
    }
  }

  /**
   * Acquire lock.
   *
   * @param string $name
   *   The name of the lock to acquire.
   * @param float $timeout
   *   The timeout in seconds for the lock.
   *
   * @return string
   *   The lock id acquired.
   */
  static public function lock($name, $timeout = 30.0) {
    // First, ensure cleanup.
    if (!isset(self::$locks)) {
      self::$locks = array();
      ultimate_cron_register_shutdown_function(array('UltimateCronLock', 'shutdown'));
    }

    $target = _ultimate_cron_get_transactional_safe_connection();

    try {
      // First we ensure that previous locks are "removed"
      // if they are expired.
      self::expire($name);

      // Ensure that the timeout is at least 1 ms.
      $timeout = max($timeout, 0.001);
      $expire = microtime(TRUE) + $timeout;

      // Now we try to acquire the lock.
      $lock_id = db_insert('ultimate_cron_lock', array('target' => $target))
        ->fields(array(
          'name' => $name,
          'current' => 0,
          'expire' => $expire,
        ))
        ->execute();

      self::$locks[$lock_id] = TRUE;

      return $lock_id;
    }
    catch (PDOException $e) {
      return FALSE;
    }
  }

  /**
   * Release lock if expired.
   *
   * Checks if expiration time has been reached, and releases the lock if so.
   *
   * @param string $name
   *   The name of the lock.
   */
  static public function expire($name) {
    if ($lock_id = self::isLocked($name, TRUE)) {
      $target = _ultimate_cron_get_transactional_safe_connection();
      $now = microtime(TRUE);
      db_update('ultimate_cron_lock', array('target' => $target))
        ->expression('current', 'lid')
        ->condition('lid', $lock_id)
        ->condition('expire', $now, '<=')
        ->execute();
    }
  }

  /**
   * Release lock.
   *
   * @param string $lock_id
   *   The lock id to release.
   */
  static public function unlock($lock_id) {
    $target = _ultimate_cron_get_transactional_safe_connection();
    $unlocked = db_update('ultimate_cron_lock', array('target' => $target))
      ->expression('current', 'lid')
      ->condition('lid', $lock_id)
      ->condition('current', 0)
      ->execute();
    self::persist($lock_id);
    return $unlocked;
  }

  /**
   * Relock.
   *
   * @param string $lock_id
   *   The lock id to relock.
   * @param float $timeout
   *   The timeout in seconds for the lock.
   *
   * @return bool
   *   TRUE if relock was successful.
   */
  static public function reLock($lock_id, $timeout = 30.0) {
    $target = _ultimate_cron_get_transactional_safe_connection();
    // Ensure that the timeout is at least 1 ms.
    $timeout = max($timeout, 0.001);
    $expire = microtime(TRUE) + $timeout;
    return (bool) db_update('ultimate_cron_lock', array('target' => $target))
      ->fields(array(
        'expire' => $expire,
      ))
      ->condition('lid', $lock_id)
      ->condition('current', 0)
      ->execute();
  }
  /**
   * Check if lock is taken.
   *
   * @param string $name
   *   Name of the lock.
   * @param bool $ignore_expiration
   *   Ignore expiration, just check if it's present.
   *   Used for retrieving the lock id of an expired lock.
   *
   * @return mixed
   *   The lock id if found, otherwise FALSE.
   */
  static public function isLocked($name, $ignore_expiration = FALSE) {
    $target = _ultimate_cron_get_transactional_safe_connection();
    $now = microtime(TRUE);
    $result = db_select('ultimate_cron_lock', 'l', array('target' => $target))
      ->fields('l', array('lid', 'expire'))
      ->condition('name', $name)
      ->condition('current', 0)
      ->execute()
      ->fetchObject();
    return $result && ($result->expire > $now || $ignore_expiration) ? $result->lid : FALSE;
  }

  /**
   * Check multiple locks.
   *
   * @param array $names
   *   The names of the locks to check.
   *
   * @return array
   *   Array of lock ids.
   */
  static public function isLockedMultiple($names) {
    $target = _ultimate_cron_get_transactional_safe_connection();
    $now = microtime(TRUE);
    $result = db_select('ultimate_cron_lock', 'l', array('target' => $target))
      ->fields('l', array('lid', 'name', 'expire'))
      ->condition('name', $names, 'IN')
      ->condition('current', 0)
      ->execute()
      ->fetchAllAssoc('name');
    foreach ($names as $name) {
      if (!isset($result[$name])) {
        $result[$name] = FALSE;
      }
      else {
        $result[$name] = $result[$name]->expire > $now ? $result[$name]->lid : FALSE;
      }
    }
    return $result;
  }

  /**
   * Cleanup expired locks.
   */
  static public function cleanup() {
    $target = _ultimate_cron_get_transactional_safe_connection();
    $class = _ultimate_cron_get_class('job');
    $now = microtime(TRUE);

    // Cleanup all expired locks.
    $count = 0;
    do {
      $lids = db_select('ultimate_cron_lock', 'l', array('target' => $target))
        ->fields('l', array('lid'))
        ->condition('current', 0)
        ->condition('expire', $now, '<=')
        ->range(0, 100)
        ->execute()
        ->fetchAll(PDO::FETCH_COLUMN);
      if ($lids) {
        $count += db_delete('ultimate_cron_lock', array('target' => $target))
          ->condition('lid', $lids, 'IN')
          ->execute();
      }
      if ($job = $class::$currentJob) {
        if ($job->getSignal('kill')) {
          watchdog('ultimate_cron', 'kill signal received', array(), WATCHDOG_NOTICE);
          return;
        }
      }
    } while ($lids);
    if ($count) {
      watchdog('ultimate_cron_lock', 'Released @count expired locks', array(
        '@count' => $count,
      ), WATCHDOG_NOTICE);
    }

    // Cleanup all released locks.
    $count = 0;
    do {
      $lids = db_select('ultimate_cron_lock', 'l', array('target' => $target))
        ->fields('l', array('lid'))
        ->where('l.current = l.lid')
        ->range(0, 100)
        ->execute()
        ->fetchAll(PDO::FETCH_COLUMN);
      if ($lids) {
        $count += db_delete('ultimate_cron_lock', array('target' => $target))
          ->condition('lid', $lids, 'IN')
          ->execute();
      }
      if ($job = $class::$currentJob) {
        if ($job->getSignal('kill')) {
          watchdog('ultimate_cron', 'kill signal received', array(), WATCHDOG_NOTICE);
          return;
        }
      }
    } while ($lids);
    watchdog('ultimate_cron_lock', 'Cleaned up @count released locks', array(
      '@count' => $count,
    ), WATCHDOG_DEBUG);
  }
}
