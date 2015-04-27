<?php

/**
 * @file
 * Uc recurring implementation for the payment pack module.
 */

/**
 * Implements hook_recurring_info().
 */
function uc_recurring_uc_payment_pack_recurring_info() {
  $items['check'] = array(
    'name' => t('Check'),
    'payment method' => 'check',
    'module' => 'uc_recurring',
    'fee handler' => 'default',
  );
  $items['cod'] = array(
    'name' => t('COD'),
    'payment method' => 'cod',
    'module' => 'uc_recurring',
    'fee handler' => 'default',
  );
  $items['other'] = array(
    'name' => t('Other'),
    'payment method' => 'other',
    'module' => 'uc_recurring',
    'fee handler' => 'default',
  );

  return $items;
}
