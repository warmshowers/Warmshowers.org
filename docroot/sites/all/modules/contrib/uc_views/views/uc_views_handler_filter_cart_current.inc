<?php

/**
 * Filter handler for the current cart.
 */
class uc_views_handler_filter_cart_current extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t("Is the current user's cart");
  }

  function query() {
    $this->ensure_my_table();
    $where = "$this->table_alias.$this->real_field ";

    if (empty($this->value)) {
      $where .= '<> "***CURRENT_CART_ID***"';
      if ($this->accept_null) {
        $where = '(' . $where . " OR $this->table_alias.$this->real_field IS NULL)";
      }
    }
    else {
      $where .= '= "***CURRENT_CART_ID***"';
    }
    $this->query->add_where($this->options['group'], $where);
  }
}
