<?php
/**
 * @file
 * Drush commands for Ultimate Cron!
 */

/**
 * Implements hook_drush_command().
 */
function ultimate_cron_drush_command() {
  $items = array();

  $items['cron-logs'] = array(
    'description' => "Show a cron jobs logs",
    'arguments' => array(
      'name' => 'Job to show logs for',
    ),
    'options' => array(
      'limit' => 'Number of log entries to show',
      'compact' => 'Only show the first line of each log entry',
    ),
    'examples' => array(
      'drush cron-logs node_cron --limit=20' => 'Show 20 last logs for the node_cron job',
    ),
  );

  $items['cron-list'] = array(
    'description' => "List cron jobs",
    'options' => array(
      'module' => 'Comma separated list of modules to show jobs from',
      'enabled' => 'Show enabled jobs',
      'disabled' => 'Show enabled jobs',
      'behind' => 'Show jobs that are behind schedule',
      'status' => 'Comma separated list of statuses to show jobs from',
      'extended' => 'Show extended information',
      'name' => 'Show name instead of title',
      'scheduled' => 'Show scheduled jobs',
    ),
    'examples' => array(
      'drush cron-list --status=running --module=node' => 'Show jobs from the node module that are currently running',
    ),
    'aliases' => array('cl'),
  );

  $items['cron-job-get'] = array(
    'description' => "Get cron job configuration",
    'arguments' => array(
      'name' => 'Job in question',
    ),
    'options' => array(
      'fallback' => 'Also show fallback settings',
    ),
    'examples' => array(
      'drush cron-job-get node_cron' => 'Get the node_cron job configuration',
    ),
    'aliases' => array('cjget'),
  );

  $items['cron-job-set'] = array(
    'description' => "Set cron job configuration",
    'arguments' => array(
      'name' => 'Job in question',
    ),
    'examples' => array(
      'drush cron-job-set node_cron {"settings":{"scheduler":{"name":"crontab"}}}' => 'Set the node_cron scheduler to "crontab"',
    ),
    'aliases' => array('cjset'),
  );

  $items['cron-run'] = array(
    'description' => "Run cron job",
    'arguments' => array(
      'name' => 'Job to run',
    ),
    'options' => array(
      'force' => 'Only effective when cron-run is run without any arguments. This options skip the schedule check for each job. Locks are still respected. This option is a synonom for --options=bypass_schedule',
      'check-schedule' => 'Checks the schedule when running a single job. The opposite of --force but for a single job only',
      'options' => 'Custom options for plugins, e.g. --options=thread=1 for serial launcher',
    ),
    'examples' => array(
      'drush cron-run node_cron' => 'Run the node_cron job',
      'drush cron-run --options=thread=1' => 'Run all scheduled jobs and instruct serial launcher only to launch thread 1 jobs',
    ),
    'aliases' => array('cr'),
  );

  $items['cron-enable'] = array(
    'description' => "Enable cron job",
    'arguments' => array(
      'name' => 'Job to enable',
    ),
    'options' => array(
      'all' => 'Enabled all jobs',
    ),
    'examples' => array(
      'drush cron-enable node_cron' => 'Enable the node_cron job',
    ),
    'aliases' => array('ce'),
  );

  $items['cron-disable'] = array(
    'description' => "Disable cron job",
    'arguments' => array(
      'name' => 'Job to disable',
    ),
    'options' => array(
      'all' => 'Enabled all jobs',
    ),
    'examples' => array(
      'drush cron-disable node_cron' => 'Disable the node_cron job',
    ),
    'aliases' => array('cd'),
  );

  $items['cron-unlock'] = array(
    'description' => "Unlock cron job",
    'arguments' => array(
      'name' => 'Job to unlock',
    ),
    'options' => array(
      'all' => 'Enabled all jobs',
    ),
    'examples' => array(
      'drush cron-unlock node_cron' => 'Unlock the node_cron job',
    ),
    'aliases' => array('cu'),
  );

  return $items;
}

/**
 * Implements hook_drush_command_alter().
 */
