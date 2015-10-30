<?php

/**
 * @file
 * Hooks provided by the Payment module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Takes action when a payment is entered for an order.
 *
 * @param $order
 *   The order object.
 * @param $method
 *   The name of the payment method used.
 * @param $amount
 *   The value of the payment.
 * @param $account
 *   The user account that entered the order. When the payment is entered
 *   during checkout, this is probably the order's user. Otherwise, it is
 *   likely a store administrator.
 * @param $data
 *   Extra data associated with the transaction.
 * @param $comment
 *   Any comments from the user about the transaction.
 */
function hook_uc_payment_entered($order, $method, $amount, $account, $data, $comment) {
  drupal_set_message(t('User @uid entered a @method payment of @amount for order @order_id.',
    array(
      '@uid' => $account->uid,
      '@method' => $method,
      '@amount' => uc_currency_format($amount),
      '@order_id' => $order->order_id,
    ))
  );
}

/**
 * Registers credit card payment gateway callbacks.
 *
 * Payment gateways handle credit card payments directly, without needing to
 * redirect off-site.
 *
 * @see http://www.ubercart.org/docs/api/hook_uc_payment_gateway
 * @see hook_uc_payment_gateway_charge()
 *
 * @return
 *   Returns an array of payment gateways, keyed by the gateway ID, and with
 *   the following members:
 *   - "title": the human-readable name of the payment method.
 *   - "description": a human-readable description of the payment method.
 *   - "settings": A callback function that returns the gateway settings form.
 *   - "credit": A callback function that processes the credit card. See
 *     hook_uc_payment_gateway_charge() for details.
 */
function hook_uc_payment_gateway() {
  $gateways['test_gateway'] = array(
    'title' => t('Test gateway'),
    'description' => t('Process credit card payments through the Test Gateway.'),
    'credit' => 'test_gateway_charge',
  );
  return $gateways;
}

/**
 * Credit card charge callback.
 *
 * Called when a credit card should be processed. Credit card details supplied
 * by the user are available in $order->payment_details[].
 *
 * @see hook_uc_payment_gateway()
 * @see uc_authorizenet_charge()
 * @see test_gateway_charge()
 *
 * @param $order_id
 *   The order ID that the payment relates to.
 * @param $amount
 *   The amount that should be charged.
 * @param $data
 *   An array of data related to the charge. By default, includes a 'txn_type'
 *   key which defines the transaction type, usually UC_CREDIT_AUTH_ONLY
 *   or UC_CREDIT_AUTH_CAPTURE.
 *
 * @return
 *   Returns an associative array with the following members:
 *   - "success": TRUE if the transaction succeeded, FALSE otherwise.
 *   - "message": a human-readable message describing the result of the
 *     transaction.
 *   - "log_payment": TRUE if the transaction should be regarded as a
 *     successful payment.
 */
function hook_uc_payment_gateway_charge($order_id, $amount, $data) {
}

/**
 * Alters payment gateways.
 *
 * @param $gateways
 *   Array of payment gateways passed by reference.  Array is structured like
 *   the return value of hook_uc_payment_gateway().
 */
function hook_uc_payment_gateway_alter(&$gateways) {
  // Change the title of the test gateway.
  $gateways['test_gateway']['title'] = t('Altered test gateway title.');
}

/**
 * Registers callbacks for payment methods.
 *
 * Payment methods are different ways to collect payment. By default, Ubercart
 * comes with support for check, credit card, and generic payments. Payment
 * methods show up at checkout or on the order administration screens, and they
 * collect different sorts of information from the user that is used to process
 * or track the payment.
 *
 * @see hook_uc_payment_method_callback()
 *
 * @return
 *   An array of payment methods. The array contains a sub-array for each
 *   payment method, with the machine-readable type name as the key. Required
 *   attributes:
 *   - "name": the human-readable name of the payment method.
 *   - "title": the human-readable title of the payment method, displayed
 *     during checkout.
 *   - "desc": a human-readable description of the payment method.
 *   - "callback": a callback function that handles operations that the method
 *     may need to perform. See hook_uc_payment_method_callback()
 *   - "weight": the default weight of the payment method.
 *   - "checkout": if TRUE, the payment method will be enabled by default.
 *   - "no_gateway": should be set to TRUE, except for uc_credit which uses
 *     payment gateways.
 *   - "redirect": if set, this payment method redirects off site; this key
 *     specifies a callback function which will be used to generate the form
 *     that redirects the user to the payment gateway pages.
 */
