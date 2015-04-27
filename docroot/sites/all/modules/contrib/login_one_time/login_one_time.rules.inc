<?php

/**
 * Implements hook_rules_event_info().
 *
 * @ingroup rules
 */
function login_one_time_rules_event_info() {
  return array(
    'login_one_time_used' => array(
      'label' => t('When the user has used their one time login.'),
      'group' => t('Login one time'),
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t("The user's account"),
        ),
      ),
    ),
  );
}

/**
 * Implementation of hook_rules_action_info().
 * @ingroup rules
 */
function login_one_time_rules_action_info() {
  return array(
    'login_one_time_send_mail' => array(
      'label' => t('Send a login one-time email.'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('Recipient User'),
        ),
        'path' => array(
          'type' => 'uri',
          'label' => t('Destination path'),
          'optional' => TRUE,
        ),
      ),
      'group' => t('User'),
      'access callback' => 'user_access',
      'access arguments' => array('send link to login one time'),
    ),
  );
}