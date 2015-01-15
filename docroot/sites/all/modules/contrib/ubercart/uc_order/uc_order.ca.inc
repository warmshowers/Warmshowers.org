<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to
 * make the order related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implements hook_ca_entity().
 */
function uc_order_ca_entity() {
  $entities['uc_order'] = array(
    '#title' => t('Ubercart order object'),
    '#type' => 'object',
    '#load' => 'uc_order_load',
    '#save' => 'uc_order_save',
  );
  $entities['uc_line_item'] = array(
    '#title' => t('Order line item'),
    '#type' => 'array',
  );

  return $entities;
}

/**
 * Implements hook_ca_trigger().
 */
function uc_order_ca_trigger() {
  $triggers['uc_order_status_update'] = array(
    '#title' => t('Order status gets updated'),
    '#category' => t('Order'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Original order'),
      ),
      'updated_order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Updated order'),
      ),
    ),
  );

  $triggers['uc_order_status_email_update'] = array(
    '#title' => t('E-mail requested for order status update'),
    '#category' => t('Order'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  $triggers['uc_order_delete'] = array(
    '#title' => t('An order is being deleted'),
    '#category' => t('Order'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  return $triggers;
}

/**
 * Implements hook_ca_predicate().
 */
function uc_order_ca_predicate() {
  $predicates = array();

  $predicates['uc_order_update_email_customer'] = array(
    '#title' => t('E-mail an order update notification'),
    '#description' => t('Notify the customer when the order status is changed.'),
    '#class' => 'notification',
    '#status' => 1,
    '#trigger' => 'uc_order_status_email_update',
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status is not still In Checkout.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'negate' => TRUE,
            'order_status' => 'in_checkout',
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email-raw]',
          'subject' => t('Order #[order-id] Update'),
          'message' => uc_get_message('order_update_email'),
          'format' => 1,
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implements hook_ca_condition().
 */
function uc_order_ca_condition() {
  $order_arg = array(
    '#entity' => 'uc_order',
  );

  $conditions['uc_order_status_condition'] = array(
    '#title' => t('Check the order status'),
    '#description' => t('Returns TRUE if the current order status matches the status specified below.'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_condition_check_order_status',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  $conditions['uc_order_state_condition'] = array(
    '#title' => t('Check the order state'),
    '#description' => t('Returns TRUE if the current order status belong to the order state specified below.'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_condition_check_order_state',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  $conditions['uc_order_condition_total'] = array(
    '#title' => t('Check the order total'),
    '#description' => t('Returns TRUE if the current order total is within the parameters below.'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_condition_total',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  $conditions['uc_order_condition_delivery_postal_code'] = array(
    '#title' => t("Check an order's shipping postal code"),
    '#category' => t('Order: Shipping address'),
    '#description' => t('Returns TRUE if the shipping postal code is in the specified area.'),
    '#callback' => 'uc_order_condition_delivery_postal_code',
    '#arguments' => array(
      'order' => $order_arg
    ),
  );
  $conditions['uc_order_condition_delivery_zone'] = array(
    '#title' => t("Check an order's shipping @zone", array('@zone' => uc_get_field_name('zone'))),
    '#category' => t('Order: Shipping address'),
    '#description' => t('Returns TRUE if the shipping @zone is in the specified list.', array('@zone' => uc_get_field_name('zone'))),
    '#callback' => 'uc_order_condition_delivery_zone',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_delivery_country'] = array(
    '#title' => t("Check an order's shipping country"),
    '#category' => t('Order: Shipping address'),
    '#description' => t('Returns TRUE if the shipping country is in the specified list.'),
    '#callback' => 'uc_order_condition_delivery_country',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_billing_postal_code'] = array(
    '#title' => t("Check an order's billing postal code"),
    '#category' => t('Order: Billing address'),
    '#description' => t('Returns TRUE if the billing postal code is in the specified area.'),
    '#callback' => 'uc_order_condition_billing_postal_code',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_billing_zone'] = array(
    '#title' => t("Check an order's billing @zone", array('@zone' => uc_get_field_name('zone'))),
    '#category' => t('Order: Billing address'),
    '#description' => t('Returns TRUE if the billing @zone is in the specified list.', array('@zone' => uc_get_field_name('zone'))),
    '#callback' => 'uc_order_condition_billing_zone',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_billing_country'] = array(
    '#title' => t("Check an order's billing country"),
    '#category' => t('Order: Billing address'),
    '#description' => t('Returns TRUE if the billing country is in the specified list.'),
    '#callback' => 'uc_order_condition_billing_country',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_has_products'] = array(
    '#title' => t("Check an order's products"),
    '#category' => t('Order: Product'),
    '#description' => t('Returns TRUE if the order has any, all, or only the products in the list.'),
    '#callback' => 'uc_order_condition_has_products',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_count_products'] = array(
    '#title' => t("Check an order's number of products"),
    '#category' => t('Order: Product'),
    '#description' => t('Determines if the order has the specified number of products, possibly of a certain type.'),
    '#callback' => 'uc_order_condition_count_products',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_products_weight'] = array(
    '#title' => t("Check an order's total weight"),
    '#category' => t('Order: Product'),
    '#description' => t('Determines if the order has the specified weight, possibly counting only a certain type of product.'),
    '#callback' => 'uc_order_condition_products_weight',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_is_shippable'] = array(
    '#title' => t('Check if an order can be shipped'),
    '#category' => t('Order'),
    '#description' => t('Returns TRUE if the order has any shippable products.'),
    '#callback' => 'uc_order_condition_is_shippable',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  $conditions['uc_order_condition_user_name'] = array(
    '#title' => t('Check the user name.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user name matches the condition.'),
    '#callback' => 'uc_order_condition_user_name',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_user_mail'] = array(
    '#title' => t('Check the user email address.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user email addresses matches the condition.'),
    '#callback' => 'uc_order_condition_user_mail',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_user_created'] = array(
    '#title' => t('Check the user creation date.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user creation date matches the condition.'),
    '#callback' => 'uc_order_condition_user_created',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_user_login'] = array(
    '#title' => t('Check the user last login date.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user last login date matches the condition.'),
    '#callback' => 'uc_order_condition_user_login',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_user_language'] = array(
    '#title' => t('Check the user language setting.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user language setting matches the condition.'),
    '#callback' => 'uc_order_condition_user_language',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $conditions['uc_order_condition_user_roles'] = array(
    '#title' => t('Check the role of the user.'),
    '#category' => t('Order: User'),
    '#description' => t('Returns TRUE if the user roles match your settings.'),
    '#callback' => 'uc_order_condition_user_roles',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  return $conditions;
}

/**
 * Implements hook_ca_action().
 */
function uc_order_ca_action() {
  $order_arg = array(
    '#entity' => 'uc_order',
    '#title' => t('Order'),
  );

  $actions['uc_order_update_status'] = array(
    '#title' => t('Update the order status'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_action_update_status',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $actions['uc_order_action_add_comment'] = array(
    '#title' => t('Add a comment to the order'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_action_add_comment',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $actions['uc_order_email'] = array(
    '#title' => t('Send an order email'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_action_email',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );
  $actions['uc_order_email_invoice'] = array(
    '#title' => t('Email an order invoice'),
    '#category' => t('Order'),
    '#callback' => 'uc_order_action_email_invoice',
    '#arguments' => array(
      'order' => $order_arg,
    ),
  );

  return $actions;
}


/******************************************************************************
 * Condition Callbacks and Forms                                              *
 ******************************************************************************/

/**
 * Check the current order status.
 *
 * @see uc_order_condition_check_order_status_form()
 */
function uc_order_condition_check_order_status($order, $settings) {
  // Return TRUE if the order status matches.
  return $order->order_status == $settings['order_status'];
}

/**
 * @see uc_order_condition_check_order_status()
 */
function uc_order_condition_check_order_status_form($form_state, $settings = array()) {
  foreach (uc_order_status_list('general') as $status) {
    $options[$status['id']] = $status['title'];
  }
  foreach (uc_order_status_list('specific') as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order status'),
    '#options' => $options,
    '#default_value' => isset($settings['order_status']) ? $settings['order_status'] : '',
  );

  return $form;
}

/**
 * Check the current order state.
 *
 * @see uc_order_condition_check_order_state_form()
 */
function uc_order_condition_check_order_state($order, $settings) {
  // Return TRUE if the order status matches.
  return uc_order_status_data($order->order_status, 'state') == $settings['order_state'];
}

/**
 * @see uc_order_condition_check_order_state()
 */
function uc_order_condition_check_order_state_form($form_state, $settings = array()) {
  foreach (uc_order_state_list('general') as $state) {
    $options[$state['id']] = $state['title'];
  }
  foreach (uc_order_state_list('specific') as $state) {
    $options[$state['id']] = $state['title'];
  }
  $form['order_state'] = array(
    '#type' => 'select',
    '#title' => t('Order state'),
    '#options' => $options,
    '#default_value' => isset($settings['order_state']) ? $settings['order_state'] : '',
  );

  return $form;
}

/**
 * Check the current order balance.
 *
 * @see uc_order_condition_total_form()
 */
function uc_order_condition_total($order, $settings) {
  $total = uc_order_get_total($order, TRUE);
  switch ($settings['order_total_comparison']) {
    case 'less':
      return $total < $settings['order_total_value'];
    case 'less_equal':
      return $total <= $settings['order_total_value'];
    case 'equal':
      return $total == $settings['order_total_value'];
    case 'greater_equal':
      return $total >= $settings['order_total_value'];
    case 'greater':
      return $total > $settings['order_total_value'];
  }
}

/**
 * @see uc_order_condition_total()
 */
function uc_order_condition_total_form($form_state, $settings = array()) {
  $form['order_total_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Order total value'),
    '#description' => t('Specify a value to compare the order total against.'),
    '#default_value' => isset($settings['order_total_value']) ? $settings['order_total_value'] : 0,
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['order_total_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Order total comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['order_total_comparison']) ? $settings['order_total_comparison'] : 'greater_equal',
  );

  return $form;
}

/**
 * Check an order's delivery postal code.
 *
 * @see uc_order_condition_delivery_postal_code_form()
 */
function uc_order_condition_delivery_postal_code($order, $settings) {
  // Trim the wildcard off the pattern.
  $pattern = rtrim($settings['pattern'], '*');

  // Return TRUE if the delivery postal code begins with the pattern.
  return strpos($order->delivery_postal_code, $pattern) === 0;
}

/**
 * @see uc_order_condition_delivery_postal_code()
 */
function uc_order_condition_delivery_postal_code_form($form_state, $settings = array()) {
  $form['pattern'] = array(
    '#type' => 'textfield',
    '#title' => uc_get_field_name('postal_code'),
    '#default_value' => isset($settings['pattern']) ? $settings['pattern'] : NULL,
    '#description' => t('Specify a postal code or postal code pattern. Use "*" as a wild card to specify a range of postal codes.<br /><b>Example:</b> In the US, 402* represents all areas from 40200 to 40299.'),
    '#size' => 15,
    // '#required' => TRUE,
  );

  return $form;
}

/**
 * Check an order's delivery zone.
 *
 * @see uc_order_condition_delivery_zone_form()
 */
function uc_order_condition_delivery_zone($order, $settings) {
  return in_array($order->delivery_zone, $settings['zones']);
}

/**
 * @see uc_order_condition_delivery_zone_form()
 */
function uc_order_condition_delivery_zone_form($form_state, $settings = array()) {
  $options = array();
  $result = db_query("SELECT z.*, c.country_name FROM {uc_zones} AS z LEFT JOIN {uc_countries} AS c ON z.zone_country_id = c.country_id ORDER BY c.country_name, z.zone_name");
  while ($zone = db_fetch_object($result)) {
    $options[t($zone->country_name)][$zone->zone_id] = $zone->zone_name;
  }
  uksort($options, 'strnatcasecmp');

  $form['zones'] = array(
    '#type' => 'select',
    '#title' => uc_get_field_name('zone'),
    '#options' => $options,
    '#default_value' => isset($settings['zones']) ? $settings['zones'] : NULL,
    '#multiple' => TRUE,
    // '#required' => TRUE,
  );

  return $form;
}

/**
 * Check an order's delivery country.
 *
 * @see uc_order_condition_delivery_country_form()
 */
function uc_order_condition_delivery_country($order, $settings) {
  return in_array($order->delivery_country, $settings['countries']);
}

/**
 * @see uc_order_condition_delivery_country()
 */
function uc_order_condition_delivery_country_form($form_state, $settings = array()) {
  $form['countries'] = uc_country_select(uc_get_field_name('country'));
  $form['countries']['#default_value'] = isset($settings['countries']) ? $settings['countries'] : NULL;
  $form['countries']['#multiple'] = TRUE;
  // $form['countries']['#required'] = TRUE;

  return $form;
}

/**
 * Check an order's billing postal code.
 *
 * @see uc_order_condition_billing_postal_code_form()
 */
function uc_order_condition_billing_postal_code($order, $settings) {
  // Trim the wildcard off the pattern.
  $pattern = rtrim($settings['pattern'], '*');

  // Return TRUE if the delivery postal code begins with the pattern.
  return strpos($order->billing_postal_code, $pattern) === 0;
}

/**
 * @see uc_order_condition_billing_postal_code()
 */
function uc_order_condition_billing_postal_code_form($form_state, $settings = array()) {
  $form['pattern'] = array(
    '#type' => 'textfield',
    '#title' => uc_get_field_name('postal_code'),
    '#default_value' => isset($settings['pattern']) ? $settings['pattern'] : NULL,
    '#description' => t('Specify a postal code or postal code pattern. Use "*" as a wild card to specify a range of postal codes.<br /><b>Example:</b> In the US, 402* represents all areas from 40200 to 40299.'),
    '#size' => 15,
    // '#required' => TRUE,
  );

  return $form;
}

/**
 * Check an order's billing zone.
 *
 * @see uc_order_condition_billing_zone_form()
 */
function uc_order_condition_billing_zone($order, $settings) {
  return in_array($order->billing_zone, $settings['zones']);
}

/**
 * @see uc_order_condition_billing_zone()
 */
function uc_order_condition_billing_zone_form($form_state, $settings = array()) {
  $result = db_query("SELECT z.*, c.country_name FROM {uc_zones} AS z LEFT JOIN {uc_countries} AS c ON z.zone_country_id = c.country_id ORDER BY c.country_name, z.zone_name");
  while ($zone = db_fetch_object($result)) {
    $options[t($zone->country_name)][$zone->zone_id] = $zone->zone_name;
  }
  uksort($options, 'strnatcasecmp');

  $form['zones'] = array(
    '#type' => 'select',
    '#title' => uc_get_field_name('zone'),
    '#options' => $options,
    '#default_value' => isset($settings['zones']) ? $settings['zones'] : NULL,
    '#multiple' => TRUE,
    // '#required' => TRUE,
  );

  return $form;
}

/**
 * Check an order's billing country.
 *
 * @see uc_order_condition_billing_country_form()
 */
function uc_order_condition_billing_country($order, $settings) {
  return in_array($order->billing_country, $settings['countries']);
}

/**
 * @see uc_order_condition_billing_country()
 */
function uc_order_condition_billing_country_form($form_state, $settings = array()) {
  $form['countries'] = uc_country_select(uc_get_field_name('country'));
  $form['countries']['#default_value'] = isset($settings['countries']) ? $settings['countries'] : NULL;
  $form['countries']['#multiple'] = TRUE;
  // $form['countries']['#required'] = TRUE;

  return $form;
}

/**
 * Check that the order has the selected combination of products.
 *
 * @see uc_order_condition_has_products_form()
 */
function uc_order_condition_has_products($order, $settings) {
  $products = array();
  foreach ($order->products as $product) {
    $products[] = $product->model;
  }
  $required = array_intersect($settings['products'], $products);
  if (isset($settings['required']) && $settings['required']) {
    $required_check = $required == $settings['products'];
  }
  else {
    $required_check = (bool)count($required);
  }
  if (isset($settings['forbidden']) && $settings['forbidden']) {
    $forbidden = array_diff($products, $settings['products']);
    $forbidden_check = (bool)count($forbidden);
  }
  else {
    $forbidden_check = FALSE;
  }
  return $required_check && !$forbidden_check;
}

/**
 * @see uc_order_condition_has_products()
 */
function uc_order_condition_has_products_form($form_state, $settings) {
  $form['required'] = array(
    '#type' => 'radios',
    '#title' => t('Require selected products'),
    '#options' => array(
      0 => t('Order has any of these products.'),
      1 => t('Order has all of these products.'),
    ),
    '#default_value' => isset($settings['required']) ? $settings['required'] : 0,
  );
  $form['forbidden'] = array(
    '#type' => 'radios',
    '#title' => t('Forbid other products'),
    '#options' => array(
      0 => t('Order may have other products.'),
      1 => t('Order has only these products.'),
    ),
    '#default_value' => isset($settings['forbidden']) ? $settings['forbidden'] : 0,
  );

  $options = array();
  $result = db_query("SELECT nid FROM {uc_products}");
  while ($nid = db_result($result)) {
    $options += uc_product_get_models($nid, FALSE);
  }
  asort($options);

  $form['products'] = array(
    '#type' => 'select',
    '#title' => t('Product models'),
    '#options' => $options,
    '#default_value' => isset($settings['products']) ? $settings['products'] : NULL,
    '#multiple' => TRUE,
  );
  return $form;
}

/**
 * Check that the order has the selected number of products.
 *
 * @see uc_order_condition_count_products_form()
 */
function uc_order_condition_count_products($order, $settings) {
  $totals = array('all' => 0);
  $total = 0;
  foreach ($order->products as $product) {
    $totals['all'] += $product->qty;
    $totals[$product->nid] += $product->qty;
  }
  if (in_array('all', $settings['products'])) {
    $total = $totals['all'];
  }
  else {
    foreach ($settings['products'] as $product) {
      $total += $totals[$product];
    }
  }
  switch ($settings['product_count_comparison']) {
    case 'less':
      return $total < $settings['product_count_value'];
    case 'less_equal':
      return $total <= $settings['product_count_value'];
    case 'equal':
      return $total == $settings['product_count_value'];
    case 'greater_equal':
      return $total >= $settings['product_count_value'];
    case 'greater':
      return $total > $settings['product_count_value'];
  }
}

/**
 * @see uc_order_condition_count_products()
 */
function uc_order_condition_count_products_form($form_state, $settings = array()) {
  $options = array('all' => t('- All products -'));
  $result = db_query("SELECT nid, model FROM {uc_products} ORDER BY model");
  while ($product = db_fetch_object($result)) {
    $options[$product->nid] = $product->model;
  }
  $form['products'] = array(
    '#type' => 'select',
    '#title' => t('Products'),
    '#options' => $options,
    '#default_value' => isset($settings['products']) ? $settings['products'] : 'all',
    '#description' => check_plain(t('Selecting "- All products -" will override any other selections and returns the total number of products in the order.')),
    '#multiple' => TRUE,
  );
  $form['product_count_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Product count value'),
    '#description' => t('Specify a value to compare the product count against.'),
    '#default_value' => isset($settings['product_count_value']) ? $settings['product_count_value'] : '',
    '#size' => 16,
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['product_count_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Product count comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['product_count_comparison']) ? $settings['product_count_comparison'] : 'greater_equal',
  );

  return $form;
}

/**
 * Checkt the weight of the order's products.
 *
 * @see uc_order_condition_products_weight_form()
 */
function uc_order_condition_products_weight($order, $settings) {
  $totals = array('all' => 0);
  $total = 0;
  foreach ($order->products as $product) {
    $unit_conversion = uc_weight_conversion($product->weight_units, $settings['weight_units']);
    $totals['all'] += $product->qty * $product->weight * $unit_conversion;
    $totals[$product->model] = $product->qty * $product->weight * $unit_conversion;
  }
  if (in_array('all', $settings['products'])) {
    $total = $totals['all'];
  }
  else {
    foreach ($settings['products'] as $product) {
      $total += $totals[$product];
    }
  }
  switch ($settings['product_weight_comparison']) {
    case 'less':
      return $total < $settings['product_weight_value'];
    case 'less_equal':
      return $total <= $settings['product_weight_value'];
    case 'equal':
      return $total == $settings['product_weight_value'];
    case 'greater_equal':
      return $total >= $settings['product_weight_value'];
    case 'greater':
      return $total > $settings['product_weight_value'];
  }
}

/**
 * @see uc_order_condition_products_weight()
 */
function uc_order_condition_products_weight_form($form_state, $settings = array()) {
  $options = array('all' => t('- All products -'));
  $result = db_query("SELECT nid, model FROM {uc_products}");
  while ($product = db_fetch_object($result)) {
    $options[$product->nid] = $product->model;
  }
  $form['products'] = array(
    '#type' => 'select',
    '#title' => t('Products'),
    '#options' => $options,
    '#default_value' => isset($settings['products']) ? $settings['products'] : 'all',
    '#description' => check_plain(t('Selecting "- All products -" will override any other selections and returns the total number of products in the order.')),
    '#multiple' => TRUE,
  );
  $form['weight_units'] = array(
    '#type' => 'select',
    '#title' => t('Unit of measurement'),
    '#default_value' => $settings['weight_units'] ? $settings['weight_units'] : variable_get('uc_weight_unit', 'lb'),
    '#options' => array(
      'lb' => t('Pounds'),
      'kg' => t('Kilograms'),
      'oz' => t('Ounces'),
      'g' => t('Grams'),
    ),
  );
  $form['product_weight_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Product weight value'),
    '#description' => t('Specify a value to compare the product weight against.'),
    '#default_value' => isset($settings['product_weight_value']) ? $settings['product_weight_value'] : 0,
    '#size' => 16,
  );

  $options = array(
    'less' => t('Total is less than specified value.'),
    'less_equal' => t('Total is less than or equal to specified value.'),
    'equal' => t('Total is equal to specified value.'),
    'greater_equal' => t('Total is greater than or equal to specified value.'),
    'greater' => t('Total is greater than specified value.'),
  );
  $form['product_weight_comparison'] = array(
    '#type' => 'radios',
    '#title' => t('Product count comparison type'),
    '#options' => $options,
    '#default_value' => isset($settings['product_weight_comparison']) ? $settings['product_weight_comparison'] : 'greater_equal',
  );

  return $form;
}

/**
 * Check that the order is shippable.
 */
function uc_order_condition_is_shippable($order, $settings) {
  return uc_order_is_shippable($order);
}

/**
 * Check a user name.
 *
 * @see uc_order_condition_user_name()
 */
function uc_order_condition_user_name($order, $settings) {
  if (empty($order->uid)) {
    // Anonymous users have no names.
    return empty($settings['name']);
  }
  else {
    // Load from the order.
    if ($account = uc_order_user_load($order)) {
      return $account->name == $settings['name'];
    }
    // Or, if the user doesn't exist anymore, return FALSE
    else {
      return FALSE;
    }
  }
}

/**
 * @see uc_order_condition_user_name()
 */
function uc_order_condition_user_name_form($form_state, $settings = array()) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#default_value' => isset($settings['name']) ? $settings['name'] : '',
    '#autocomplete_path' => 'user/autocomplete',
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );

  return $form;
}

/**
 * Check an user email.
 *
 * @see uc_order_condition_user_email_form()
 */
function uc_order_condition_user_mail($order, $settings) {
  $account = uc_order_user_load($order);
  return $account->mail == $settings['mail'];
}

/**
 * @see uc_order_condition_user_mail()
 */
function uc_order_condition_user_mail_form($form_state, $settings = array()) {
  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('User email'),
    '#default_value' => isset($settings['mail']) ? $settings['mail'] : '',
  );

  return $form;
}

/**
 * Check an user creation date.
 *
 * @see uc_order_condition_user_created_form()
 */
function uc_order_condition_user_created($order, $settings) {
  $account = uc_order_user_load($order);

  // Get the beginning of the day the user registered.
  $user_created = $account->created - ($account->created % 86400);
  $settings_created = gmmktime(0, 0, 0, $settings['created']['month'], $settings['created']['day'], $settings['created']['year']);

  switch ($settings['operator']) {
    case 'less':
      return $user_created < $settings_created;
    case 'less_equal':
      return $user_created <= $settings_created;
    case 'equal':
      return $user_created == $settings_created;
    case 'not_equal':
      return $user_created != $settings_created;
    case 'greater_equal':
      return $user_created >= $settings_created;
    case 'greater':
      return $user_created > $settings_created;
  }
}

/**
 * @see uc_order_condition_user_created()
 */
function uc_order_condition_user_created_form($form_state, $settings = array()) {
  $form['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#options' => array(
      'less' => t('User was created before the specified date.'),
      'less_equal' => t('User was created on or before the specified date.'),
      'equal' => t('User was created on the specified date.'),
      'not_equal' => t('User was not created on the specified date.'),
      'greater_equal' => t('User was created on or after the specified date.'),
      'greater' => t('User was created after the specified date.'),
    ),
    '#default_value' => isset($settings['operator']) ? $settings['operator'] : 'equal',
  );
  $form['created'] = array(
    '#type' => 'date',
    '#title' => t('User creation date'),
    '#default_value' => isset($settings['created']) ? $settings['created'] : '',
  );

  return $form;
}

/**
 * Check an user last login date.
 *
 * @see uc_order_condition_user_login_form()
 */
function uc_order_condition_user_login($order, $settings) {
  $account = uc_order_user_load($order);

  // Get the beginning of the day the user last logged in.
  $user_login = $account->login - ($account->login % 86400);
  $settings_login = gmmktime(0, 0, 0, $settings['login']['month'], $settings['login']['day'], $settings['login']['year']);

  switch ($settings['operator']) {
    case 'less':
      return $user_login < $settings_login;
    case 'less_equal':
      return $user_login <= $settings_login;
    case 'equal':
      return $user_login == $settings_login;
    case 'not_equal':
      return $user_login != $settings_login;
    case 'greater_equal':
      return $user_login >= $settings_login;
    case 'greater':
      return $user_login > $settings_login;
  }
}

/**
 * @see uc_order_condition_user_login()
 */
function uc_order_condition_user_login_form($form_state, $settings = array()) {
  $form['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#options' => array(
      'less' => t('User last logged in before the specified date.'),
      'less_equal' => t('User last logged in on, or before the specified date.'),
      'equal' => t('User last logged in on the specified date.'),
      'not_equal' => t('User did not log in last on the specified date.'),
      'greater_equal' => t('User last logged in on or after the specified date.'),
      'greater' => t('User last logged after the specified date.'),
    ),
    '#default_value' => isset($settings['operator']) ? $settings['operator'] : 'equal',
  );
  $form['login'] = array(
    '#type' => 'date',
    '#title' => t('User last login date'),
    '#default_value' => isset($settings['created']) ? $settings['created'] : '',
  );

  return $form;
}

/**
 * Check an user language setting.
 *
 * @see uc_order_condition_user_language_form()
 */
function uc_order_condition_user_language($order, $settings) {
  $account = uc_order_user_load($order);
  $user_prefered_language = user_preferred_language($account);
  return $user_prefered_language->language == $settings['language'];
}

/**
 * @see uc_order_condition_user_language()
 */
function uc_order_condition_user_language_form($form_state, $settings = array()) {
  $languages = language_list();
  $options = array();
  foreach ($languages as $language) {
    $options[$language->language] = $language->name .' ('. $language->native .')';
  }
  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('User language setting'),
    '#options' => $options,
    '#default_value' => isset($settings['language']) ? $settings['language'] : '',
  );

  return $form;
}

/**
 * Check an user roles.
 *
 * @see uc_order_condition_user_roles_form()
 */
function uc_order_condition_user_roles($order, $settings) {
  $account = uc_order_user_load($order);
  $settings['roles'] = array_filter($settings['roles']);

  if ($settings['operator'] == 'AND') {
    foreach ($settings['roles'] as $key) {
      if (!isset($account->roles[$key])) {
        return FALSE;
      }
    }
    return TRUE;
  }
  else {
    foreach ($settings['roles'] as $key) {
      if (isset($account->roles[$key])) {
        return TRUE;
      }
    }
    return FALSE;
  }
}

/**
 * @see uc_order_condition_user_roles()
 */
function uc_order_condition_user_roles_form($form_state, $settings = array()) {
  $form['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#description' => t('If you use an <em>AND</em> case and want to check a custom role, make sure <em>authenticated user</em> is checked too or the condition will return FALSE.'),
    '#options' => array(
      'AND' => t('AND: If the user has all of these roles.'),
      'OR' => t('OR: If the user has any of these roles.'),
    ),
    '#default_value' => isset($settings['operator']) ? $settings['operator'] : 'AND',
  );
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => user_roles(),
    '#default_value' => isset($settings['roles']) ? $settings['roles'] : array(),
  );

  return $form;
}

/******************************************************************************
 * Action Callbacks and Forms                                                 *
 ******************************************************************************/

/**
 * Update an order's status.
 *
 * @see uc_order_action_update_status_form()
 */
function uc_order_action_update_status(&$order, $settings) {
  if (uc_order_update_status($order->order_id, $settings['order_status'])) {
    $order->order_status = $settings['order_status'];
  }
}

/**
 * @see uc_order_action_update_status()
 */
function uc_order_action_update_status_form($form_state, $settings = array()) {
  foreach (uc_order_status_list('general') as $status) {
    $options[$status['id']] = $status['title'];
  }
  foreach (uc_order_status_list('specific') as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order status'),
    '#options' => $options,
    '#default_value' => isset($settings['order_status']) ? $settings['order_status'] : '',
  );

  return $form;
}

/**
 * Add a comment to an order.
 *
 * @see uc_order_action_add_comment_form()
 */
function uc_order_action_add_comment($order, $settings) {
  uc_order_comment_save($order->order_id, 0,
    token_replace_multiple($settings['comment'], array('global' => NULL, 'order' => $order)),
    $settings['comment_type'] == 'admin' ? 'admin' : 'order',
    $order->order_status, $settings['comment_type'] == 'notified');
}

/**
 * @see uc_order_action_add_comment()
 */
function uc_order_action_add_comment_form($form_state, $settings = array()) {
  $form['comment_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select an order comment type'),
    '#options' => array(
      'admin' => t('Enter this as an admin comment.'),
      'order' => t('Enter this as a customer order comment.'),
      'notified' => t('Enter this as a customer order comment with a notified icon.'),
    ),
    '#default_value' => isset($settings['comment_type']) ? $settings['comment_type'] : 'admin',
  );
  $form['comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#description' => t('Enter the comment message. Uses <a href="!url">order and global tokens</a>.', array('!url' => url('admin/store/help/tokens'))),
    '#default_value' => isset($settings['comment']) ? $settings['comment'] : '',
    '#wysiwyg' => FALSE,
  );

  return $form;
}

/**
 * Sends an email concerning an order.
 *
 * The 'Sender', 'Recipients', 'Subject', and 'Message' fields accept
 * order, global, and user token replacements.
 *
 * @see uc_order_action_email_form()
 */
function uc_order_action_email($order, $settings) {
  $account = uc_order_user_load($order);

  // Token replacements for the from and recipient addresses
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
  );

  // Apply token replacements to from e-mail address.
  $from = token_replace_multiple($settings['from'], $settings['replacements']);
  if (empty($from)) {
    $from = uc_store_email_from();
  }

  // Apply token replacements to recipient e-mail addresses.
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  // Split up our recipient e-mail addresses.
  $recipients = array();
  foreach (explode("\n", $addresses) as $address) {
    $address = trim($address);
    // Remove blank lines
    if (!empty($address)) {
      $recipients[] = $address;
    }
  }

  if (empty($recipients)) {
    watchdog('uc_order', 'Attempted to send an order e-mail with no recipient.', array(), WATCHDOG_ERROR);
    return;
  }

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $from);

    if (!$sent['result']) {
      watchdog('uc_order', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * @see uc_order_action_email()
 */
function uc_order_action_email_form($form_state, $settings = array()) {
  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => isset($settings['from']) ? $settings['from'] : uc_store_email_from(),
    '#description' => t("Enter the 'From' email addresses, or leave blank to use your store email address. You may use order tokens for dynamic email addresses."),
    '#required' => TRUE,
  );
  $form['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => isset($settings['addresses']) ? $settings['addresses'] : '[order-email-raw]',
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => isset($settings['subject']) ? $settings['subject'] : '',
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => isset($settings['message']) ? $settings['message'] : '',
    '#wysiwyg' => FALSE,
  );

  // We add the #is_format element to allow us to locate and configure the filters.
  $form['format'] = filter_form($settings['format']) + array('#is_format' => TRUE);

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the e-mail from and recipients fields, the subject, and the message body.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (array('global', 'order') as $name) {
    $form['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }

  return $form;
}

/**
 * Email an invoice.
 *
 * The subject and addresses fields take order token replacements.
 *
 * @see uc_order_action_email_invoice_form()
 */
function uc_order_action_email_invoice($order, $settings) {
  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
  );

  // Apply token replacements to from e-mail address.
  $from = token_replace_multiple($settings['from'], $settings['replacements']);
  if (empty($from)) {
    $from = uc_store_email_from();
  }

  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);

  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  $settings['message'] = theme('uc_order', $order, $settings['view'], $settings['template']);

  if (empty($recipients)) {
    watchdog('ca', 'Attempted to e-mail an invoice with no recipient.', array(), WATCHDOG_ERROR);
    return;
  }

  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $from);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail invoice for order @order_id to @email failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * @see uc_order_action_email_invoice()
 */
function uc_order_action_email_invoice_form($form_state, $settings = array()) {
  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => isset($settings['from']) ? $settings['from'] : uc_store_email_from(),
    '#description' => t("Enter the 'From' email addresses, or leave blank to use your store email address. You may use order tokens for dynamic email addresses."),
    '#required' => TRUE,
  );
  $form['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => isset($settings['addresses']) ? $settings['addresses'] : '[order-email-raw]',
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => isset($settings['subject']) ? $settings['subject'] : '',
    '#required' => TRUE,
  );

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the recipients, the subject, and the template file.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (array('global', 'order') as $name) {
    $form['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }

  $form['template'] = array(
    '#type' => 'select',
    '#title' => t('Invoice template'),
    '#description' => t('Select the invoice template to use for this email.'),
    '#options' => uc_order_template_options(),
    '#default_value' => isset($settings['template']) ? $settings['template'] : '',
  );
  $form['view'] = array(
    '#type' => 'radios',
    '#title' => t('Included information'),
    '#options' => array(
      'print' => t('Show the business header and shipping method.'),
      'admin-mail' => t('Show all of the above plus the help text, email text, and store footer.'),
      'checkout-mail' => t('Show all of the above plus the "thank you" message.'),
    ),
    '#default_value' => isset($settings['view']) ? $settings['view'] : 'print',
  );

  return $form;
}
