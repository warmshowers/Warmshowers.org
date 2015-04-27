<?php

/**
 * @file
 * Rules hooks for uc_product.module.
 */

/**
 * Implements hook_rules_event_info().
 */
function uc_product_rules_event_info() {
  $events['uc_product_load'] = array(
    'label' => t('A product is being loaded'),
    'group' => t('Node'),
    'variables' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
        'skip save' => TRUE,
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_product_rules_condition_info() {
  $items['node_is_product'] = array(
    'label' => t('Content is a product'),
    'base' => 'uc_product_rules_condition_node_is_product',
    'parameter' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'group' => t('Node'),
    'access callback' => 'rules_node_integration_access',
  );

  return $items;
}

/**
 * Condition: Check for product content types
 */
function uc_product_rules_condition_node_is_product($node) {
  return in_array($node->type, uc_product_types());
}

/**
 * Provides the content types of products as asserted metadata.
 */
function uc_product_rules_condition_node_is_product_assertions($element) {
  return array('node' => array('bundle' => uc_product_types()));
}
