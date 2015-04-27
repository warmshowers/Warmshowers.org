<?php

/**
 * @file
 * Recurring payments administration page callbacks and form builder functions.
 */

/**
 * Displays a table for the administration of recurring fees.
 */
function uc_recurring_admin($filter = NULL, $id = NULL) {
  $item = menu_get_item();

  // Add the filter form to the top of the table.
  $output = render(drupal_get_form('uc_recurring_admin_filter_form', $item));

  // Build an array of fees to include in the admin table.
  $fees = array();

  $footer_link = TRUE;

  $header = array(
    array(
      'data' => t('ID'),
      'field' => 'ru.rfid',
    ),
    array(
      'data' => t('Order'),
      'field' => 'ru.order_id',
    ),
    array(
      'data' => t('Status'),
      'field' => 'ru.status',
    ),
    array('data' => t('Account')),
    array(
      'data' => t('Next'),
      'field' => 'ru.next_charge',
      'sort' => 'asc',
    ),
    array('data' => t('Amount')),
    array('data' => t('Interval')),
    array('data' => t('Left')),
    array('data' => t('Total')),
    array('data' => t('Operations')),
  );

  // If we're on the overview page listing all fees...
  if (empty($filter)) {
    // Get all recurring fees in the system.
    $fees = uc_recurring_get_all_fees(TRUE, $header);
    $footer_link = FALSE;
  }
  elseif ($filter == 'order') {
    // Get all recurring fees in a specific order if it is valid.
    if ($order = uc_order_load($id)) {
      $fees = uc_recurring_get_fees($order);
    }
  }
  elseif ($fee = uc_recurring_fee_user_load($id)) {
    // Get a single recurring fee.
    $fees[] = $fee;
  }

  // If we actually found fees to display...
  if (!empty($fees)) {
    $output .= theme('uc_recurring_admin_table', array('fees' => $fees));
  }

  // Display a link back to the full list if necessary.
  if ($footer_link) {
    $output .= l(t('Back to the full list'), 'admin/store/orders/recurring');
  }

  return $output;
}

/**
 * Filter by a specific order ID.
 *
 * @param $item
 *   A menu item.
 */
