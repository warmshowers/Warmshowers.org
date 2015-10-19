<?php
/**
 * @file
 * Crontab cron job scheduler for Ultimate Cron.
 */

/**
 * Crontab scheduler.
 */
class UltimateCronCrontabScheduler extends UltimateCronScheduler {
  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'rules' => array('*/10+@ * * * *'),
      'catch_up' => '86400',
    );
  }

  /**
   * Label for schedule.
   */
  public function formatLabel($job) {
    $settings = $job->getSettings($this->type);
    return implode("\n", $settings['rules']);
  }

  /**
   * Label for schedule.
   */
  public function formatLabelVerbose($job) {
    $settings = $job->getSettings($this->type);
    $job->log_entry = isset($job->log_entry) ? $job->log_entry : $job->loadLatestLogEntry();

    $parsed = '';
    $next_schedule = NULL;
    $time = REQUEST_TIME;
    $skew = $this->getSkew($job);
    foreach ($settings['rules'] as $rule) {
      $cron = CronRule::factory($rule, $time, $skew);
      $parsed .= $cron->parseRule() . "\n";
      $result = $cron->getNextSchedule();
      $next_schedule = is_null($next_schedule) || $next_schedule > $result ? $result : $next_schedule;
      $result = $cron->getLastSchedule();

      // If job didn't run at its last schedule, check if the catch up time
      // will triger it, and adjust $next_schedule accordingly.
      if ($job->log_entry->start_time < $result && $time < $result + $settings['catch_up']) {
        $result = floor($time / 60) * 60 + 60;
        $next_schedule = $next_schedule > $result ? $result : $next_schedule;
      }
    }
    $parsed .= t('Next scheduled run at @datetime', array(
      '@datetime' => format_date($next_schedule, 'custom', 'Y-m-d H:i:s'),
    ));
    return $parsed;
  }

  /**
   * Settings form for the crontab scheduler.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    $rules = is_array($values['rules']) ? implode(';', $values['rules']) : '';

    $elements['rules'] = array(
      '#title' => t("Rules"),
      '#type' => 'textfield',
      '#default_value' => $rules,
      '#description' => t('Semi-colon separated list of crontab rules.'),
      '#fallback' => TRUE,
      '#required' => TRUE,
      '#element_validate' => array('ultimate_cron_plugin_crontab_element_validate_rule'),
    );
    $elements['rules_help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rules help'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $elements['rules_help']['info'] = array(
      '#markup' => file_get_contents(drupal_get_path('module', 'ultimate_cron') . '/help/rules.html'),
    );
    $elements['catch_up'] = array(
      '#title' => t("Catch up"),
      '#type' => 'textfield',
      '#default_value' => $values['catch_up'],
      '#description' => t("Don't run job after X seconds of rule."),
      '#fallback' => TRUE,
      '#required' => TRUE,
    );
  }

  /**
   * Submit handler.
   */
  public function settingsFormSubmit(&$form, &$form_state, $job = NULL) {
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    if (!empty($values['rules'])) {
      $rules = explode(';', $values['rules']);
      $values['rules'] = array_map('trim', $rules);
    }
  }

  /**
   * Schedule handler.
   */
  public function isScheduled($job) {
    $settings = $job->getSettings($this->type);
    $log_entry = isset($job->log_entry) ? $job->log_entry : $job->loadLatestLogEntry();
    $skew = $this->getSkew($job);
    $class = get_class($this);
    return $class::shouldRun($settings['rules'], $log_entry->start_time, NULL, $settings['catch_up'], $skew) !== FALSE ? TRUE : FALSE;
  }

  /**
   * Check crontab rules against times.
   */
  static public function shouldRun($rules, $job_last_ran, $time = NULL, $catch_up = 0, $skew = 0) {
    $time = is_null($time) ? time() : $time;
    foreach ($rules as $rule) {
      $cron = CronRule::factory($rule, $time, $skew);
      $cron_last_ran = $cron->getLastSchedule();

      if ($job_last_ran < $cron_last_ran && $cron_last_ran <= $time) {
        if ($time <= $cron_last_ran + $catch_up) {
          return $time - $job_last_ran;
        }
      }
    }
    return FALSE;
  }

  /**
   * Determine if job is behind schedule.
   */
  public function isBehind($job) {
    // Disabled jobs are not behind!
    if (!empty($job->disabled)) {
      return FALSE;
    }

    $log_entry = isset($job->log_entry) ? $job->log_entry : $job->loadLatestLogEntry();
    // If job hasn't run yet, then who are we to say it's behind its schedule?
    // Check the registered time, and use that if it's available.
    $job_last_ran = $log_entry->start_time;
    if (!$job_last_ran) {
      $registered = variable_get('ultimate_cron_hooks_registered', array());
      if (empty($registered[$job->name])) {
        return FALSE;
      }
      $job_last_ran = $registered[$job->name];
    }

    $settings = $job->getSettings($this->type);

    $skew = $this->getSkew($job);
    $next_schedule = NULL;
    foreach ($settings['rules'] as $rule) {
      $cron = CronRule::factory($rule, $job_last_ran, $skew);
      $time = $cron->getNextSchedule();
      $next_schedule = is_null($next_schedule) || $time < $next_schedule ? $time : $next_schedule;
    }
    $behind = REQUEST_TIME - $next_schedule;

    return $behind > $settings['catch_up'] ? $behind : FALSE;
  }

  /**
   * Get a "unique" skew for a job.
   */
  protected function getSkew($job) {
    return $job->getUniqueID() & 0xff;
  }
}