function hook_uc_payment_method() {
  $methods['check'] = array(
    'name' => t('Check'),
    'title' => t('Check or money order'),
    'desc' => t('Pay by mailing a check or money order.'),
    'callback' => 'uc_payment_method_callback',
    'weight' => 1,
    'checkout' => TRUE,
  );
  return $methods;
}

/**
 * Callback function to perform various operations for a payment method.
 *
 * Possible operations are as follows:
 * - "cart-details": The payment method has been selected at checkout. Return
 *   a form or render array to be displayed in the payment method pane.
 * - "cart-process": Called when the user submits the checkout form with this
 *   payment method selected, used to process any form elements output by the
 *   'cart-details' op. Return FALSE to abort the checkout process, or NULL or
 *   TRUE to continue with checkout.
 * - "cart-review": Called when the checkout review page is being displayed.
 *   Return an array of data to be displayed below the payment method title on
 *   the checkout review page.
 * - "customer-view": Called when the order is being displayed to a customer.
 *   Return a render array to be displayed to customers.
 * - "order-delete": Called when an order is being deleted. Payment methods
 *   should clean up any extra data they stored related to the order.
 * - "order-details": Called when an order is being edited by an administrator.
 *   Return a string or a form array to be displayed to the administator.
 * - "order-load": Called from hook_uc_order('load') when this payment method
 *   is selected for the order.
 * - "order-process": Called when an order has been edited by an administrator.
 *   Process any form elements returned by the "order-details" op.
 * - "order-save": Called from hook_uc_order('save') when this payment method
 *   is selected for the order.
 * - "order-submit": Called from hook_uc_order('submit') when this payment
 *   method is selected for the order.
 * - "order-view": Called when the order is being displayed on the order admin
 *   pages. Return a render array to be displayed to administrators.
 * - "settings": Called when the payment methods page is being displayed.
 *   Return a system settings form array to configure the payment method.
 *
 * @see hook_uc_payment_method()
 *
 * @param $op
 *   The operation being performed.
 * @param &$order
 *   The order object that relates to this operation.
 * @param $form
 *   Where applicable, the form object that relates to this operation.
 * @param &$form_state
 *   Where applicable, the form state that relates to this operation.
 *
 * @return
 *   Dependent on $op.
 */
function hook_uc_payment_method_callback($op, &$order, $form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'cart-details':
      return array('#markup' => t('Continue with checkout to complete payment.'));

    case 'settings':
      $form['uc_payment_method_account_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Payment gateway account number'),
        '#default_value' => variable_get('uc_payment_method_account_number', ''),
      );
      return $form;
  }
}

/**
 * Alter payment methods.
 *
 * @param $methods
 *   Array of payment methods passed by reference.  Array is structured like
 *   the return value of hook_uc_payment_method().
 */
function hook_uc_payment_method_alter(&$methods) {
  // Change the title of the Check payment method.
  $methods['check']['title'] = t('Cheque');
}

/**
 * Alter payment methods available at checkout.
 *
 * @param $methods
 *   Array of payment methods passed by reference. Keys are payment method IDs,
 *   strings are payment method titles.
 * @param $order
 *   The order that is being checked out.
 */
function hook_uc_payment_method_checkout_alter(&$methods, $order) {
  // Remove the Check payment method for orders under $100.
  if ($order->order_total < 100) {
    unset($methods['check']);
  }
}

/**
 * @} End of "addtogroup hooks".
 */
