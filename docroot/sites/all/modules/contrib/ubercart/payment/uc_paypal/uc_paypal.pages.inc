<?php

/**
 * @file
 * Paypal administration menu items.
 */

/**
 * Process Instant Payment Notifiations from PayPal.
 */
function uc_paypal_ipn() {
  if (!isset($_POST['invoice'])) {
    watchdog('uc_paypal', 'IPN attempted with invalid order ID.', array(), WATCHDOG_ERROR);
    return;
  }

  if (strpos($_POST['invoice'], '-') > 0) {
    list($order_id, $cart_id) = explode('-', $_POST['invoice']);

    // Sanitize order ID and cart ID
    $order_id = intval($order_id);
    $cart_id  = check_plain($cart_id);

    if (!empty($cart_id)) {
      // Needed later by uc_complete_sale to empty the correct cart
      $_SESSION['uc_cart_id'] = $cart_id;
    }
  }
  else {
    $order_id = intval($_POST['invoice']);
  }

  watchdog('uc_paypal', 'Receiving IPN at URL for order @order_id. <pre>@debug</pre>', array('@order_id' => $order_id, '@debug' => variable_get('uc_paypal_wps_debug_ipn', FALSE) ? print_r($_POST, TRUE) : ''));

  $order = uc_order_load($order_id);

  if ($order == FALSE) {
    watchdog('uc_paypal', 'IPN attempted for non-existent order @order_id.', array('@order_id' => $order_id), WATCHDOG_ERROR);
    return;
  }

  // Assign posted variables to local variables
  $payment_status = check_plain($_POST['payment_status']);
  $payment_amount = check_plain($_POST['mc_gross']);
  $payment_currency = check_plain($_POST['mc_currency']);
  $payment_fee = check_plain($_POST['mc_fee']);
  $receiver_email = check_plain($_POST['business']);
  if ($receiver_email == '') {
    $receiver_email = check_plain($_POST['receiver_email']);
  }
  $txn_id = check_plain($_POST['txn_id']);
  $txn_type = check_plain($_POST['txn_type']);
  $payer_email = check_plain($_POST['payer_email']);

  // Express Checkout IPNs may not have the WPS email stored. But if it is,
  // make sure that the right account is being paid.
  $uc_paypal_wps_email = variable_get('uc_paypal_wps_email', '');
  if (!empty($uc_paypal_wps_email) && drupal_strtolower($receiver_email) != drupal_strtolower($uc_paypal_wps_email)) {
    watchdog('uc_paypal', 'IPN for a different PayPal account attempted.', array(), WATCHDOG_ERROR);
    return;
  }

  $req = '';

  foreach ($_POST as $key => $value) {
    $value = urlencode(stripslashes($value));
    $req .= $key .'='. $value .'&';
  }

  $req .= 'cmd=_notify-validate';

  if (variable_get('uc_paypal_wpp_server', '') == 'https://api-3t.paypal.com/nvp') {
    $host = 'https://www.paypal.com/cgi-bin/webscr';
  }
  else {
    $host = variable_get('uc_paypal_wps_server', 'https://www.sandbox.paypal.com/cgi-bin/webscr');
  }

  $response = drupal_http_request($host, array(), 'POST', $req);

  // TODO: Change this to property_exists when we have a PHP requirement >= 5.1.
  if (array_key_exists('error', $response)) {
    watchdog('uc_paypal', 'IPN failed with HTTP error @error, code @code.', array('@error' => $response->error, '@code' => $response->code), WATCHDOG_ERROR);
    return;
  }

  if (strcmp($response->data, 'VERIFIED') == 0) {
    watchdog('uc_paypal', 'IPN transaction verified.');

    $duplicate = db_result(db_query("SELECT COUNT(*) FROM {uc_payment_paypal_ipn} WHERE txn_id = '%s' AND status <> 'Pending'", $txn_id));
    if ($duplicate > 0) {
      if ($order->payment_method != 'credit') {
        watchdog('uc_paypal', 'IPN transaction ID has been processed before.', array(), WATCHDOG_NOTICE);
      }
      return;
    }

    db_query("INSERT INTO {uc_payment_paypal_ipn} (order_id, txn_id, txn_type, mc_gross, status, receiver_email, payer_email, received) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', %d)",
      $order_id, $txn_id, $txn_type, $payment_amount, $payment_status, $receiver_email, $payer_email, time());

    $context = array(
      'revision' => 'formatted-original',
      'type' => 'amount',
    );
    $options = array(
      'sign' => FALSE,
    );

    switch ($payment_status) {
      case 'Canceled_Reversal':
        uc_order_comment_save($order_id, 0, t('PayPal has cancelled the reversal and returned !amount !currency to your account.', array('!amount' => uc_price($payment_amount, $context, $options), '!currency' => $payment_currency)), 'admin');
        break;

      case 'Completed':
        if (abs($payment_amount - $order->order_total) > 0.01) {
          watchdog('uc_paypal', 'Payment @txn_id for order @order_id did not equal the order total.', array('@txn_id' => $txn_id, '@order_id' => $order->order_id), WATCHDOG_WARNING, l(t('view'), 'admin/store/orders/'. $order->order_id));
        }
        $comment = t('PayPal transaction ID: @txn_id', array('@txn_id' => $txn_id));
        uc_payment_enter($order_id, 'paypal_wps', $payment_amount, $order->uid, NULL, $comment);
        uc_cart_complete_sale($order);
        uc_order_comment_save($order_id, 0, t('PayPal IPN reported a payment of @amount @currency.', array('@amount' => uc_price($payment_amount, $context, $options), '@currency' => $payment_currency)));
        break;

      case 'Denied':
        uc_order_comment_save($order_id, 0, t("You have denied the customer's payment."), 'admin');
        break;

      case 'Expired':
        uc_order_comment_save($order_id, 0, t('The authorization has failed and cannot be captured.'), 'admin');
        break;

      case 'Failed':
        uc_order_comment_save($order_id, 0, t("The customer's attempted payment from a bank account failed."), 'admin');
        break;

      case 'Pending':
        uc_order_update_status($order_id, 'paypal_pending');
        uc_order_comment_save($order_id, 0, t('Payment is pending at PayPal: @reason', array('@reason' => _uc_paypal_pending_message(check_plain($_POST['pending_reason'])))), 'admin');
        break;

      // You, the merchant, refunded the payment.
      case 'Refunded':
        $comment = t('PayPal transaction ID: @txn_id', array('@txn_id' => $txn_id));
        uc_payment_enter($order_id, 'paypal_wps', $payment_amount, $order->uid, NULL, $comment);
        break;

      case 'Reversed':
        watchdog('uc_paypal', 'PayPal has reversed a payment!', array(), WATCHDOG_ERROR);
        uc_order_comment_save($order_id, 0, t('Payment has been reversed by PayPal: @reason', array('@reason' => _uc_paypal_reversal_message(check_plain($_POST['reason_code'])))), 'admin');
        break;

      case 'Processed':
        uc_order_comment_save($order_id, 0, t('A payment has been accepted.'), 'admin');
        break;

      case 'Voided':
        uc_order_comment_save($order_id, 0, t('The authorization has been voided.'), 'admin');
        break;
    }
  }
  elseif (strcmp($response->data, 'INVALID') == 0) {
    watchdog('uc_paypal', 'IPN transaction failed verification.', array(), WATCHDOG_ERROR);
    uc_order_comment_save($order_id, 0, t('An IPN transaction failed verification for this order.'), 'admin');
  }
}

