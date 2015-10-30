<?php

/**
 * @file
 * Order total cost field handler.
 */

/**
 * Field handler: displays the total cost of an order.
 */
class uc_order_handler_field_order_cost extends uc_order_handler_field_money_amount {

  /**
   * Constructor.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['order_id'] = 'order_id';
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    // Do nothing else with the query, we'll be retrieving the information directly.
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $this->field_alias = 'order_cost';
    $cost = db_query("SELECT SUM(cost * qty) FROM {uc_order_products} WHERE order_id = :oid", array(':oid' => $values->{$this->aliases['order_id']}))->fetchField();
    $values->{$this->field_alias} = $cost;
    return parent::render($values);
  }

}
