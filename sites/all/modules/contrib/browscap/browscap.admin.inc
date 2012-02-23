<?php
/**
 * @file
 * Generate configuration form.
 */

/**
 * Implementation of hook_form().
 */
function browscap_settings_form(&$form_state) {
  // Check the local browscap data version number
  $version = variable_get('browscap_version', 0);

  // If the version number is 0 then browscap data has never been fetched
  if ($version == 0) {
    $version = t('Never fetched');
  }

  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('User agent detection settings'),
  );
  $form['data']['browscap_data_version'] = array(
    '#value' => '<p>' . t('Current browscap data version: %fileversion.', array('%fileversion' => $version)) . '</p>',
  );
  $form['data']['browscap_enable_automatic_updates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic updates'),
    '#default_value' => variable_get('browscap_enable_automatic_updates', TRUE),
    '#description' => t('Automatically update the user agent detection information.'),
  );
  $form['data']['browscap_automatic_updates_timer'] = array(
    '#type' => 'select',
    '#title' => t('Check for new user agent detection information every'),
    '#default_value' => variable_get('browscap_automatic_updates_timer', 604800),
    '#options' => drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval'),
    '#description' => t('Newer user agent detection information will be automatically downloaded and installed. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('http://drupal.org/cron'))),
  );
  $form['user_agent_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('User agent log settings'),
  );
  $form['user_agent_log']['browscap_enable_user_agent_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable user agent log'),
    '#default_value' => variable_get('browscap_enable_user_agent_log', FALSE),
    '#description' => t('Increment a counter each time content is viewed by a user agent.'),
  );
  $form['buttons']['browscap_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh browscap data'),
    '#submit' => array('browscap_refresh_submit'),
    '#weight' => 10,
  );

  return system_settings_form($form);
}

/**
 * Submit handler for the refresh browscap button.
 */
function browscap_refresh_submit($form, &$form_state) {
  // Update the browscap information
  _browscap_import(FALSE);

  // Record when the browscap information was updated
  variable_set('browscap_imported', REQUEST_TIME);
}
