<?php
/**
 * @file
 * Views handler: Node filter on "product-kit-ness".
 */

/**
 * Filters nodes based on whether they have an entry in the uc_product_kit table.
 */
class uc_product_kit_handler_filter_product_kit extends views_handler_filter_boolean_operator {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->query->add_field('node', 'type');
    $this->query->add_where($this->options['group'], "node.type " . (empty($this->value) ? "NOT " : "") . "IN ('product_kit')");
  }
}
