<?php

/**
 * @file
 * Menu callbacks for shipping quotes requested through AJAX.
 */

/**
 * Callback to return the shipping quote(s) of the appropriate quoting method(s).
 */
function uc_quote_request_quotes() {

  /* print '<pre>';
  print_r($_POST);
  print '</pre>'; */

  $products = array();
  foreach (explode('|', urldecode($_POST['products'])) as $item) {
    $props = explode('^', $item);
    $product = new stdClass();
    $product->nid = $props[0];
    $product->title = $props[1];
    $product->model = $props[2];
    $product->qty = $props[3];
    $product->cost = $props[4];
    $product->price = $props[5];
    $product->weight = $props[6];
    if ($data = unserialize($props[7])) {
      $product->data = $data;
    }
    else {
      $product->data = $props[7];
    }
    if ($product->nid) {
      $node = (array) node_load($product->nid);
      foreach ($node as $key => $value) {
        if (!isset($product->$key)) {
          $product->$key = $value;
        }
      }
    }
    $products[] = $product;
  }
  $fake_order = new stdClass();
  $fake_order->uid = $_POST['uid'];
  if (isset($_POST['payment_method'])) {
    $fake_order->payment_method = $_POST['payment_method'];
  }
  $fake_order->products = $products;
  foreach ((array) $_POST['details'] as $type => $address) {
    foreach ($address as $key => $value) {
      if ($key == 'country' AND $value == '') {
        $value = variable_get('uc_store_country', 840);
      }
      $field = $type .'_'. $key;
      $fake_order->$field = $value;
    }
  }
  // Consider the total to be from products only, because line items are
  // mostly non-existent at this point.
  $fake_order->order_total = uc_order_get_total($fake_order, TRUE);
  // Get all quote types necessary to fulfill order.
  $quote_data = _uc_quote_assemble_quotes($fake_order);
  //drupal_set_message('<pre>'. print_r($methods, TRUE) .'</pre>');
  //drupal_set_message('<pre>'. print_r($quote_data, TRUE) .'</pre>');
  $return_quotes = array();
  foreach ($quote_data as $method_id => $options) {
    foreach ($options as $accsrl => $data) {
      $return_quotes[$method_id .'---'. $accsrl] = $data;
    }
  }
  drupal_json($return_quotes);
}

/**
 * Pulls the get_quote_from_* triggers and assembles their returned data.
 */
function _uc_quote_assemble_quotes($order) {
  global $user;
  $account = user_load($order->uid);
  if (!$account) {
    $account = $user;
  }

  $products = $order->products;
  $shipping_types = array();
  foreach ($products as $product) {
    $shipping_types[] =  uc_product_get_shipping_type($product);
  }
  $shipping_types = array_unique($shipping_types);
  $all_types = uc_quote_get_shipping_types();
  $shipping_type = '';

  // Use the most prominent shipping type (highest weight).
  // In theory, you can ship lighter products with heavier by the same
  // method, but not vice versa.
  $type_weight = -1000; // arbitrary low number
  foreach ($shipping_types as $type) {
    if ($all_types[$type]['weight'] > $type_weight) {
      $shipping_type = $all_types[$type]['id'];
      $type_weight = $all_types[$type]['weight'];
    }
  }
  $methods = array_filter(module_invoke_all('shipping_method'), '_uc_quote_method_enabled');
  uasort($methods, '_uc_quote_type_sort');
  foreach ($methods as $id => $method) {
    if ($method['quote']['type'] != 'order' && $method['quote']['type'] != $shipping_type) {
      unset($methods[$id]);
    }
  }

  $context = array(
    'revision' => 'formatted',
    'type' => 'line_item',
    'subject' => array(
      'order' => $order,
    ),
  );

  //drupal_set_message('<pre>'. print_r($products, TRUE) .'</pre>');
  $quote_data = array();
  $arguments = array(
    'order' => array(
      '#entity' => 'uc_order',
      '#title' => t('Order'),
      '#data' => $order,
    ),
    'method' => array(
      '#entity' => 'quote_method',
      '#title' => t('Quote method'),
      // #data => each $method in the following foreach() loop;
    ),
    'account' => array(
      '#entity' => 'user',
      '#title' => t('User'),
      '#data' => $account,
    ),
  );
  foreach ($methods as $method) {
    $arguments['method']['#data'] = $method;
    $predicates = ca_load_trigger_predicates('get_quote_from_'. $method['id']);
    $predicate = array_shift($predicates);
    if ($predicate && ca_evaluate_conditions($predicate, $arguments)) {
      $data = uc_quote_action_get_quote($order, $method);

      foreach ($data as &$quote) {
        if (isset($quote['rate']) && !isset($quote['format'])) {
          $context['subject']['line_item'] = array(
            'type' => 'shipping',
            'name' => $quote['option_label'],
            'amount' => $quote['rate'],
            'weight' => 1,
          );

          $quote['format'] = uc_price($quote['rate'], $context);
        }
      }
      $quote_data[$method['id']] = $data;
    }
  }
  return $quote_data;
}