// Handles the review page for Express Checkout Mark Flow.
function uc_paypal_ec_review_redirect() {
  if (!isset($_SESSION['TOKEN']) || ($order = uc_order_load($_SESSION['cart_order'])) == FALSE) {
    unset($_SESSION['cart_order']);
    unset($_SESSION['have_details']);
    unset($_SESSION['TOKEN'], $_SESSION['PAYERID']);
    drupal_set_message(t('An error has occurred in your PayPal payment. Please review your cart and try again.'));
    drupal_goto('cart');
  }

  $nvp_request = array(
    'METHOD' => 'GetExpressCheckoutDetails',
    'TOKEN' => $_SESSION['TOKEN'],
  );

  $nvp_response = uc_paypal_api_request($nvp_request, variable_get('uc_paypal_wpp_server', 'https://api-3t.sandbox.paypal.com/nvp'));

  $_SESSION['PAYERID'] = $nvp_response['PAYERID'];

  drupal_goto('cart/checkout/review');
}

// Handles the review page for Express Checkout Shortcut Flow.
function uc_paypal_ec_review() {
  if (!isset($_SESSION['TOKEN']) || ($order = uc_order_load($_SESSION['cart_order'])) == FALSE) {
    unset($_SESSION['cart_order']);
    unset($_SESSION['have_details']);
    unset($_SESSION['TOKEN'], $_SESSION['PAYERID']);
    drupal_set_message(t('An error has occurred in your PayPal payment. Please review your cart and try again.'));
    drupal_goto('cart');
  }

  if ($_SESSION['have_details'][$order->order_id] !== TRUE) {
    $nvp_request = array(
      'METHOD' => 'GetExpressCheckoutDetails',
      'TOKEN' => $_SESSION['TOKEN'],
    );

    $nvp_response = uc_paypal_api_request($nvp_request, variable_get('uc_paypal_wpp_server', 'https://api-3t.sandbox.paypal.com/nvp'));

    $_SESSION['PAYERID'] = $nvp_response['PAYERID'];

    $shipname = check_plain($nvp_response['SHIPTONAME']);
    if (strpos($shipname, ' ') > 0) {
      $order->delivery_first_name = substr($shipname, 0, strrpos(trim($shipname), ' '));
      $order->delivery_last_name = substr($shipname, strrpos(trim($shipname), ' ') + 1);
    }
    else {
      $order->delivery_first_name = $shipname;
      $order->delivery_last_name = '';
    }

    $country_id = db_result(db_query("SELECT country_id FROM {uc_countries} WHERE country_iso_code_2 = '%s'", $nvp_response['SHIPTOCOUNTRYCODE']));
    $zone = $nvp_response['SHIPTOSTATE'];
    $zone_id = 0;

    if (!empty($country_id) && !empty($zone)) {
      $zone_id = db_result(db_query("SELECT zone_id FROM {uc_zones} WHERE zone_country_id = '%s' AND (zone_code = '%s' OR zone_name = '%s')", $country_id, $zone, $zone));
    }

    $order->delivery_street1 = check_plain($nvp_response['SHIPTOSTREET']);
    $order->delivery_street2 = check_plain($nvp_response['SHIPTOSTREET2']);
    $order->delivery_city = check_plain($nvp_response['SHIPTOCITY']);
    $order->delivery_zone = !empty($zone_id) ? $zone_id : 0;
    $order->delivery_postal_code = check_plain($nvp_response['SHIPTOZIP']);
    $order->delivery_country = !empty($country_id) ? $country_id : 840;

    $order->billing_first_name = check_plain($nvp_response['FIRSTNAME']);
    $order->billing_last_name = check_plain($nvp_response['LASTNAME']);
    $order->billing_street1 = check_plain($nvp_response['EMAIL']);

    if (empty($order->primary_email)) {
      $order->primary_email = $nvp_response['EMAIL'];
    }
    $order->payment_method = 'paypal_ec';

    uc_order_save($order);

    $_SESSION['have_details'][$order->order_id] = TRUE;
  }

  $output = t("Your order is almost complete!  Please fill in the following details and click 'Continue checkout' to finalize the purchase.");

  $output .= drupal_get_form('uc_paypal_ec_review_form', $order);

  return $output;
}

