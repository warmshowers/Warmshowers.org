<?php

/**
 * @file
 * Views handler: Filter on comparison of stock level to threshold.
 */

/**
 * Filters nodes based on comparison of stock value to stock threshold.
 */
class uc_stock_handler_filter_below_threshold extends views_handler_filter_boolean_operator {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], "$this->table_alias.stock " . (empty($this->value) ? '>=' : '<') . " $this->table_alias.threshold");
  }
}
