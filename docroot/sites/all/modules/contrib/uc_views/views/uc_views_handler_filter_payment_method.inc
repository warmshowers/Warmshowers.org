<?php

/**
 * @file
 * Original patch by psy
 * Views handler: Return payment method name for payment method id
 */

/**
 * Filter by payment method
 *
 */
class uc_views_handler_filter_payment_method extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Payment Method');
      $this->value_options = _distinct_payment_methods(); // $method;
    }
  }
}

/**
 *_distinct_payment_methods()
 *returns an array of DISTINCT payment methods from the uc_oders table
*/
function _distinct_payment_methods() {
  $methods = array();

  $result = db_query("SELECT DISTINCT payment_method AS payment_methods FROM {uc_orders}");
  while ($pm = db_fetch_object($result)) {
    $methods[$pm->payment_methods] = $pm->payment_methods;
  }

  return $methods;
}