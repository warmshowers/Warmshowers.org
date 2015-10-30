<?php

/**
 * @file
 * Views handler.
 */

/**
 * Filters by country code.
 */
class uc_order_handler_filter_country extends views_handler_filter_in_operator {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Country');
      $this->value_options = uc_country_option_list();
    }
  }

}
