<?php

/**
 * @file
 * Token hooks for the uc_shipping module.
 */


/**
 * Implements hook_token_info().
 */
function uc_shipping_token_info() {
  $tokens = array();

  $tokens['tracking-number'] = array(
    'name' => t('Shipment tracking number(s)'),
    'description' => t('Tracking number(s) (if applicable) for product shipments.'),
  );

  return array(
    'tokens' => array('uc_order' => $tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function uc_shipping_tokens($type, $tokens, $data = array(), $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);

  if ($type == 'uc_order' && !empty($data['uc_order'])) {
    $order = $data['uc_order'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'tracking-number':
          $result = db_query('SELECT tracking_number FROM {uc_shipments} WHERE order_id = :order_id', array(':order_id' => $order->order_id));
          $tracking_numbers = array();
          foreach ($result as $record) {
            if ((isset($record->tracking_number)) && (!empty($record->tracking_number))) {
              $tracking_numbers[] = $record->tracking_number;
            }
          }
          $tracking = implode(', ', $tracking_numbers);
          $replacements[$original] = $sanitize ? check_plain($tracking) : $tracking;
          break;
      }
    }
  }

  return $replacements;
}
