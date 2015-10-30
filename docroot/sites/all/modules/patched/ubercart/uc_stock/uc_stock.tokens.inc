<?php

/**
 * @file
 * Token hooks for the uc_stock module.
 */

/**
 * Implements hook_token_info().
 */
function uc_stock_token_info() {
  $type = array(
    'name' => t('Stock level'),
    'description' => t('Tokens for the stock levels of products.'),
    'needs-data' => 'uc_stock',
  );

  $tokens['level'] = array(
    'name' => t('Level'),
    'description' => t('The current stock level.'),
  );
  $tokens['model'] = array(
    'name' => t('Model'),
    'description' => t('The model or SKU of the stock level.'),
  );
  $tokens['threshold'] = array(
    'name' => t('Threshold'),
    'description' => t('The threshold or warning limit of the stock level.'),
  );

  return array(
    'types' => array('uc_stock' => $type),
    'tokens' => array('uc_stock' => $tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function uc_stock_tokens($type, $tokens, $data = array(), $options = array()) {
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'uc_stock' && !empty($data['uc_stock'])) {
    $object = $data['uc_stock'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'level':
          $replacements[$original] = $object->stock;
          break;

        case 'model':
          $replacements[$original] = $sanitize ? check_plain($object->sku) : $object->sku;
          break;

        case 'threshold':
          $replacements[$original] = $object->threshold;
          break;
      }
    }
  }

  return $replacements;
}
