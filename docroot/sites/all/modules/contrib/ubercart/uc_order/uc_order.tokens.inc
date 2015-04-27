<?php

/**
 * @file
 * Token hooks for the uc_order module.
 */

/**
 * Implements hook_token_info().
 */
function uc_order_token_info() {
  $types = array(
    'name' => t('Orders'),
    'description' => t('Tokens related to Ubercart orders.'),
    'needs-data' => 'uc_order',
  );

  $tokens = array();

  $tokens['new-username'] = array(
    'name' => t('New username'),
    'description' => t('New username associated with an order if applicable.'),
  );
  $tokens['new-password'] = array(
    'name' => t('New password'),
    'description' => t('New password associated with an order if applicable.'),
  );
  $tokens['order-number'] = array(
    'name' => t('Order number'),
    'description' => t('The unique identifier of the order.'),
  );
  $tokens['url'] = array(
    'name' => t('URL'),
    'description' => t('The URL to the order'),
    'type' => 'url',
  );
  $tokens['link'] = array(
    'name' => t('Link'),
    'description' => t('A link to the order using the order ID.'),
  );
  $tokens['admin-url'] = array(
    'name' => t('Admin URL'),
    'description' => t('The URL to the admin view page using the order ID.'),
    'type' => 'url',
  );
  $tokens['admin-link'] = array(
    'name' => t('Admin link'),
    'description' => t('A link to the order admin view page using the order ID.'),
  );
  $tokens['subtotal'] = array(
    'name' => t('Subtotal'),
    'description' => t('The subtotal of products on an order.'),
  );
  $tokens['total'] = array(
    'name' => t('Total'),
    'description' => t('The order total.'),
  );
  $tokens['email'] = array(
    'name' => t('Email'),
    'description' => t('The primary e-mail address of the order.'),
  );
  // Duplicate [uc_order:email] as [uc_order:mail] as VBO validates tokens ending in :mail.
  $tokens['mail'] = $tokens['email'];
  $tokens['shipping-method'] = array(
    'name' => t('Shipping method'),
    'description' => t('The title of the first shipping line item.'),
  );
  $tokens['shipping-address'] = array(
    'name' => t('Shipping address'),
    'description' => t('The order shipping address.'),
  );
  $tokens['shipping-phone'] = array(
    'name' => t('Shipping phone number'),
    'description' => t('The phone number for the shipping address.'),
  );
  $tokens['billing-address'] = array(
    'name' => t('Billing address'),
    'description' => t('The order billing address.'),
  );
  $tokens['billing-phone'] = array(
    'name' => t('Billing phone number'),
    'description' => t('The phone number for the billing address.'),
  );
  $tokens['first-name'] = array(
    'name' => t("Customer's first name"),
    'description' => t('The first name associated with the order.'),
  );
  $tokens['last-name'] = array(
    'name' => t("Customer's last name"),
    'description' => t('The last name associated with the order.'),
  );
  $tokens['comments'] = array(
    'name' => t('Comments'),
    'description' => t('Comments left by the customer.'),
  );
  $tokens['last-comment'] = array(
    'name' => t('Last comment'),
    'description' => t('Last order comment left by an administrator (not counting the order admin comments).'),
  );
  $tokens['order-status'] = array(
    'name' => t('Order status'),
    'description' => t('The current order status.'),
  );

  $tokens['customer'] = array(
    'name' => t('Customer'),
    'description' => t('The user associated with the order.'),
    'type' => 'user',
  );
  $tokens['created'] = array(
    'name' => t('Created'),
    'description' => t('The date and time when the order was created.'),
    'type' => 'date',
  );
  $tokens['modified'] = array(
    'name' => t('Modified'),
    'description' => t('The date and time when the order was last modified.'),
    'type' => 'date',
  );

  $tokens['products'] = array(
    'name' => t('Products'),
    'description' => t('A list of products in the order.'),
  );

  return array(
    'types' => array('uc_order' => $types),
    'tokens' => array('uc_order' => $tokens),
  );
}

/**
 * Implements hook_tokens().
 */
