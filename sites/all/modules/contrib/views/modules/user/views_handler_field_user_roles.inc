<?php
// $Id: views_handler_field_user_roles.inc,v 1.1 2008/09/03 19:21:30 merlinofchaos Exp $
/**
 * Field handler to provide a list of roles.
 */
class views_handler_field_user_roles extends views_handler_field_prerender_list {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['uid'];
  }

  function pre_render($values) {
    $uids = array();
    $this->items = array();

    foreach ($values as $result) {
      $uids[] = $result->{$this->aliases['uid']};
    }

    if ($uids) {
      $result = db_query("SELECT u.uid, u.rid, r.name FROM {role} r INNER JOIN {users_roles} u ON u.rid = r.rid WHERE u.uid IN (" . implode(', ', $uids) . ") ORDER BY r.name");
      while ($role = db_fetch_object($result)) {
        $this->items[$role->uid][$role->rid] = check_plain($role->name);
      }
    }
  }
}
