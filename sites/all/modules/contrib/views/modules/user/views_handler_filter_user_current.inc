<?php
// $Id: views_handler_filter_user_current.inc,v 1.1 2008/09/03 19:21:30 merlinofchaos Exp $

/**
 * Filter handler for the current user
 */
class views_handler_filter_user_current extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is the logged in user');
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . (empty($this->value) ? '!=' : '=') . " ***CURRENT_USER***");
  }
}