// Returns the form for the custom Review Payment screen for Express Checkout.
function uc_paypal_ec_review_form($form_state, $order) {
  if (module_exists('uc_quote') && variable_get('uc_paypal_ec_review_shipping', TRUE) && uc_order_is_shippable($order)) {
    $result = uc_checkout_pane_quotes('view', $order, NULL);
    $form['panes']['quotes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Calculate shipping'),
      '#collapsible' => FALSE,
    );
    $form['panes']['quotes'] += $result['contents'];
    $form['panes']['quotes']['quote_button']['#value'] = t('Click to refresh shipping options');

    $form['panes']['quotes']['quote_div'] = array(
      '#value' => '<div id="quote"></div>',
    );

    // small hack; automatically looks for shipping quotes on page load
    drupal_add_js("\$(document).ready(function() { \$(' #edit-quote-button').click(); });", 'inline');

    // Fake the checkout form delivery information.
    $form['delivery_first_name'] = array('#type' => 'hidden', '#value' => $order->delivery_first_name);
    $form['delivery_last_name'] = array('#type' => 'hidden', '#value' => $order->delivery_last_name);
    $form['delivery_company'] = array('#type' => 'hidden', '#value' => $order->delivery_company);
    $form['delivery_street1'] = array('#type' => 'hidden', '#value' => $order->delivery_street1);
    $form['delivery_street2'] = array('#type' => 'hidden', '#value' => $order->delivery_street2);
    $form['delivery_city'] = array('#type' => 'hidden', '#value' => $order->delivery_city);
    $form['delivery_postal_code'] = array('#type' => 'hidden', '#value' => $order->delivery_postal_code);
    $form['delivery_phone'] = array('#type' => 'hidden', '#value' => $order->delivery_phone);
    $form['delivery_zone'] = array(
      '#type' => 'select',
      '#options' => drupal_map_assoc(array($order->delivery_zone)),
      '#default_value' => $order->delivery_zone,
      '#attributes' => array('style' => 'display: none;'),
    );
    $form['delivery_country'] = array(
      '#type' => 'select',
      '#options' => drupal_map_assoc(array($order->delivery_country)),
      '#default_value' => $order->delivery_country,
      '#attributes' => array('style' => 'display: none;'),
    );

    $form['shippable'] = array('#type' => 'hidden', '#value' => 'true');
  }

  if (variable_get('uc_paypal_ec_review_company', TRUE)) {
    $form['delivery_company'] = array(
      '#type' => 'textfield',
      '#title' => uc_get_field_name('company'),
      '#description' => uc_order_is_shippable($order) ? t('Leave blank if shipping to a residence.') : '',
      '#default_value' => $order->delivery_company,
    );
  }

  if (variable_get('uc_paypal_ec_review_phone', TRUE)) {
    $form['delivery_phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact phone number'),
      '#default_value' => $order->delivery_phone,
      '#size' => 24,
    );
  }

  if (variable_get('uc_paypal_ec_review_comment', TRUE)) {
    $form['order_comments'] = array(
      '#type' => 'textarea',
      '#title' => t('Order comments'),
      '#description' => t('Special instructions or notes regarding your order.'),
      '#wysiwyg' => FALSE,
    );
  }

  if (empty($form)) {
    drupal_goto('cart/echeckout/submit');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue checkout'),
  );

  return $form;
}

