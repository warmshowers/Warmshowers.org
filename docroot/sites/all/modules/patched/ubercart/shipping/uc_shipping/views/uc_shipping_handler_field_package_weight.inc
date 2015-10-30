<?php

/**
 * @file
 * Total package weight field handler.
 */

/**
 * Field handler: displays the weight of the package.
 *
 * We cannot use a subquery because there is no way to make sure that all products
 * in packages have the same weight unit.
 */
class uc_shipping_handler_field_package_weight extends uc_product_handler_field_weight {
  /**
   * Overrides views_handler::use_group_by().
   *
   * Disables aggregation for this field.
   */
  function use_group_by() {
    return FALSE;
  }

  /**
   * Overrides uc_product_handler_field_weight::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides uc_product_handler_field_weight::render().
   */
  function render($values) {
    $package = uc_shipping_package_load($values->{$this->aliases['package_id']});

    if ($this->options['format'] == 'numeric') {
      return $package->weight;
    }

    if ($this->options['format'] == 'uc_weight') {
      return uc_weight_format($package->weight, $package->weight_units);
    }
  }
}
