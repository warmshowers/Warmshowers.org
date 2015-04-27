<?php

/**
 * @file
 * Variable module hook implementations.
 */


/**
 * Implements hook_variable_group_info().
 */
function uc_payment_pack_variable_group_info() {
  $groups['uc_payment_pack'] = array(
    'title' => t('Ubercart payment pack settings'),
    'access' => 'administer store',
    'path' => array('admin/store/settings/payment/method'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function uc_payment_pack_variable_info($options) {
  $variables['uc_cod_policy'] = array(
    'type' => 'text',
    'title' => t('COD policy message', array(), $options),
    'description' => t('Policy message for Cash On Delivery', array(), $options),
    'group' => 'uc_payment_pack',
    'default' => t('Full payment is expected upon delivery or prior to pick-up.', array(), $options),
  );
  $variables['uc_check_policy'] = array(
    'type' => 'text',
    'title' => t('Check payment policy', array(), $options),
    'description' => t('Instructions for customers on the checkout page.', array(), $options),
    'group' => 'uc_payment_pack',
    'default' => t('Personal and business checks will be held for up to 10 business days to ensure payment clears before an order is shipped.', array(), $options),
  );
  return $variables;
}
