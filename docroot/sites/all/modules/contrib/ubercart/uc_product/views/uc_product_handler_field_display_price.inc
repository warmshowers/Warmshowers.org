<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Returns a formatted price value to display in the View.
 */
class uc_product_handler_field_display_price extends uc_product_handler_field_price {

  /**
   * Overrides views_handler::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['label']['default'] = t('Price');

    return $options;
  }

  /**
   * Overrides views_handler_field::get_value().
   */
  function get_value($values, $field = NULL) {
    $nid = parent::get_value($values, $field);
    // !TODO Refactor so that all variants are loaded at once in the pre_render hook.
    $node = node_view(node_load($nid), 'teaser');
    return $node['display_price']['#value'];
  }

  /**
   * Overrides views_handler_field::click_sort().
   */
  function click_sort($order) {
    $params = $this->options['group_type'] != 'group' ? array('function' => $this->options['group_type']) : array();
    $this->query->add_orderby(NULL, NULL, $order, 'sell_price', $params);
  }
}
