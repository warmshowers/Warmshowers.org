<?php

/**
 * @file
 * Views handler: Order total weight
 */

/**
 * Return the full name
 */
class uc_views_handler_field_order_weight extends views_handler_field_numeric {
  function option_definition() {
    $options = parent::option_definition();
    $options['weight_units'] = array('default' => variable_get('uc_weight_unit', 'lb'));
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['weight_units'] = array('#type' => 'select',
      '#title' => t('Unit of measurement'),
      '#default_value' => $this->options['weight_units'],
      '#options' => array(
        'lb' => t('Pounds'),
        'kg' => t('Kilograms'),
        'oz' => t('Ounces'),
        'g' => t('Grams'),
      ),
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $oid = $values->{$this->aliases['order_id']};
    $order = uc_order_load($oid);
    $total = 0;
    // TODO: Replace this for a mysql query like the cost field handler

    foreach ($order->products as $product) {
      $unit_conversion = uc_weight_conversion($product->weight_units, $this->options['weight_units']);
      $total += $product->qty * $product->weight * $unit_conversion;
    }

    $this->field_alias = 'order_weight';
    $values->{$this->field_alias} = $total;
    return parent::render($values);
  }
}