function ultimate_cron_drush_command_alter(&$command) {
  // Let's hijack the core cron command.
  if ($command['command'] == 'core-cron') {
    $command['command-hook'] = 'cron-run';
    $command['commandfile'] = 'ultimate_cron';
    $command['path'] = drupal_get_path('module', 'ultimate_cron');
    $command['callback'] = 'drush_ultimate_cron_cron_run';
    if (!variable_get('ultimate_cron_check_schedule_on_core_cron', FALSE)) {
      drush_set_option('force', TRUE);
    }
  }
}

/**
 * Implements hook_drush_help().
 */
function ultimate_cron_drush_help($section) {
  switch ($section) {
    case 'drush:cron-list':
      return dt("This command will list cron jobs");

    case 'drush:cron-run':
      return dt("This command will run a cron job");

    case 'drush:cron-enable':
      return dt("This command will enable a cron job");

    case 'drush:cron-disable':
      return dt("This command will disable a cron job");

    case 'drush:cron-unlock':
      return dt("This command will unlock a cron job");
  }
}

/**
 * List cron jobs.
 */
function drush_ultimate_cron_cron_list() {
  $module = drush_get_option('module');
  $enabled = drush_get_option('enabled');
  $disabled = drush_get_option('disabled');
  $behind = drush_get_option('behind');
  $extended = drush_get_option('extended');
  $statuses = drush_get_option('status');
  $scheduled = drush_get_option('scheduled');
  $showname = drush_get_option('name');

  $module = $module ? explode(",", $module) : array();
  $statuses = $statuses ? explode(",", $statuses) : array();

  $jobs = _ultimate_cron_job_load_all();

  $table = array();
  $table[] = array(
    '',
    dt('Module'),
    dt('Title'),
    dt('Scheduled'),
    dt('Started'),
    dt('Duration'),
    dt('Status'),
  );

  $class = _ultimate_cron_get_class('job');
  $lock_ids = $class::isLockedMultiple($jobs);
  $log_entries = $class::loadLatestLogEntries($jobs);
  $progresses = $class::getProgressMultiple($jobs);

  $print_legend = FALSE;

  foreach ($jobs as $name => $job) {
    if ($module && !in_array($job->hook['module'], $module)) {
      continue;
    }

    if ($enabled && !empty($job->disabled)) {
      continue;
    }

    if ($disabled && empty($job->disabled)) {
      continue;
    }

    if ($scheduled && !$job->isScheduled()) {
      continue;
    }

    $legend = '';

    if (!empty($job->disabled)) {
      $legend .= 'D';
      $print_legend = TRUE;
    }

    $job->lock_id = $lock_ids[$job->name];
    $job->log_entry = $log_entries[$job->name];
    $job->progress = $progresses[$job->name];

    if ($job->log_entry->lid && $job->lock_id && $job->log_entry->lid !== $job->lock_id) {
      $job->log_entry = $job->loadLogEntry($job->lock_id);
    }

    if ($time = $job->isBehindSchedule()) {
      $legend .= 'B';
      $print_legend = TRUE;
    }

    if ($behind && !$time) {
      continue;
    }

    if ($job->lock_id && $job->log_entry->lid == $job->lock_id) {
      $legend .= 'R';
      list($null, $status) = $job->getPlugin('launcher')->formatRunning($job);
      $print_legend = TRUE;
    }
    elseif ($job->log_entry->start_time && !$job->log_entry->end_time) {
      list($null, $status) = $job->getPlugin('launcher')->formatUnfinished($job);
    }
    else {
      list($null, $status) = $job->log_entry->formatSeverity();
    }
    if ($statuses && !in_array($status, $statuses)) {
      continue;
    }

    $progress = $job->lock_id ? $job->formatProgress() : '';

    $table[$name][] = $legend;
    $table[$name][] = $job->getModuleName();
    $table[$name][] = $showname ? $job->name : $job->title;
    $table[$name][] = $job->getPlugin('scheduler')->formatLabel($job);
    $table[$name][] = $job->log_entry->formatStartTime();
    $table[$name][] = $job->log_entry->formatDuration() . ' ' . $progress;
    $table[$name][] = $status;

    if ($extended) {
      $table['extended:' . $name][] = '';
      $table['extended:' . $name][] = '';
      $table['extended:' . $name][] = $job->name;
      $table['extended:' . $name][] = $job->getPlugin('scheduler')->formatLabelVerbose($job);
      $table['extended:' . $name][] = $job->log_entry->init_message;
      $table['extended:' . $name][] = $job->log_entry->message;
    }
  }
  drush_print_table($table);
  if ($print_legend) {
    drush_print("\n" . dt('Legend: D = Disabled, R = Running, B = Behind schedule'));
  }
}