function uc_recurring_admin_filter_form($form, $form_state, $item) {
  $form['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'order' => t('Order ID'),
      'fee' => t('Fee ID'),
    ),
    '#default_value' => (isset($item['map'][5]) && $item['map'][5] == 'fee') ? 'fee' : 'order',
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['id'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($item['map'][6]) ? $item['map'][6] : '',
    '#size' => 10,
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#attributes' => array('style' => 'margin: .85em 0em;'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function uc_recurring_admin_filter_form_submit($form, &$form_state) {
  if (intval($form_state['values']['id']) > 0) {
    $form_state['redirect'] = 'admin/store/orders/recurring/view/' . $form_state['values']['type'] . '/' . $form_state['values']['id'];
  }
}


/**
 * Confirm a recurring fee charge.
 *
 * @param $form_state
 * @param $uid
 * @return unknown_type
 */
function uc_recurring_admin_charge_form($form, $form_state, $rfid, $fee_handler) {
  $fee = uc_recurring_fee_user_load($rfid);
  if ($fee->fee_handler == $fee_handler) {
    $form['rfid'] = array(
      '#type' => 'hidden',
      '#value' => $rfid,
    );
    $form['message'] = array(
      '#markup' => '<div>' . t('Are you sure you want to charge the customer !amount at this time?', array('!amount' => theme('uc_price', array('price' => $fee->fee_amount)))) . '</div>',
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Process Renewal'),
      '#suffix' => l(t('Cancel'), request_uri()),
    );
    return $form;
  }
  else {
    drupal_set_message(t('Invalid fee handler was given, try again from the operations for this <a href="@url">recurring fee</a>, if problem persists contact the site administrator.', array('@url' => url(request_uri()))), 'error');
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function uc_recurring_admin_charge_form_submit($form, &$form_state) {
  $rfid = $form_state['values']['rfid'];
  $fee = uc_recurring_fee_user_load($rfid);

  // Attempt to renew the charge.
  if (uc_recurring_renew($fee)) {
    drupal_set_message(t('Recurring fee @fee charged successfully.', array('@fee' => $rfid)));
  }
  else {
    drupal_set_message(t('Attempt to charge recurring fee @fee failed.', array('@fee' => $rfid)), 'error');
  }
}

/**
 * Let an admin edit a recurring fee.
 */
function uc_recurring_admin_edit_form($form, $form_state, $rfid, $fee_handler) {
  $fee = uc_recurring_fee_user_load($rfid);
  if ($fee->fee_handler == $fee_handler) {
    drupal_add_css(drupal_get_path('module', 'uc_recurring') . '/uc_recurring.css');
    drupal_add_js(drupal_get_path('module', 'uc_recurring') . '/uc_recurring.js');

    $form['rfid'] = array(
      '#type' => 'hidden',
      '#value' => $rfid,
    );
    $form['fee_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Recurring order title'),
      '#description' => t('This is the text that appears on the order as the name of the product.'),
      '#default_value' => $fee->fee_title,
      '#size' => 60,
    );

    $form['fee_status'] = array(
      '#type' => 'select',
      '#title' => t('Recurring fee status'),
      '#default_value' => $fee->status,
      '#options' => uc_recurring_fee_status_label(),
    );

    $form['fee_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Recurring fee amount'),
      '#description' => t('Charge this amount each billing period.'),
      '#default_value' => $fee->fee_amount,
      '#size' => 16,
      '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
      '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
    );

    $form['num_interval'] = array(
      '#type' => 'fieldset',
      '#title' => t('Number of billing periods'),
      '#collapsible' => FALSE,
      '#description' => t('Specify how many more recurring fees will be charged.'),
    );

    $unlimited_intervals_attributes = array();
    $number_intervals_attributes = array();
    
    if (($fee->remaining_intervals < 0 && !isset($form_state['values']['unlimited_intervals'])) || (isset($form_state['values']) && isset($form_state['values']['unlimited_intervals']) && $form_state['values']['unlimited_intervals'] == 1 )) {
      $unlimited_intervals_attributes['checked'] = 'checked';
      $number_intervals_default_value = 1;
      $number_intervals_attributes = array('disabled' => 'disabled');
    }
    else{
      $number_intervals_default_value = ($fee->remaining_intervals > 0) ? $fee->remaining_intervals : 1;
    }

    $form['num_interval']['unlimited_intervals'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unlimited rebillings.'),
      '#attributes' => $unlimited_intervals_attributes,
      '#ajax' => array(
        'callback' => 'rebilling_num_intervals_callback',
        'wrapper' => 'rebilling_num_intervals_replace',
      ),
    );
    
    $form['num_interval']['number_intervals'] = array(
      '#type' => 'textfield',
      '#title' => t('Remaining billing periods'),
      '#size' => 16,
      '#prefix' => '<div id="rebilling_num_intervals_replace">',
      '#suffix' => '</div>',      
      '#default_value' => $number_intervals_default_value,
      '#attributes' => $number_intervals_attributes,
    );

    $form['next_charge'] = array(
      '#type' => 'fieldset',
      '#title' => t('Next charge'),
      '#collapsible' => FALSE,
      '#description' => t('Specify when the next charge should occur.'),
      '#attributes' => array('class' => array('interval-fieldset')),
    );
    if (module_exists('date_popup')) {
      $form['next_charge']['next_charge_time'] = array(
        '#type' => 'date_popup',
        '#title' => t('Date/Time'),
        '#date_format' => 'm/d/Y H:i',
        '#default_value' => date('Y-m-d H:i', $fee->next_charge),
        '#timezone' => 'UTC',
      );
    }
    else {
      $form['next_charge']['next_charge_date'] = array(
        '#type' => 'date',
        '#title' => t('Date'),
        '#default_value' => array(
          'year' => date('Y', $fee->next_charge),
          'month' => date('n', $fee->next_charge),
          'day' => date('j', $fee->next_charge),
        ),
      );
    }

    $form['regular'] = array(
      '#type' => 'fieldset',
      '#title' => t('Regular interval'),
      '#collapsible' => FALSE,
      '#description' => t('Modify the length of the billing period for this fee. Changing this value will reset the timer for the next charge. You can also charge the fee manually to collect payment ahead of time and reset the interval.'),
      '#attributes' => array('class' => array('interval-fieldset')),
    );
    // There are different interval settings for recurring orders & products.
    if ($fee->module == 'uc_recurring_product') {
      list($fee->regular_interval_value, $fee->regular_interval_unit) = explode(' ', $fee->regular_interval);

      $form['regular']['regular_interval_value'] = array(
        '#type' => 'select',
        '#options' => drupal_map_assoc(range(1, 52)),
        '#default_value' => $fee->regular_interval_value,
      );
      $form['regular']['regular_interval_unit'] = array(
        '#type' => 'select',
        '#options' => array(
          'days' => t('day(s)'),
          'weeks' => t('week(s)'),
          'months' => t('month(s)'),
          'years' => t('year(s)'),
        ),
        '#default_value' => $fee->regular_interval_unit,
      );
    }
    else {
      $form['regular']['regular_interval'] = array(
        '#type' => 'select',
        '#options' => uc_recurring_order_get_intervals(),
        '#default_value' => $fee->regular_interval,
      );
    }
    $form['reset_next_charge'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset the next charge timer upon form submission using the specified interval.'),
      '#description' => t('This will ignore the next charge field above and instead reset the next charge date to one regular interval from now.'),
      '#attributes' => array('class' => array('reset-next-charge')),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#suffix' => l(t('Cancel'), request_uri()),
    );
    return $form;
  }
  else {
    drupal_set_message(t('Invalid fee handler was given, try again from the operations for this <a href="@url">recurring fee</a>, if problem persists contact the site administrator.', array('@url' => url(request_uri()))), 'error');
  }
}

function rebilling_num_intervals_callback($form, $form_state) {  
  return $form['num_interval']['number_intervals'];
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function uc_recurring_admin_edit_form_validate($form, &$form_state) {
  //dpm($form_state);
  if (($form_state['values']['unlimited_intervals'] == 0 || empty($form_state['values']['unlimited_intervals'])) && intval($form_state['values']['number_intervals']) < 1) {
    form_set_error('number_intervals', t('Only positive whole number values are accepted for the number of billing periods.'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function uc_recurring_admin_edit_form_submit($form, &$form_state) {
  global $user;
  $rfid = $form_state['values']['rfid'];
  $fee = uc_recurring_fee_user_load($rfid);

  $member = user_load($fee->uid);

  // There are different interval settings for recurring orders & products.
  $interval = isset($form_state['values']['regular_interval']) ? $form_state['values']['regular_interval'] : $form_state['values']['regular_interval_value'] . ' ' . $form_state['values']['regular_interval_unit'];
  $number_intervals = empty($form_state['values']['unlimited_intervals']) ? $form_state['values']['number_intervals'] : UC_RECURRING_UNLIMITED_INTERVALS;
  $fee_amount = $form_state['values']['fee_amount'];
  $fee_title = $form_state['values']['fee_title'];
  $fee_status = $form_state['values']['fee_status'];

  $next_charge = $fee->next_charge;
  if ($form_state['values']['reset_next_charge']) {
    $next_charge = strtotime('+' . $interval);
  }
  elseif (!empty($form_state['values']['next_charge_time'])) {
    $next_charge = strtotime($form_state['values']['next_charge_time']);
  }
  elseif (!empty($form_state['values']['next_charge_date'])) {
    extract($form_state['values']['next_charge_date']);
    $hour = date('H', $fee->next_charge);
    $min = date('i', $fee->next_charge);
    $sec = date('s', $fee->next_charge);
    $next_charge = mktime($hour, $min, $sec, $month, $day, $year);
  }

  $status = $fee->status;
  if ($fee->status != UC_RECURRING_FEE_STATUS_ACTIVE && $number_intervals != 0) { 
    $status = UC_RECURRING_FEE_STATUS_ACTIVE;
  }

  // todo - replace with a drupal_write_record()
  db_update('uc_recurring_users')
    ->fields(array(
      'fee_title' => $fee_title,
      'fee_amount' => $fee_amount,
      'regular_interval' => $interval,
      'remaining_intervals' => $number_intervals,
      'next_charge' => $next_charge,
      'status' => $status,
    ))
    ->condition('rfid', $rfid)
    ->execute();

  $context = array(
    'revision' => 'themed-original',
    'location' => 'recurring-admin',
  );
  drupal_set_message(t('The changes to <a href="@user-link">@user</a> recurring fee @rfid (@title) has been saved. The next charge of !fee_amount will be on @date.', array('@user' => $member->name, '@user-link' => url('user/' . $member->uid), '@rfid' => $rfid, '@title' => $fee_title, '!fee_amount' => theme('uc_price',  array('price' => $fee_amount)), '@date' => format_date($next_charge))));

  // Fee was already cancelled and now has been resumed, add a comment to the order.
  if ($fee->status != $status && $status == UC_RECURRING_FEE_STATUS_ACTIVE) {
    uc_order_comment_save($fee->order_id, $user->uid, t('<a href="@user-link">@user</a> resumed the recurring fee <a href="@fee-link">@fee</a>.', array('@user-link' => url('user/'. $user->uid), '@user' => $user->name, '@fee-link' => url('admin/store/orders/recurring/view/fee/'. $rfid), '@fee'=> $rfid)));
  }
  // If the number of intervals was set to zero then also call cancel.
  if ($fee->status == UC_RECURRING_FEE_STATUS_ACTIVE && $number_intervals == 0) {
    uc_recurring_fee_cancel($rfid, $fee);
  }
}

/**
 * Recurring payment settings form.
 */
function uc_recurring_payment_form($form, &$form_state) {
  $options = array();
  $methods = uc_recurring_get_recurring_info();
  foreach ($methods as $fee_handler => $method) {
    if (!uc_recurring_payment_method_supported($fee_handler)) {
      continue;
    }
    if (isset($method['payment method']) && $fee_handler == $method['payment method']) {
      $options[$fee_handler] = $method['name'] . ' (' . $method['fee handler'] . ')';
    }
  }
  if (empty($options)) {
    drupal_set_message('No supported payment methods found. Recurring cannot be collected without a supported payment method.', 'error');
  }
  $form['uc_recurring_payment_methods'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Valid payment methods for orders with recurring fees'),
    '#description' => t('Only selected payment methods will be available for customers purchasing products with recurring fees.<br/>It is up to you to make sure your chosen handler is compatible with the payment methods you select.<br />For example, the uc_recurring handler is only compatible with the Credit Card payment method.'),
    '#options' => $options,
    '#default_value' => variable_get('uc_recurring_payment_methods', array()),
  );
  $form['uc_recurring_checkout_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Recurring fee checkout form message'),
    '#description' => t('Enter a message to be displayed on the checkout form page when a customer has products in the cart with recurring fees.<br />Leave blank to not display any message.'),
    '#default_value' => variable_get('uc_recurring_checkout_message', ''),
  );
  $form['uc_recurring_trigger_renewals'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable triggered renewals'),
    '#description' => t('Enable recurring fees to be triggered on cron jobs. Disabling this is a simple way to stop recurring billing from happening for gateways where uc_recurring is responsible for triggering payments (this is especially useful when working on a development copy).'),
    '#default_value' => variable_get('uc_recurring_trigger_renewals', TRUE),
  );
  $form['uc_recurring_checkout_process'] = array(
    '#type' => 'checkbox',
    '#title' => t('Attempt to process recurring fees during checkout.'),
    '#description' => t('If not selected, you must have an alternate way of processing fees.<br />With the default handler, this is only possible in credit card debug mode.'),
    '#default_value' => variable_get('uc_recurring_checkout_process', TRUE),
  );
  $form['uc_recurring_checkout_fail'] = array(
    '#type' => 'radios',
    '#title' => t('Action to take if a recurring fee fails to process during checkout'),
    '#description' => t('Regardless of your selection, an admin comment will report the failure.<br/><strong>Note:</strong> Even if you select the first option, checkout will complete if another payment has already been captured.'),
    '#options' => array(
      'fail' => t('Return a failed message and do not complete checkout.'),
      'proceed' => t('Return a failed message but complete checkout.'),
      'silent' => t('Show no message and complete checkout.'),
    ),
    '#default_value' => variable_get('uc_recurring_checkout_fail', 'fail'),
  );

  $form['extensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extensions'),
    '#description' => t('Configure how many time and at what intervals to attempt rebilling on a failed recurring payment.'),
  );
  $extensions = uc_recurring_get_extension_list();
  $extend_days = array();
  foreach ($extensions as $ext) {
    $days = $ext->time_to_extend / (24 * 60 * 60);
    $rows[] = array(
      'attempt' => $ext->rebill_attempt + 1,
      'time_to_extent' => $ext->time_to_extend == 0 ? t('expire') : t('@num_days days', array('@num_days' => $days)),
    );
    if ($ext->time_to_extend == 0) {
      break;
    }
    $extend_days[] = $days;
  }
  $form['extensions']['default'] = array(
    '#value' => theme('table', array('header' => array(
        'header' => array(t('Attempt #'), t('On a failed payment re-attempt renew after')),
        'rows' => $rows,
        'attributes' => array('style' => "width: auto;"),
      ))),
  );
  $form['extensions']['edit_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Reattempt on Failure'),
    '#description' => t('Enter comma (,) seperated list of days to reattempt failed recurring charges, for example: 3,5 would mean that if the renewal failed it would first be extended by 3 days and re-attempted again, if that failed it would be extended for another 5 days, if the final attempt failed it would expire.<br/><br/>Note: that this feature only works when renewals are processed through a gateway that Ubercart Recurring Fees actually triggers. That usually means if you are using a hosted gateway (e.g. Paypal Subscriptions) these settings will not be used, instead the default renewal settings of that gateway will be used.'),
    '#default_value' => implode(',', $extend_days),
  );

  $form['#submit'][] = 'uc_recurring_payment_form_save_extensions';

  return system_settings_form($form);
}

/**
 * Saves the extension options.
 */
function uc_recurring_payment_form_save_extensions($form, &$form_state) {
  if ($form_state['values']['submit'] == t('Save configuration')) {
    uc_recurring_save_extensions($form_state['values']['edit_extensions']);
  }
}

/**
 * Display recurring information about this order.
 */
function uc_recurring_order_information($order_id) {
  $order = uc_order_load($order_id);

  $order_id = db_query("SELECT original_order_id FROM {uc_recurring_orders} WHERE renewal_order_id = :renewal_order_id", array(':renewal_order_id' => $order_id))->fetchField();
  if (!empty($order_id)) {
    $order = uc_order_load($order_id);
  }

  // Recurring fee details.
  $output = '';
  if (!empty($order)) {
    $fees = uc_recurring_get_fees($order);
    $output .= '<h2>' . t('Original order ID: @order_id', array('@order_id' => $order->order_id)) . '</h2>';
    $output .= theme('uc_recurring_admin_table', array('fees' => $fees));

    $output .= '<h2>' . t('Renewals') . '</h2>';
    module_load_include('inc', 'uc_order', 'uc_order.admin');

    // We can't use a subquery here because they don't play nice with extenders
    // and this will be a pager query. See http://drupal.org/node/753084
    // So just pass in the order ID's.
    $renewal_orders = db_query("SELECT renewal_order_id FROM {uc_recurring_orders} WHERE original_order_id = :order_id", array(':order_id' => $order->order_id))->fetchCol();
    // Because of the way this works if we don't have any orders we need to
    // make sure no orders show up in the results. We can't send an empty array.
    if (empty($renewal_orders)) {
      $renewal_orders[] = -1;
    }
    $condition = db_and();
    $condition->condition('o.order_id', $renewal_orders, 'IN');

    $output .= drupal_render(uc_recurring_order_admin($condition, TRUE));
  }
  else {
    $output = t('No recurring fees associated with this order');
  }

  return $output;
}


/**
 * Latest version of ubercart 3.x doesn't have this function anymore.
 * (uses views instead to create the admin interface)
 * However uc_recurring currently still looks for this function (see http://drupal.org/node/1283352)
 * 
 * As a temporary work-around, this function was copied 
 * from ubercart-7.x-3.0-beta4/uc_order/uc_order.admin.inc
 * 
 */

/**
 * Displays the main order admin screen, an overview of all received orders.
 */
function uc_recurring_order_admin($condition = NULL, $search = FALSE) {
  $header = array(
    array('data' => t('Actions')),
    array('data' => t('Order ID'), 'field' => 'o.order_id', 'sort' => 'desc'),
    array('data' => t('Customer')),
    array('data' => t('Total'), 'align' => 'center', 'field' => 'o.order_total'),
    array('data' => t('Purchase date'), 'align' => 'center', 'field' => 'o.created'),
    array('data' => t('Status'), 'field' => 'os.title'),
  );

  $address = variable_get('uc_customer_list_address', 'billing') == 'shipping' ? 'delivery' : 'billing';

  $query = db_select('uc_orders', 'o')
    ->fields('o', array(
      'order_id',
      'uid',
      'order_total',
      'created',
      $address . '_first_name',
      $address . '_last_name',
      'order_status',
    ));

  $query->leftJoin('uc_order_statuses', 'os', 'o.order_status = os.order_status_id');
  $query->fields('os', array('title'));

  if (!is_null($condition) && $condition->count()) {
    $query->condition($condition);
  }

  if (!isset($_GET['status']) || $_GET['status'] != 'all') {
    if (!empty($_GET['status']) && is_string($_GET['status'])) {
      $query->condition('o.order_status', $_GET['status']);
    }
    else {
      $query->condition('o.order_status', uc_order_status_list('general', TRUE), 'IN');
    }
  }

  $query = $query->extend('PagerDefault')->extend('TableSort')
    ->orderByHeader($header)
    ->limit(variable_get('uc_order_number_displayed', 30));

  $rows = array();

  $result = $query->execute();
  foreach ($result as $order) {
    $order_name = trim($order->{$address . '_first_name'} . ' ' . $order->{$address . '_last_name'});
    if (!$order_name) {
      if ($order->uid && $account = db_query("SELECT name FROM {users} WHERE uid = :uid", array(':uid' => $order->uid))->fetchField()) {
        $order_name = t('User: !name', array('!name' => $account));
      }
      else {
        $order_name = t('User: none');
      }
    }

    $rows[] = array(
      'data' => array(
        array('data' => uc_order_actions($order, TRUE), 'nowrap' => 'nowrap'),
        array('data' => $order->order_id),
        array('data' => check_plain($order_name), 'nowrap' => 'nowrap'),
        array('data' => array('#theme' => 'uc_price', '#price' => $order->order_total), 'align' => 'right'),
        array('data' => format_date($order->created, 'uc_store'), 'align' => 'center'),
        array('data' => $order->title),
      ),
      'id' => 'order-' . $order->order_id,
    );
  }

  drupal_add_js(array(
    'ucURL' => array(
      'adminOrders' => url('admin/store/orders/'),
    ),
  ), 'setting');
  drupal_add_js(drupal_get_path('module', 'uc_order') . '/uc_order.js');

  $build = array();

  if ($search === FALSE) {
    $build['order_overview_select_form'] = drupal_get_form('uc_order_select_form') + array(
      '#prefix' => '<div class="order-overview-form">',
      '#suffix' => '</div>',
    );
    $build['order_overview_admin_sort_form'] = drupal_get_form('uc_order_admin_sort_form') + array(
      '#prefix' => '<div class="order-overview-form">',
      '#suffix' => '</div>',
    );
  }

  $build['orders'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-orders-table')),
    '#weight' => 1,
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#element' => 0,
    '#weight' => 5,
  );

  return $build;
}
