<?php
// $Id: views_handler_filter_boolean_operator_string.inc,v 1.1 2008/12/16 19:12:27 merlinofchaos Exp $
/**
 * Simple filter to handle matching of boolean values.
 *
 * This handler checks to see if a string field is empty (equal to '') or not.
 * It is otherwise identical to the parent operator.
 *
 * Definition items:
 * - label: (REQUIRED) The label for the checkbox.
 */
class views_handler_filter_boolean_operator_string extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . (empty($this->value) ? '=' : '<>') . " ''");
  }
}
