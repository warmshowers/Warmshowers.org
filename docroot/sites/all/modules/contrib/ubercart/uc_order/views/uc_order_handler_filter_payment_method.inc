<?php

/**
 * @file
 * Views handler: Returns payment method name for payment method id.
 */

/**
 * Filters by payment method.
 */
class uc_order_handler_filter_payment_method extends views_handler_filter_in_operator {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = array();

      if (module_exists('uc_payment')) {
        $methods = _uc_payment_method_list();
        foreach ($methods as $method) {
          $this->value_options[$method['id']] = $method['name'];
        }
      }
    }
  }

}
