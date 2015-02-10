<?php

/**
 * @file
 * Order balance field handler
 */

class uc_views_handler_field_order_balance extends uc_views_handler_field_money_amount {
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $order->order_id = $values->{$this->aliases['order_id']};
    $order->order_total = $values->{$this->aliases['order_total']};
    $values->{$this->field_alias} = uc_payment_balance($order);
    return parent::render($values);
  }
}