function uc_order_tokens($type, $tokens, $data = array(), $options = array()) {
  $language_code = NULL;
  if (isset($options['language'])) {
    $language_code = $options['language']->language;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'uc_order' && !empty($data['uc_order'])) {

    $order = $data['uc_order'];
    $path = 'user/' . $order->uid . '/orders/' . $order->order_id;
    $admin_path = 'admin/store/orders/' . $order->order_id;

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'new-username':
          if (isset($order->data['new_user']['name'])) {
            $replacements[$original] = $sanitize ? check_plain($order->data['new_user']['name']) : $order->data['new_user']['name'];
          }
          break;

        case 'new-password':
          $replacements[$original] = isset($order->password) ? $order->password : t('Your password');
          break;

        case 'order-number':
          $replacements[$original] = $order->order_id;
          break;

        case 'url':
          $replacements[$original] = url($path, array('absolute' => TRUE));
          break;

        case 'link':
          $replacements[$original] = l($order->order_id, url($path, array('absolute' => TRUE)));
          break;

        case 'admin-url':
          $replacements[$original] = url($admin_path, array('absolute' => TRUE));
          break;

        case 'admin-link':
          $replacements[$original] = l($order->order_id, url($admin_path, array('absolute' => TRUE)));
          break;

        case 'subtotal':
          $subtotal = '';
          if (is_array($order->line_items)) {
            foreach ($order->line_items as $key => $value) {
              if ($value['type'] == 'subtotal') {
                $subtotal = uc_currency_format($order->line_items[$key]['amount']);
              }
            }
          }
          $replacements[$original] = $subtotal;
          break;

        case 'subtotal-value':
          if (is_array($order->line_items)) {
            foreach ($order->line_items as $key => $value) {
              if ($value['type'] == 'subtotal') {
                $subtotal = $order->line_items[$key]['amount'];
              }
            }
          }
          $replacements[$original] = $subtotal;
          break;

        case 'total':
          $replacements[$original] = uc_currency_format($order->order_total);
          break;

        case 'total-value':
          $replacements[$original] = $order->order_total;
          break;

        case 'email':
        case 'mail':
          $replacements[$original] = $sanitize ? check_plain($order->primary_email) : $order->primary_email;
          break;

        case 'shipping-method':
          if (is_array($order->line_items)) {
            foreach ($order->line_items as $key => $value) {
              if ($value['type'] == 'shipping' && !isset($ship_method)) {
                $ship_method = $value['title'];
                break;
              }
            }
          }
          $replacements[$original] = !isset($ship_method) ? t('Standard delivery') : $ship_method;
          break;

        case 'shipping-address':
          $address = uc_order_address($order, 'delivery');
          $address = str_replace(array('<br />', '<BR />'), "<br />\n", $address);
          $replacements[$original] = $address;
          break;

        case 'shipping-phone':
          $replacements[$original] = check_plain($order->delivery_phone);
          break;

        case 'billing-address':
          $address = uc_order_address($order, 'billing');
          $address = str_replace(array('<br />', '<BR />'), "<br />\n", $address);
          $replacements[$original] = $address;
          break;

        case 'billing-phone':
          $replacements[$original] = check_plain($order->billing_phone);
          break;

        case 'first-name':
          if (variable_get('uc_customer_list_address', 'billing') == 'shipping') {
            $replacements[$original] = $sanitize ? check_plain($order->delivery_first_name) : $order->delivery_first_name;
          }
          else {
            $replacements[$original] = $sanitize ? check_plain($order->billing_first_name) : $order->billing_first_name;
          }
          break;

        case 'last-name':
          if (variable_get('uc_customer_list_address', 'billing') == 'shipping') {
            $replacements[$original] = $sanitize ? check_plain($order->delivery_last_name) : $order->delivery_last_name;
          }
          else {
            $replacements[$original] = $sanitize ? check_plain($order->billing_last_name) : $order->billing_last_name;
          }
          break;

        case 'comments':
          $result = db_query_range("SELECT message FROM {uc_order_comments} WHERE order_id = :order_id AND uid = :uid ORDER BY created DESC", 0, 1, array(':order_id' => $order->order_id, ':uid' => 0))->fetchField();
          if ($sanitize) {
            $result = check_plain($result);
          }
          $replacements[$original] = empty($result) ? t('<i>No comments left.</i>') : $result;
          break;

        case 'last-comment':
          $result = db_query_range("SELECT message FROM {uc_order_comments} WHERE order_id = :order_id AND uid > :uid ORDER BY created DESC", 0, 1, array(':order_id' => $order->order_id, ':uid' => 0))->fetchField();
          if ($sanitize) {
            $result = check_plain($result);
          }
          $replacements[$original] = empty($result) ? t('<i>No comment found.</i>') : $result;
          break;

        case 'order-status':
          $replacements[$original] = uc_order_status_data($order->order_status, 'title');
          break;

        case 'customer':
          $replacements[$original] = $order->uid;
          break;

        case 'created':
          $replacements[$original] = format_date($order->created, 'short');
          break;

        case 'modified':
          $replacements[$original] = format_date($order->modified, 'short');
          break;

        case 'products':
          $products = array();
          foreach ($order->products as $product) {
            $products[] = theme('uc_qty', array('qty' => $product->qty)) . ' ' . $product->title;
          }
          if ($sanitize) {
            $products = array_map('check_plain', $products);
          }
          $replacements[$original] = implode("<br />\n", $products);
          break;
      }
    }

    // Handles chaining for tokens that have 'type' defined in hook_token_info()
    if ($link_tokens = token_find_with_prefix($tokens, 'url')) {
      $replacements += token_generate('url', $link_tokens, array('path' => $path), $options);
    }

    if ($link_tokens = token_find_with_prefix($tokens, 'admin-url')) {
      $replacements += token_generate('url', $link_tokens, array('path' => $admin_path), $options);
    }

    if ($customer_tokens = token_find_with_prefix($tokens, 'customer')) {
      $customer = user_load($order->uid);
      $replacements += token_generate('user', $customer_tokens, array('user' => $customer), $options);
    }

    if ($created_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += token_generate('date', $created_tokens, array('date' => $order->created), $options);
    }

    if ($changed_tokens = token_find_with_prefix($tokens, 'modified')) {
      $replacements += token_generate('date', $changed_tokens, array('date' => $order->modified), $options);
    }
  }

  return $replacements;
}
