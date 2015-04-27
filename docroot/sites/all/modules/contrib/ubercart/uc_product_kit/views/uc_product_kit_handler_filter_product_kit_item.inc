<?php
/**
 * @file
 * Views handler: Node filter on "product-kit-part-ness".
 */

/**
 * Filters nodes based on whether they are part of a product kit.
 */
class uc_product_kit_handler_filter_product_kit_item extends views_handler_filter_boolean_operator {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->query->add_field('node', 'nid');
    $this->query->add_where($this->options['group'], "node.nid " . (empty($this->value) ? "NOT " : "") . "IN (SELECT product_id FROM {uc_product_kits})");
  }
}

