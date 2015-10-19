<?php
/**
 * @file
 * Background Process 1.x launcher for Ultimate Cron.
 */

/**
 * Ultimate Cron launcher plugin class.
 */
class UltimateCronBackgroundProcessLegacyLauncher extends UltimateCronLauncher {
  public $scheduledLaunch = FALSE;
  public $weight = -10;

  /**
   * Implements hook_cron_alter().
   */
  public function cron_alter(&$jobs) {
    unset($jobs['background_process_cron']);
    if (isset($jobs['ultimate_cron_plugin_launcher_background_process_legacy_cleanup'])) {
      $job = &$jobs['ultimate_cron_plugin_launcher_background_process_legacy_cleanup'];
      $job->hook['scheduler'] = isset($job->hook['scheduler']) ? $job->hook['scheduler'] : array();
      $scheduler =& $job->hook['scheduler'];
      $scheduler['simple'] = isset($scheduler['simple']) ? $scheduler['simple'] : array();
      $scheduler['crontab'] = isset($scheduler['crontab']) ? $scheduler['crontab'] : array();
      $scheduler['simple'] += array(
        'rules' => array('* * * * *'),
      );
      $scheduler['crontab'] += array(
        'rules' => array('* * * * *'),
      );
    }
  }

  /**
   * Implements hook_cron_alter().
   */
  public function cron_pre_schedule($job) {
    if ($job->name !== 'ultimate_cron_plugin_launcher_background_process_legacy_cleanup') {
      return;
    }

    $job->hook['override_congestion_protection'] = TRUE;

    // Unlock background if too old.
    // @todo Move to some access handler or pre-execute?
    if ($lock_id = $job->isLocked()) {
      $process = background_process_get_process('uc-ultimate_cron_plugin_launcher_background_process_legacy_cleanup');
      if ($process && $process->start + variable_get('background_process_cleanup_age', BACKGROUND_PROCESS_CLEANUP_AGE) < time()) {
        $log_entry = $job->resumeLog($lock_id);
        $log_entry->log('bgpl_launcher', 'Self unlocking stale lock', array(), WATCHDOG_NOTICE);
        $log_entry->finish();
        $job->sendSignal('background_process_legacy_dont_log');
        $job->unlock($lock_id);
        unset($job->lock_id);
      }
    }
  }

  /**
   * Handle end_daemonize signal.
   */
  public function signal($item, $signal) {
    switch ($signal) {
      case 'end_daemonize':
        $item->sendSignal('end_daemonize', TRUE);
        $item->sendSignal('kill', TRUE);
        return;
    }
  }

