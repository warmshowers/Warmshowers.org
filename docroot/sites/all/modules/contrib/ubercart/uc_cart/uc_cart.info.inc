<?php

/**
 * @file
 * Entity metadata hooks for uc_cart.module.
 */


/**
 * Implements hook_entity_property_info().
 */
function uc_cart_entity_property_info() {
  $info = array();
  $properties = &$info['uc_cart_item']['properties'];

  $properties['cart_item_id'] = array(
    'type' => 'integer',
    'label' => t('Cart item ID'),
    'description' => t('The unique ID of the cart item.'),
    'validation callback' => 'entity_metadata_validate_integer_positive',
    'schema field' => 'cart_item_id',
  );
  $properties['cart_id'] = array(
    'type' => 'text',
    'label' => t('Cart ID'),
    'description' => t('The user ID or anonymous hash ID of the cart.'),
    'schema field' => 'cart_id',
  );
  $properties['nid'] = array(
    'type' => 'integer',
    'label' => t('Node ID'),
    'description' => t('The node ID of the cart item.'),
    'validation callback' => 'entity_metadata_validate_integer_positive',
    'schema field' => 'nid',
  );
  $properties['qty'] = array(
    'type' => 'integer',
    'label' => t('Quantity'),
    'description' => t('The number of this product in the cart.'),
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'qty',
  );
  $properties['changed'] = array(
    'type' => 'date',
    'label' => t('Changed'),
    'description' => t('The time that the product was last changed.'),
    'schema field' => 'changed',
  );
  $properties['node'] = array(
    'type' => 'node',
    'label' => t('Node'),
    'description' => t('The node representing the product.'),
    'getter callback' => 'uc_order_product_node_property_get',
  );

  return $info;
}
