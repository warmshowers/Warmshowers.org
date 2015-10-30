<?php

/**
 * @file
 * Rules default configurations for uc_stock.module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_stock_default_rules_configuration() {
  $configs = array();

  $rule = rules_reaction_rule();
  $rule->label = t('Decrement stock upon order submission');
  $rule->active = TRUE;
  $rule->event('uc_checkout_complete')
    ->action('uc_stock_action_decrement_stock', array('order:select' => 'order'));
  $configs['uc_stock_decrement_on_order'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Increment stock on cancelling order');
  $rule->active = FALSE;
  $rule->event('uc_order_status_update')
    ->condition(rules_condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'canceled')))
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'canceled'))->negate())
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'in_checkout'))->negate())
    ->action('uc_stock_action_increment_stock', array('order:select' => 'order'));
  $configs['uc_stock_increment_on_cancel'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Increment stock on deleting an order');
  $rule->active = FALSE;
  $rule->event('uc_order_delete')
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'canceled'))->negate())
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'in_checkout'))->negate())
    ->action('uc_stock_action_increment_stock', array('order:select' => 'order'));
  $configs['uc_stock_increment_on_delete'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Decrement stock when order cancellation is being undone');
  $rule->active = FALSE;
  $rule->event('uc_order_status_update')
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'canceled')))
    ->condition(rules_condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'canceled'))->negate())
    ->condition(rules_condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'in_checkout'))->negate())
    ->action('uc_stock_action_decrement_stock', array('order:select' => 'order'));
  $configs['uc_stock_decrement_on_uncancel'] = $rule;

  return $configs;
}
