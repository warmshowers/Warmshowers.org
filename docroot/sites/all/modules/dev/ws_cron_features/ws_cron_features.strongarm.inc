<?php
/**
 * @file
 * ws_cron_features.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ws_cron_features_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'cron_safe_threshold';
  $strongarm->value = 0;
  $export['cron_safe_threshold'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uc-progress:ultimate_cron_plugin_launcher_serial_cleanup';
  $strongarm->value = FALSE;
  $export['uc-progress:ultimate_cron_plugin_launcher_serial_cleanup'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uc-progress:ultimate_cron_plugin_logger_database_cleanup';
  $strongarm->value = FALSE;
  $export['uc-progress:ultimate_cron_plugin_logger_database_cleanup'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uc-progress:ultimate_cron_plugin_settings_queue_cleanup';
  $strongarm->value = FALSE;
  $export['uc-progress:ultimate_cron_plugin_settings_queue_cleanup'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ultimate_cron_plugin_scheduler_default';
  $strongarm->value = 'simple';
  $export['ultimate_cron_plugin_scheduler_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ultimate_cron_plugin_scheduler_simple_settings';
  $strongarm->value = array(
    'rules' => array(
      0 => '0+@ 0 * * *',
    ),
  );
  $export['ultimate_cron_plugin_scheduler_simple_settings'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'ultimate_cron_plugin_settings_poorman_settings';
  $strongarm->value = array(
    'launcher' => '',
    'early_page_flush' => 1,
    'user_agent' => 'Ultimate Cron',
  );
  $export['ultimate_cron_plugin_settings_poorman_settings'] = $strongarm;

  return $export;
}
