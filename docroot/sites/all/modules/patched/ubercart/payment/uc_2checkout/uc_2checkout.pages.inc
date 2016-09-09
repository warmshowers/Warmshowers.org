<?php

/**
 * @file
 * 2Checkout menu items.
 */

/**
 * Finalizes 2Checkout transaction.
 */
function uc_2checkout_complete($cart_id = 0) {
  watchdog('uc_2checkout', 'Receiving new order notification for order !order_id.', array('!order_id' => check_plain($_REQUEST['merchant_order_id'])));

  $order = uc_order_load($_REQUEST['merchant_order_id']);

  if ($order === FALSE) {
    return t('An error has occurred during payment.  Please contact us to ensure your order has submitted.');
  }
  $key = $_REQUEST['key'];
  $order_number = variable_get('uc_2checkout_demo', TRUE) ? 1 : $_REQUEST['order_number'];
  $valid = md5(variable_get('uc_2checkout_secret_word', 'tango') . $_REQUEST['sid'] . $order_number . $_REQUEST['total']);
  if (drupal_strtolower($key) != drupal_strtolower($valid)) {
    uc_order_comment_save($order->order_id, 0, t('Attempted unverified 2Checkout completion for this order.'), 'admin');
    return MENU_ACCESS_DENIED;
  }

  if ($_REQUEST['demo'] == 'Y' xor variable_get('uc_2checkout_demo', TRUE)) {
    watchdog('uc_2checkout', 'The 2Checkout payment for order <a href="@order_url">@order_id</a> demo flag was set to %flag, but the module is set to %mode mode.', array(
      '@order_url' => url('admin/store/orders/' . $order->order_id),
      '@order_id' => $order->order_id,
      '%flag' => $_REQUEST['demo'] == 'Y' ? 'Y' : 'N',
      '%mode' => variable_get('uc_2checkout_demo', TRUE) ? 'Y' : 'N',
    ), WATCHDOG_ERROR);

    if (!variable_get('uc_2checkout_demo', TRUE)) {
      return MENU_ACCESS_DENIED;
    }
  }

  $order->billing_street1 = $_REQUEST['street_address'];
  $order->billing_street2 = $_REQUEST['street_address2'];
  $order->city = $_REQUEST['city'];
  $order->billing_postal_code = $_REQUEST['zip'];
  $order->billing_phone = $_REQUEST['phone'];

  $zone_id = db_query("SELECT zone_id FROM {uc_zones} WHERE zone_code LIKE :code", array(':code' => $_REQUEST['state']))->fetchField();
  if (!empty($zone_id)) {
    $order->billing_zone = $zone_id;
  }

  $country_id = db_query("SELECT country_id FROM {uc_countries} WHERE country_name LIKE :name", array(':name' => $_REQUEST['country']))->fetchField();
  if (!empty($country_id)) {
    $order->billing_country = $country_id;
  }

  // Save changes to order without it's completion.
  uc_order_save($order);

  if (drupal_strtolower($_REQUEST['email']) !== drupal_strtolower($order->primary_email)) {
    uc_order_comment_save($order->order_id, 0, t('Customer used a different e-mail address during payment: !email', array('!email' => check_plain($_REQUEST['email']))), 'admin');
  }

  if ($_REQUEST['credit_card_processed'] == 'Y' && is_numeric($_REQUEST['total'])) {
    $comment = t('Paid by !type, 2Checkout.com order #!order.', array('!type' => $_REQUEST['pay_method'] == 'CC' ? t('credit card') : t('echeck'), '!order' => check_plain($_REQUEST['order_number'])));
    uc_payment_enter($order->order_id, '2Checkout', $_REQUEST['total'], 0, NULL, $comment);
  }
  else {
    drupal_set_message(t('Your order will be processed as soon as your payment clears at 2Checkout.com.'));
    uc_order_comment_save($order->order_id, 0, t('!type payment is pending approval at 2Checkout.com.', array('!type' => $_REQUEST['pay_method'] == 'CC' ? t('Credit card') : t('eCheck'))), 'admin');
  }

  // Empty that cart...
  uc_cart_empty($cart_id);

  // Add a comment to let sales team know this came in through the site.
  uc_order_comment_save($order->order_id, 0, t('Order created through website.'), 'admin');

  $build = uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));

  $page = variable_get('uc_cart_checkout_complete_page', '');

  if (!empty($page)) {
    drupal_goto($page);
  }

  return $build;
}

/**
 * React on status changes from 2CO.
 */
function uc_2checkout_process_notification() {
  $values = $_POST;
  watchdog('uc_2checkout', 'Received 2Checkout notification with following data: !data', array('!data' => print_r($values, TRUE)));

  if (!empty($values['message_type']) && !empty($values['md5_hash']) && !empty($values['message_id'])) {
    // Validate the hash.
    $secret_word = variable_get('uc_2checkout_secret_word', 'tango');
    $sid = variable_get('uc_2checkout_sid', '');
    $twocheckout_order_id = $values['sale_id'];
    $twocheckout_invoice_id = $values['invoice_id'];
    $hash = strtoupper(md5($twocheckout_order_id . $sid . $twocheckout_invoice_id . $secret_word));

    if ($hash != $values['md5_hash']) {
      watchdog('uc_2checkout', '2CO notification #@num had a wrong hash.', array('@num' => $values['message_id']));
      die('Hash Incorrect');
    }

    $order_id = $values['vendor_order_id'];
    if ($values['message_type'] == 'FRAUD_STATUS_CHANGED') {
      switch ($values['fraud_status']) {
        case 'fail':
          uc_order_update_status($order_id, uc_order_state_default('canceled'));
          uc_order_comment_save($order_id, 0, t('Order have not passed 2Checkout fraud review.'));
          die('fraud');
          break;
      }
    }
    elseif ($values['message_type'] == 'REFUND_ISSUED') {
      uc_order_update_status($order_id, uc_order_state_default('canceled'));
      uc_order_comment_save($order_id, 0, t('Order have been refunded through 2Checkout.'));
      die('refund');
    }
  }
  die('ok');
}
