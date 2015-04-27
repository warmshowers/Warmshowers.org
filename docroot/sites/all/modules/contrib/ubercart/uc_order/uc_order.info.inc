<?php

/**
 * @file
 * Entity Metadata hooks
 */

/**
 * Implements hook_entity_property_info().
 */
function uc_order_entity_property_info() {
  $address_info = uc_address_property_info();

  return array(
    'uc_order' => array(
      'properties' => array(
        'order_id' => array(
          'type' => 'integer',
          'label' => t('Order ID'),
          'description' => t('Primary key: the order ID.'),
          'query callback' => 'entity_metadata_table_query',
        ),
        'uid' => array(
          'type' => 'integer',
          'label' => t('User ID'),
          'description' => t('The unique ID of the customer who placed the order.'),
          'clear' => array('customer'),
          'query callback' => 'entity_metadata_table_query',
          'setter permission' => 'edit orders',
        ),
        'customer' => array(
          'type' => 'user',
          'label' => t('Customer'),
          'description' => t('The user who placed the order.'),
          'getter callback' => 'uc_order_uc_order_get_properties',
          'setter callback' => 'uc_order_uc_order_set_properties',
          'setter permission' => 'edit orders',
          'clear' => array('uid'),
        ),
        'delivery_address' => array(
          'type' => 'struct',
          'label' => t('Delivery address'),
          'description' => t('The destination of the shipped products.'),
          'getter callback' => 'uc_order_address_property_get',
          'setter callback' => 'uc_order_address_property_set',
          'setter permission' => 'edit orders',
          'property info' => $address_info,
        ),
        'billing_address' => array(
          'type' => 'struct',
          'label' => t('Billing address'),
          'description' => t('The destination of the bill and invoice.'),
          'getter callback' => 'uc_order_address_property_get',
          'setter callback' => 'uc_order_address_property_set',
          'setter permission' => 'edit orders',
          'property info' => $address_info,
        ),
        'order_status' => array(
          'type' => 'text',
          'label' => t('Order status'),
          'description' => t('The status of the order.'),
          'required' => TRUE,
          'options list' => 'uc_order_status_options_list',
          'setter permission' => 'edit orders',
          'query callback' => 'entity_metadata_table_query',
        ),
        'order_total' => array(
          'type' => 'decimal',
          'label' => t('Order total'),
          'description' => t('The total amount due for the order.'),
          'getter callback' => 'uc_order_get_total',
          'query callback' => 'entity_metadata_table_query',
        ),
        'primary_email' => array(
          'type' => 'text',
          'label' => t('Primary email'),
          'description' => t('The primary email address of the customer.'),
          'setter permission' => 'edit orders',
          'query callback' => 'entity_metadata_table_query',
        ),
        'payment_method' => array(
          'type' => 'text',
          'label' => t('Payment method'),
          'description' => t('The method of payment.'),
          'query callback' => 'entity_metadata_table_query',
        ),
        'created' => array(
          'type' => 'date',
          'label' => t('Date created'),
          'description' => t('The date the order was placed.'),
          'query callback' => 'entity_metadata_table_query',
        ),
        'modified' => array(
          'type' => 'date',
          'label' => t('Date modified'),
          'description' => t('The date the order was most recently changed.'),
          'query callback' => 'entity_metadata_table_query',
        ),
        'host' => array(
          'type' => 'text',
          'label' => t('Host IP'),
          'description' => t('Host IP address of the person paying for the order.'),
          'query callback' => 'entity_metadata_table_query',
        ),
        'products' => array(
          'type' => 'list<uc_order_product>',
          'label' => t('Products'),
          'description' => t('The products that have been ordered.'),
          'setter permission' => 'edit orders',
          'getter callback' => 'uc_order_get_product_list',
          'clear' => array('order_total'),
        ),
      ),
    ),
  );
}

/**
 * Metadata controller class for uc_order_product entities.
 */
class UcOrderProductMetadataController extends EntityDefaultMetadataController {
  public function entityPropertyInfo() {
    $props = parent::entityPropertyInfo();
    // Copy the descriptions from the schema. Drupal discards this information, so we have to
    // call uc_order_schema() directly.
    module_load_include('install', 'uc_order', 'uc_order');
    $schema = uc_order_schema();
    foreach ($schema['uc_order_products']['fields'] as $name => $info) {
      if (is_array($props['uc_order_product']['properties'][$name]) && !empty($info['description'])) {
        $props['uc_order_product']['properties'][$name]['description'] = $info['description'];
      }
    }

    // Add the 'node' property.
    $props['uc_order_product']['properties']['node'] = array(
      'type' => 'node',
      'label' => t('Node'),
      'description' => t('The node representing the product.'),
      'getter callback' => 'uc_order_product_node_property_get',
      'setter callback' => 'uc_order_product_node_property_set',
    );

    return $props;
  }
}
