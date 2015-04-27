<?php

/**
 * @file
 * Views handler: Return actions for order id.
 */

/**
 * Returns actions for an order.
 */
class uc_order_handler_field_order_actions extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $order = uc_order_load($values->order_id);
    return uc_order_actions($order, TRUE);
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    // Do nothing.
  }

}
