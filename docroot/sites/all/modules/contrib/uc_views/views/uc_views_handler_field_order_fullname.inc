<?php

/**
 * @file
 * Views handler: Full name field handler (first and last)
 */

/**
 * Return the full name
 */
class uc_views_handler_field_order_fullname extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'first_last');
    return $options;
  }

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

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

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
