<?php
/**
 * @file
 * File containing functions for Ultimate Cron signal handling using cache.
 */

class UltimateCronSignalCache {
  /**
   * Get a signal without claiming it.
   *
   * @param string $name
   *   The name of the job.
   * @param string $signal
   *   The name of the signal.
   *
   * @return string
   *   The signal if any.
   */
  static public function peek($name, $signal) {
    $bin = variable_get('ultimate_cron_signal_cache_bin', 'signal');
    $cache = cache_get("signal-$name-$signal", $bin);
    if ($cache) {
      $flushed = cache_get("flushed-$name", $bin);
      if (!$flushed || $cache->created > $flushed->created) {
        return $cache->data;
      }
    }
    return FALSE;
  }

  /**
   * Get and claim signal.
   *
   * @param string $name
   *   The name of the job.
   * @param string $signal
   *   The name of the signal.
   *
   * @return string
   *   The signal if any. If a signal is found, it is "claimed" and therefore
   *   cannot be claimed again.
   */
  static public function get($name, $signal) {
    if (lock_acquire("signal-$name-$signal")) {
      $result = self::peek($name, $signal);
      self::clear($name, $signal);
      lock_release("signal-$name-$signal");
      return $result;
    }
    return FALSE;
  }

  /**
   * Set signal.
   *
   * @param string $name
   *   The name of the job.
   * @param string $signal
   *   The name of the signal.
   *
   * @return bool
   *   TRUE if the signal was set.
   */
  static public function set($name, $signal) {
    $bin = variable_get('ultimate_cron_signal_cache_bin', 'signal');
    cache_set("signal-$name-$signal", TRUE, $bin);
  }

  /**
   * Clear signal.
   *
   * @param string $name
   *   The name of the job.
   * @param string $signal
   *   The name of the signal.
   */
  static public function clear($name, $signal) {
    $bin = variable_get('ultimate_cron_signal_cache_bin', 'signal');
    cache_clear_all("signal-$name-$signal", $bin);
  }

  /**
   * Clear signals.
   *
   * @param string $name
   *   The name of the job.
   */
  static public function flush($name) {
    $bin = variable_get('ultimate_cron_signal_cache_bin', 'signal');
    cache_set("flushed-$name", microtime(TRUE), $bin);
  }
}
