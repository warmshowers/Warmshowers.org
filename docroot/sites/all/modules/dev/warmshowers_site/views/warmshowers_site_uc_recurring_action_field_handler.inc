<?php
// $Id:

/**
 * @file
 * The Views status field handler for warmshowers uc_recurring action fields.
 */

/**
 * Field handler for uc_recurring status field.
 */
class warmshowers_site_uc_recurring_action_field_handler extends views_handler_field {

  function render($values) {
     var_dump($values = $values->uid); exit;

      return parent::render($values);
  }
}
