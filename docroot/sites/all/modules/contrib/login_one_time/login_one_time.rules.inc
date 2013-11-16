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
      'module' => 'Login one time',
      'arguments' => array(
        'account' => array(
          'type' => 'user',
          'label' => t("The user's account"),
        ),
      ),
    ),
  );
}
