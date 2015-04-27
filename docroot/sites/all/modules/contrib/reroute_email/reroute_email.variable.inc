<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_info().
 */
function reroute_email_variable_info() {
  $variable = array();

  $variable[REROUTE_EMAIL_ENABLE] = array(
    'title' => t('Enable rerouting'),
    'description' => t('Check this box if you want to enable email rerouting. Uncheck to disable rerouting.'),
    'type' => 'boolean',
    'default' => 0,
    'group' => 'reroute_email',
  );

  $variable[REROUTE_EMAIL_ADDRESS] = array(
    'title' => t('Email addresses'),
    'description' => t('Provide a space, comma, or semicolon-delimited list of email addresses to pass through. Every destination email address which is not on this list will be rerouted to the first address on the list.'),
    'default' => variable_get('site_mail', ini_get('sendmail_from')),
    'group' => 'reroute_email',
  );

  $variable[REROUTE_EMAIL_ENABLE_MESSAGE] = array(
    'title' => t('Show rerouting description in mail body'),
    'description' => t('Check this box if you want a message to be inserted into the email body when the mail is being rerouted. Otherwise, SMTP headers will be used to describe the rerouting. If sending rich-text email, leave this unchecked so that the body of the email will not be disturbed.'),
    'type' => 'boolean',
    'default' => 1,
    'group' => 'reroute_email',
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function reroute_email_variable_group_info() {
  $groups['reroute_email'] = array(
    'title' => t('Reroute Email'),
    'description' => t('Reroute email configuration settings'),
    'access' => 'administer reroute email',
    'path' => array('admin/config/development/reroute_email'),
  );
  return $groups;
}