  /**
   * Use ajax for run, since we're launching in the background.
   */
  public function build_operations_alter($job, &$allowed_operations) {
    if (!empty($allowed_operations['run'])) {
      $allowed_operations['run']['attributes'] = array('class' => array('use-ajax'));
    }
    else {
      $settings = $job->getSettings('launcher');
      if ($settings['daemonize'] && !$job->peekSignal('end_daemonize')) {
        unset($allowed_operations['kill']);
        $allowed_operations['end_daemonize'] = array(
          'title' => t('Kill daemon'),
          'href' => 'admin/config/system/cron/jobs/list/' . $job->name . '/signal/' . $this->type . '/' . $this->name . '/end_daemonize',
          'attributes' => array('class' => array('use-ajax')),
          'query' => array('token' => drupal_get_token('signal')),
        );
      }
    }
  }

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'recheck' => 0,
      'service_group' => variable_get('background_process_default_service_group', 'default'),
      'max_threads' => 2,
      'daemonize' => FALSE,
      'daemonize_interval' => 10,
      'daemonize_delay' => 1,
      'poorman_service_group' => variable_get('background_process_default_service_group', 'default'),
    ) + parent::defaultSettings();
  }

  /**
   * Only expose this plugin, if Background Process is 1.x.
   */
  public function isValid($job = NULL) {
    static $correct_version;
    if (!isset($correct_version)) {
      $correct_version = FALSE;
      // Interimistic way of determining version of Background Process.
      // Background Process 1.x has a dependency on the Progress module.
      if (module_exists('background_process')) {
        $info = system_get_info('module', 'background_process');
        if (!empty($info['dependencies']) && in_array('progress', $info['dependencies'])) {
          $correct_version = TRUE;
        }
      }
    }

    return $correct_version && parent::isValid($job);
  }

  /**
   * Label for settings.
   */
  public function settingsLabel($name, $value) {
    switch ($name) {
      case 'recheck':
        return $value ? t('Yes') : t('No');
    }
    return parent::settingsLabel($name, $value);
  }

  /**
   * Settings form for the crontab scheduler.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    $elements['recheck'] = array(
      '#title' => t("Re-check schedule"),
      '#type' => 'select',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => $values['recheck'],
      '#description' => t('If checked, the jobs schedule will be re-checked after launch in order to make sure, that the job is not run outside its launch window.'),
      '#fallback' => TRUE,
      '#required' => TRUE,
    );

    if (!$job) {
      $elements['max_threads'] = array(
        '#title' => t("Max threads"),
        '#type' => 'textfield',
        '#default_value' => $values['max_threads'],
        '#description' => t('Maximum number of concurrent cron jobs to run.'),
        '#fallback' => TRUE,
        '#required' => TRUE,
      );
    }

    $methods = module_invoke_all('service_group');
    $options = $this->getServiceGroups();
    foreach ($options as $key => &$value) {
      $value = (empty($value['description']) ? $key : $value['description']) . ' (' . implode(',', $value['hosts']) . ') : ' . $methods['methods'][$value['method']];
    }
    $elements['service_group'] = array(
      '#title' => t("Service group"),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $values['service_group'],
      '#description' => $job ? t('Service group to use for this job.') : t('Service group to use for jobs.'),
      '#fallback' => TRUE,
      '#required' => TRUE,
    );

    if (!$job) {
      $elements['poorman_service_group'] = array(
        '#title' => t("Poormans Cron service group"),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $values['poorman_service_group'],
        '#description' => t('Service group to use for the poormans cron launcher.'),
        '#fallback' => TRUE,
        '#required' => TRUE,
      );
    }

    $elements['daemonize'] = array(
      '#title' => t('Daemonize'),
      '#type' => 'checkbox',
      '#default_value' => $values['daemonize'],
      '#description' => t('Relaunch job immediately after it is finished.'),
    );
    $elements['daemonize_interval'] = array(
      '#title' => t('Interval'),
      '#type' => 'textfield',
      '#default_value' => $values['daemonize_interval'],
      '#description' => t('Seconds to run the job in the same thread before relaunching.'),
      '#states' => array(
        'visible' => array(':input[name="settings[' . $this->type . '][' . $this->name . '][daemonize]"]' => array('checked' => TRUE)),
      ),
    );
    $elements['daemonize_delay'] = array(
      '#title' => t('Delay'),
      '#type' => 'textfield',
      '#default_value' => $values['daemonize_delay'],
      '#description' => t('Delay in seconds between in job execution.'),
      '#states' => array(
        'visible' => array(':input[name="settings[' . $this->type . '][' . $this->name . '][daemonize]"]' => array('checked' => TRUE)),
      ),
    );
  }

  /**
   * Get service hosts defined in the system.
   */
  protected function getServiceGroups() {
    if (function_exists('background_process_get_service_groups')) {
      return background_process_get_service_groups();
    }

    // Fallback for setups that havent upgraded Background Process.
    // We have this to avoid upgrade dependencies or majer version bump.
    $service_groups = variable_get('background_process_service_groups', array());
    $service_groups += array(
      'default' => array(
        'hosts' => array(variable_get('background_process_default_service_host', 'default')),
      ),
    );
    foreach ($service_groups as &$service_group) {
      $service_group += array(
        'method' => 'background_process_service_group_random',
      );
    }
    return $service_groups;
  }

  /**
   * Lock job.
   *
   * Background Process doesn't internally provide a unique id
   * for the running process, so we'll have to add that ourselves.
   * We store the unique lock id in the second callback argument.
   */
  public function lock($job) {
    $handle = 'uc-' . $job->name;
    $process = new BackgroundProcess($handle);
    if (!$process->lock()) {
      return FALSE;
    }
    $lock_id = $job->name . ':' . uniqid('bgpl', TRUE);
    global $user;
    background_process_set_process($process->handle, '__LOCKED__', $user->uid, array($job->name, $lock_id), $process->token);
    return $lock_id;
  }

  /**
   * Unlock background process.
   */
  public function unlock($lock_id, $manual = FALSE) {
    if (!preg_match('/(.*):bgpl.*/', $lock_id, $matches)) {
      watchdog('bgpl_launcher', 'Invalid lock id @lock_id', array(
        '@lock_id' => $lock_id,
      ), WATCHDOG_ERROR);
      return FALSE;
    }
    $job_name = $matches[1];
    $handle = 'uc-' . $job_name;
    if ($manual) {
      $job = _ultimate_cron_job_load($job_name);
      $job->sendSignal('background_process_legacy_dont_log');
    }
    return background_process_unlock($handle);
  }

  /**
   * Check locked state.
   *
   * Because Background Process doesn't support a unique id per
   * process, we return the second callback argument from the process,
   * where we previously stored the unique lock id.
   */
  public function isLocked($job) {
    $process = background_process_get_process('uc-' . $job->name);
    if ($process) {
      return $process->args[1];
    }
    return FALSE;
  }

  /**
   * Check locked state for multiple jobs.
   *
   * This has yet to be optimized.
   */
  public function isLockedMultiple($jobs) {
    $handles = array();
    foreach ($jobs as $job) {
      $handles[] = 'uc-' . $job->name;
    }
    try {
      $old_db = db_set_active('background_process');
      $processes = db_select('background_process', 'bp')
        ->fields('bp', array('handle', 'args'))
        ->condition('handle', $handles, 'IN')
        ->execute()
        ->fetchAllAssoc('handle', PDO::FETCH_OBJ);
      db_set_active($old_db);
    }
    catch (Exception $e) {
      db_set_active($old_db);
      throw $e;
    }

    $lock_ids = array();
    foreach ($jobs as $job) {
      $lock_ids[$job->name] = FALSE;
      if (isset($processes['uc-' . $job->name])) {
        $process = $processes['uc-' . $job->name];
        $process->args = unserialize($process->args);
        $lock_ids[$job->name] = $process->args[1];
      }
    }

    return $lock_ids;
  }

  /**
   * Background process cleanup handler.
   *
   * Use drupal_set_message() to inform about the result of the cleanup.
   *
   * We trap these, and log them as watchdog messages.
   */
  public function cleanup() {
    $before = drupal_get_messages(NULL, FALSE);
    background_process_cron();
    $after = drupal_get_messages(NULL, FALSE);
    foreach ($after as $type => $a_messages) {
      $b_messages = isset($before[$type]) ? $before[$type] : array();
      $messages = array_diff($a_messages, $b_messages);
      foreach ($messages as $message) {
        switch ($type) {
          case 'status':
            $severity = WATCHDOG_INFO;
            break;

          case 'warning':
            $severity = WATCHDOG_WARNING;
            break;

          case 'error':
            $severity = WATCHDOG_ERROR;
            break;

          default:
            $severity = WATCHDOG_CRITICAL;
            break;
        }
        watchdog('bpgl_launcher', $message, array(), $severity);
      }
    }
  }

  /**
   * Background Process launch.
   */
  public function launch($job) {
    $lock_id = $job->lock();
    if (!$lock_id) {
      return FALSE;
    }

    $settings = $job->getSettings($this->type);

    $handle = 'uc-' . $job->name;
    $process = new BackgroundProcess($handle);
    $this->exec_status = $this->status = BACKGROUND_PROCESS_STATUS_LOCKED;

    // Always run cron job as anonymous user.
    $process->uid = 0;
    $process->service_group = $settings['service_group'];
    $service_host = $process->determineServiceHost();

    if ($this->scheduledLaunch) {
      $init_message = t('Launched at service host @name', array(
        '@name' => $service_host,
      ));
    }
    else {
      $init_message = t('Launched manually at service host @name', array(
        '@name' => $service_host,
      ));
    }

    // We know that the latest log entry has been used for determining the
    // schedule. Load the latest log entry and store the timestamp of it for
    // later use.
    if (!empty($job->recheck)) {
      $log_entry = isset($job->log_entry) ? $job->log_entry : $job->loadLatestLogEntry();
      $recheck = $log_entry->start_time;
    }
    else {
      $recheck = FALSE;
    }

    $log_entry = $job->startLog($lock_id, $init_message);

    if (!$process->execute(
      array(get_class($this), 'job_callback'),
      array($job->name, $lock_id, $recheck)
    )) {
      watchdog('bgpl_launcher', 'Could not execute background process dispatch for handle @handle', array(
        '@handle' => $handle,
      ), WATCHDOG_ERROR);
      $log_entry->finish();
      $this->unlock($lock_id);
      return FALSE;
    }

    // We want to finish the log in the sub-request.
    $log_entry->unCatchMessages();

    drupal_set_message(t('@name: @init_message', array(
      '@name' => $job->name,
      '@init_message' => $init_message,
    )));
    return TRUE;
  }

  /**
   * Launch manager.
   */
  public function launchJobs($jobs) {
    $this->scheduledLaunch = TRUE;
    $settings = $this->getDefaultSettings();

    // Don't use more than 55 seconds for launching jobs.
    // If we fail, we will try again next time.
    $timeout = 55;
    $expire = microtime(TRUE) + 55;

    while ($jobs && microtime(TRUE) < $expire) {
      $threads = $this->numberOfProcessesRunning();
      foreach ($jobs as $job) {
        if (!$job->isScheduled()) {
          unset($jobs[$job->name]);
          continue;
        }

        if (empty($job->hook['override_congestion_protection'])) {
          // Skip if we're congested.
          if ($threads >= $settings['max_threads']) {
            continue;
          }
        }

        // Everything's good. Launch job!
        $job_settings = $job->getSettings($this->type);
        $job->recheck = !self::getGlobalOption('bypass_schedule') && $job_settings['recheck'];
        $job->launch();
        unset($jobs[$job->name]);
        $threads = $this->numberOfProcessesRunning();
      }

      // If there are still jobs left to be launched, wait a little.
      if ($jobs) {
        sleep(1);
      }
    }

    // Bail out if we expired.
    if (microtime(TRUE) >= $expire) {
      watchdog('bgpl_launcher', 'Background Process launcher exceed time limit of @timeout seconds.', array(
        '@timeout' => $timeout,
      ), WATCHDOG_NOTICE);
    }

    if ($jobs) {
      watchdog('bgpl_launcher', '@jobs jobs missed their schedule due to congestion.', array(
        '@jobs' => count($jobs),
      ), WATCHDOG_NOTICE);
      foreach ($jobs as $name => $job) {
        if ($lock_id = $job->lock()) {
          $log_entry = $jobs[$name]->startLog($lock_id, 'congestion');
          $log_entry->log('bgpl_launcher', 'Missed schedule due to congestion', array(), WATCHDOG_NOTICE);
          $log_entry->finish();
          $job->sendSignal('background_process_legacy_dont_log');
          $job->unlock();
        }
      }
    }
  }

  /**
   * Poorman launcher.
   */
  public function launchPoorman() {
    $settings = $this->getDefaultSettings();
    $class = _ultimate_cron_get_class('lock');
    if ($lock_id = $class::lock('ultimate_cron_poorman_bgpl', 60)) {
      $process = new BackgroundProcess();
      $process->uid = 0;
      $process->service_group = $settings['poorman_service_group'];
      $process->start(array(get_class($this), 'poormanLauncher'), array($lock_id));
      $class::persist($lock_id);
    }
  }

  /**
   * Poorman launcher background process callback.
   *
   * @param string $lock_id
   *   The lock id used for this process.
   */
  static public function poormanLauncher($lock_id) {
    $class = _ultimate_cron_get_class('lock');
    // Bail out if someone stole our lock.
    if (!$class::reLock($lock_id, 60)) {
      return;
    }

    ultimate_cron_poorman_page_flush();

    // Wait until it's our turn (0 seconds at next minute).
    $cron_last = variable_get('cron_last', 0);
    $cron_next = floor(($cron_last + 60) / 60) * 60;
    $time = time();
    if ($time < $cron_next) {
      $sleep = $cron_next - $time;
      sleep($sleep);
    }

    // Get settings, so we can determine the poormans cron service group.
    $plugin = _ultimate_cron_plugin_load('launcher', 'background_process_legacy');
    if (!$plugin) {
      $class::unlock($lock_id);
      throw new Exception(t('Failed to load launcher plugin?!?!?!?!'));
    }
    $settings = $plugin->getDefaultSettings();

    // In case launchers fail, we don't want to relaunch this process
    // immediately.
    _ultimate_cron_variable_save('cron_last', time());

    // It's our turn!
    $launchers = array();
    foreach (_ultimate_cron_job_load_all() as $job) {
      $launcher = $job->getPlugin('launcher');
      $launchers[$launcher->name] = $launcher->name;
    }

    foreach ($launchers as $name) {
      $process = new BackgroundProcess('_ultimate_cron_poorman_' . $name);
      $process->uid = 0;
      $process->service_group = $settings['poorman_service_group'];
      $process->start('ultimate_cron_run_launchers', array(array($name)));
    }

    // Bail out if someone stole our lock.
    if (!$class::reLock($lock_id, 60)) {
      return;
    }

    // Wait until it's our turn (0 seconds at next minute).
    $cron_last = _ultimate_cron_variable_load('cron_last', 0);
    $cron_next = floor(($cron_last + 60) / 60) * 60;
    $time = time();
    if ($time < $cron_next) {
      $sleep = $cron_next - $time;
      sleep($sleep);
    }

    // Check poorman settings. If launcher has changed, we don't want
    // to keepalive.
    $poorman = _ultimate_cron_plugin_load('settings', 'poorman');
    if (!$poorman) {
      return;
    }
    $settings = $poorman->getDefaultSettings();
    if (!$settings['launcher'] || $settings['launcher'] !== 'background_process_legacy') {
      return;
    }

    background_process_keepalive();
  }

  /**
   * Background process version of initializeProgress().
   *
   * @param UltimateCronJob $job
   *   Job to initialize progress for.
   */
  public function initializeProgress($job) {
    // Progress is initialized by Background Process.
  }

  /**
   * Background process version of finishProgress().
   *
   * @param UltimateCronJob $job
   *   Job to finish progress for.
   */
  public function finishProgress($job) {
    // Progress is finished by Background Process.
  }

  /**
   * Implementation of getProgress().
   *
   * @param UltimateCronJob $job
   *   Job to get progress for.
   *
   * @return float
   *   Progress for the job.
   */
  public function getProgress($job) {
    $handle = 'uc-' . $job->name;
    $progress = progress_get_progress($handle);
    return $progress ? $progress->progress : FALSE;
  }

  /**
   * Implementation of getProgressMultiple().
   *
   * @param UltimateCronJob $jobs
   *   Jobs to get progresses for, keyed by job name.
   *
   * @return array
   *   Progresses, keyed by job name.
   */
  public function getProgressMultiple($jobs) {
    $names = array();
    foreach ($jobs as $job) {
      $names[] = 'uc-' . $job->name;
    }
    $result = db_select('progress', 'p')
      ->fields('p', array('name', 'progress'))
      ->condition('name', $names, 'IN')
      ->execute()
      ->fetchAllAssoc('name');
    $progresses = array();
    foreach ($jobs as $job) {
      $progresses[$job->name] = isset($result['uc-' . $job->name]) ? $result['uc-' . $job->name]->progress : FALSE;
    }
    return $progresses;
  }

  /**
   * Implementation of setProgress().
   *
   * @param UltimateCronJob $job
   *   Job to set progress for.
   * @param float $progress
   *   Progress (0-1).
   */
  public function setProgress($job, $progress) {
    $handle = 'uc-' . $job->name;
    return progress_set_progress($handle, '', $progress);
  }

  /**
   * Format running state.
   */
  public function formatRunning($job) {
    $settings = $job->getSettings('launcher');
    if (empty($settings['daemonize'])) {
      return parent::formatRunning($job);
    }
    $file = drupal_get_path('module', 'ultimate_cron') . '/icons/hourglass_go.png';
    $status = theme('image', array('path' => $file));
    $title = t('daemonized');
    return array($status, $title);
  }

  /**
   * Get the number of cron background processes currently running.
   */
  public function numberOfProcessesRunning() {
    $query = db_select('background_process', 'bp')
      ->condition('bp.handle', db_like('uc-') . '%', 'LIKE');
    $query->addExpression('COUNT(1)', 'processes');
    $result = $query
      ->execute()
      ->fetchField();
    return $result ? $result : 0;
  }

  /**
   * Background Process legacy callback for running cron jobs.
   *
   * @param string $name
   *   The name of the job.
   * @param string $lock_id
   *   The lock id.
   */
  static public function job_callback($name, $lock_id, $recheck = FALSE) {
    $job = _ultimate_cron_job_load($name);

    $log_entry = $job->resumeLog($lock_id);

    // If set, $recheck contains the timestamp of the last schedule check.
    if ($recheck) {
      // Simulate schedule check by setting a mock log entry object with the
      // recheck timestamp.
      $job->log_entry = $job->getPlugin('logger')->factoryLogEntry($job->name);
      $job->log_entry->start_time = $recheck;

      // Now we can check the scheduler.
      if (!$job->getPlugin('scheduler')->isScheduled($job)) {
        watchdog('bgpl_launcher', 'Recheck failed at @time', array(
          '@time' => format_date(time(), 'custom', 'Y-m-d H:i:s'),
        ), WATCHDOG_ERROR);
        $job->sendSignal('background_process_legacy_dont_log');
        $log_entry->finish();
        $job->unlock($lock_id);
        return;
      }
      unset($job->log_entry);
    }

    // Run job.
    try {
      if ($job->getPlugin('launcher')->name != 'background_process_legacy') {
        // Launcher has changed, end job/daemon.
        $log_entry->finish();
        $job->unlock($lock_id);
        return;
      }

      $settings = $job->getSettings('launcher');
      if ($settings['daemonize']) {
        $keepalive = TRUE;
        $expire = microtime(TRUE) + (float) $settings['daemonize_interval'];
        do {
          $job->run();
          if ($settings['daemonize_delay']) {
            usleep(((float) $settings['daemonize_delay']) * 1000000);
          }

          if ($job->getSignal('end_daemonize')) {
            watchdog('bgpl_launcher', 'end daemonize signal received', array(), WATCHDOG_NOTICE);
            $keepalive = FALSE;
            break;
          }
        } while (microtime(TRUE) < $expire);

        // Refresh disabled value.
        $job = _ultimate_cron_job_load($name, TRUE);
        $settings = $job->getSettings('launcher');

        $keepalive &= empty($job->disabled);
        $keepalive &= !empty($settings['daemonize']);
        $keepalive &= !$job->getSignal('end_daemonize');

        if ($keepalive) {
          // Make sure recheck isn't kept alive, as this does not make
          // any sense.
          background_process_keepalive($name, $lock_id);

          // Save a copy of the log.
          $log_entry->lid = $lock_id . '-' . uniqid('', TRUE);
          $job->sendSignal('background_process_legacy_dont_log');
          $log_entry->finish();

          // Restart log for keepalive.
          $log_entry->lid = $lock_id;
          $handle = background_process_current_handle();
          $process = background_process_get_process($handle);
          $log_entry->init_message = t('Re-launched at service host @name', array(
            '@name' => $process->service_host,
          ));

          $log_entry->message = '';
          $log_entry->end_time = 0;
          $log_entry->start_time = microtime(TRUE);
          $log_entry->save();
        }
        else {
          $job->sendSignal('background_process_legacy_dont_log');
          $log_entry->finish();
          $job->unlock($lock_id);
        }
      }
      else {
        $job->run();
        $job->sendSignal('background_process_legacy_dont_log');
        $log_entry->finish();
        $job->unlock($lock_id);
      }

    }
    catch (Exception $e) {
      watchdog('bgpl_launcher', 'Error executing %job: @error', array('%job' => $job->name, '@error' => (string) $e), WATCHDOG_ERROR);
      $job->sendSignal('background_process_legacy_dont_log');
      $log_entry->finish();
      $job->unlock($lock_id);
    }
  }

}
