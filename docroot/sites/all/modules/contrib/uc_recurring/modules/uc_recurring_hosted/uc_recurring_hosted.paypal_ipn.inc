<?php

/**
 * @file
 * Handle paypal IPN callbacks for recurring payments
 */

/**
 * Handle IPN callbacks for PayPal recurring payments
 */
function uc_recurring_hosted_paypal_ipn($order_id = NULL) {
  // Assign posted variables to local variables
  $ipn = (object) $_POST;
  $ipn->received = REQUEST_TIME;

  if (!$order_id) {
    $order_id = $ipn->rp_invoice_id;
  }

  watchdog('uc_recurring_hosted', 'Receiving recurring IPN at URL for order @order_id. !debug',
    array('@order_id' => $order_id, '!debug' => variable_get('uc_paypal_wps_debug_ipn', FALSE) ? '<pre>' . check_plain(print_r($_POST, TRUE)) . '</pre>' : ''));

  $order = _uc_recurring_hosted_paypal_ipn_order($ipn);

  if ($order == FALSE) {
    watchdog('uc_recurring_hosted', 'IPN attempted for non-existent order.', array(), WATCHDOG_ERROR);
    return;
  }

  $ipn->order_id = $order->order_id;

  // Log the IPN against the actual order if debug enabled.
  if (variable_get('uc_paypal_wps_debug_ipn', FALSE)) {
    uc_order_log_changes($order->order_id, array('<pre>' . print_r($ipn, TRUE) . '</pre>'));
  }

  if (_uc_recurring_hosted_paypal_ipn_validate($ipn)) {
    watchdog('uc_recurring_hosted', 'Paypal IPN transaction type %txn_type for order id @order_id', array('%txn_type' => $ipn->txn_type, '@order_id' => $order->order_id));
    switch ($ipn->txn_type) {
      case 'subscr_signup':
      case 'recurring_payment_profile_created':
        // First we need to create the recurring fee as paypal_wps overrides the
        // submit order function where this normally happens.
        uc_recurring_product_process_order($order);
        // Subscriptions do not have a txn_id, so we will use the subscr_id.
        $ipn->txn_id = $ipn->subscr_id;
        break;
      case 'subscr_payment':
      case 'recurring_payment':
        // If the order already has been paid for then this must be a renewal.
        if (uc_payment_balance($order) <= 0) {
          // Fetch fee from database.
          $fees = uc_recurring_get_fees($order);
          foreach ($fees as $fee) {
            $fee->ipn = $ipn;
            uc_recurring_renew($fee);
          }
        }
        else {
          // Process the first subscription payment.
          _uc_recurring_hosted_paypal_ipn_payment($ipn);
        }
        break;
      case 'subscr_failed':
        // Calculate when the next retry will be and then extend.
        $retry_at = strtotime(check_plain($_POST['retry_at']));
        $fees = uc_recurring_get_fees($order);
        foreach ($fees as $fee) {
          $extend_seconds = $retry_at - REQUEST_TIME;
          uc_recurring_extend_fee($fee, $extend_seconds);
        }
        return;
      case 'recurring_payment_failed':
        // Calculate when the next retry will be and then extend.
        $next_payment_date = strtotime(check_plain($_POST['NEXTBILLINGDATE']));
        $fees = uc_recurring_get_fees($order);
        foreach ($fees as $fee) {
          $extend_seconds = $next_payment_date - REQUEST_TIME;
          uc_recurring_extend_fee($fee, $extend_seconds);
        }
        return;
      case 'subscr_eot':
      case 'subscr_cancel':
        $fees = uc_recurring_get_fees($order);
        if ($fee = $fees[0]) {
          uc_recurring_fee_cancel($fee->rfid, $fee);
        }
        $ipn->txn_id = check_plain($_POST['subscr_id']); // subscriptions do not have a txn_id, so we will record the subscr_id instead
        break;
      case 'recurring_payment_skipped':
        $ipn->txn_id = $ipn->recurring_payment_id;

        $fees = uc_recurring_get_fees($order);
        foreach ($fees as $fee) {
          $fee->ipn = $ipn;
          uc_recurring_renew($fee);
        }

        break;
      case 'recurring_payment_suspended_due_to_max_failed_payment':
        $ipn->txn_id = $ipn->recurring_payment_id;

        $fees = uc_recurring_get_fees($order);
        foreach ($fees as $fee) {
          uc_recurring_hosted_paypal_wpp_suspension_log($fee, $ipn);
        }
        break;
      case 'recurring_payment_profile_cancel':
        $fees = uc_recurring_get_fees($order);
        if ($fee = $fees[0]) {
          $ipn->txn_id = $ipn->recurring_payment_id;
          if ($fee->remaining_intervals == -1) {
            uc_recurring_fee_cancel($fee->rfid, $fee);
          }
          uc_order_comment_save($fee->order_id, NULL, t('PayPal confirmed cancellation of billing agreement.'));
        }
        break;
    }

    // For some reason paypal sets this as the new order id, which is a fail
    $ipn->order_id = $order->order_id;


    //save our ipn transaction
    _uc_recurring_hosted_paypal_ipn_save($ipn);
  }

  return;
}
/**
 * Handles payment information from IPN message.
 */
