<?php

/**
 * @file
 * Views handler: Simpler "Add to cart" form as a field.
 */

/**
 * Displays the simpler Add to cart form like the catalog.
 */
class uc_product_handler_field_buyitnow extends views_handler_field {

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $product = node_load($values->{$this->aliases['nid']});
    if (uc_product_is_product($product)) {
      $form = drupal_get_form('uc_catalog_buy_it_now_form_' . $values->{$this->aliases['nid']}, $product);
      return drupal_render($form);
    }
  }
}
