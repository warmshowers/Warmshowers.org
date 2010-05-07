<?php
// $Id: system_service.inc,v 1.1.2.5.2.5 2010/02/27 17:20:40 heyrocker Exp $

/**
 * @file
 *  Link general system functionalities to services module.
 */

/**
 * Create a new anonymous session for services.
 *
 * @return 
 *   A new session object.
 */
function system_service_connect() {
  global $user;

  $return = new stdClass();
  $return->sessid = session_id();
  $return->user = $user;

  return $return;
}

/**
 * Implementation of hook_mail().
 */
function system_service_mail($mailkey, &$message, $params) {
  $language = $message['language'];
  $variables = user_mail_tokens($params['account'], $language);
  $message['subject'] = t($params['subject'], $variables, $language->language);
  $message['body'] = t($params['body'], $variables, $language->language);
  foreach ($params['headers'] as $header => $val) {
    $message['headers'][$header] = $val;
  }
}

/**
 * Send an email using the Services module.
 *
 * @param $mailkey
 *   A key to identify the e-mail sent.
 * @param $to
 *   The recipient's email address(es).  The formatting of this string must 
 *   comply with RFC 2822.
 * @param $subject
 *   The subject of the email.
 * @param $body
 *   The body of the email. 
 * @param $from
 *   The sender's email address.
 * @param $headers
 *   An associative array of optional mail headers.
 *
 * @return
 *   An array structure containing all details of the message.
 *
 * @see hook_mail()
 * @see drupal_mail()
 */
function system_service_mailprepare($mailkey, $to, $subject, $body, $from = NULL, $headers = array()) {
  $params = array();
  $params['subject'] = $subject;
  $params['body'] = $body;
  $params['headers'] = $headers;
  $status = drupal_mail('system_service', $mailkey, $to, user_preferred_language($to), $params, $from, TRUE);

  if (!$status) {
    return services_error(t('There was a problem sending your email.'));
  }
  return $status;
}

/**
 * Services implementation of variable_get().
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The value to use if the variable has never been set.
 *
 * @return
 *   The value of the variable.
 *
 * @see variable_get()
 */
function system_service_getvariable($name, $default = NULL) {
  return variable_get($name, $default);
}

/**
 * Services implementation of variable_set().
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set this variable to.
 *
 * @see variable_set()
 */
function system_service_setvariable($name, $value) {
  variable_set($name, $value);
}

/**
 * Check if a module is enabled. If so, return its version.
 *
 * @param $module
 *   The name of the module to check.
 *
 * @return
 *   The module's version string, or nothing if the module
 *   is not enable or installed.
 */
function system_service_module_exists($module) {
  if (module_exists($module)) {
    $modules = module_rebuild_cache();
    if (array_key_exists($module, $modules)) {
      return (string)$modules[$module]->info['version'];
    }
  }

  return "";
}

/**
 * Return and array of all the available services.
 *
 * @return array
 *   An array containing all services and thir methods
 *
 * @see services_get_all()
 */
function system_service_getservices() {
  return services_get_all();
}

/**
 * Services implementation of cache_clear_all().
 *
 * @see cache_clear_all()
 */
function system_service_cache_clear_all() {
  drupal_flush_all_caches();
  watchdog('system service', 'caches cleared');
}

/**
 * Log a system message.
 *
 * @param $type
 *   The category to which this message belongs.
 * @param $message
 *   The message to store in the log.
 * @param $variables
 *   Array of variables to replace in the message on display.
 * @param $severity
 *   The severity of the message, as per RFC 3164.
 * @param $link
 *   A link to associate with the message.
 *
 * @see watchdog()
 **/
function system_service_watchdog_send($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
  watchdog($type, $message, $variables, $severity, $link);
}
