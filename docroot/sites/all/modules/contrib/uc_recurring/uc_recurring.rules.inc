<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implement hook_rules_data_info().
 */
function uc_recurring_rules_data_info() {
  $types['uc_recurring'] = array(
    'label' => t('Ubercart recurring fee object'),
    'wrap' => TRUE,
    'group' => t('Ubercart'),
  );
  return $types;
}

/**
 * Implement hook_rules_event_info().
 */
function uc_recurring_rules_event_info() {
  $events['uc_recurring_renewal_complete'] = array(
    'label' => t('Recurring order renewal succeeded'),
    'group' => t('Recurring Fees'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'recurring_fee' => array(
        'type' => 'uc_recurring_fee',
        'label' => t('Recurring Fee'),
      ),
    ),
  );

  $events['uc_recurring_renewal_failed'] = array(
    'label' => t('Recurring order renewal failed'),
    'group' => t('Recurring Fees'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'recurring_fee' => array(
        'type' => 'uc_recurring_fee',
        'label' => t('Recurring Fee'),
      ),
    ),
  );

  $events['uc_recurring_cancel'] = array(
    'label' => t('Recurring order cancelled'),
    'group' => t('Recurring Fees'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'recurring_fee' => array(
        'type' => 'uc_recurring_fee',
        'label' => t('Recurring Fee'),
      ),
    ),
  );

  $events['uc_recurring_renewal_expired'] = array(
    'label' => t('Recurring order renewal expired'),
    'group' => t('Recurring Fees'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'recurring_fee' => array(
        'type' => 'uc_recurring_fee',
        'label' => t('Recurring Fee'),
      ),
    ),
  );

  return $events;
}

/**
 * Implement hook_rules_condition_info().
 */
function uc_recurring_rules_condition_info() {
  $conditions['uc_recurring_condition_order_contains_renewals'] = array(
    'label' => t('Check if the order is a renewal'),
    'group' => t('Recurring'),
    'base' => 'uc_recurring_condition_order_contains_renewals',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
        'restriction' => 'selector',
      ),
    ),
  );
  $conditions['uc_recurring_condition_order_has_expired'] = array(
    'label' => t('Check if the recurring fee has expired'),
    'group' => t('Recurring'),
    'base' => 'uc_recurring_condition_order_has_expired',
    'parameter' => array(
      'recurring_fee' => array(
        'type' => 'uc_recurring_fee',
        'label' => t('Recurring Fee'),
        'restriction' => 'selector',
      ),
    ),
  );

  return $conditions;
}

/**
 * Check if the order contains a renewal product.
 *
 * @param $order
 *   The order object.
 * @param $recurring_fee
 *   The recurring fee object.
 */
function uc_recurring_condition_order_contains_renewals($order) {
  foreach ($order->products as $products) {
    if (!empty($products->data['recurring_fee'])) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Check if the recurring fee has expired.
 */
function uc_recurring_condition_order_has_expired($recurring_fee, $settings) {
  return ($recurring_fee->remaining_intervals == 0);
}