function _uc_recurring_hosted_paypal_ipn_payment($ipn) {
  global $user;
  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );
  $options = array('sign' => FALSE);

  switch ($ipn->payment_status) {
    case 'Canceled_Reversal':
      uc_order_comment_save($ipn->order_id, 0, t('PayPal has cancelled the reversal and returned !amount !currency to your account.', array('!amount' => uc_currency_format($ipn->mc_gross, $options['sign']), '!currency' => $ipn->mc_currency)), 'admin');
      break;

    case 'Completed':
      $comment = t('PayPal transaction ID: @txn_id', array('@txn_id' => $ipn->txn_id));
      // save the transaction in the order log
      uc_order_comment_save($ipn->order_id, 0, t('Payment of @amount @currency submitted through PayPal.', array('@amount' => uc_currency_format($ipn->mc_gross, $options['sign']), '@currency' => $ipn->mc_currency)), 'order', 'payment_received');
      uc_order_comment_save($ipn->order_id, 0, t('PayPal IPN reported a payment of @amount @currency.', array('@amount' => uc_currency_format($ipn->mc_gross, $options['sign']), '@currency' => $ipn->mc_currency)));
      uc_payment_enter($ipn->order_id, 'paypal_wps', $ipn->mc_gross, $user->uid, NULL, $comment);
      break;

    case 'Denied':
      uc_order_comment_save($ipn->order_id, 0, t("You have denied the customer's payment."), 'admin');
      break;

    case 'Expired':
      uc_order_comment_save($ipn->order_id, 0, t('The authorization has failed and cannot be captured.'), 'admin');
      break;

    case 'Failed':
      uc_order_comment_save($ipn->order_id, 0, t("The customer's attempted payment from a bank account failed."), 'admin');
      break;

    case 'Pending':
      uc_order_update_status($ipn->order_id, 'paypal_pending');
      uc_order_comment_save($ipn->order_id, 0, t('Payment is pending at PayPal: @reason', array('@reason' => _uc_paypal_pending_message(check_plain($ipn->pending_reason)))), 'admin');
      break;

      // You, the merchant, refunded the payment.
    case 'Refunded':
      $comment = t('PayPal transaction ID: @txn_id', array('@txn_id' => $ipn->txn_id));
      uc_payment_enter($ipn->order_id, 'paypal_wps', $ipn->mc_gross, $ipn->uid, NULL, $comment);
      break;

    case 'Reversed':
      watchdog('uc_recurring_hosted', 'PayPal has reversed a payment!', array(), WATCHDOG_ERROR);
      uc_order_comment_save($ipn->order_id, 0, t('Payment has been reversed by PayPal: @reason', array('@reason' => _uc_paypal_reversal_message($ipn->reason_code))), 'admin');
      break;

    case 'Processed':
      uc_order_comment_save($ipn->order_id, 0, t('A payment has been accepted.'), 'admin');
      break;

    case 'Voided':
      uc_order_comment_save($ipn->order_id, 0, t('The authorization has been voided.'), 'admin');
      break;
      
    default:
      watchdog('uc_recurring_hosted', 'Payment status @status not recognized.  Nothing to do. !debug',
        array('@status' => $ipn->payment_status, '!debug' => variable_get('uc_paypal_wps_debug_ipn', FALSE) ? '<pre>' . check_plain(print_r($_POST, TRUE)) . '</pre>' : ''));
      break;
  }
}

