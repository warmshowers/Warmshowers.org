<?php
// $Id: coder_50.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * This include file implements coder functionality for 4.7 -> 5.x upgrades.
 *
 * @note
 *   It is inappropriately named coder_50 instead of coder_5, due to confussion
 *   of the new numbering standard that began with Drupal 5, yet it's not worth
 *   changing all the names at this point.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_50_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]user_mail\s*\(',
      '#warning_callback' => '_coder_50_user_mail_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]user_mail_wrapper\s*\(',
      '#warning_callback' => '_coder_50_user_mail_wrapper_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]message_na\s*\(',
      '#warning_callback' => '_coder_50_message_na_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]module_exist\s*\(',
      '#warning_callback' => '_coder_50_module_exist_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_call_js\s*\(',
      '#warning_callback' => '_coder_50_drupal_call_js_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]system_listing\s*\(',
      '#warning_callback' => '_coder_50_system_listing_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_get_name\s*\(',
      '#warning_callback' => '_coder_50_node_get_name_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\$_POST\s*\[\s*[\'"]?op[\'"]?\s*\]',
      '#warning' => '$_POST[\'op\'] deprecated in favor of $form_values[\'op\']',
      '#severity' => 'normal',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_50_callback',
      '#warning_callback' => '_coder_50_callback_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '[\s\(]theme_add_style\s*\(',
      '#warning_callback' => '_coder_50_theme_add_style_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '[\s\(]form_render\s*\(',
      '#warning_callback' => '_coder_50_form_render_warning',
    ),
  );
  $review = array(
    '#title' => t('Converting 4.7.x modules to 5.x'),
    '#link' => 'http://drupal.org/node/64279',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade50' => $review);
}

/**
 * Define the rule callbacks for 5.x, see do_coder_review_callback().
 */
function _coder_50_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // Only perform this check on module's (not includes).
  $filename = $coder_args['#filename'];
  if (substr($filename, -7) == '.module') {
    // Make sure that a .info file exists.
    $filename = drupal_substr(realpath($filename), 0, -7) .'.info';
    if (!file_exists($filename)) {
      $severity_name = _coder_severity_name($coder_args, $review, $rule);
      _coder_error($results, $rule, $severity_name);
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_50_callback_warning() {
  return t('All modules now need to have a <a href="@info">modulename.info file</a>', array('@info' => 'http://drupal.org/node/101009'));
}

function _coder_50_user_mail_warning() {
  return t('!user_mail() is replaced by !drupal_mail()',
    array(
      '!user_mail' => theme('drupalapi', 'user_mail', '4.7'),
      '!drupal_mail' => theme('drupalapi', 'drupal_mail', '5'),
    )
  );
}

function _coder_50_user_mail_wrapper_warning() {
  return t('!user_mail_wrapper() changed to !drupal_mail_wrapper()',
    array(
      '!user_mail_wrapper' => theme('drupalapi', 'user_mail_wrapper', '4.7'),
      '!drupal_mail_wrapper' => theme('drupalapi', 'drupal_mail_wrapper', '5'),
    )
  );
}

function _coder_50_message_na_warning() {
  return t('The function !message_na() was removed, remove it from your modules as well and replace it with !t(\'n/a\')',
    array(
      '!message_na' => theme('drupalapi', 'message_na', '4.7'),
      '!t' => theme('drupalapi', 't', '5'),
    )
  );
}

function _coder_50_module_exist_warning() {
  return t('!module_exist() is now !module_exists()',
    array(
      '!module_exist' => theme('drupalapi', 'module_exist', '4.7'),
      '!module_exists' => theme('drupalapi', 'module_exists', '5'),
    )
  );
}

function _coder_50_drupal_call_js_warning() {
  return t('Remove !drupal_call_js(), use "!drupal_add_js(\'myCustomFunction(your, parameters, here)\', \'inline\');" instead',
    array(
      '!drupal_call_js' => theme('drupalapi', 'drupal_call_js', '4.7'),
      '!drupal_add_js' => theme('drupalapi', 'drupal_add_js', '5'),
    )
  );
}

function _coder_50_system_listing_warning() {
  return t('!system_listing() is now !drupal_system_listing()',
    array(
      '!system_listing' => theme('drupalapi', 'system_listing', '4.7'),
      '!drupal_system_listing' => theme('drupalapi', 'drupal_system_listing', '5'),
    )
  );
}

function _coder_50_theme_add_style_warning() {
  return t('Replace !theme_add_style() with !drupal_add_css()',
    array(
      '!theme_add_style' => theme('drupalapi', 'theme_add_style', '4.7'),
      '!drupal_add_css' => theme('drupalapi', 'drupal_add_css', '5'),
    )
  );
}

function _coder_50_form_render_warning() {
  return t('Replace !form_render() with !drupal_render()',
    array(
      '!form_render' => theme('drupalapi', 'form_render', '4.7'),
      '!drupal_render' => theme('drupalapi', 'drupal_render', '5'),
    )
  );
}

function _coder_50_node_get_name_warning() {
  return t('Replace !node_get_name($node) with !node_get_types(\'name\', $node)',
    array(
      '!node_get_name' => theme('drupalapi', 'node_get_name', '4.7'),
      '!node_get_types' => theme('drupalapi', 'node_get_types', '5'),
    )
  );
}
