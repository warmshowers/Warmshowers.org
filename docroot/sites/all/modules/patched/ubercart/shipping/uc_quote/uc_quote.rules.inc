<?php

/**
 * @file
 * Rules hooks for uc_quote.module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function uc_quote_rules_condition_info() {
  return array(
    'uc_quote_condition_order_shipping_method' => array(
      'label' => t("Order has a shipping quote from a particular method"),
      'group' => t('Order: Shipping Quote'),
      'base' => 'uc_quote_condition_order_shipping_method',
      'parameter' => array(
        'order' => array('type' => 'uc_order', 'label' => t('Order')),
        'method' => array('type' => 'text', 'label' => t('Shipping method'), 'options list' => 'uc_quote_condition_order_shipping_method_options'),
      ),
    ),
  );
}

/**
 * Checks an order's shipping method.
 */
function uc_quote_condition_order_shipping_method($order, $method) {
  // Check the easy way first.
  if (!empty($order->quote)) {
    return $order->quote['method'] == $method;
  }
  // Otherwise, look harder.
  if (!empty($order->line_items)) {
    $methods = module_invoke_all('uc_shipping_method');
    $accessorials = $methods[$method]['quote']['accessorials'];

    foreach ($order->line_items as $line_item) {
      if ($line_item['type'] == 'shipping' && in_array($line_item['title'], $accessorials)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Options callback.
 *
 * @see uc_quote_condition_order_shipping_method()
 */
function uc_quote_condition_order_shipping_method_options() {
  $methods = module_invoke_all('uc_shipping_method');
  $enabled = variable_get('uc_quote_enabled', array());

  $options = array();
  foreach ($methods as $id => $method) {
    $options[$id] = $method['title'];
    if (!isset($enabled[$id]) || !$enabled[$id]) {
      $options[$id] .= ' ' . t('(disabled)');
    }
  }

  return $options;
}
