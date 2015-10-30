<?php

/**
 * @file
 * This file contains the default Rules configurations that allow conditions to
 * be applied to taxes.
 */

/**
 * Implements hook_default_rules_configuration().
 *
 * Creates a condition set for each tax rule.
 */
function uc_taxes_default_rules_configuration() {
  $configs = array();

  // Loop through all the defined tax rates.
  foreach (uc_taxes_rate_load() as $rate) {
    $set = rules_and(array(
      'order' => array('type' => 'uc_order', 'label' => 'Order'),
    ));
    $set->label = t('@name conditions', array('@name' => $rate->name));

    $configs['uc_taxes_' . $rate->id] = $set;
  }

  return $configs;
}
