<?php
/**
 * @file
 * Serial cron job launcher for Ultimate Cron.
 */

/**
 * Ultimate Cron launcher plugin class.
 */
class UltimateCronSerialLauncher extends UltimateCronLauncher {
  public $currentThread = NULL;
  public $currentThreadLockId = NULL;

  /**
   * Implements hook_cron_alter().
   */
  public function cron_alter(&$jobs) {
    $class = _ultimate_cron_get_class('lock');
    if (isset($jobs['ultimate_cron_plugin_launcher_serial_cleanup']) && !empty($class::$killable)) {
      $jobs['ultimate_cron_plugin_launcher_serial_cleanup']->hook['tags'][] = 'killable';
    }
  }

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array(
      'max_threads' => 1,
      'thread' => 'any',
      'lock_timeout' => 3600,
      'poorman_keepalive' => FALSE,
    ) + parent::defaultSettings();
  }

  /**
   * Settings form for the crontab scheduler.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];

    $elements['timeouts'] = array(
      '#type' => 'fieldset',
      '#title' => t('Timeouts'),
    );
    $elements['launcher'] = array(
      '#type' => 'fieldset',
      '#title' => t('Launching options'),
    );

    $elements['timeouts']['lock_timeout'] = array(
      '#parents' => array('settings', $this->type, $this->name, 'lock_timeout'),
      '#title' => t("Job lock timeout"),
      '#type' => 'textfield',
      '#default_value' => $values['lock_timeout'],
      '#description' => t('Number of seconds to keep lock on job.'),
      '#fallback' => TRUE,
      '#required' => TRUE,
    );

    if (!$job) {
      $max_threads = $values['max_threads'];
      $elements['launcher']['max_threads'] = array(
        '#parents' => array('settings', $this->type, $this->name, 'max_threads'),
        '#title' => t("Maximum number of launcher threads"),
        '#type' => 'textfield',
        '#default_value' => $max_threads,
        '#description' => t('The maximum number of launch threads that can be running at any given time.'),
        '#fallback' => TRUE,
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
        '#weight' => 1,
      );
      $elements['launcher']['poorman_keepalive'] = array(
        '#parents' => array(
          'settings',
          $this->type, $this->name,
          'poorman_keepalive',
        ),
        '#title' => t("Poormans cron keepalive"),
        '#type' => 'checkbox',
        '#default_value' => $values['poorman_keepalive'],
        '#description' => t('Retrigger poormans cron after it has finished. Requires $base_url to be accessible from the webserver.'),
        '#fallback' => TRUE,
        '#weight' => 3,
      );
    }
    else {
      $settings = $this->getDefaultSettings();
      $max_threads = $settings['max_threads'];
    }

    $options = array(
      'any' => '-- ' . t('Any') . ' --',
      'fixed' => '-- ' . t('Fixed') . ' --',
    );
    for ($i = 1; $i <= $max_threads; $i++) {
      $options[$i] = $i;
    }
    $elements['launcher']['thread'] = array(
      '#parents' => array('settings', $this->type, $this->name, 'thread'),
      '#title' => t("Run in thread"),
      '#type' => 'select',
      '#default_value' => $values['thread'],
      '#options' => $options,
      '#description' => t('Which thread to run jobs in.') . "<br/>" .
      t('<strong>Any</strong>: Just use any available thread') . "<br/>" .
      t('<strong>Fixed</strong>: Only run in one specific thread. The maximum number of threads is spread across the jobs.') . "<br/>" .
      t('<strong>1-?</strong>: Only run when a specific thread is invoked. This setting only has an effect when cron is run through cron.php with an argument ?thread=N or through Drush with --options=thread=N.'),
      '#fallback' => TRUE,
      '#required' => TRUE,
      '#weight' => 2,
    );
  }

  /**
   * Settings form validator.
   */
  public function settingsFormValidate(&$form, &$form_state, $job = NULL) {
    $elements = &$form['settings'][$this->type][$this->name];
    $values = &$form_state['values']['settings'][$this->type][$this->name];
    if (!$job) {
      if (intval($values['max_threads']) <= 0) {
        form_set_error("settings[$this->type][$this->name", t('%title must be greater than 0', array(
          '%title' => $elements['launcher']['max_threads']['#title'],
        )));
      }
    }
  }

  /**
   * Lock job.
   */
  public function lock($job) {
    $settings = $job->getSettings($this->type);
    $timeout = $settings['lock_timeout'];

    $class = _ultimate_cron_get_class('lock');
    if ($lock_id = $class::lock($job->name, $timeout)) {
      $lock_id = $this->name . '-' . $lock_id;
      return $lock_id;
    }
    return FALSE;
  }

  /**
   * Unlock job.
   */
  public function unlock($lock_id, $manual = FALSE) {
    list($launcher, $lock_id) = explode('-', $lock_id, 2);
    $class = _ultimate_cron_get_class('lock');
    return $class::unlock($lock_id);
  }

  /**
   * Check if job is locked.
   */
  public function isLocked($job) {
    $class = _ultimate_cron_get_class('lock');
    $lock_id = $class::isLocked($job->name);
    return $lock_id ? $this->name . '-' . $lock_id : $lock_id;
  }

  /**
   * Check lock for multiple jobs.
   */
  public function isLockedMultiple($jobs) {
    $names = array();
    foreach ($jobs as $job) {
      $names[] = $job->name;
    }
    $class = _ultimate_cron_get_class('lock');
    $lock_ids = $class::isLockedMultiple($names);
    foreach ($lock_ids as &$lock_id) {
      $lock_id = $lock_id ? $this->name . '-' . $lock_id : $lock_id;
    }
    return $lock_ids;
  }

  /**
   * Cleanup.
   */
  public function cleanup() {
    $class = _ultimate_cron_get_class('lock');
    $class::cleanup();
  }

  /**
   * Launcher.
   */
  public function launch($job) {
    $lock_id = $job->lock();

    if (!$lock_id) {
      return FALSE;
    }

    if ($this->currentThread) {
      $init_message = t('Launched in thread @current_thread', array(
        '@current_thread' => $this->currentThread,
      ));
    }
    else {
      $init_message = t('Launched manually');
    }
    $log_entry = $job->startLog($lock_id, $init_message);

    drupal_set_message(t('@name: @init_message', array(
      '@name' => $job->name,
      '@init_message' => $init_message,
    )));

    $class = _ultimate_cron_get_class('lock');
    try {
      // Allocate time for the job's lock if necessary.
      $settings = $job->getSettings($this->type);
      $lock_timeout = drupal_set_time_limit($settings['lock_timeout']);

      // Relock cron thread with proper timeout.
      if ($this->currentThreadLockId) {
        $class::reLock($this->currentThreadLockId, $settings['lock_timeout']);
      }

      // Run job.
      $job->run();
    }
    catch (Exception $e) {
      watchdog('serial_launcher', 'Error executing %job: @error', array('%job' => $job->name, '@error' => (string) $e), WATCHDOG_ERROR);
      $log_entry->finish();
      $job->unlock($lock_id);
      return FALSE;
    }

    $log_entry->finish();
    $job->unlock($lock_id);
    return TRUE;
  }

  /**
   * Find a free thread for running cron jobs.
   */
  public function findFreeThread($lock, $lock_timeout = NULL, $timeout = 3) {
    $settings = $this->getDefaultSettings();

    // Find a free thread, try for 3 seconds.
    $delay = $timeout * 1000000;
    $sleep = 25000;

    $class = _ultimate_cron_get_class('lock');
    do {
      for ($thread = 1; $thread <= $settings['max_threads']; $thread++) {
        if ($thread != $this->currentThread) {
          $lock_name = 'ultimate_cron_serial_launcher_' . $thread;
          if (!$class::isLocked($lock_name)) {
            if ($lock) {
              if ($lock_id = $class::lock($lock_name, $lock_timeout)) {
                return array($thread, $lock_id);
              }
            }
            else {
              return array($thread, FALSE);
            }
          }
        }
      }
      if ($delay > 0) {
        usleep($sleep);
        // After each sleep, increase the value of $sleep until it reaches
        // 500ms, to reduce the potential for a lock stampede.
        $delay = $delay - $sleep;
        $sleep = min(500000, $sleep + 25000, $delay);
      }
    } while ($delay > 0);
    return array(FALSE, FALSE);
  }

  /**
   * Launch manager.
   */
  public function launchJobs($jobs) {
    $class = _ultimate_cron_get_class('lock');
    $settings = $this->getDefaultSettings();

    // We only lock for 55 seconds at a time, to give room for other cron
    // runs.
    $lock_timeout = 55;

    if (!empty($_GET['thread'])) {
      self::setGlobalOption('thread', $_GET['thread']);
    }

    if ($thread = intval(self::getGlobalOption('thread'))) {
      if ($thread < 1 || $thread > $settings['max_threads']) {
        watchdog('serial_launcher', "Invalid thread available for starting launch thread", array(), WATCHDOG_ERROR);
        return;
      }

      $lock_name = 'ultimate_cron_serial_launcher_' . $thread;
      $lock_id = NULL;
      if (!$class::isLocked($lock_name)) {
        $lock_id = $class::lock($lock_name, $lock_timeout);
      }
      if (!$lock_id) {
        watchdog('serial_launcher', "Thread @thread is already running", array(
          '@thread' => $thread,
        ), WATCHDOG_WARNING);
      }
    }
    else {
      $timeout = 1;
      list($thread, $lock_id) = $this->findFreeThread(TRUE, $lock_timeout, $timeout);
    }

    if (!$thread) {
      watchdog('serial_launcher', "No free threads available for launching jobs", array(), WATCHDOG_WARNING);
      return;
    }

    watchdog('serial_launcher', "Cron thread %thread started", array('%thread' => $thread), WATCHDOG_DEBUG);

    $this->runThread($lock_id, $thread, $jobs);
    $class::unlock($lock_id);
  }

  /**
   * Run jobs in thread.
   *
   * @param string $lock_id
   *   The lock id.
   * @param string $thread
   *   The tread number.
   * @param array $jobs
   *   The UltimateCronJobs to run.
   */
  public function runThread($lock_id, $thread, $jobs) {
    $this->currentThread = $thread;
    $this->currentThreadLockId = $lock_id;

    $class = _ultimate_cron_get_class('lock');
    $lock_name = 'ultimate_cron_serial_launcher_' . $thread;
    foreach ($jobs as $job) {
      $settings = $job->getSettings($this->type);
      switch ($settings['thread']) {
        case 'any':
          $settings['thread'] = $thread;
          break;

        case 'fixed':
          $settings['thread'] = ($job->getUniqueID() % $settings['max_threads']) + 1;
          break;
      }
      if ((!self::getGlobalOption('thread') || $settings['thread'] == $thread) && $job->isScheduled()) {
        $job->launch();
        // Be friendly, and check if someone else has taken the lock.
        // If they have, bail out, since someone else is now handling
        // this thread.
        if ($current_lock_id = $class::isLocked($lock_name)) {
          if ($current_lock_id !== $lock_id) {
            return;
          }
        }
        else {
          // If lock is free, then take the lock again.
          $lock_id = $class::lock($lock_name);
          if (!$lock_id) {
            // Race-condition, someone beat us to it.
            return;
          }
        }
      }
    }
  }

  /**
   * Poormans cron launcher.
   */
  public function launchPoorman() {
    $class = _ultimate_cron_get_class('lock');
    $settings = $this->getDefaultSettings();
    // Is it time to run cron?
    $cron_last = variable_get('cron_last', 0);
    $cron_next = floor(($cron_last + 60) / 60) * 60;
    $time = time();
    if ($time < $cron_next) {
      if ($settings['poorman_keepalive'] && $lock_id = $class::lock('ultimate_cron_poorman_serial', 60)) {
        ultimate_cron_poorman_page_flush();
        $sleep = $cron_next - $time;
        sleep($sleep);
        ultimate_cron_poorman_trigger();
        $class::unLock($lock_id);
      }
      return;
    }

    unset($_GET['thread']);
    ultimate_cron_poorman_page_flush();
    ultimate_cron_run_launchers();

    // Check poorman settings. If launcher has changed, we don't want
    // to keepalive.
    $poorman = _ultimate_cron_plugin_load('settings', 'poorman');
    if (!$poorman) {
      return;
    }

    $settings = $poorman->getDefaultSettings();
    if (!$settings['launcher'] || $settings['launcher'] !== $this->name) {
      return;
    }

    $settings = $this->getDefaultSettings();
    if ($settings['poorman_keepalive'] && $lock_id = $class::lock('ultimate_cron_poorman_serial', 60)) {
      // Is it time to run cron? If not wait before re-launching.
      $cron_last = variable_get('cron_last', 0);
      $cron_next = floor(($cron_last + 60) / 60) * 60;
      $time = time();
      if ($time < $cron_next) {
        $sleep = $cron_next - $time;
        sleep($sleep);
      }

      $class::unLock($lock_id);
      ultimate_cron_poorman_trigger();
    }
  }
}
