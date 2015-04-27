<?php
// $Id:

/**
 * @file
 * The Views status field handler for uc_recurring.
 */

/**
 * Field handler for uc_recurring status field.
 */
class uc_recurring_status_views_handler_field extends views_handler_field {
  // We need a handler on the status to convert from integer to a label.
  function render($values) {
    $status = $values->uc_recurring_users_status;
    // If no status assigned, then it spits an array. Set that to N/A.
    // Note that 0 is a valid value equal to Active.
    $label = ($status != '') ? uc_recurring_fee_status_label($status) : 'N/A';
    return $label;
    //return var_dump($values);
  }
}
