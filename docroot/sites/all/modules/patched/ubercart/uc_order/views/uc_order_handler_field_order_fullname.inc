<?php

/**
 * @file
 * Views handler: Full name field handler (first and last).
 */

/**
 * Returns the customer's full name.
 */
class uc_order_handler_field_order_fullname extends views_handler_field {

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'first_last');
    return $options;
  }

  /**
   * Overrides views_handler::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['format'] =  array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => array(
        'first_last' => t('First Last'),
        'last_c_first' => t('Last, First'),
        'last_first' => t('Last First'),
      ),
      '#default_value' => $this->options['format'],
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
    $prefix = $this->definition['prefix'];
    $first = check_plain($values->{$this->aliases[$prefix . '_first_name']});
    $last = check_plain($values->{$this->aliases[$prefix . '_last_name']});
    switch ($this->options['format']) {
      case 'last_first':
        return "$last $first";
      case 'last_c_first':
        return "$last, $first";
      case 'first_last':
        return "$first $last";
    }
  }

}
