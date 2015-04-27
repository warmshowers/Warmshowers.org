<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * Implement hook_default_rules_configuration().
 */
function uc_recurring_default_rules_configuration() {
  $configs = array();

  // Set the order status to "Completed" when checkout is complete, none
  // of the products are shippable, and the balance is less than or equal to 0.
  $rule = rules_reaction_rule();
  $rule->label = t('Update order status upon renewal completion with full payment');
  $rule->active = TRUE;
  $rule->event('uc_recurring_renewal_complete')
    ->condition('uc_payment_condition_order_balance', array(
      'order:select' => 'order',
      'balance_comparison' => 'less_equal',
    ))
    ->condition(rules_condition('uc_order_condition_is_shippable', array(
        'order:select' => 'order',
      ))
      ->negate())
    ->action('uc_order_update_status', array(
      'order:select' => 'order',
      'order_status' => 'completed',
    ));
  $configs['uc_recurring_renewal_paid'] = $rule;

  return $configs;
}
