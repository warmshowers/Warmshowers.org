<?php

/**
 * Implements hook_drush_help().
 */
function mailaview_drush_help($section) {
  switch ($section) {
    case 'drush:mailaview':
      return dt('Mail a view.');
  }
}

/**
 * Implements hook_drush_command().
 */
function mailaview_drush_command() {
  $items = array();
  $items['mailaview'] = array(
    'description' => 'Mail a view.',
    'callback' => 'mailaview_drush_mailaview',
    'examples' => array(
      'drush --user=1 mav --view=view_dailysummary --display=master --to=admin@example.com,admin2@example.com' => 'Email "view_dailysummary:master" to admins, running drush as uid=1.',
    ),
    'options' => array(
      'view' => 'the name of the view',
      'display' => 'the id of the display',
      'to' => 'a comma separated list of email addresses to mail',
      'subject' => '(optional) the email subject to use, otherwise the view title is used',
    ),
    'aliases' => array('mav'),
  );
  return $items;
}

/**
 * Check all "processing" orders against OMS.
 *
 * If the order is completed, fire an event to let Rules take care of it.
 */
function mailaview_drush_mailaview() {
  // Get parameters.
  $view_id = drush_get_option('view');
  $display_id = drush_get_option('display');
  $to = drush_get_option('to');
  $emails = explode(",", $to);
  // @todo Implement 'args' drush option.
  $args = drush_get_option('args', array());
  $subject = drush_get_option('subject', NULL);
  // Check if there is such view.
  $view_name = "$view_id:$display_id";
  $views = views_get_views_as_options();
  if (empty($views[$view_name])) {
    return drush_set_error('', 'Unable to find selected display for selected view.');
  }
  // Mail a view.
  module_load_include('inc', 'mailaview', 'mailaview.rules');
  mailaview_mail_view($emails, $view_name, $args, $subject);
}
