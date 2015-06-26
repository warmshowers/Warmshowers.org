<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * Implements hook_default_rules_configuration().
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
  
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a renewal has occurred.');
  $rule->weight = 2;
  $rule->active = FALSE;
  $rule->event('uc_recurring_renewal_complete')
    ->condition('uc_recurring_condition_order_contains_renewals', array(
      'order:select' => 'order',
    ))
    ->action('uc_recurring_renewal_email', array(
      'order:select' => 'order',
      'recurring_fee:select' => 'recurring_fee',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => uc_get_message('uc_recurring_renewal_completed_subject'),
      'message' => uc_get_message('uc_recurring_renewal_completed_message'),
      'format' => filter_fallback_format(),
    ));
  $configs['uc_recurring_renewal_email_completed'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a renewal has failed.');
  $rule->weight = 1;
  $rule->active = FALSE;
  $rule->event('uc_recurring_renewal_failed')
    ->condition(rules_and()
      ->condition('uc_recurring_condition_order_contains_renewals', array(
        'order:select' => 'order',
      ))
      //@@@ Um, rules_and() what exactly? Some way to differentiate between
      // this and uc_recurring_renewal_email_expired maybe?
    )
    ->action('uc_recurring_renewal_email', array(
      'order:select' => 'order',
      'recurring_fee:select' => 'recurring_fee',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => uc_get_message('uc_recurring_renewal_failed_subject'),
      'message' => uc_get_message('uc_recurring_renewal_failed_message'),
      'format' => filter_fallback_format(),
    ));
  $configs['uc_recurring_renewal_email_failed'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a renewal has expired due to failed payment.');
  $rule->weight = 1;
  $rule->active = FALSE;
  $rule->event('uc_recurring_renewal_failed')
    ->condition(rules_and()
      ->condition('uc_recurring_condition_order_contains_renewals', array(
        'order:select' => 'order',
      ))
      ->condition('uc_recurring_condition_order_has_expired', array(
        'recurring_fee:select' => 'recurring_fee',
      ))
    )
    ->action('uc_recurring_renewal_email', array(
      'order:select' => 'order',
      'recurring_fee:select' => 'recurring_fee',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => uc_get_message('uc_recurring_renewal_expired_subject'),
      'message' => uc_get_message('uc_recurring_renewal_expired_message'),
      'format' => filter_fallback_format(),
    ));
  $configs['uc_recurring_renewal_email_expired'] = $rule;

  return $configs;
}
