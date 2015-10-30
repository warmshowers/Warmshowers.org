<?php

/**
 * @file
 * Rules configurations for shipping quotes modules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_quote_default_rules_configuration() {
  $configs = array();
  $methods = module_invoke_all('uc_shipping_method');

  foreach ($methods as $method) {
    $set = rules_and(array(
      'order' => array('type' => 'uc_order', 'label' => t('Order')),
    ));
    $set->label = t('@method conditions', array('@method' => $method['title']));

    $configs['get_quote_from_' . $method['id']] = $set;
  }

  return $configs;
}
