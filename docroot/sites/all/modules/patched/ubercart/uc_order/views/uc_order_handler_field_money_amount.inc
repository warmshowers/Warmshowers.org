<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Returns a formatted price value to display in the View.
 */
class uc_order_handler_field_money_amount extends uc_product_handler_field_price {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    if ($this->options['format'] == 'numeric') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'uc_price') {
      return uc_currency_format($values->{$this->field_alias});
    }
  }

}
