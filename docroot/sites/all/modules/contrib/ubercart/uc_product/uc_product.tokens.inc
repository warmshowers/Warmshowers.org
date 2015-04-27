<?php

/**
 * @file
 * Token hooks for the uc_product module.
 */

/**
 * Implements hook_token_info().
 */
function uc_product_token_info() {
  $node['product'] = array(
    'name' => t('Product'),
    'description' => t('Ubercart product data of the node.'),
    'type' => 'uc_product',
  );

  $types['uc_product'] = array(
    'name' => t('Product'),
    'description' => t('Tokens for Ubercart products.'),
    'needs-data' => 'node',
  );
  $product['model'] = array(
    'name' => t('SKU / Model'),
    'description' => t("The product's SKU or model number."),
  );
  $product['list-price'] = array(
    'name' => t('List price'),
    'description' => t("The product's list price."),
    'type' => 'uc_price',
  );
  $product['cost'] = array(
    'name' => t('Cost'),
    'description' => t("The product's cost."),
    'type' => 'uc_price',
  );
  $product['sell-price'] = array(
    'name' => t('Sell price'),
    'description' => t("The product's sell price."),
    'type' => 'uc_price',
  );
  $product['weight'] = array(
    'name' => t('Weight'),
    'description' => t("The product's weight."),
    'type' => 'uc_weight',
  );
  $product['length'] = array(
    'name' => t('Length'),
    'description' => t("The product's length."),
    'type' => 'uc_dimension',
  );
  $product['width'] = array(
    'name' => t('Width'),
    'description' => t("The product's width."),
    'type' => 'uc_dimension',
  );
  $product['height'] = array(
    'name' => t('Height'),
    'description' => t("The product's height."),
    'type' => 'uc_dimension',
  );

  $types['uc_price'] = array(
    'name' => t('Price'),
    'description' => 'Tokens for price formats.',
    'needs-data' => 'uc_price',
  );
  $price['formatted'] = array(
    'name' => t('Formatted'),
    'description' => t('A formatted price.'),
  );
  $price['raw'] = array(
    'name' => t('Raw value'),
    'description' => t('A decimal price.'),
  );

  $types['uc_weight'] = array(
    'name' => t('Weight'),
    'description' => 'Tokens for weight formats.',
    'needs-data' => 'uc_weight',
  );
  $weight['formatted'] = array(
    'name' => t('Formatted'),
    'description' => t('A formatted weight.'),
  );
  $weight['raw'] = array(
    'name' => t('Raw value'),
    'description' => t('A decimal weight.'),
  );
  $weight['units'] = array(
    'name' => t('Units'),
    'description' => t('The unit of measurement.'),
  );

  $types['uc_dimension'] = array(
    'name' => t('Dimension'),
    'description' => 'Tokens for length, width, and height.',
    'needs-data' => 'uc_dimension',
  );
  $dimension['formatted'] = array(
    'name' => t('Formatted'),
    'description' => t('A formatted dimension.'),
  );
  $dimension['raw'] = array(
    'name' => t('Raw value'),
    'description' => t('A decimal dimension.'),
  );
  $dimension['units'] = array(
    'name' => t('Units'),
    'description' => t('The unit of measurement.'),
  );

  return array(
    'types' => $types,
    'tokens' => array(
      'node' => $node,
      'uc_product' => $product,
      'uc_price' => $price,
      'uc_weight' => $weight,
      'uc_dimension' => $dimension,
    ),
  );
}

/**
 * Implements hook_token_info_alter().
 *
 * Remove unwanted entity tokens.
 */
function uc_product_token_info_alter(&$info) {
  $tokens = array(
    'model', 'list-price', 'cost', 'sell-price', 'weight', 'weight-units',
    'length', 'width', 'height', 'length_units', 'pkg-qty', 'ordering', 'shippable',
  );

  foreach ($tokens as $token) {
    if (!empty($info['tokens']['node'][$token]['entity-token'])) {
      unset($info['tokens']['node'][$token]);
    }
  }
}

/**
 * Implements hook_tokens().
 */
function uc_product_tokens($type, $tokens, $data = array(), $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $replacements = array();

  if ($type == 'node' && !empty($data['node']) && uc_product_is_product($data['node'])) {
    $product = &$data['node'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'product:model':
          $replacements[$original] = $sanitize ? check_plain($product->model) : $product->model;
          break;

        case 'product:list-price':
        case 'product:list-price:formatted':
          $replacements[$original] = uc_currency_format($product->list_price);
          break;

        case 'product:cost':
        case 'product:cost:formatted':
          $replacements[$original] = uc_currency_format($product->cost);
          break;

        case 'product:sell-price':
        case 'product:sell-price:formatted':
          $replacements[$original] = uc_currency_format($product->sell_price);
          break;

        case 'product:list-price:raw':
          $replacements[$original] = $product->list_price;
          break;

        case 'product:cost:raw':
          $replacements[$original] = $product->cost;
          break;

        case 'product:sell-price:raw':
          $replacements[$original] = $product->sell_price;
          break;

        case 'product:weight':
        case 'product:weight:formatted':
          $replacements[$original] = uc_weight_format($product->weight, $product->weight_units);
          break;

        case 'product:weight:raw':
          $replacements[$original] = $product->weight;
          break;

        case 'product:weight:units':
          $replacements[$original] = $product->weight_units;
          break;

        case 'product:length':
        case 'product:length:formatted':
          $replacements[$original] = uc_length_format($product->length, $product->length_units);
          break;

        case 'product:width':
        case 'product:width:formatted':
          $replacements[$original] = uc_length_format($product->width, $product->length_units);
          break;

        case 'product:height':
        case 'product:height:formatted':
          $replacements[$original] = uc_length_format($product->height, $product->length_units);
          break;

        case 'product:length:raw':
          $replacements[$original] = $product->length;
          break;

        case 'product:width:raw':
          $replacements[$original] = $product->width;
          break;

        case 'product:height:raw':
          $replacements[$original] = $product->height;
          break;

        case 'product:length:units':
        case 'product:width:units':
        case 'product:height:units':
          $replacements[$original] = $product->length_units;
          break;
      }
    }

    return $replacements;
  }
}
