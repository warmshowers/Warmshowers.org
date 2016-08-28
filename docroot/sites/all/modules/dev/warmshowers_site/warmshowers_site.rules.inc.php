<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implements hook_rules_condition_info().
 */
function warmshowers_site_rules_condition_info() {
  return array(
    'warmshowers_site_condition_role_product_contains_renewals' => array(
      'label' => t('Check if the role product is a renewal'),
      'group' => t('Recurring'),
      'base' => '_warmshowers_site_get_recurring_fees_by_role_expiration',
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User'),
          'restriction' => 'selector',
        ),
        'roles' => array(
          'type' => 'role',
          'label' => t('Roles'),
          'restriction' => 'selector',
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
function _warmshowers_site_get_recurring_fees_by_role_expiration($account, $role) {
  if (empty($role->rid) || empty($account->uid)) {
    return FALSE;
  }

  foreach (_warmshowers_site_get_orders_with_expirations($account, NULL, $role->rid) as $order) {
    // if there are recurring fees and role expirations then we've matched.
    if (!empty($order['expiration']) && !empty($order['recurring_fees'])) {
      return TRUE;
    }
  }

  return FALSE;
}