<?php
/**
 * Filter by order status
 * Thank you river_t for the Patch!
 */
class views_handler_filter_order_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Order Status');
      $statuses =  uc_order_status_list();
      foreach ($statuses as $status => $name) {
        $options[$name['id']] = $name['title'];
      }
      $this->value_options = $options;
    }
  }
}

