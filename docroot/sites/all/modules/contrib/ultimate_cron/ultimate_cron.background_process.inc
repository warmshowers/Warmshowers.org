<?php
/**
 * @file
 * Hooks for legacy handling of Background Process 1.x.
 */

/**
 * Implements hook_background_process_shutdown().
 */
function ultimate_cron_background_process_shutdown($process = NULL, $msg = NULL) {
  // Some sanity checking.
  if (!$process || !is_object($process) || empty($process->callback)) {
    return;
  }
  // We only handle the legacy callback here.
  if (
    !is_array($process->callback) ||
    $process->callback[0] !== 'UltimateCronBackgroundProcessLegacyLauncher' ||
    $process->callback[1] !== 'job_callback'
  ) {
    return;
  }

  // Close the Ultimate Cron log entry.
  list ($name, $lock_id) = $process->args;

  // If someone has set the dont log signal, it means that they
  // are handling the shutdown of the log.
  $job = _ultimate_cron_job_load($name);
  if (!$job) {
    return;
  }

  if ($job->getSignal('background_process_legacy_dont_log')) {
    // Resend within this request, just in case.
    $job->sendSignal('background_process_legacy_dont_log');
    return;
  }

  $log_entry = $job->resumeLog($lock_id);

  // Rewrite message to conform with Ultimate Cron style, if this is
  // a manual unlock.
  global $user;
  $username = !empty($user->uid) ? $user->name : '';
  if ($msg == t('Manually unlocked by !name', array('!name' => $username))) {
    $username = !empty($user->uid) ? $user->name : t('anonymous');
    $msg = t('@name manually unlocked by user @username (@uid)', array(
      '@name' => $name,
      '@username' => $username,
      '@uid' => !empty($user->uid) ? $user->uid : 0,
    ));
  }

  $log_entry->log('bgpl_launcher', $msg, array(), WATCHDOG_NOTICE);
  $log_entry->finish();
}
