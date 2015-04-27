<?php

/**
 * @file
 * Total weight field handler.
 */

/**
 * Field handler: displays a weight multiplied by the quantity.
 */
class uc_order_handler_field_weight_total extends uc_product_handler_field_weight {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();

    $table = $this->table_alias;
    $field = $this->real_field;
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $this->field_alias = $this->query->add_field(NULL, "$table.$field * $table.qty", $this->table . '_' . $this->field, $params);

    $this->add_additional_fields();
  }

}
