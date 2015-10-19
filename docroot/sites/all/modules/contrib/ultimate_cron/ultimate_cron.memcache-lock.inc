<?php
/**
 * @file
 * A memcached-mediated implementation of a locking mechanism.
 *
 * Supports cross request persistance.
 */

/**
 * Class for handling lock functions.
 *
 * This is a pseudo namespace really. Should probably be refactored...
 */
class UltimateCronLockMemcache {
  private static $locks = NULL;

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
      ultimate_cron_register_shutdown_function(array('UltimateCronLockMemcache', 'shutdown'));
    }

    // Ensure that the timeout is at least 1 sec. This is a limitation
    // imposed by memcached.
    $timeout = (int) max($timeout, 1);

    $bin = variable_get('ultimate_cron_lock_memcache_bin', 'semaphore');
    $lock_id = $name . ':' . uniqid('memcache-lock', TRUE);
    if (dmemcache_add($name, $lock_id, $timeout, $bin)) {
      self::$locks[$lock_id] = $lock_id;
      return $lock_id;
    }
    return FALSE;
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
    // Nothing to do here. Memcache handles this internally.
  }

  /**
   * Release lock.
   *
   * @param string $lock_id
   *   The lock id to release.
   */
  static public function unlock($lock_id) {
    if (!preg_match('/(.*):memcache-lock.*/', $lock_id, $matches)) {
      return FALSE;
    }
    $name = $matches[1];

    $result = FALSE;
    if (!($semaphore = self::lock("lock-$name"))) {
      return FALSE;
    }

    if (self::isLocked($name) == $lock_id) {
      $result = self::unlockRaw($name);
      unset(self::$locks[$lock_id]);
    }
    else {
      unset(self::$locks[$lock_id]);
    }
    self::unlockRaw("lock-$name", $semaphore);

    return $result;
  }

  /**
   * Real unlock.
   *
   * @param string $name
   *   Name of lock.
   *
   * @return bool
   *   Result of unlock.
   */
  static private function unlockRaw($name, $lock_id = NULL) {
    $bin = variable_get('ultimate_cron_lock_memcache_bin', 'semaphore');
    if ($lock_id) {
      unset(self::$locks[$lock_id]);
    }
    return dmemcache_delete($name, $bin);
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
    if (!preg_match('/(.*):memcache-lock.*/', $lock_id, $matches)) {
      return FALSE;
    }
    $name = $matches[1];

    if (!($semaphore = self::lock("lock-$name"))) {
      return FALSE;
    }

    $bin = variable_get('ultimate_cron_lock_memcache_bin', 'semaphore');
    $current_lock_id = self::isLocked($name);
    if ($current_lock_id === FALSE || $current_lock_id === $lock_id) {
      if (dmemcache_set($name, $lock_id, $timeout, $bin)) {
        self::unlockRaw("lock-$name", $semaphore);
        self::$locks[$lock_id] = TRUE;
        return self::$locks[$lock_id];
      }
    }
    self::unlockRaw("lock-$name", $semaphore);
    return FALSE;
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
    $bin = variable_get('ultimate_cron_lock_memcache_bin', 'semaphore');
    $result = dmemcache_get($name, $bin);
    return $result ? $result : FALSE;
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
    $bin = variable_get('ultimate_cron_lock_memcache_bin', 'semaphore');
    $locks = dmemcache_get_multi($names, $bin);
    foreach ($names as $name) {
      if (!isset($locks[$name])) {
        $locks[$name] = FALSE;
      }
    }
    return $locks;
  }

  /**
   * Cleanup expired locks.
   */
  static public function cleanup() {
    // Nothing to do here. Memcache handles this internally.
  }
}
