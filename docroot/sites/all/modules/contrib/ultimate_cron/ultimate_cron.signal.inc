<?php
/**
 * @file
 * File containing functions for Ultimate Cron signal handling.
 */

/**
 * Class for handling Ultimate Cron signals.
 */
class UltimateCronSignal {
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
    $target = _ultimate_cron_get_transactional_safe_connection();
    return db_select('ultimate_cron_signal', 's', array('target' => $target))
      ->fields('s', array('job_name'))
      ->condition('job_name', $name)
      ->condition('signal_name', $signal)
      ->condition('claimed', 0)
      ->execute()
      ->fetchField();
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
    $target = _ultimate_cron_get_transactional_safe_connection();
    $claimed = db_update('ultimate_cron_signal', array('target' => $target))
      ->fields(array('claimed' => 1))
      ->condition('job_name', $name)
      ->condition('signal_name', $signal)
      ->condition('claimed', 0)
      ->execute();
    if ($claimed) {
      db_delete('ultimate_cron_signal', array('target' => $target))
        ->condition('job_name', $name)
        ->condition('signal_name', $signal)
        ->condition('claimed', 1)
        ->execute();
    }
    return $claimed;
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
    $target = _ultimate_cron_get_transactional_safe_connection();
    return db_merge('ultimate_cron_signal', array('target' => $target))
      ->key(array(
        'job_name' => $name,
        'signal_name' => $signal,
      ))
      ->fields(array('claimed' => 0))
      ->execute();
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
    $target = _ultimate_cron_get_transactional_safe_connection();
    db_delete('ultimate_cron_signal', array('target' => $target))
      ->condition('job_name', $name)
      ->condition('signal_name', $signal)
      ->execute();
  }

  /**
   * Clear signals.
   *
   * @param string $name
   *   The name of the job.
   */
  static public function flush($name) {
    $target = _ultimate_cron_get_transactional_safe_connection();
    db_delete('ultimate_cron_signal', array('target' => $target))
      ->condition('job_name', $name)
      ->execute();
  }

}
