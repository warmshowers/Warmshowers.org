<?php
/**
 * @file
 * Pseudo namespace for progress functions.
 */

/**
 * Class for using memcache as storage for progress.
 */
class UltimateCronProgressMemcache {
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
      self::$instances[$name] = new UltimateCronProgressMemcache($name, $interval);
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
    $bin = variable_get('ultimate_cron_progress_memcache_bin', 'progress');
    return dmemcache_get($name, $bin);
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
    $bin = variable_get('ultimate_cron_progress_memcache_bin', 'progress');
    $values = dmemcache_get_multi($keys, $bin);

    $result = array();
    foreach ($names as $name) {
      $result[$name] = isset($values['uc-progress:' . $name]) ? $values['uc-progress:' . $name] : FALSE;
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
      $bin = variable_get('ultimate_cron_progress_memcache_bin', 'progress');
      dmemcache_set($name, $progress, 0, $bin);
      $this->progressUpdated = microtime(TRUE);
      return TRUE;
    }
    return FALSE;
  }

}
