<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implements hook_rules_condition_info().
 */
function uc_recurring_rules_condition_info() {
  return array(
    'uc_recurring_condition_order_contains_renewals' => array(
      'label' => t('Check if the order is a renewal'),
      'group' => t('Recurring'),
      'base' => 'warmshowers_site_condition_user_with_order_with_role_contains_renewals',
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User')
        ),
        'roles' => array(
          'type' => 'list<integer>',
          'label' => t('Roles'),
          'options list' => 'rules_user_roles_options_list',
        ),
      ),
    ),
  );
}

/**
 * Check if the user has a renewal product with certain roles.
 *
 * @param $account
 *   A user account object.
 * @param $roles
 *   A list of role objects.
 */
function warmshowers_site_condition_user_with_order_with_role_contains_renewals($account, $role) {

  // @TODO Put this in a wrapper function
  // $result = _warmshowers_site_get_recurring_fees_for_role_expiration($account, $role);

  return FALSE;
}