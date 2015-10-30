<?php

/**
 * @file
 * Rules hooks for Ubercart attributes.
 */

/**
 * Implements hook_rules_condition_info().
 */
function uc_attribute_rules_condition_info() {
  $conditions = array();

  $conditions['uc_attribute_ordered_product_option'] = array(
    'label' => t('Order has a product with a particular attribute option'),
    'description' => t('Search the products of an order for a particular option.'),
    'group' => t('Order: Product'),
    'base' => 'uc_attribute_condition_ordered_product_option',
    'parameter' => array(
      'order' => array('type' => 'uc_order', 'label' => t('Order')),
      'option' => array('type' => 'integer', 'label' => t('Attribute option'), 'options list' => 'uc_attribute_condition_ordered_product_options_list'),
    ),
  );

  return $conditions;
}

/**
 * Returns TRUE if a product in the given order has the selected option.
 *
 * @see uc_attribute_condition_ordered_product_option_form()
 */
function uc_attribute_condition_ordered_product_option($order, $oid) {
  $option = uc_attribute_option_load($oid);
  $attribute = uc_attribute_load($option->aid);

  foreach ($order->products as $product) {
    if (!isset($product->data['attributes'])) {
      continue;
    }

    $attributes = $product->data['attributes'];

    // Once the order is made, the attribute data is changed to just the names.
    // If we can't find it by ID, check the names.
    if (is_int(key($attributes))) {
      if (isset($attributes[$oid])) {
        return TRUE;
      }
    }
    elseif (isset($attributes[$attribute->name][$oid])) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Options callback.
 *
 * @see uc_attribute_condition_ordered_product_option()
 */
function uc_attribute_condition_ordered_product_options_list() {
  $options = array();
  $result = db_query("SELECT a.aid, a.name AS attr_name, a.ordering, o.oid, o.name AS opt_name, o.ordering FROM {uc_attributes} a JOIN {uc_attribute_options} o ON a.aid = o.aid ORDER BY a.ordering, o.ordering");
  foreach ($result as $option) {
    $options[$option->attr_name][$option->oid] = $option->opt_name;
  }

  return $options;
}