/**
 * List cron jobs.
 */
function drush_ultimate_cron_cron_logs($name = NULL) {
  if (!$name) {
    return drush_set_error(dt('No job specified?'));
  }
  $job = _ultimate_cron_job_load($name);
  if (!$job) {
    return drush_set_error(dt('@name not found', array('@name' => $name)));
  }

  $compact = drush_get_option('compact');
  $limit = drush_get_option('limit');
  $limit = $limit ? $limit : 10;

  $table = array();
  $table[] = array(
    '',
    dt('Started'),
    dt('Duration'),
    dt('User'),
    dt('Initial message'),
    dt('Message'),
    dt('Status'),
  );

  $lock_id = $job->isLocked();
  $log_entries = $job->getLogEntries($limit);
  $progress = $job->getProgress();

  foreach ($log_entries as $log_entry) {
    $progress = '';
    if ($log_entry->lid && $lock_id && $log_entry->lid === $lock_id) {
      $progress = $job->getProgress();
      $progress = is_numeric($progress) ? sprintf(" (%d%%)", round($progress * 100)) : '';
    }

    $legend = '';
    if ($lock_id && $log_entry->lid == $lock_id) {
      $legend .= 'R';
      list($null, $status) = $job->getPlugin('launcher')->formatRunning($job);
    }
    elseif ($log_entry->start_time && !$log_entry->end_time) {
      list($null, $status) = $job->getPlugin('launcher')->formatUnfinished($job);
    }
    else {
      list($null, $status) = $log_entry->formatSeverity();
    }

    $table[$log_entry->lid][] = $legend;
    $table[$log_entry->lid][] = $log_entry->formatStartTime();
    $table[$log_entry->lid][] = $log_entry->formatDuration() . $progress;
    $table[$log_entry->lid][] = $log_entry->formatUser();
    if ($compact) {
      $table[$log_entry->lid][] = trim(reset(explode("\n", $log_entry->init_message)), "\n");
      $table[$log_entry->lid][] = trim(reset(explode("\n", $log_entry->message)), "\n");
    }
    else {
      $table[$log_entry->lid][] = trim($log_entry->init_message, "\n");
      $table[$log_entry->lid][] = trim($log_entry->message, "\n");
    }
    $table[$log_entry->lid][] = $status;
  }
  drush_print_table($table);
}

/**
 * Get a cron jobs configuration.
 */
function drush_ultimate_cron_cron_job_get($name = NULL) {
  if (!$name) {
    return drush_set_error(dt('No job specified?'));
  }
  $job = _ultimate_cron_job_load($name);
  require_once drupal_get_path('module', 'ultimate_cron') . '/includes/nicejson.php';
  $settings = drush_get_option('fallback') ? $job->getSettings() : $job->settings;
  $nicejson = json_format(drupal_json_encode($settings));
  drush_print($nicejson);
}

/**
 * Set a cron jobs configuration.
 */
