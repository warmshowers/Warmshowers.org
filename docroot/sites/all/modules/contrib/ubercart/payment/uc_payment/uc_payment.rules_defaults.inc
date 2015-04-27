<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_payment_default_rules_configuration() {
  $configs = array();

  // Set the order status to "Payment Received" when a payment is received
  // and the balance is less than or equal to 0.
  $rule = rules_reaction_rule();
  $rule->label = t('Update order status on full payment');
  $rule->active = TRUE;
  $rule->event('uc_payment_entered')
    ->condition('uc_payment_condition_order_balance', array(
      'order:select' => 'order',
      'balance_comparison' => 'less_equal',
    ))
    ->condition(rules_or()
      ->condition('uc_order_condition_order_state', array(
        'order:select' => 'order',
        'order_state' => 'in_checkout',
      ))
      ->condition('uc_order_condition_order_state', array(
        'order:select' => 'order',
        'order_state' => 'post_checkout',
      ))
    )
    ->action('uc_order_update_status', array(
      'order:select' => 'order',
      'order_status' => 'payment_received',
    ));
  $configs['uc_payment_received'] = $rule;

  // Set the order status to "Completed" when checkout is complete, none
  // of the products are shippable, and the balance is less than or equal to 0.
  $rule = rules_reaction_rule();
  $rule->label = t('Complete non-shippable order after payment received');
  $rule->active = TRUE;
  $rule->event('uc_order_status_update')
    ->condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'payment_received'))
    ->condition(rules_condition('uc_order_condition_is_shippable', array(
        'order:select' => 'updated_order',
      ))
      ->negate())
    ->action('uc_order_update_status', array(
      'order:select' => 'order',
      'order_status' => 'completed',
    ));
  $configs['uc_checkout_complete_paid'] = $rule;

  $methods = _uc_payment_method_list();
  foreach ($methods as $id => $method) {
    $set = rules_and(array(
      'order' => array('type' => 'uc_order', 'label' => t('Order')),
    ));
    $set->label = t('@method conditions', array('@method' => $method['name']));

    $configs['uc_payment_method_' . $method['id']] = $set;
  }

  return $configs;
}