function uc_paypal_ec_review_form_validate($form, &$form_state) {
  if ($form_state['values']['shippable'] == 'true') {
    if (!isset($_POST['quote-option'])) {
      form_set_error('shipping', t('You must calculate and select a shipping option.'));
    }
  }
}

function uc_paypal_ec_review_form_submit($form, &$form_state) {
  $order = uc_order_load($_SESSION['cart_order']);

  if ($form_state['values']['shippable'] == 'true') {
    $method_accessorials= split("---", isset($_POST['quote-option']) ? strval($_POST['quote-option']) : 0);
    $order->quote['method'] = $method_accessorials[0];
    $order->quote['accessorials'] = $method_accessorials[1];
    $order->quote['rate'] = $_POST['rate'][isset($_POST['quote-option']) ? strval($_POST['quote-option']) : 0];

    $order->quote['quote_form'] = rawurldecode($_POST['quote-form']);
    $methods = module_invoke_all('shipping_method');
    $method = $methods[$order->quote['method']];
    $label = is_null($_POST['quote-option']) ? t('Error calculating shipping') : $method['quote']['accessorials'][$order->quote['accessorials']];
    $result = db_query("SELECT line_item_id FROM {uc_order_line_items} WHERE order_id = %d AND type = 'shipping'", $order->order_id);
    if ($lid = db_result($result)) {
      uc_order_update_line_item($lid, $label, $order->quote['rate']);
    }
    else {
      uc_order_line_item_add($order->order_id, 'shipping', $label, $order->quote['rate']);
    }
  }

  if (variable_get('uc_paypal_ec_review_company', TRUE)) {
    $order->delivery_company = $form_state['values']['delivery_company'];
  }

  if (variable_get('uc_paypal_ec_review_phone', TRUE)) {
    $order->delivery_phone = $form_state['values']['delivery_phone'];
  }

  if (variable_get('uc_paypal_ec_review_comment', TRUE)) {
    db_query("DELETE FROM {uc_order_comments} WHERE order_id = %d", $order->order_id);
    uc_order_comment_save($order->order_id, 0, $form_state['values']['order_comments'], 'order');
  }

  uc_order_save($order);

  $form_state['redirect'] = 'cart/echeckout/submit';
}

