<?php
/**
 * @file
 * ws_cron_features.ultimate_cron.inc
 */

/**
 * Implements hook_default_ultimate_cron_job().
 */
function ws_cron_features_default_ultimate_cron_job() {
  $export = array();

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'advanced_forum_cron';
  $job->title = 'Default cron handler';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['advanced_forum_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'browscap_cron';
  $job->title = 'Default cron handler';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['browscap_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'ctools_cron';
  $job->title = 'Clean up old caches';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['ctools_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'dblog_cron';
  $job->title = 'Remove expired log messages and flood control events';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['dblog_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'search_cron';
  $job->title = 'Update indexes';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(
        'rules' => array(
          0 => '0+@ 0 * * 0',
        ),
      ),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['search_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'uc_cart_cron';
  $job->title = 'Default cron handler';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['uc_cart_cron'] = $job;

  $job = new stdClass();
  $job->disabled = FALSE; /* Edit this to true to make a default job disabled initially */
  $job->api_version = 3;
  $job->name = 'ultimate_cron_plugin_logger_database_cleanup';
  $job->title = 'Ultimate Cron Logger Database cleanup';
  $job->settings = array(
    'scheduler' => array(
      'name' => '',
      'simple' => array(
        'rules' => array(
          0 => '0+@ * * * *',
        ),
      ),
    ),
    'launcher' => array(
      'name' => '',
      'serial' => array(),
    ),
    'logger' => array(
      'name' => '',
      'database' => array(),
    ),
    'settings' => array(
      'poorman' => array(),
      'queue' => array(),
      'general' => array(),
    ),
  );
  $export['ultimate_cron_plugin_logger_database_cleanup'] = $job;

  return $export;
}
