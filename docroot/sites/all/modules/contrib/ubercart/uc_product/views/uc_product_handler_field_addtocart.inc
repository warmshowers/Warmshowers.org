<?php

/**
 * @file
 * Views handler: "Add to cart" form as a field.
 */

/**
 * Displays the "Add to cart" form like the product page.
 */
class uc_product_handler_field_addtocart extends views_handler_field {

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
    if (uc_product_is_product($values->{$this->aliases['type']})) {
      $type = node_type_get_type($values->{$this->aliases['type']});
      $base = $type->base;
      $product = node_load($values->{$this->aliases['nid']});
      if (function_exists($base . '_add_to_cart_form')) {
        $form = drupal_get_form($base . '_add_to_cart_form_' . $values->{$this->aliases['nid']}, $product);
        return drupal_render($form);
      }
    }
  }
}
