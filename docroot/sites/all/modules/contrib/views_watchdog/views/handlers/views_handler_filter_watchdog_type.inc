<?php

/**
 * @file
 * Views filter handler for the views_watchdog module.
 */


/**
 * Provides type filter options for the watchdog entry.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_watchdog_type extends views_handler_filter_in_operator {


  function get_value_options() {

    if (!isset($this->value_options)) {

      $options = array();
      foreach (_views_watchdog_get_type() as $key => $value) {
        $options[$value] = $value;
      }

      $this->value_options = $options;
    }
  }
}
