<?php

/**
 * @file
 * Views handler: Order status field.
 */

/**
 * Return a human readable text for order status to display in the View.
 */
class uc_views_handler_field_order_status extends views_handler_field {
  function render($values) {
    return uc_order_status_data($values->{$this->field_alias}, 'title');
  }
}