function drush_ultimate_cron_cron_job_set($name = NULL, $settings = NULL) {
  if (!$name) {
    return drush_set_error(dt('No job specified?'));
  }
  if (!$settings) {
    return drush_set_error(dt('No settings specified?'));
  }
  $settings = drupal_json_decode($settings);
  if ($settings === FALSE) {
    return drush_set_error(dt('Invalid JSON'));
  }
  $job = _ultimate_cron_job_load($name);

  $plugin_types = ctools_plugin_get_plugin_type_info();
  foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
    $class = $info['defaults']['static']['class'];
    if (!$class::$multiple) {
      if (isset($settings[$plugin_type])) {
        $job->settings[$plugin_type] = $settings[$plugin_type] + $job->settings[$plugin_type];
      }
    }
    else {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if (isset($settings[$plugin_type][$plugin->name])) {
          $job->settings[$plugin_type][$plugin->name] = $settings[$plugin_type][$plugin->name] + $job->settings[$plugin_type][$plugin->name];
        }
      }
    }
  }

  require_once drupal_get_path('module', 'ultimate_cron') . '/includes/nicejson.php';
  $nicejson = json_format(drupal_json_encode($job->settings));
  drush_print($nicejson);
  if (drush_confirm(dt('Do you want to update the job with the above settings?'))) {
    ctools_include('export');
    ctools_export_crud_save('ultimate_cron_job', $job);
    drush_print(dt('Job has been updated'));
  }
}

/**
 * Run cron job(s).
 */
function drush_ultimate_cron_cron_run($name = NULL) {
  if ($options = drush_get_option('options')) {
    $pairs = explode(',', $options);
    foreach ($pairs as $pair) {
      list($key, $value) = explode('=', $pair);
      UltimateCronPlugin::setGlobalOption(trim($key), trim($value));
    }
  }
  if (!$name) {
    if (drush_get_option('force')) {
      UltimateCronPlugin::setGlobalOption('bypass_schedule', TRUE);
    }
    ultimate_cron_run_scheduled(FALSE);
  }
  else {
    $job = _ultimate_cron_job_load($name);
    if (!$job) {
      return drush_set_error(dt('@name not found', array('@name' => $name)));
    }
    if (drush_get_option('check-schedule')) {
      if (!$job->isScheduled()) {
        return;
      }
    }
    $job->launch();
  }
}

/**
 * Enable a cron job.
 */
function drush_ultimate_cron_cron_enable($name = NULL) {
  if (!$name) {
    if (!drush_get_option('all')) {
      return drush_set_error(dt('No job specified?'));
    }
    foreach (_ultimate_cron_job_load_all() as $job) {
      drush_ultimate_cron_cron_enable($job->name);
    }
    return;
  }
  $job = _ultimate_cron_job_load($name);
  if ($job->enable()) {
    drush_print(dt('@name enabled', array('@name' => $name)));
  }
}

/**
 * Disable a cron job.
 */
function drush_ultimate_cron_cron_disable($name = NULL) {
  if (!$name) {
    if (!drush_get_option('all')) {
      return drush_set_error(dt('No job specified?'));
    }
    foreach (_ultimate_cron_job_load_all() as $job) {
      drush_ultimate_cron_cron_disable($job->name);
    }
    return;
  }
  $job = _ultimate_cron_job_load($name);
  if ($job->disable()) {
    drush_print(dt('@name disabled', array('@name' => $name)));
  }
}

/**
 * Unlock a cron job.
 */
function drush_ultimate_cron_cron_unlock($name = NULL) {
  if (!$name) {
    if (!drush_get_option('all')) {
      return drush_set_error(dt('No job specified?'));
    }
    foreach (_ultimate_cron_job_load_all() as $job) {
      if ($job->isLocked()) {
        drush_ultimate_cron_cron_unlock($job->name);
      }
    }
    return;
  }
  $job = _ultimate_cron_job_load($name);
  if (!$job) {
    return drush_set_error(dt('@name not found', array('@name' => $name)));
  }

  $lock_id = $job->isLocked();
  if (!$lock_id) {
    return drush_set_error(dt('@name is not running', array('@name' => $name)));
  }

  // Unlock the process.
  if ($job->unlock($lock_id, TRUE)) {
    $log_entry = $job->resumeLog($lock_id);
    global $user;
    watchdog('ultimate_cron', '@name manually unlocked by user @username (@uid)', array(
      '@name' => $job->name,
      '@username' => $user->name,
      '@uid' => $user->uid,
    ), WATCHDOG_NOTICE);
    $log_entry->finish();

    drush_print(dt('Cron job @name unlocked', array('@name' => $name)));
  }
  else {
    drush_set_error(dt('Could not unlock cron job @name', array('@name' => $name)));
  }
}