/**
 * Mock page for testing the Paypal verify call.
 */
function _uc_recurring_hosted_paypal_mock_web_page() {
  echo 'VERIFIED';
  exit();
}

// Helper functions for paypal IPN.

/**
 * Loads the order object associated with either a WPS or WPP IPN.
 *
 * @return
 *   Ubercart Order object.
 */
function _uc_recurring_hosted_paypal_ipn_order($ipn) {
  if (!isset($ipn->invoice) && !isset($ipn->rp_invoice_id)) {
    watchdog('uc_recurring_hosted', 'IPN attempted with invalid order ID.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  if (isset($ipn->rp_invoice_id)) {
    // We are using wpp, use rp_invoice_id
    $order_id = intval($_POST['rp_invoice_id']);
  }
  elseif (isset($ipn->invoice)) {
    if (($len = strpos($_POST['invoice'], '-')) > 0) {
      $order_id = intval(substr($_POST['invoice'], 0, $len));
    }
    else {
      $order_id = intval($_POST['invoice']);
    }
  }

  return uc_order_load($order_id);
}

/**
 * Validate Paypal IPN.
 */
function _uc_recurring_hosted_paypal_ipn_validate($ipn) {
  if (_uc_recurring_hosted_paypal_ipn_is_duplicate($ipn)) {
    return FALSE;
  }

  // @todo: any reason we can't use http_build_query() here?
  $req = '';
  foreach ($_POST as $key => $value) {
    $value = urlencode(stripslashes($value));
    $req .= $key . '=' . $value . '&';
  }

  $req .= 'cmd=_notify-validate';

  if (variable_get('uc_paypal_wpp_server', '') == 'https://api-3t.paypal.com/nvp') {
    $host = 'https://www.paypal.com/cgi-bin/webscr';
  }
  else {
    $host = variable_get('uc_paypal_wps_server', 'https://www.sandbox.paypal.com/cgi-bin/webscr');
  }
  $response = drupal_http_request($host, array(
        'headers' => array(),
        'method' => 'POST',
        'data' => $req,
      ));

  // @todo: Change this to property_exists when we have a PHP requirement >= 5.1.
  if (array_key_exists('error', $response)) {
    watchdog('uc_recurring_hosted', 'IPN failed with HTTP error @error, code @code.', array('@error' => $response->error, '@code' => $response->code), WATCHDOG_ERROR);
    return FALSE;
  }

  if (strcmp($response->data, 'VERIFIED') == 0) {
    watchdog('uc_recurring_hosted', 'PayPal Recurring IPN Transaction Verified.');
    return TRUE;
  }
  elseif (strcmp($response->data, 'INVALID') == 0) {
    watchdog('uc_recurring_hosted', 'IPN transaction failed verification.', array(), WATCHDOG_ERROR);
    uc_order_comment_save($ipn->order_id, 0, t('An IPN transaction failed verification for this order.'), 'admin');
  }
  return FALSE;
}

/**
 * Check if we have already recieved a IPN with the same details.
 */
function _uc_recurring_hosted_paypal_ipn_is_duplicate($ipn) {
  if (isset($ipn->txn_id)) {
    $duplicate = db_query("SELECT COUNT(*) FROM {uc_payment_paypal_ipn} WHERE txn_id = :txn_id AND txn_type = :txn_type AND status <> :status", 
        array(
          ':txn_id' => $ipn->txn_id,
          ':txn_type' => $ipn->txn_type,
          ':status' => 'Pending',
        )
      )->fetchField();
    if ($duplicate > 0) {
      watchdog('uc_recurring_hosted', 'IPN (Order:@order_id Txn: @txn_id) has been processed before.', array('@order_id' => $ipn->order_id, '@txn_id' => $ipn->txn_id), WATCHDOG_NOTICE);
      return TRUE;
    }
  }

  return FALSE;
}
/**
 * Paypal IPN save function.
 */
function _uc_recurring_hosted_paypal_ipn_save($ipn) {
  drupal_write_record('uc_payment_paypal_ipn', $ipn);
}
