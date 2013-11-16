<?php

/**
 * @file
 * Views filter handler for the views_watchdog module.
 */


/**
 * Provides severity filter options for the watchdog entry.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_watchdog_severity extends views_handler_filter_in_operator {


  function get_value_options() {

    if (!isset($this->value_options)) {

      $options = array();
      foreach (_views_watchdog_get_severity() as $key => $value) {
        $options[$key] = $value['title'];
      }

      $this->value_options = $options;
    }
  }
}
