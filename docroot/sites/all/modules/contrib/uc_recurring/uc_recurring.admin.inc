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
  $output .= drupal_get_form('uc_recurring_admin_filter_form', $item);

  // Build an array of fees to include in the admin table.
  $fees = array();

  $footer_link = TRUE;

  $header = array(
    array('data' => t('ID'), 'field' => 'ru.rfid'),
    array('data' => t('Order'), 'field' => 'ru.order_id'),
    array('data' => t('Status'), 'field' => 'ru.status'),
    array('data' => t('Account')),
    array('data' => t('Next'), 'field' => 'ru.next_charge', 'sort' => 'asc'),
    array('data' => t('Amount')),
    array('data' => t('Interval')),
    array('data' => t('Left')),
    array('data' => t('Total')),
    array('data' => t('Operations')),
  );

  // If we're on the overview page listing all fees...
  if (empty($filter)) {
    // Get all recurring fees in the system.
    $order = tablesort_sql($header);
    $fees = uc_recurring_get_all_fees(TRUE, $order);
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
    $output .= theme('uc_recurring_admin_table', $fees);
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
function uc_recurring_admin_filter_form($form_state, $item) {
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

function uc_recurring_admin_filter_form_submit($form, &$form_state) {
  if (intval($form_state['values']['id']) > 0) {
    $form_state['redirect'] = 'admin/store/orders/recurring/view/'. $form_state['values']['type'] .'/'. $form_state['values']['id'];
  }
}


/**
 * Confirm a recurring fee charge.
 *
 * @param $form_state
 * @param $uid
 * @return unknown_type
 */
function uc_recurring_admin_charge_form($form_state, $rfid, $fee_handler) {
  $fee = uc_recurring_fee_user_load($rfid);
  if ($fee->fee_handler == $fee_handler) {
    $context = array(
      'revision' => 'formatted-original',
      'location' => 'recurring-charge-form',
    );
    $form['rfid'] = array('#type' => 'hidden', '#value' => $rfid);
    $form['message'] = array(
      '#value' => '<div>'. t('Are you sure you want to charge the customer @amount at this time?', array('@amount' => uc_price($fee->fee_amount, $context))) .'</div>',
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Process Renewal'),
      '#suffix' => l(t('Cancel'), uc_referer_uri()),
    );
    return $form;
  }
  else {
    drupal_set_message(t('Invalid fee handler was given, try again from the operations for this <a href="@url">recurring fee</a>, if problem persists contact the site administrator.', array('@url' => url(uc_referer_uri()))), 'error');
  }
}

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
function uc_recurring_admin_edit_form($form_state, $rfid, $fee_handler) {
  $fee = uc_recurring_fee_user_load($rfid);
  if ($fee->fee_handler == $fee_handler) {
    drupal_add_css(drupal_get_path('module', 'uc_recurring') .'/uc_recurring.css');
    drupal_add_js(drupal_get_path('module', 'uc_recurring') .'/uc_recurring.js', 'module');

    list($fee->regular_interval_value, $fee->regular_interval_unit) = explode(' ', $fee->regular_interval);

    $form['rfid'] = array('#type' => 'hidden', '#value' => $rfid);
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

    if ($fee->remaining_intervals < 0) {
      $attributes['checked'] = 'checked';
    }
    $form['num_interval']['unlimited_intervals'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unlimited rebillings.'),
      '#attributes' => $attributes,
    );
    $form['num_interval']['number_intervals'] = array(
      '#type' => 'textfield',
      '#title' => t('Remaining billing periods'),
      '#size' => 16,
      '#default_value' => $fee->remaining_intervals < 0 ? '' : $fee->remaining_intervals,
      '#attributes' => $fee->remaining_intervals < 0 ? array('disabled' => 'disabled') : array(),
    );

    $form['next_charge'] = array(
      '#type' => 'fieldset',
      '#title' => t('Next charge'),
      '#collapsible' => FALSE,
      '#description' => t('Specify when the next charge should occur.'),
      '#attributes' => array('class' => 'interval-fieldset'),
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
        '#default_value' => array('year' => date('Y', $fee->next_charge), 'month' => date('n', $fee->next_charge), 'day' => date('j', $fee->next_charge)),
      );
    }

    $form['regular'] = array(
      '#type' => 'fieldset',
      '#title' => t('Regular interval'),
      '#collapsible' => FALSE,
      '#description' => t('Modify the length of the billing period for this fee. Changing this value will reset the timer for the next charge. You can also charge the fee manually to collect payment ahead of time and reset the interval.'),
      '#attributes' => array('class' => 'interval-fieldset'),
    );
    $form['regular']['regular_interval_value'] = array(
      '#type' => 'select',
      '#options' => drupal_map_assoc(uc_range(1, 52)),
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
    $form['reset_next_charge'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset the next charge timer upon form submission using the specified interval.'),
      '#description' => t('This will ignore the next charge field above and instead reset the next charge date to one regular interval from now.'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#suffix' => l(t('Cancel'), uc_referer_uri()),
    );
    return $form;
  }
  else {
    drupal_set_message(t('Invalid fee handler was given, try again from the operations for this <a href="@url">recurring fee</a>, if problem persists contact the site administrator.', array('@url' => url(uc_referer_uri()))), 'error');
  }
}

function uc_recurring_admin_edit_form_validate($form, &$form_state) {
  if (empty($form_state['values']['unlimited_intervals']) && intval($form_state['values']['number_intervals']) < 0) {
    form_set_error('number_intervals', t('Only positive whole number values are accepted for the number of billing periods.'));
  }
}

function uc_recurring_admin_edit_form_submit($form, &$form_state) {
  global $user;
  $rfid = $form_state['values']['rfid'];
  $fee = uc_recurring_fee_user_load($rfid);

  $member = user_load($fee->uid);

  $interval = $form_state['values']['regular_interval_value'] .' '. $form_state['values']['regular_interval_unit'];
  $number_intervals = empty($form_state['values']['unlimited_intervals']) ? $form_state['values']['number_intervals'] : UC_RECURRING_UNLIMITED_INTERVALS;
  $fee_amount = $form_state['values']['fee_amount'];
  $fee_title = $form_state['values']['fee_title'];
  $fee_status = $form_state['values']['fee_status'];

  $next_charge = $fee->next_charge;
  if ($form_state['values']['reset_next_charge']) {
    $next_charge = strtotime('+'. $interval);
  }
  else if (!empty($form_state['values']['next_charge_time'])) {
    $next_charge = strtotime($form_state['values']['next_charge_time']);
  }
  else if (!empty($form_state['values']['next_charge_date'])) {
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
  db_query("UPDATE {uc_recurring_users} SET fee_title = '%s', fee_amount = %f, regular_interval = '%s', "
          ."remaining_intervals = %d, next_charge = %d, status = %d WHERE rfid = %d", $fee_title, $fee_amount,
           $interval, $number_intervals, $next_charge, $status, $rfid);


  $context = array(
    'revision' => 'themed-original',
    'location' => 'recurring-admin',
  );
  drupal_set_message(t('The changes to <a href="@user-link">@user</a> recurring fee @rfid (@title) has been saved. The next charge of !fee_amount will be on @date.', array('@user' => $member->name, '@user-link' => url('user/'. $member->uid), '@rfid' => $rfid, '@title' => $fee_title, '!fee_amount' => uc_price($fee_amount, $context), '@date' => format_date($next_charge))));

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
function uc_recurring_payment_form() {
  $options = array();
  $methods = uc_recurring_get_recurring_info();
  foreach ($methods as $fee_handler => $method) {
    if (!uc_recurring_payment_method_supported($fee_handler)) {
      continue;
    }
    if ($fee_handler == $method['payment method']) {
      $options[$fee_handler] = $method['name'] .' ('. $method['fee handler'] .')';
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
    $days = $ext->time_to_extend / (24*60*60);
    $rows[] = array(
      'attempt' => $ext->rebill_attempt + 1,
      'time_to_extent' => $ext->time_to_extend == 0 ? t('expire') : t('@num_days days', array('@num_days' => $days)),
    );
    if ($ext->time_to_extend == 0) break;
    $extend_days[] = $days;
  }
  $form['extensions']['default'] = array(
    '#value' => theme('table', array(t('Attempt #'), t('On a failed payment re-attempt renew after')), $rows, array('style' => "width: auto;"))
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

  $order_id = db_result(db_query("SELECT original_order_id FROM {uc_recurring_orders} WHERE renewal_order_id = %d", $order_id));
  if (!empty($order_id)) {
    $order = uc_order_load($order_id);
  }

  // Recurring fee details.
  if (!empty($order)) {
    $fees = uc_recurring_get_fees($order);
    $output .= '<h2>'. t('Original order ID: @order_id', array('@order_id' => $order->order_id)) .'</h2>';
    $output .= theme('uc_recurring_admin_table', $fees);

    $header = array(
      array('data' => t('Actions')),
      array('data' => t('Order ID'), 'field' => 'o.order_id', 'sort' => 'desc'),
      array('data' => t('Customer')),
      array('data' => t('Total'), 'align' => 'center', 'field' => 'o.order_total'),
      array('data' => t('Purchase date'), 'align' => 'center', 'field' => 'o.created'),
      array('data' => t('Status'), 'field' => 'os.title'),
    );

    $output .= '<h2>'. t('Renewals') .'</h2>';
    module_load_include('inc', 'uc_order', 'uc_order.admin');
    $sql = 'SELECT o.order_id, o.uid, o.billing_first_name, o.billing_last_name, o.order_total, o.order_status, o.created, os.title
            FROM {uc_orders} o
              LEFT JOIN {uc_order_statuses} os ON o.order_status = os.order_status_id
              LEFT JOIN {uc_recurring_orders} ro ON ro.renewal_order_id=o.order_id
            WHERE ro.original_order_id = %d '. tablesort_sql($header);
    $output .= uc_order_admin($sql, array($order->order_id), TRUE);
  }
  else {
    $output = t('No recurring fees associated with this order');
  }

  return $output;
}
