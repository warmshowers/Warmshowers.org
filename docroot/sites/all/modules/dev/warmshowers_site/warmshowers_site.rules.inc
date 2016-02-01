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
        'expiration' => array(
          'type' => 'uc_roles_expiration',
          'label' => t('Roles'),
          'restriction' => 'selector',
        ),
      ),
    ),
    'warmshowers_site_condition_role_expiration_multiple' => array(
      'label' => t('Check if the role expiration is not unique'),
      'group' => t('Recurring'),
      'base' => '_warmshowers_site_get_role_expirations_by_user',
      'parameter' => array(
        'expiration' => array(
          'type' => 'uc_roles_expiration',
          'label' => t('Roles'),
          'restriction' => 'selector',
        ),
      ),
    ),
  );
}