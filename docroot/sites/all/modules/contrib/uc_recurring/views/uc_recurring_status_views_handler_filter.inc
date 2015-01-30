<?php
// $Id:

/**
 * @file
 * The Views status filter handler for uc_recurring.
 */

/**
 * Filter handler for uc_recurring status field.
 */
class uc_recurring_status_views_handler_filter extends views_handler_filter_in_operator {
  // Get the status list to display for selection.
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status');
      $options = uc_recurring_fee_status_label();
      $this->value_options = $options;
    }
  }
}
