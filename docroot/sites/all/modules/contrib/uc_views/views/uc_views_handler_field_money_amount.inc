<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Return a formatted price value to display in the View.
 */
class uc_views_handler_field_money_amount extends uc_product_handler_field_price {
  function options(&$options) {
    parent::options($options);

    $options['revision'] = 'themed-original';
  }

  function render($values) {
    if ($this->options['format'] == 'numeric') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'uc_price') {
      $context = array(
          'revision' => $this->options['revision'],
          'type' => 'amount',
          'class' => array(
            $this->field,
        ),
          'field' => $this->real_field,
      );

      return uc_price($values->{$this->field_alias}, $context);
    }
  }
}