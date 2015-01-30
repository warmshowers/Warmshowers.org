<?php

/**
 * @file
 * Conditional Actions file for Ubercart attributes.
 */

/**
 * Implements hook_ca_condition().
 */
function uc_attribute_ca_condition() {
  $conditions = array();

  $conditions['uc_attribute_ordered_product_option'] = array(
    '#title' => t('Order has a product with a particular attribute option'),
    '#description' => t('Search the products of an order for a particular option.'),
    '#category' => t('Order: Product'),
    '#callback' => 'uc_attribute_condition_ordered_product_option',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );

  return $conditions;
}

/**
 * Return TRUE if a product in the given order has the selected option.
 *
 * @see uc_attribute_condition_ordered_product_option_form()
 */
function uc_attribute_condition_ordered_product_option($order, $settings) {
  foreach ($order->products as $product) {
    if (empty($product->data['attributes'])) {
      continue;
    }

    $attributes = $product->data['attributes'];

    // Once the order is made, the attribute data is changed to just the names.
    // If we can't find it by ID, check the names.
    if (is_int(array_shift(array_keys($attributes)))) {
      foreach ($attributes as $options) {
        // Only checkboxes are stored in an array, so fix up other option types.
        if (!is_array($options)) {
          $options = array($options);
        }

        if (in_array($settings['attribute_option'], $options)) {
          return TRUE;
        }
      }
    }
    else {
      // Load the attribute data once, only if we need it.
      if (!isset($option)) {
        if ($option = uc_attribute_option_load($settings['attribute_option'])) {
          $attribute = uc_attribute_load($option->aid);
        }
      }

      if (isset($attribute) && isset($attributes[$attribute->name])) {
        $options = $attributes[$attribute->name];

        // Orders created before checkbox options were introduced are not stored in an array.
        if (!is_array($options)) {
          $options = array($options);
        }

        if (in_array($option->name, $options)) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
 * @see uc_attribute_condition_ordered_product_option()
 */
function uc_attribute_condition_ordered_product_option_form($form_state, $settings = array()) {
  $form = array();

  $options = array();
  $result = db_query("SELECT a.aid, a.name AS attr_name, a.ordering, o.oid, o.name AS opt_name, o.ordering FROM {uc_attributes} AS a JOIN {uc_attribute_options} AS o ON a.aid = o.aid ORDER BY a.ordering, o.ordering");
  while ($option = db_fetch_object($result)) {
    $options[$option->attr_name][$option->oid] = $option->opt_name;
  }

  $form['attribute_option'] = array(
    '#type' => 'select',
    '#title' => t('Attribute option'),
    '#default_value' => $settings['attribute_option'],
    '#options' => $options,
  );

  return $form;
}