// Present the final total to the user for checkout!
function uc_paypal_ec_submit() {
  if (!isset($_SESSION['TOKEN']) || ($order = uc_order_load($_SESSION['cart_order'])) == FALSE) {
    unset($_SESSION['cart_order'], $_SESSION['have_details']);
    unset($_SESSION['TOKEN'], $_SESSION['PAYERID']);
    drupal_set_message(t('An error has occurred in your PayPal payment. Please review your cart and try again.'));
    drupal_goto('cart');
  }

  drupal_add_css(drupal_get_path('module', 'uc_cart') .'/uc_cart.css');
  $output = '<div>'. theme('cart_review_table', FALSE) .'</div>';
  $output .= uc_order_pane_line_items('customer', $order);
  $output .= '<p>'. t("Your order is not complete until you click the 'Submit order' button below. Your PayPal account will be charged for the amount shown above once your order is placed. You will receive confirmation once your payment is complete.") .'</p>';
  $output .= drupal_get_form('uc_paypal_ec_submit_form');

  return $output;
}

// Submits an order, calling the NVP API to send the order total to PayPal.
function uc_paypal_ec_submit_form() {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit order'),
  );

  return $form;
}

// Handles a complete Website Payments Standard sale.
function uc_paypal_complete($order) {
  // If the order ID specified in the return URL is not the same as the one in
  // the user's session, we need to assume this is either a spoof or that the
  // user tried to adjust the order on this side while at PayPal. If it was a
  // legitimate checkout, the IPN will still come in from PayPal so the order
  // gets processed correctly. We'll leave an ambiguous message just in case.
  if (intval($_SESSION['cart_order']) != $order->order_id) {
    drupal_set_message(t('Thank you for your order! PayPal will notify us once your payment has been processed.'));
    drupal_goto('cart');
  }

  // Ensure the payment method is PayPal WPS.
  if ($order->payment_method != 'paypal_wps') {
    drupal_goto('cart');
  }

  // This lets us know it's a legitimate access of the complete page.
  $_SESSION['do_complete'] = TRUE;

  drupal_goto('cart/checkout/complete');
}

// Handles a canceled Website Payments Standard sale.
function uc_paypal_cancel() {
  unset($_SESSION['cart_order']);

  drupal_set_message(t('Your PayPal payment was cancelled. Please feel free to continue shopping or contact us for assistance.'));

  drupal_goto(variable_get('uc_paypal_wps_cancel_return_url', 'cart'));
}
