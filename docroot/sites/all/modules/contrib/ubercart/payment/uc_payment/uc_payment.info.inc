<?php

/**
 * @file
 * Entity Metadata hooks
 */

/**
 * Implements hook_entity_property_info().
 */
function uc_payment_entity_property_info() {
  return array(
    'uc_order' => array(
      'properties' => array(
        'payment_balance' => array(
          'type' => 'decimal',
          'label' => t('Balance'),
          'description' => t('The total amount remaining for the order.'),
          'getter callback' => 'uc_payment_balance',
          'query callback' => 'entity_metadata_table_query',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_entity_property_info_alter().
 */
function uc_payment_entity_property_info_alter(&$info) {
  $info['uc_order']['properties']['payment_method']['options list'] = 'uc_payment_method_options_list';
}
