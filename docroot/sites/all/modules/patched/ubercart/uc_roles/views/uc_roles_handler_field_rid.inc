<?php

/**
 * @file
 * Views handler: Role ID field.
 */

/**
 * Returns a role id rendered as a role name to display in the View.
 */
class uc_roles_handler_field_rid extends views_handler_field {

  /**
   * Overrides views_handler_field::pre_render().
   */
  function pre_render(&$values) {
    $roles = array();
    $this->items = array();

    // Get all the unique role ids into the keys of $roles. Initializing into
    // array_keys helps prevent us from having a list where the same rid appears
    // over and over and over.
    foreach ($values as $result) {
      $roles[$this->get_value($result, NULL, TRUE)] = FALSE;
    }

    if ($roles) {
      $result = db_query("SELECT r.rid, r.name FROM {role} r WHERE r.rid IN (:rids) ORDER BY r.name",
        array(':rids' => array_keys($roles)));
      foreach ($result as $role) {
        $this->items[$role->rid]['role'] = check_plain($role->name);
        $this->items[$role->rid]['rid'] = $role->rid;
      }
    }
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    // Return the role name corresponding to the role ID.
    // @todo: Should we be using this->get_value() here?
    $rid = $values->uc_roles_expirations_rid;
    if ($rid) {
      $role = $this->items[$rid]['role'];
      if (!empty($role)) {
        return $role;
      }
    }
  }

}
