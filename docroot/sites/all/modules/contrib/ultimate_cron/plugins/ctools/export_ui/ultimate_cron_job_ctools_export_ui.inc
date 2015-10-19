<?php
/**
 * @file
 * Export UI integration.
 */

/**
 * Plugin definition for Ctools Export UI
 */
$plugin = array(
  'schema' => 'ultimate_cron_job',
  'menu' => array(
    'menu prefix' => 'admin/config/system/cron',
    'menu item' => 'jobs',
    'menu description' => 'Manage cron jobs',
    'items' => array(
      'list callback' => array(
        'type' => MENU_LOCAL_TASK,
        'weight' => 5,
      ),
      'run_scheduled' => array(
        'path' => 'run_scheduled',
        'title' => 'Run scheduled jobs',
        'page callback' => 'ultimate_cron_run_scheduled_page',
        'page arguments' => array(),
        'access arguments' => array('run cron jobs'),
        'module' => 'ultimate_cron',
        'file path' => drupal_get_path('module', 'ultimate_cron'),
        'file' => 'ultimate_cron.module',
        'type' => MENU_LOCAL_ACTION,
      ),
      'run' => array(
        'path' => 'list/%ctools_export_ui/run',
        'title' => 'Run',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('ultimate_cron_job_ctools_export_ui', 'run', 6),
        'load arguments' => array('ultimate_cron_job_ctools_export_ui'),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array('ultimate_cron_job_ctools_export_ui', 'run'),
        'type' => MENU_CALLBACK,
      ),
      'unlock' => array(
        'path' => 'list/%ctools_export_ui/unlock',
        'title' => 'Unlock',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('ultimate_cron_job_ctools_export_ui', 'unlock', 6),
        'load arguments' => array('ultimate_cron_job_ctools_export_ui'),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array('ultimate_cron_job_ctools_export_ui', 'unlock'),
        'type' => MENU_CALLBACK,
      ),
      'logs' => array(
        'path' => 'list/%ctools_export_ui/logs',
        'title' => 'Logs',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('ultimate_cron_job_ctools_export_ui', 'logs', 6),
        'load arguments' => array('ultimate_cron_job_ctools_export_ui'),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array('ultimate_cron_job_ctools_export_ui', 'logs'),
        'type' => MENU_VISIBLE_IN_BREADCRUMB,
      ),
      'signal' => array(
        'path' => 'list/%ctools_export_ui/signal',
        'title' => 'Signal action',
        'page callback' => 'ctools_export_ui_switcher_page',
        'page arguments' => array('ultimate_cron_job_ctools_export_ui', 'signal', 6),
        'load arguments' => array('ultimate_cron_job_ctools_export_ui'),
        'access callback' => 'ctools_export_ui_task_access',
        'access arguments' => array('ultimate_cron_job_ctools_export_ui', 'signal'),
        'type' => MENU_VISIBLE_IN_BREADCRUMB,
      ),
    ),
  ),

  // Fine grained access criterias.
  'logs access' => 'view cron jobs',
  'export access' => 'view cron jobs',
  'run access' => 'run cron jobs',
  'unlock access' => 'run cron jobs',
  'signal access' => 'run cron jobs',
  'edit access' => 'administer ultimate cron',
  'enable access' => 'administer ultimate cron',
  'disable access' => 'administer ultimate cron',
  'revert access' => 'administer ultimate cron',
  'import' => 'administer ultimate cron',

  // Add our custom operations.
  'allowed operations' => array(
    'run'  => array('title' => t('Run'), 'ajax' => FALSE, 'token' => TRUE),
    'unlock'  => array('title' => t('Unlock'), 'ajax' => TRUE, 'token' => TRUE),
    // 'kill' => array('title' => t('Kill')),
    'logs' => array('title' => t('Logs')),
    // 'custom' => array('title' => t('Custom actions')),
  ),

  'handler' => array(
    'class' => 'ultimate_cron_job_ctools_export_ui',
    'parent' => 'ctools_export_ui',
  ),

  'title' => t('Cron'),

  'title singular' => t('cron job'),
  'title plural' => t('cron jobs'),
  'title singular proper' => t('Cron job'),
  'title plural proper' => t('Cron jobs'),
);

/**
 * Form to edit the settings of an Ultimate Cron job.
 */
function ultimate_cron_job_ctools_export_ui_form(&$form, &$form_state) {
  $job = $form_state['item'];

  $form['info']['name'] = array_merge($form['info']['name'], array(
    '#type' => 'value',
  ));

  $form['jid'] = array(
    '#type'  => 'value',
    '#value' => isset($job->jid) ? $job->jid : '',
  );

  // Setup vertical tabs.
  $form['settings_tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  // Base the form on the actual form data, in case of AJAX request.
  if (isset($form_state['input'])) {
    $form_state['values'] = $form_state['input'];
  }

  // Sanitize input values.
  if (!isset($form_state['values']['settings'])) {
    $form_state['values']['settings'] = array();
  }
  $form_state['values']['settings'] += $job->settings;

  // Load settings for each plugin in its own vertical tab.
  ctools_include('plugins');
  $plugin_types = ctools_plugin_get_plugin_type_info();
  foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
    $static = $info['defaults']['static'];
    $class = $static['class'];
    $class::jobSettingsForm($form, $form_state, $plugin_type, $job);
  }

  // What the hell?
  // Why do we need to reset 'input'? Why is it used for the form fields
  // instead of 'values'?
  if (isset($form_state['input'])) {
    $form_state['input'] = $form_state['values'];
  }
  $form['#attached']['js'][] = drupal_get_path('module', 'ultimate_cron') . '/js/ultimate_cron.job.js';

}

/**
 * Ajax callback for interchangable plugin settings.
 */
function ultimate_cron_job_plugin_settings_ajax($form, &$form_state) {
  $plugin_type = $form_state['triggering_element']['#parents'][count($form_state['triggering_element']['#parents']) - 2];
  $name = $form_state['values']['settings'][$plugin_type]['name'];
  if (empty($name)) {
    $job = $form_state['item'];
    $name = $job->hook[$plugin_type]['name'];
  }

  return $form['settings'][$plugin_type][$name];
}

/**
 * Validate submission of the preset edit form.
 */
function ultimate_cron_job_ctools_export_ui_form_validate(&$form, &$form_state) {
  $job = $form_state['item'];
  ctools_include('plugins');
  $plugin_types = ctools_plugin_get_plugin_type_info();

  foreach ($form_state['values']['settings'] as $plugin_type => &$settings) {
    $static = $plugin_types['ultimate_cron'][$plugin_type]['defaults']['static'];
    $class = $static['class'];
    $class::jobSettingsFormValidate($form, $form_state, $plugin_type, $job);
  }
}

/**
 * Submit handler of the preset edit form.
 *
 * The submit handler is only responsible for moving data from
 * $form_state['values'] to $form_state['item'].
 */
function ultimate_cron_job_ctools_export_ui_form_submit(&$form, &$form_state) {
  $job = $form_state['item'];
  ctools_include('plugins');
  $plugin_types = ctools_plugin_get_plugin_type_info();

  foreach ($form_state['values']['settings'] as $plugin_type => &$settings) {
    $static = $plugin_types['ultimate_cron'][$plugin_type]['defaults']['static'];
    $class = $static['class'];
    $class::jobSettingsFormSubmit($form, $form_state, $plugin_type, $job);
  }
}
