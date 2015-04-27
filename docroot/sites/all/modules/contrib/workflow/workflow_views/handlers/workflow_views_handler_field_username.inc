<?php
/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Field handler to allow linking to a user account.
 */
class workflow_views_handler_field_username extends views_handler_field_user_name {

  /**
   * Return user name as default value.
   */
  function get_value($values, $field = NULL) {
    if ($field == NULL) {
      if ($uid = $this->get_value($values, 'uid')) {
        $account = user_load($uid);
        return $account->name;
      }
    }
    return parent::get_value($values, $field);
  }

}
