<?php

/**
 * @file
 * This file contains the Rules hooks and functions necessary to make the order
 * related entity, conditions, events, and actions work.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_order_rules_data_info() {
  $types['uc_order'] = array(
    'label' => t('Ubercart order object'),
    'wrap' => TRUE,
    'group' => t('Ubercart'),
  );
  $types['uc_order_product'] = array(
    'label' => t('Ubercart ordered product'),
    'wrap' => TRUE,
    'parent' => 'node',
    'group' => t('Ubercart'),
  );

  $types['uc_line_item'] = array(
    'label' => t('Order line item'),
    'wrap' => TRUE,
    'group' => t('Ubercart'),
    'token type' => FALSE,
  );

  return $types;
}

/**
 * Implements hook_rules_event_info().
 */
function uc_order_rules_event_info() {
  $events['uc_order_status_update'] = array(
    'label' => t('Order status gets updated'),
    'group' => t('Order'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Original order'),
      ),
      'updated_order' => array(
        'type' => 'uc_order',
        'label' => t('Updated order'),
      ),
    ),
  );

  $events['uc_order_status_email_update'] = array(
    'label' => t('E-mail requested for order status update'),
    'group' => t('Order'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
  );

  $events['uc_order_delete'] = array(
    'label' => t('An order is being deleted'),
    'group' => t('Order'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_order_rules_condition_info() {
  $conditions['uc_order_condition_order_state'] = array(
    'label' => t("Check an order's state"),
    'group' => t('Order'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'order_state' => array(
        'type' => 'text',
        'label' => t('Order state'),
        'options list' => 'uc_order_condition_order_state_options',
        'restriction' => 'input',
      ),
    ),
  );

  $conditions['uc_order_condition_delivery_country'] = array(
    'label' => t("Check an order's shipping country"),
    'group' => t('Order'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'countries' => array(
        'type' => 'list<integer>',
        'label' => t('Countries'),
        'options list' => 'uc_country_option_list',
        'restriction' => 'input',
      ),
    ),
  );

  $conditions['uc_order_condition_billing_country'] = array(
    'label' => t("Check an order's billing country"),
    'group' => t('Order'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'countries' => array(
        'type' => 'list<integer>',
        'label' => t('Countries'),
        'options list' => 'uc_country_option_list',
        'restriction' => 'input',
      ),
    ),
  );

  $conditions['uc_order_condition_has_products'] = array(
    'label' => t("Check an order's products"),
    'group' => t('Order: Product'),
    'base' => 'uc_order_condition_has_products',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'products' => array(
        'type' => 'list<text>',
        'label' => t('Products'),
        'options list' => 'uc_order_condition_has_products_options',
        'restriction' => 'input',
      ),
      'required' => array(
        'type' => 'boolean',
        'label' => t('Require all selected products'),
        'description' => t('Select to require that order must contain all selected products.  Otherwise, order must contain at least one of the selected products.'),
      ),
      'forbidden' => array(
        'type' => 'boolean',
        'label' => t('Forbid other products'),
      ),
    ),
  );

  $conditions['uc_order_condition_count_products'] = array(
    'label' => t("Check an order's number of products"),
    'group' => t('Order: Product'),
    'base' => 'uc_order_condition_count_products',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'products' => array(
        'type' => 'list<integer>',
        'label' => t('Products'),
        'options list' => 'uc_order_condition_products_options',
        'restriction' => 'input',
      ),
      'product_count_value' => array(
        'type' => 'integer',
        'label' => t('Product count value'),
      ),
      'product_count_comparison' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'uc_order_condition_value_operator_options',
      ),
    ),
  );
  $conditions['uc_order_condition_products_weight'] = array(
    'label' => t("Check an order's total weight"),
    'group' => t('Order: Product'),
    'help' => t('Compare the weight of all of the products, or the weight of just one type in the order.'),
    'base' => 'uc_order_condition_products_weight',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'products' => array(
        'type' => 'list<integer>',
        'label' => t('Products'),
        'options list' => 'uc_order_condition_products_options',
        'restriction' => 'input',
      ),
      'weight_units' => array(
        'type' => 'text',
        'label' => t('Unit of measurement'),
        'options list' => 'uc_order_condition_weight_units_options',
      ),
      'product_weight_value' => array(
        'type' => 'decimal',
        'label' => t('Product weight value'),
      ),
      'product_weight_comparison' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'uc_order_condition_value_operator_options',
      ),
    ),
  );
  $conditions['uc_order_condition_is_shippable'] = array(
    'label' => t('Check if an order can be shipped'),
    'group' => t('Order'),
    'base' => 'uc_order_condition_is_shippable',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
  );

  $conditions['uc_order_condition_has_product_class'] = array(
    'label' => t("Check an order's product classes"),
    'group' => t('Order: Product'),
    'base' => 'uc_order_condition_has_product_class',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'product_classes' => array(
        'type' => 'list<text>',
        'label' => t('Product Classes'),
        'options list' => 'uc_order_condition_has_product_class_classes_options',
        'restriction' => 'input',
      ),
      'required' => array(
        'type' => 'boolean',
        'label' => t('Require all selected product classes'),
        'description' => t('Select to require that order must contain all selected product classes.  Otherwise, order must contain at least one of the selected product classes.'),
      ),
      'forbidden' => array(
        'type' => 'boolean',
        'label' => t('Forbid other product classes'),
      ),
    ),
  );

  $conditions['uc_order_condition_total'] = array(
    'label' => t("Check an order's total"),
    'group' => t('Order'),
    'base' => 'uc_order_condition_total',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator.'),
        'default value' => '==',
        'options list' => 'uc_order_condition_value_operator_options',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Data value'),
        'description' => t('The value to compare the data with.'),
      ),
    ),
  );

  $conditions['uc_order_condition_subtotal'] = array(
    'label' => t("Check an order's subtotal"),
    'group' => t('Order'),
    'base' => 'uc_order_condition_subtotal',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'op' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The comparison operator.'),
        'default value' => '==',
        'options list' => 'uc_order_condition_value_operator_options',
        'restriction' => 'input',
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Data value'),
        'description' => t('The value to compare the data with.'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function uc_order_rules_action_info() {
  $order_arg = array(
    'type' => 'uc_order',
    'label' => t('Order'),
  );

  $actions['uc_order_update_status'] = array(
    'label' => t('Update the order status'),
    'group' => t('Order'),
    'base' => 'uc_order_action_update_status',
    'parameter' => array(
      'order' => $order_arg,
      'order_status' => array(
        'type' => 'text',
        'label' => t('Status'),
        'options list' => 'uc_order_action_update_status_options',
      ),
    ),
  );
  $actions['uc_order_action_add_comment'] = array(
    'label' => t('Add a comment to the order'),
    'group' => t('Order'),
    'base' => 'uc_order_action_add_comment',
    'parameter' => array(
      'order' => $order_arg,
      'comment' => array(
        'type' => 'text',
        'label' => t('Comment'),
      ),
      'comment_type' => array(
        'type' => 'text',
        'label' => t('Comment type'),
        'restriction' => 'input',
        'options list' => 'uc_order_action_order_comment_types',
      ),
    ),
  );
  $actions['uc_order_email'] = array(
    'label' => t('Send an order email'),
    'group' => t('Order'),
    'base' => 'uc_order_action_email',
    'parameter' => array(
      'order' => $order_arg,
      'from' => array(
        'type' => 'text',
        'label' => t('Sender'),
        'description' => t("Enter the 'From' email address, or leave blank to use your store email address. You may use order tokens for dynamic email addresses."),
        'optional' => TRUE,
      ),
      'addresses' => array(
        'type' => 'text',
        'label' => t('Recipients'),
        'description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
        'translatable' => TRUE,
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'translatable' => TRUE,
      ),
      'format' => array(
        'type' => 'text',
        'label' => t('Message format'),
        'options list' => 'uc_order_message_formats',
      ),
    ),
  );
  $actions['uc_order_email_invoice'] = array(
    'label' => t('Email an order invoice'),
    'group' => t('Order'),
    'base' => 'uc_order_action_email_invoice',
    'parameter' => array(
      'order' => $order_arg,
      'from' => array(
        'type' => 'text',
        'label' => t('Sender'),
        'description' => t("Enter the 'From' email address, or leave blank to use your store email address. You may use order tokens for dynamic email addresses."),
        'optional' => TRUE,
      ),
      'addresses' => array(
        'type' => 'text',
        'label' => t('Recipients'),
        'description' => t('Enter the email addresses to receive the invoice, one on each line. You may use order tokens for dynamic email addresses.'),
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
        'translatable' => TRUE,
      ),
      'template' => array(
        'type' => 'text',
        'label' => t('Invoice template'),
        'options list' => 'uc_order_template_options',
        'restriction' => 'input',
      ),
      'view' => array(
        'type' => 'text',
        'label' => t('Included information'),
        'options list' => 'uc_order_action_email_invoice_view_options',
        'restriction' => 'input',
      ),
    ),
  );

  return $actions;
}

/**
 * Checks the current order state.
 */
function uc_order_condition_order_state($order, $order_state) {
  return uc_order_status_data($order->order_status, 'state') == $order_state;
}

/**
 * Options callback.
 *
 * @see uc_order_condition_order_state()
 */
function uc_order_condition_order_state_options() {
  foreach (uc_order_state_list('general') as $id => $state) {
    $options[$id] = $state['title'];
  }
  foreach (uc_order_state_list('specific') as $id => $state) {
    $options[$id] = $state['title'];
  }

  return $options;
}

/**
 * Checks that the order has one of the selected delivery countries.
 */
function uc_order_condition_delivery_country($order, $countries) {
  return in_array($order->delivery_country, $countries);
}

/**
 * Checks that the order has one of the selected billing countries.
 */
function uc_order_condition_billing_country($order, $countries) {
  return in_array($order->billing_country, $countries);
}

/**
 * Checks that the order has the selected combination of product classes.
 *
 * @param object $order
 *   The order to check.
 * @param array $product_classes
 *   An array of strings containing the product classes (node content
 *   types) to check against.
 * @param boolean $required
 *   TRUE to require all product classes be present in the order.  FALSE
 *   to require at least one be present.
 * @param boolean $forbidden
 *   TRUE to require that only the listed product classes be present.  FALSE
 *   to allow products with other classes.
 *
 * @return boolean
 *   Whether the order meets the specified conditions.
 */
function uc_order_condition_has_product_class($order, $product_classes, $required, $forbidden) {
  $order_product_classes = array();
  foreach ($order->products as $product) {
    if (!empty($product->type)) {
      // If present, use the product type from {uc_order_products}.data.type.
      $order_product_classes[] = $product->type;
    }
    else {
      // Otherwise, use the node type.  If the node can't be loaded, ignore
      // this product.
      $node = node_load($product->nid);
      if (!empty($node)) {
        $order_product_classes[] = $node->type;
      }
    }
  }
  $required_product_classes = array_intersect($product_classes, $order_product_classes);
  if ($required) {
    $required_check = ($required_product_classes == $product_classes);
  }
  else {
    $required_check = (bool) count($required_product_classes);
  }
  if ($forbidden) {
    $forbidden_product_classes = array_diff($order_product_classes, $product_classes);
    $forbidden_check = (bool) count($forbidden_product_classes);
  }
  else {
    $forbidden_check = FALSE;
  }

  return $required_check && !$forbidden_check;
}

/**
 * Options callback.
 *
 * @return array
 *   Associative array of all Ubercart product classes indexed by class ID.
 *
 * @see uc_order_condition_has_product_class()
 */
function uc_order_condition_has_product_class_classes_options() {
  $options = array();

  $result = db_query('SELECT * FROM {uc_product_classes}');
  foreach ($result as $class) {
    $options += array($class->pcid => $class->name);
  }

  return $options;
}

/**
 * Checks that the order has the selected combination of products.
 */
function uc_order_condition_has_products($order, $products, $required, $forbidden) {
  $order_products = array();
  foreach ($order->products as $product) {
    $order_products[] = $product->model;
  }
  $required_products = array_intersect($products, $order_products);
  if ($required) {
    $required_check = $required_products == $products;
  }
  else {
    $required_check = (bool)count($required_products);
  }
  if ($forbidden) {
    $forbidden_products = array_diff($order_products, $products);
    $forbidden_check = (bool)count($forbidden_products);
  }
  else {
    $forbidden_check = FALSE;
  }
  return $required_check && !$forbidden_check;
}

/**
 * Options callback.
 *
 * @see uc_order_condition_has_products()
 */
function uc_order_condition_has_products_options() {
  $options = array();
  $result = db_query('SELECT nid FROM {uc_products}');
  foreach ($result as $row) {
    $options += uc_product_get_models($row->nid, FALSE);
  }
  asort($options);

  return $options;
}

/**
 * Checks that the order has the selected number of products.
 *
 * @see uc_order_condition_count_products_form()
 */
function uc_order_condition_count_products($order, $products, $count, $op) {
  $totals = array('all' => 0);
  $total = 0;
  foreach ($order->products as $product) {
    $totals['all'] += $product->qty;
    if (isset($totals[$product->nid])) {
      $totals[$product->nid] += $product->qty;
    }
    else {
      $totals[$product->nid] = $product->qty;
    }
  }
  if (in_array('all', $products)) {
    $total = $totals['all'];
  }
  else {
    foreach ($products as $product) {
      if (isset($totals[$product])) {
        $total += $totals[$product];
      }
    }
  }
  return uc_order_condition_value_operator_comparison($total, $op, $count);
}

/**
 * Product options callback.
 */
function uc_order_condition_products_options() {
  $options = array('all' => t('- All products -'));
  $options += db_query('SELECT nid, model FROM {uc_products} ORDER BY model')->fetchAllKeyed();

  return $options;
}

/**
 * Operator options callback.
 */
function uc_order_condition_value_operator_options() {
  return array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
}

/**
 * Checks the weight of the order's products.
 *
 * @see uc_order_condition_products_weight_form()
 */
function uc_order_condition_products_weight($order, $products, $weight_units, $weight_value, $op) {
  $totals = array('all' => 0);
  $total = 0;
  foreach ($order->products as $product) {
    $unit_conversion = uc_weight_conversion($product->weight_units, $weight_units);
    $totals['all'] += $product->qty * $product->weight * $unit_conversion;
    $totals[$product->nid] = $product->qty * $product->weight * $unit_conversion;
  }
  if (in_array('all', $products)) {
    $total = $totals['all'];
  }
  else {
    foreach ($products as $product) {
      if (isset($totals[$product])) {
        $total += $totals[$product];
      }
    }
  }
  return uc_order_condition_value_operator_comparison($total, $op, $weight_value);
}

/**
 * Weight units options callback.
 */
function uc_order_condition_weight_units_options() {
  return array(
    'lb' => t('Pounds'),
    'kg' => t('Kilograms'),
    'oz' => t('Ounces'),
    'g' => t('Grams'),
  );
}

/**
 * Checks that the order is shippable.
 */
function uc_order_condition_is_shippable($order, $settings) {
  return uc_order_is_shippable($order);
}

/**
 * Updates an order's status.
 *
 * @see uc_order_action_update_status_form()
 */
function uc_order_action_update_status($order, $status) {
  if (uc_order_update_status($order->order_id, $status)) {
    $order->order_status = $status;
  }
}

/**
 * @see uc_order_action_update_status()
 */
function uc_order_action_update_status_options() {
  $options = array();

  foreach (uc_order_status_list('general') as $status) {
    $options[$status['id']] = $status['title'];
  }
  foreach (uc_order_status_list('specific') as $status) {
    $options[$status['id']] = $status['title'];
  }

  return $options;
}

/**
 * Adds a comment to an order.
 *
 * @see uc_order_action_add_comment_form()
 */
function uc_order_action_add_comment($order, $comment, $comment_type) {
  uc_order_comment_save($order->order_id, 0,
    token_replace($comment, array('uc_order' => $order)),
    $comment_type == 'admin' ? 'admin' : 'order',
    $order->order_status, $comment_type == 'notified');
}

/**
 * @see uc_order_action_add_comment()
 */
function uc_order_action_order_comment_types() {
  return array(
    'admin' => t('Enter this as an admin comment.'),
    'order' => t('Enter this as a customer order comment.'),
    'notified' => t('Enter this as a customer order comment with a notified icon.'),
  );
}

/**
 * Sends an email concerning an order.
 *
 * The 'Sender', 'Recipients', 'Subject', and 'Message' fields accept
 * order token replacements.
 *
 * @see uc_order_action_email_form()
 */
function uc_order_action_email($order, $from, $addresses, $subject, $message, $format) {
  $settings = array(
    'from' => $from,
    'addresses' => $addresses,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
  );

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'uc_order' => $order,
  );

  // Apply token replacements to the 'from' e-mail address.
  $from = token_replace($settings['from'], $settings['replacements']);
  if (empty($from)) {
    $from = uc_store_email_from();
  }

  // Apply token replacements to 'recipient' e-mail addresses.
  $addresses = token_replace($settings['addresses'], $settings['replacements']);
  // Split up our recipient e-mail addresses.
  $recipients = array();
  foreach (explode("\n", $addresses) as $address) {
    $address = trim($address);
    // Remove blank lines
    if (!empty($address)) {
      $recipients[] = $address;
    }
  }

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $from);

    if (!$sent['result']) {
      watchdog('uc_order', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Options list callback for message formats.
 */
function uc_order_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }

  return $options;
}

/**
 * Emails an invoice.
 *
 * The 'Sender', 'Subject' and 'Addresses' fields take order token replacements.
 */
function uc_order_action_email_invoice($order, $from, $addresses, $subject, $template, $view) {
  $settings = array(
    'from' => $from,
    'addresses' => $addresses,
    'subject' => $subject,
    'template' => $template,
    'view' => $view,
  );
  // Token replacements for the from, subject and body
  $settings['replacements'] = array(
    'uc_order' => $order,
  );

  // Apply token replacements to the 'from' e-mail address.
  $from = token_replace($settings['from'], $settings['replacements']);
  if (empty($from)) {
    $from = uc_store_email_from();
  }

  // Apply token replacements to 'recipient' e-mail addresses.
  $addresses = token_replace($settings['addresses'], $settings['replacements']);
  // Split up our recipient e-mail addresses.
  $recipients = array();
  foreach (explode("\n", $addresses) as $address) {
    $address = trim($address);
    // Remove blank lines
    if (!empty($address)) {
      $recipients[] = $address;
    }
  }

  $settings['message'] = theme('uc_order', array('order' => $order, 'op' => $settings['view'], 'template' => $settings['template']));

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $from);

    if (!$sent['result']) {
      watchdog('uc_order', 'Attempt to e-mail invoice for order @order_id to @email failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * @see uc_order_action_email_invoice()
 */
function uc_order_action_email_invoice_view_options() {
  return array(
    'print' => t('Show the business header and shipping method.'),
    'admin-mail' => t('Show all of the above plus the help text, email text, and store footer.'),
    'checkout-mail' => t('Show all of the above plus the "thank you" message.'),
  );
}

/**
 * Value comparison.
 *
 * @param float $source
 *   The source value.
 * @param string $op
 *   The comparison operator.
 * @param float $target
 *   The target value.
 *
 * @return bool
 *   Whether the comparison meets the specified conditions.
 *
 * @see uc_order_condition_value_operator_options
 */
function uc_order_condition_value_operator_comparison($source, $op, $target) {
  switch ($op) {
    case 'less':
      return $source < $target;

    case 'less_equal':
      return $source <= $target;

    case 'equal':
      return $source == $target;

    case 'greater_equal':
      return $source >= $target;

    case 'greater':
      return $source > $target;
  }
}

/**
 * Compare order total.
 *
 * @param object $order
 *   The order to check.
 * @param string $op
 *   The comparison operator.
 * @param float $value
 *   The target value.
 *
 * @return bool
 *   Whether the order total meets the specified condition.
 *
 * @see uc_order_condition_value_operator_options
 */
function uc_order_condition_total($order, $op, $value) {
  return uc_order_condition_value_operator_comparison($order->order_total, $op, $value);
}

/**
 * Compare order subtotal.
 *
 * @param object $order
 *   The order to check.
 * @param string $op
 *   The comparison operator.
 * @param float $value
 *   The target value.
 *
 * @return bool
 *   Whether the order subtotal meets the specified condition.
 *
 * @see uc_order_condition_value_operator_options
 */
function uc_order_condition_subtotal($order, $op, $value) {
  if (is_array($order->line_items)) {
    foreach ($order->line_items as $line_item) {
      if ($line_item['type'] == 'subtotal') {
        $subtotal = $line_item['amount'];
        return uc_order_condition_value_operator_comparison($subtotal, $op, $value);
      }
    }
  }
  return FALSE;
}
