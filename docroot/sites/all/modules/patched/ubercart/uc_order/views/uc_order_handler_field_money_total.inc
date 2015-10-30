<?php

/**
 * @file
 * Total price field handler.
 */

/**
 * Field handler: displays a price multiplied by the quantity.
 */
class uc_order_handler_field_money_total extends uc_order_handler_field_money_amount {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();

    $table = $this->table_alias;
    $field = $this->real_field;
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $this->field_alias = $this->query->add_field(NULL, "$table.$field * $table.qty", $this->table . '_' . $this->field, $params);
  }

}
