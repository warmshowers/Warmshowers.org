<?php

/**
 * @file
 * Total weight field handler.
 */

/**
 * Field handler: displays a weight multiplied by the quantity.
 */
class uc_order_handler_field_order_weight_total extends uc_product_handler_field_weight {

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

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $oid = $values->{$this->aliases['order_id']};
    $order = uc_order_load($oid);
    $total = 0;

    foreach ($order->products as $product) {
      $unit_conversion = uc_weight_conversion($product->weight_units, $this->options['weight_units']);
      $total += $product->qty * $product->weight * $unit_conversion;
    }

    $this->field_alias = 'order_weight';
    $values->{$this->field_alias} = $total;

    if ($this->options['format'] == 'numeric') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'uc_weight') {
      return uc_weight_format($values->{$this->field_alias}, $this->options['weight_units']);
    }
  }

}
