<?php

/**
 * @file
 * Views handler.
 */

/**
 * Filters by zone code.
 */
class uc_order_handler_filter_zone extends views_handler_filter_in_operator {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Zone');
      $this->value_options = uc_zone_option_list();
    }
  }

}
