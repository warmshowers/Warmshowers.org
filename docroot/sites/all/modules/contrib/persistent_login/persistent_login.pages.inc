<?php

/**
 * @file
 * Implementation of Persistent Login forms.
 */

/**
 * Module settings form.
 */
function persistent_login_admin_settings($form, &$form_state) {
  $form = array();

  if (ini_get('session.cookie_lifetime') > 0) {
    drupal_set_message(_persistent_login_get_config_warning_msg(), 'error');
  }

  $form['persistent_login_welcome'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show welcome message on persistent login.'),
    '#default_value' => variable_get('persistent_login_welcome', TRUE),
    '#description' => t('If checked, the message \'Welcome back, <em>username</em>\' will be displayed each time a new login session is created via a Persistent Login cookie.'),
  );

  $form['persistent_login_maxlife'] = array(
    '#type' => 'textfield',
    '#title' => t('Days to remember the user'),
    '#size' => 10,
    '#maxlength' => 10,
    '#default_value' => variable_get('persistent_login_maxlife', PERSISTENT_LOGIN_MAXLIFE),
    '#description' => t('The maximum number of days for which a persistent login session is valid; afterwards, the user will need to log in again.  Enter 0 for no expiration.'),
  );

  $form['persistent_login_maxlogins'] = array(
    '#type' => 'select',
    '#title' => t('Remembered logins per user'),
    '#default_value' => variable_get('persistent_login_maxlogins', 10),
    '#options' => drupal_map_assoc(array(5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100)),
    '#description' => t('The maximum number of Persistent Logins remembered per user.'),
  );

  $form['persistent_login_cookie_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie name prefix'),
    '#size' => 40,
    '#maxlength' => 40,
    '#default_value' => variable_get('persistent_login_cookie_prefix', 'PERSISTENT_LOGIN_'),
    '#description' => t('A string used as prefix to build the persistent login cookie name. Characters allowed: ASCII letters ([A-Z], [a-z]), digits ([0-9]), hyphens ("-") or underscores ("_"). Previous cookies stored in user browsers will be lost when changing this value.'),
  );

  $form['persistent_login_secure'] = array(
    '#type' => 'radios',
    '#title' => t('Pages which require an explicit login'),
    '#default_value' => variable_get('persistent_login_secure', 1),
    '#options' => array(
      t('Every page except the listed pages.'),
      t('Only the listed pages.'),
    ),
  );

  $form['persistent_login_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('persistent_login_pages', PERSISTENT_LOGIN_SECURE_PATHS),
    '#cols' => 40,
    '#rows' => 5,
    '#description' => t("Enter one page per line as Drupal paths.  The '*' character is a wildcard.  Example paths are '<em>blog</em>' for the blog page, '<em>blog/*</em>' for every personal blog, and '<em>blog*</em>' for both. '<em>&lt;front&gt;</em>' is the front page."),
  );

  $form['persistent_login_history'] = array(
    '#type' => 'textfield',
    '#title' => t('Days to keep a log of persistent login actions'),
    '#default_value' => variable_get('persistent_login_history', 0),
    '#size' => 4,
    '#maxlength' => 10,
    '#description' => t('The number of days to keep an entry in the persistent_login_history table for each time a login is invalidated, including when a user logs in.  Entering 0 will disable logging.  This setting is intended for debugging purposes.'),
  );

  $form['#validate'] = array('persistent_login_admin_settings_validate');

  return system_settings_form($form);
}

/**
 * Validate the module settings form.
 */
function persistent_login_admin_settings_validate($form, &$form_state) {
  $form_values = &$form_state['values'];
  if (!preg_match('`^[0-9]+$`', $form_values['persistent_login_maxlife'])) {
    form_set_error('persistent_login_maxlife', t('%name: You should specify a valid integer value.', array('%name' => $form['persistent_login_maxlife']['#title'])));
  }
  if (!preg_match('`^[-_A-Za-z0-9]+$`', $form_values['persistent_login_cookie_prefix'])) {
    form_set_error('persistent_login_cookie_prefix', t('%name: Characters allowed: ascii letters ([A-Z], [a-z]), digits ([0-9]), hyphens ("-") or underscores ("_").', array('%name' => $form['persistent_login_cookie_prefix']['#title'])));
  }
  elseif ($form_values['persistent_login_cookie_prefix'] == 'SESS') {
    form_set_error('persistent_login_cookie_prefix', t('%name: Invalid value. "SESS" is already used by the Drupal session cookie.', array('%name' => $form['persistent_login_cookie_prefix']['#title'])));
  }
  if (!preg_match('`^[0-9]+$`', $form_values['persistent_login_history'])) {
    form_set_error('persistent_login_history', t('%name: You should specify a valid integer value.', array('%name' => $form['persistent_login_history']['#title'])));
  }
}

/**
 * Menu callback to erase a user's Persistent Login records. The menu
 * entry guarantees that the user is logged in.
 *
 * Redirects to user/$uid if allowed or calls drupal_access_denied().
 *
 * @param $uid
 *  The user id for whom to erase PL records.  A user can specify any
 *  uid for the callback, but only the current user's id will work
 *  unless the user has 'administer persistent login' permission.
 */
function persistent_login_erase($uid = NULL) {
  global $user;
  if (!isset($uid)) {
    $uid = $user->uid;
  }
  if ($user->uid == $uid || user_access('administer persistent login')) {
    _persistent_login_invalidate('erase', 'uid = :uid', array(':uid' => $uid));
    drupal_goto('user/' . $uid);
  }
  drupal_access_denied();
}

/**
 * After_build callback for login forms.
 */
function persistent_login_form_after_build($form, &$form_state) {
  // Get a reference to the portion of the form we are interested in.
  if (isset($form['account']) && is_array($form['account'])) {
    $form_reference = &$form['account'];
  }
  else {
    $form_reference = &$form;
  }

  // If we don't have a submit button, then it's a sign someone else altered
  // the login form in a way that cannot be submitted, so we remove the
  // "Remember me" checkbox we added in hook_form_alter() and quit.
  if (!isset($form['actions']['submit'])) {
    unset($form_reference['persistent_login']);
    return $form;
  }

  // FAPI ensures a weight is assigned to all elements before after_build
  // callback is invoked.

  // Get the weight assigned to the password field.
  $original_pass_weight = $form_reference['pass']['#weight'];

  // Increase the weight of all elements with a weight greater to the one assigned
  // to the password field to make room for the "Remember me" checkbox.
  foreach (element_children($form_reference) as $key) {
    if (isset($form_reference[$key]) && $form_reference[$key]) {
      if ($form_reference[$key]['#weight'] > $original_pass_weight) {
        $form_reference[$key]['#weight'] += 1;
      }
    }
  }

  // Give the "Remember me" checkbox a weight based on the one assigned
  // to the password field so it renders just after that.
  $form_reference['persistent_login']['#weight'] = $original_pass_weight + 0.5;

  // Ensure drupal_render() performs the sort by weight step on the form.
  unset($form_reference['#sorted']);

  // Adjust the tabindex of the plain login form.
  if (isset($form['actions']['submit']['#attributes']) && isset($form['actions']['submit']['#attributes']['tabindex'])) {
    $tabindex = (int) $form['actions']['submit']['#attributes']['tabindex'];
    $form_reference['persistent_login']['#attributes']['tabindex'] = $tabindex;
    $form['actions']['submit']['#attributes']['tabindex'] = $tabindex + 1;
  }

  return $form;
}
