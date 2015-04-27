<?php

/**
 * @file
 * Contains the callbacks for the default line items for orders and
 * the various functions that make line items work.
 *
 * Line items are defined using hook_uc_line_item() and use a callback to
 * handle the different processes involved in line item
 * viewing/editing/calculating. The default line items are defined in
 * uc_order_uc_line_item() in uc_order.module.
 */

/**
 * Handles the subtotal line item.
 */
function uc_line_item_subtotal($op, $order) {
  switch ($op) {
    case 'load':
      $lines[] = array(
        'id' => 'subtotal',
        'title' => t('Subtotal'),
        'amount' => uc_order_get_total($order, TRUE),
      );
      return $lines;
  }
}

/**
 * Handles the total line item.
 */
function uc_line_item_total($op, $order) {
  switch ($op) {
    case 'display':
      $lines[] = array(
        'id' => 'total',
        'title' => t('Order total'),
        'amount' => uc_order_get_total($order),
      );
      return $lines;
  }
}

/**
 * Calculates the total value of line items of types that should be calculated.
 */
function uc_line_items_calculate($order) {
  $total = 0;

  if (is_array($order->line_items)) {
    foreach ($order->line_items as $item) {
      if (_uc_line_item_data($item['type'], 'calculated') == TRUE) {
        $total += $item['amount'];
      }
    }
  }

  return $total;
}

/**
 * Updates a line item.
 */
function uc_order_update_line_item($id, $title, $amount, $data = NULL) {
  $fields = array(
    'title' => $title,
    'amount' => $amount,
  );

  if (!is_null($data)) {
    $fields['data'] = serialize($data);
  }

  db_update('uc_order_line_items')
    ->fields($fields)
    ->condition('line_item_id', $id)
    ->execute();

  return TRUE;
}

/**
 * Deletes a specific line item, or every line item in an order.
 *
 * @param $id
 *   The line item ID, or order ID.
 * @param $order_id
 *   If FALSE, deletes the line item with the specified ID (default).
 *   If TRUE, deletes all line items on the order with the specified ID.
 */
function uc_order_delete_line_item($id, $order = FALSE) {
  if ($order === FALSE) {
    db_delete('uc_order_line_items')
      ->condition('line_item_id', $id)
      ->execute();
  }
  else {
    db_delete('uc_order_line_items')
      ->condition('order_id', $id)
      ->execute();
  }

  return TRUE;
}

/**
 * Adds a line item to an order.
 */
function uc_order_line_item_add($order_id, $type, $title, $amount, $weight = NULL, $data = NULL) {
  if (is_null($weight)) {
    $weight = _uc_line_item_data($type, 'weight');
  }

  $line_item = array(
    'order_id' => $order_id,
    'type' => $type,
    'title' => $title,
    'amount' => $amount,
    'weight' => $weight,
    'data' => $data,
  );
  drupal_write_record('uc_order_line_items', $line_item);

  return $line_item;
}

/**
 * Builds a list of line items defined in the enabled modules.
 */
function _uc_line_item_list($action = NULL) {
  static $items;

  if (count($items) > 0 && $action !== 'rebuild') {
    return $items;
  }

  $items = array();
  foreach (module_invoke_all('uc_line_item') as $id => $item) {
    // Preserve backward compatibility for methods with no key specified.
    if (is_numeric($id)) {
      $id = $item['id'];
    }

    // Set defaults.
    $item += array(
      'id' => $id,
      'enabled' => TRUE,
      'weight' => 1,
    );

    // Merge in any settings.
    $items[$id] = array_merge($item, array(
      'enabled' => variable_get('uc_li_' . $id . '_enabled', $item['enabled']),
      'weight' => variable_get('uc_li_' . $id . '_weight', $item['weight']),
    ));
  }

  drupal_alter('uc_line_item_data', $items);

  uasort($items, 'uc_weight_sort');

  return $items;
}

/**
 * Returns data from a line item by ID and the array key.
 */
function _uc_line_item_data($item_id, $key) {
  $items = _uc_line_item_list();
  return isset($items[$item_id][$key]) ? $items[$item_id][$key] : NULL;
}
