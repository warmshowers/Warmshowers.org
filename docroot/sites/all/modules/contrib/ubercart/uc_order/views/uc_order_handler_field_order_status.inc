<?php

/**
 * @file
 * Views handler: Order status field.
 */

/**
 * Returns a human readable text for order status to display in the View.
 */
class uc_order_handler_field_order_status extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return check_plain(uc_order_status_data($this->get_value($values), 'title'));
  }

}
