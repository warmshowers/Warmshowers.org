<?php

/**
 * @file
 * Views handler.
 */

/**
 * Filters by order status.
 */
class uc_order_handler_filter_order_status extends views_handler_filter_in_operator {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $options['_active'] = t('Active');
      foreach (uc_order_status_list() as $status => $name) {
        $options[$name['id']] = $name['title'];
      }

      $this->value_title = t('Order status');
      $this->value_options = $options;
    }
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    if (is_array($this->value) && in_array('_active', $this->value)) {
      $this->value = array_merge($this->value, array_values(uc_order_status_list('general', TRUE)));
    }

    parent::query();
  }
}
