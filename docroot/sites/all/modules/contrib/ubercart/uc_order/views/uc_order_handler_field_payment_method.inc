<?php

/**
 * @file
 * Payment method views field handler.
 */

/**
 *
 */
class uc_order_handler_field_payment_method extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return _uc_payment_method_data($values->{$this->field_alias}, 'name');
  }
}
