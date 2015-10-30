<?php

/**
 * @file
 * Views handler: Product length field.
 */

/**
 * Returns a formatted length value to display in the View.
 */
class uc_product_handler_field_length extends views_handler_field_numeric {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return uc_length_format($values->{$this->field_alias}, $values->{$this->aliases['length_units']});
  }
}
