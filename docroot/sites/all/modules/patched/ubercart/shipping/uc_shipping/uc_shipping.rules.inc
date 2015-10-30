<?php

/**
 * @file
 * Rules hooks for uc_shipping.module.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_shipping_rules_data_info() {
  $address_info = uc_address_property_info();

  $entities['uc_shipment'] = array(
    'label' => t('Ubercart shipment object'),
    'group' => t('Ubercart'),
    'wrap' => TRUE,
    'property info' => array(
      'sid' => array(
        'type' => 'integer',
        'label' => t('Shipment ID'),
      ),
      'order_id' => array(
        'type' => 'integer',
        'label' => t('Order ID'),
      ),
      'origin' => array(
        'type' => 'struct',
        'label' => t('Origin address'),
        'description' => t('The origin location for the shipment.'),
        'getter callback' => 'uc_shipping_address_property_get',
        'setter callback' => 'uc_shipping_address_property_set',
        'setter permission' => 'fulfill orders',
        'property info' => $address_info,
      ),
      'destination' => array(
        'type' => 'struct',
        'label' => t('Destination address'),
        'description' => t('The destination location for the shipment.'),
        'getter callback' => 'uc_shipping_address_property_get',
        'setter callback' => 'uc_shipping_address_property_set',
        'setter permission' => 'fulfill orders',
        'property info' => $address_info,
      ),
      'shipping_method' => array(
        'type' => 'text',
        'label' => t('Shipping method'),
        'description' => t('The transportation method used to ship.'),
      ),
      'accessorials' => array(
        'type' => 'text',
        'label' => t('Accessorials'),
        'description' => t('Shipping options and special instructions.'),
      ),
      'carrier' => array(
        'type' => 'text',
        'label' => t('Carrier'),
        'description' > t('The company making the delivery.'),
      ),
      'transaction_id' => array(
        'type' => 'text',
        'label' => t('Transaction ID'),
        'description' => t("The carrier's shipment identifier."),
      ),
      'tracking_number' => array(
        'type' => 'text',
        'label' => t('Tracking number'),
        'description' => t('The number used by the carrier to locate the shipment while it is in transit.'),
      ),
      'ship_date' => array(
        'type' => 'date',
        'label' => t('Ship date'),
        'description' => t('The time the shipment was sent out.'),
      ),
      'expected_delivery' => array(
        'type' => 'date',
        'label' => t('Expected delivery'),
        'description' => t('The time the shipment is expected to be delivered'),
      ),
      'cost' => array(
        'type' => 'decimal',
        'label' => t('Cost'),
        'description' => t('The cost of the shipment.'),
      ),
      'packages' => array(
        'type' => 'list<struct>',
        'label' => t('Packages'),
        'description' => t('The physical items being shipped.'),
        'property info' => array(
          'package_id' => array(
            'type' => 'integer',
            'label' => t('Package ID'),
          ),
          'shipping_type' => array(
            'type' => 'text',
            'label' => t('Shipping type'),
            'description' => t('The basic type of shipment, e.g.: small package, freight, etc.'),
          ),
          'pkg_type' => array(
            'type' => 'text',
            'label' => t('Package type'),
            'description' => t('The type of packaging.'),
          ),
          'length' => array(
            'type' => 'decimal',
            'label' => t('Length'),
            'description' => t('The package length.'),
          ),
          'width' => array(
            'type' => 'decimal',
            'label' => t('Width'),
            'description' => t('The package width.'),
          ),
          'height' => array(
            'type' => 'decimal',
            'label' => t('Height'),
            'description' => t('The package height.'),
          ),
          'value' => array(
            'type' => 'decimal',
            'label' => t('Value'),
            'description' => t('The monetary value of the package contents.'),
          ),
          'tracking_number' => array(
            'type' => 'text',
            'label' => t('Tracking number'),
            'description' => t('The number used by the carrier to locate the shipment while it is in transit.'),
          ),
          'description' => array(
            'type' => 'text',
            'label' => t('Description'),
            'description' => t('The package description'),
          ),
          'weight' => array(
            'type' => 'decimal',
            'label' => t('Weight'),
            'description' => t('The physical weight of the package.'),
          ),
          'products' => array(
            'type' => 'list<uc_order_product>',
            'label' => t('Products'),
            'description' => t('The package contents.'),
          ),
          'label_image' => array(
            'type' => 'file',
            'label' => t('Label image'),
            'description' => t('An image of the shipping label.'),
          ),
        ),
      ),
    ),
  );

  return $entities;
}

/**
 * Entity metadata callback to get origin or destination address of an shipment.
 */
function uc_shipping_address_property_get($shipment, array $options, $name, $entity_type) {
  switch ($name) {
    case 'origin':
      $type = 'o_';
      break;

    case 'destination':
      $type = 'd_';
      break;

    default:
      return NULL;
  }

  $address = new UcAddress();

  foreach ($address as $field => $value) {
    $address->{$field} = $shipment->{$type . $field};
  }

  return $address;
}

/**
 * Entity metadata callback to set origin or destination address of an order.
 */
function uc_shipping_address_property_set($shipment, $name, $address) {
  switch ($name) {
    case 'origin':
      $type = 'o_';
      break;

    case 'destination':
      $type = 'd_';
      break;

    default:
      return;
  }

  foreach ($address as $field => $value) {
    $shipment->{$type . $field} = $value;
  }
}

/**
 * Implements hook_rules_event_info().
 */
function uc_shipping_rules_event_info() {
  $events['uc_shipment_save'] = array(
    'label' => t('A shipment is saved'),
    'group' => t('Fulfillment'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'shipment' => array(
        'type' => 'uc_shipment',
        'label' => t('Shipment'),
      ),
    ),
  );

  return $events;
}
