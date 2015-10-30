<?php

/**
 * @file
 * Default Rules configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_cart_default_rules_configuration() {
  // Setup an example rule for limiting product quantities.
  $rule = rules_reaction_rule();
  $rule->label = t('Maximum product quantity');
  $rule->active = FALSE;
  $rule->event('uc_cart_item_presave')
    ->condition('data_is', array(
      'data:select' => 'uc-cart-item:qty',
      'op' => '>',
      'value' => '10',
    ))
    ->action('data_set', array(
      'data:select' => 'uc-cart-item:qty',
      'value' => '10',
    ))
    ->action('drupal_message', array(
      'message' => 'You are only allowed to order a maximum of 10 of [uc-cart-item:node:title].',
      'type' => 'warning',
    ));

  $configs['uc_cart_maximum_product_qty'] = $rule;

  // Setup a default configuration for customer checkout notifications.
  $rule = rules_reaction_rule();
  $rule->label = t('E-mail customer checkout notification');
  $rule->active = TRUE;
  $rule->event('uc_checkout_complete')
    ->action('uc_order_email_invoice', array(
      'order:select' => 'order',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => t('Your Order at [store:name]'),
      'template' => 'customer',
      'view' => 'checkout-mail',
    ));

  $configs['uc_checkout_customer_notification'] = $rule;

  // Setup a default predicate for admin checkout notifications.
  $rule = rules_reaction_rule();
  $rule ->label = t('E-mail admin checkout notification');
  $rule->active = TRUE;
  $rule->event('uc_checkout_complete')
    ->action('uc_order_email_invoice', array(
      'order:select' => 'order',
      'from' => uc_store_email_from(),
      'addresses' => uc_store_email(),
      'subject' => t('New Order at [store:name]'),
      'template' => 'admin',
      'view' => 'admin-mail',
    ));

  $configs['uc_checkout_admin_notification'] = $rule;

  return $configs;
}
