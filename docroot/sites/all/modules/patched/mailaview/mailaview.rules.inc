<?php

/**
 * Implements hook_rules_action_info().
 */
function mailaview_rules_action_info() {
  return array(
    'mailaview_mail_view' => array(
      'label' => t('Mail a view'),
      'group' => t('Mail-a-view'),
      'parameter' => array(
        'emails' => array(
          'type' => 'text',
          'label' => t('Send to'),
          'description' => t('Emails of the recipients, one per line.'),
        ),
        'view_name' => array(
          'type' => 'text',
          'label' => t('View and display'),
          'options list' => 'mailaview_views_as_options',
          'description' => t('Select the view and display you want to mail.'),
          'restriction' => 'input',
        ),
        'args' => array(
          'type' => 'text',
          'label' => t('Arguments'),
          'description' => t('Any arguments to pass to the view, one per line.
            You may use token replacement patterns.'),
          'optional' => TRUE,
          'restriction' => 'input',
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Subject'),
          'description' => t('Subject of the email.'),
          'optional' => TRUE,
          'default mode' => 'input',
        ),
      ),
    ),
  );
}

/**
 * Action Implementation: Send by mail a view.
 * @ingroup rules
 */
function mailaview_mail_view($emails, $view_name, $args = array(), $subject = NULL) {
  // Split the view name.
  list($view_id, $display_id) = explode(':', $view_name);
  // Render the view.
  $view = NULL;
  $output = mailaview_render_view($view_id, $display_id, $view, $args);
  // Get the subject.
  $subject = !empty($subject) ? $subject : $view->get_title();
  // Send the email.
  mailaview_sendmail($emails, $output, $subject);
}

/**
 * Helper function: Callback for 'options list'.
 */
function mailaview_views_as_options($element, $name) {
  return views_get_views_as_options(FALSE, 'all', NULL, TRUE, TRUE);
}
