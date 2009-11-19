<?php
// $Id: views_handler_field_node_type.inc 530 2009-07-19 23:14:54Z rfay $

/**
 * Field handler to translate a node type into its readable form.
 */
class views_handler_field_node_type extends views_handler_field_node {
  function render($values) {
    $value = node_get_types('name', $values->{$this->field_alias});
    return $this->render_link(t(check_plain($value)), $values);
  }
}

