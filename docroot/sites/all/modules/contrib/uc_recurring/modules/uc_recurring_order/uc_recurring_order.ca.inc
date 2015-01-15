<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions for the
 * UC Recurring Order module.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implementation of hook_ca_condition().
 */
function uc_recurring_order_ca_condition() {
  $conditions['uc_recurring_order_is_order_recurring_condition'] = array(
    '#title' => t('Check if the order is a recurring order'),
    '#description' => t('Returns TRUE if the current order is a recurring order.'),
    '#category' => t('Order: Recurring'),
    '#callback' => 'uc_recurring_order_condition_is_order_recurring',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Check if the order is a recurring order.
 *
 * @param $order
 *   The order object.
 * @param $settings
 *   The condition settings.
 *
 * @return
 *   Boolean for whether or not the order is a recurring order.
 */
function uc_recurring_order_condition_is_order_recurring($order, $settings) {
  if (empty($order->data['recurring_option'])) {
    return FALSE;
  }

  $recurring_option = $order->data['recurring_option'];
  $next_renewal = strtotime('+' . $recurring_option);
  if ($next_renewal > time()) {
    return TRUE;
  }

  return FALSE;
}
