<?php
/**
 * @file
 * Extra Views definitions for warmshowers
 */

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers views uses.
 */
function warmshowers_site_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'warmshowers_site') . '/views',
    ),
    'handlers' => array(
      'warmshowers_site_uc_recurring_action_field_handler' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function warmshowers_site_views_data() {
  $data = array();

  // Add custom field
  $data['uc_recurring_users']['cancel_action'] = array(
    'title' => t('Cancel Fees'),
    'help' => t('Provides a cancel action.'),
    'field' => array(
      'handler' => 'warmshowers_site_uc_recurring_action_field_handler',
    ),
  );

  return $data;
}