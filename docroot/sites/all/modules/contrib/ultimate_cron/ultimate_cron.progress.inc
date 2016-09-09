<?php
/**
 * @file
 * Pseudo namespace for progress functions.
 */

/**
 * Class for handling Ultimate Cron progress.
 */
class UltimateCronProgress {
  public $name;
  public $progressUpdated = 0;
  public $interval = 1;
  static public $instances = array();

  /**
   * Constructor.
   *
   * @param string $name
   *   Name of job.
   * @param float $interval
   *   How often the database should be updated with the progress.
   */
  public function __construct($name, $interval = 1) {
    $this->name = $name;
    $this->interval = $interval;
  }

  /**
   * Singleton factory.
   *
   * @param string $name
   *   Name of job.
   * @param float $interval
   *   How often the database should be updated with the progress.
   *
   * @return UltimateCronProgress
   *   The object.
   */
  static public function factory($name, $interval = 1) {
    if (!isset(self::$instances[$name])) {
      self::$instances[$name] = new UltimateCronProgress($name, $interval);
    }
    self::$instances[$name]->interval = $interval;
    return self::$instances[$name];
  }

  /**
   * Get job progress.
   *
   * @return float
   *   The progress of this job.
   */
  public function getProgress() {
    $name = 'uc-progress:' . $this->name;
    $target = _ultimate_cron_get_transactional_safe_connection();
    $value = db_select('variable', 'v', array('target' => $target))
      ->fields('v', array('value'))
      ->condition('v.name', $name)
      ->execute()
      ->fetchField();
    $value = $value ? unserialize($value) : FALSE;
    return $value;
  }

  /**
   * Get multiple job progresses.
   *
   * @param array $names
   *   Job names to get progress for.
   *
   * @return array
   *   Progress of jobs, keyed by job name.
   */
  static public function getProgressMultiple($names) {
    $keys = array();
    foreach ($names as $name) {
      $keys[] = 'uc-progress:' . $name;
    }
    $target = _ultimate_cron_get_transactional_safe_connection();
    $values = db_select('variable', 'v', array('target' => $target))
      ->fields('v', array('name', 'value'))
      ->condition('v.name', $keys, 'IN')
      ->execute()
      ->fetchAllAssoc('name', PDO::FETCH_OBJ);

    $result = array();
    foreach ($names as $name) {
      $result[$name] = isset($values['uc-progress:' . $name]) ? unserialize($values['uc-progress:' . $name]->value) : FALSE;
    }
    return $result;
  }

  /**
   * Set job progress.
   *
   * @param float $progress
   *   The progress (0 - 1).
   */
  public function setProgress($progress) {
    if (microtime(TRUE) >= $this->progressUpdated + $this->interval) {
      $name = 'uc-progress:' . $this->name;
      $target = _ultimate_cron_get_transactional_safe_connection();
      db_merge('variable', array('target' => $target))
        ->key(array('name' => $name))
        ->fields(array('value' => serialize($progress)))
        ->execute();
      $this->progressUpdated = microtime(TRUE);
      return TRUE;
    }
    return FALSE;
  }

}
