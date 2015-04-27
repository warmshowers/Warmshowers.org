<?php

/**
 * @file
 * Uc recurring implementation for the CyberSource module.
 */

/**
 * Implements hook_recurring_info().
 */
function uc_recurring_uc_cybersource_recurring_info() {
  $items['cybersource'] = array(
    'name' => t('CyberSource'),
    'payment method' => 'credit',
    'module' => 'uc_recurring',
    'fee handler' => 'cybersource',
    'renew callback' => 'uc_recurring_cybersource_renew',
    'process callback' => 'uc_recurring_cybersource_process',
    'saved profile' => TRUE,
    'menu' => array(
      'charge' => UC_RECURRING_MENU_DEFAULT,
      'edit' => UC_RECURRING_MENU_DEFAULT,
      'update' => array(
        'title' => 'Update Account Details',
        'page arguments' => array('uc_recurring_cybersource_update_form'),
        'file' => 'includes/uc_recurring.uc_cybersource.inc',
      ),
      'profile' => array(
        'title' => 'Customer Profile',
        'page arguments' => array('uc_recurring_cybersource_profile_form'),
        'access callback' => 'user_access',
        'access arguments' => array('administer recurring fees'),
        'file' => 'includes/uc_recurring.uc_cybersource.inc',
      ),
      'cancel' => UC_RECURRING_MENU_DEFAULT,
    ), // Use the default user operation defined in uc_recurring.
  );
  return $items;
}

/**
 * Set up the recurring fee by creating a CIM profile for future payments
 *
 * @param $order
 *   The order object.
 * @param $fee
 *   The fee object.
 * @return
 *   TRUE if recurring fee setup
 */
function uc_recurring_cybersource_process($order, &$fee) {
  $fee->fee_handler = 'cybersource';
  if (variable_get('uc_cybersource_soap_create_profile', FALSE) == FALSE) {
    // The uc_cybersource module doesn't yet support UC_CREDIT_REFERENCE_SET
    return _uc_recurring_cybersource_create_profile($order);
  }
  return TRUE;
}

/**
 * Process a renewal using the CIM profile.
 *
 * @param $order
 *   The order object.
 * @param $fee
 *   The fee object.
 * @return
 *   TRUE if renewal succeeded
 */
function uc_recurring_cybersource_renew($order, &$fee) {
  if (!empty($order->data['cc_txns']['references'])) {
    $refs = array_keys($order->data['cc_txns']['references']);
    $data = array(
      'txn_type' => UC_CREDIT_REFERENCE_TXN,
      'ref_id' => end($refs),
    );
    $result = uc_cybersource_charge($order->order_id, $order->order_total, $data);

    if ($result['success'] == TRUE) {
      uc_payment_enter($order->order_id, $order->payment_method, $order->order_total, $fee->uid, $result['data'], $result['comment']);
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Create form for updating credit card details for recurring fee.
 */
function uc_recurring_cybersource_update_form($form, $form_state, $rfid) {
  // Load fee.
  $fee = uc_recurring_fee_user_load($rfid);
  // Load corresponding order.
  $order = uc_order_load($fee->order_id);

  $form['rfid'] = array(
    '#type' => 'value',
    '#value' => $rfid,
  );
  $form['cc_data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit card details'),
    '#theme' => 'uc_payment_method_credit_form',
    '#tree' => TRUE,
  );
  $form['cc_data'] = array_merge($form['cc_data'], uc_payment_method_credit_form($form, $form_state, $order));
  unset($form['cc_data']['cc_policy']);

  // Make credit card info form items required
  $form['cc_data']['cc_owner']['#required'] = TRUE;
  $form['cc_data']['cc_number']['#required'] = TRUE;
  $form['cc_data']['cc_exp_month']['#required'] = TRUE;
  $form['cc_data']['cc_exp_year']['#required'] = TRUE;
  $form['cc_data']['cc_cvv']['#required'] = TRUE;

  // Add billing address form
  if ($billing_items = uc_order_pane_bill_to('edit-form', $order)) {
    $form = array_merge($form, $billing_items);
    $form['bill_to']['#title'] = t('Billing address');
    $form['bill_to']['#description'] = t('Credit card information must be provided to update billing address.');
    $form['bill_to']['#collapsible'] = FALSE;
    $form['bill_to']['#theme'] = 'uc_recurring_cybersource_billto_form';
    
    // Make billing info form items required
    $form['bill_to']['billing_first_name']['#required'] = TRUE;
    $form['bill_to']['billing_last_name']['#required'] = TRUE;
    $form['bill_to']['billing_street1']['#required'] = TRUE;
    $form['bill_to']['billing_city']['#required'] = TRUE;
    $form['bill_to']['billing_country']['#required'] = TRUE;
    $form['bill_to']['billing_postal_code']['#required'] = TRUE;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#suffix' => l(t('Cancel'), 'admin/store/orders/recurring/view/fee/' . $rfid),
  );

  return $form;
}

/**
 * Implements update form validation for the cybersource CIM gateway.
 */
function uc_recurring_cybersource_update_form_validate(&$form, &$form_state) {
  $has_errors = FALSE;
  $values = $form_state['values'];
  
  // Make sure an owner value was entered.
  if (variable_get('uc_credit_owner_enabled', FALSE) && empty($values['cc_data']['cc_owner'])) {
    form_set_error('cc_data][cc_owner', t('Enter the owner name as it appears on the card.'));
    $has_errors = TRUE;
  }
  
  // Validate the CC number if that's turned on/check for non-digits.
  if (variable_get('uc_credit_validate_numbers', TRUE)) {
    // Remove the non-numeric characters
    $cc_number = preg_replace('/[^0-9]/', '', $values['cc_data']['cc_number']);
    form_set_value($form['cc_data']['cc_number'], $cc_number, $form_state);
    if (!_uc_credit_valid_card_number($cc_number) || !ctype_digit($cc_number)) {
      form_set_error('cc_data][cc_number', t('You have entered an invalid credit card number.'));
      $has_errors = TRUE;
    }
  }
  
  // Validate the start date (if entered).
  if (variable_get('uc_credit_start_enabled', FALSE) && !_uc_credit_valid_card_start($values['cc_data']['cc_start_month'], $values['cc_data']['cc_start_year'])) {
    form_set_error('cc_data][cc_start_month', t('The start date you entered is invalid.'));
    form_set_error('cc_data][cc_start_year', t('The start date you entered is invalid.'));
    $has_errors = TRUE;
  }
  
  // Validate the card expiration date.
  if (!_uc_credit_valid_card_expiration($values['cc_data']['cc_exp_month'], $values['cc_data']['cc_exp_year'])) {
    form_set_error('cc_data][cc_exp_month', t('The credit card you entered has expired.'));
    form_set_error('cc_data][cc_exp_year', t('The credit card you entered has expired.'));
    $has_errors = TRUE;
  }
  
  // Validate the issue number (if entered).  With issue numbers, '01' is
  // different from '1', but is_numeric() is still appropriate.
  if (variable_get('uc_credit_issue_enabled', FALSE) && !_uc_credit_valid_card_issue($values['cc_data']['cc_issue'])) {
    form_set_error('cc_data][cc_issue', t('The issue number you entered is invalid.'));
    $has_errors = TRUE;
  }

  // Validate the CVV number if enabled.
  if (variable_get('uc_credit_cvv_enabled', TRUE) && !_uc_credit_valid_cvv($values['cc_data']['cc_cvv'])) {
    form_set_error('cc_data][cc_cvv', t('You have entered an invalid CVV number.'));
    $has_errors = TRUE;
  }

  // Validate the bank name if enabled.
  if (variable_get('uc_credit_bank_enabled', FALSE) && empty($values['cc_data']['cc_bank'])) {
    form_set_error('cc_data][cc_bank', t('You must enter the issuing bank for that card.'));
    $has_errors = TRUE;
  }

  //Form requires rebuilding form so codes and expiration blocks will display correctly
  if ($has_errors) {
    // remove duplicate error messages when two fields share a message
    $_SESSION['messages']['error'] = array_unique($_SESSION['messages']['error']);
    $form_state["rebuild"] = TRUE;
  }
}

/**
 * Implements update form submit for the cybersource CIM gateway.
 */
function uc_recurring_cybersource_update_form_submit(&$form, &$form_state) {
  $fee = uc_recurring_fee_user_load($form_state['values']['rfid']);
  $order = uc_order_load($fee->order_id);
  $last4old = substr($order->payment_details['cc_number'], -4);
  $order->payment_details = $form_state['values']['cc_data'];
  $log = array();

  // Process billing address form changes
  if (($bill_changes = uc_order_pane_bill_to('edit-process', $order, $form, $form_state)) != NULL) {
    foreach ($bill_changes as $key => $value) {
      if ($order->$key != $value) {
        if (!is_array($value)) {
          $log[$key] = array('old' => $order->$key, 'new' => $value);
        }
        $order->$key = $value;
      }
    }
  }

  $refs = array_keys($order->data['cc_txns']['references']);
  $subscription_id = end($refs);
  if ($message = _uc_recurring_uc_cybersource_update_paymentprofile($order, $subscription_id)) {
    drupal_set_message(t('Account update failed: @error', array('@error' => $message)), 'error');
    $form_state['redirect'] = FALSE;
  }
  else {
    drupal_set_message(t('Account updated'));
    uc_order_save($order);
    $last4new = substr($order->payment_details['cc_number'], -4);
    if ($last4new != $last4old) {
      $log[t('Credit card')] = array('old' => $last4old, 'new' => $last4new);
    }
    if ($log) {
      uc_order_log_changes($order->order_id, $log);
    }
  }
}

/**
 * Theme billing address form items
 */
function theme_uc_recurring_cybersource_billto_form($form) {
  $output = '<table class="order-edit-table">';
  foreach (element_children($form) as $field) {
    $title = $form[$field]['#title'];
    $form[$field]['#title'] = NULL;
    $output .= '<tr><td class="oet-label">'. $title .':</td><td>' . drupal_render($form[$field]) .'</td></tr>';
  }
  $output .= '</table>';
  
  return $output;
}

/**
 * Update the payment profile
 */
function _uc_recurring_uc_cybersource_update_paymentprofile($order, $subscription_id) {
  module_load_include('inc', 'uc_cybersource', 'uc_cybersource.soap');

  try {
    $soapClient = new CyberSourceSoapClient(_uc_recurring_cybersource_soap_wsdl_url(), array());
    $login = _uc_cybersource_soap_login_data();
    // Create the request with some meta data.
    $request = new stdClass();
    $request->merchantID = $login['merchant_id'];
    $request->merchantReferenceCode = $order->order_id;

    $paySubscriptionUpdateService = new stdClass();
    $paySubscriptionUpdateService->run = 'true';
    $request->paySubscriptionUpdateService = $paySubscriptionUpdateService;

    $recurringSubscriptionInfo = new stdClass();
    $recurringSubscriptionInfo->subscriptionID = $subscription_id;
    $request->recurringSubscriptionInfo = $recurringSubscriptionInfo;

    $request->billTo = _uc_recurring_cybersource_billto_obj($order);
    $request->card = _uc_recurring_cybersource_card_obj($order);

    $reply = $soapClient->runTransaction($request);
  }
  catch (SoapFault $exception) {
    // Log and display errors if Ubercart is unable to connect via SOAP.
    watchdog('uc_cybersource', 'Unable to connect to CyberSource via SOAP.', array(), WATCHDOG_ERROR);
    drupal_set_message(t('We apologize for the delay, but we are unable to process your credit card at this time. Please <a href="!url">contact sales</a> to complete your order.', array('!url' => url('contact'))), 'error');
  }

  // Process a reply from CyberSource.
  if (isset($reply->paySubscriptionUpdateReply)) {
    if ($reply->paySubscriptionUpdateReply->reasonCode == 100) {
      uc_order_comment_save($order->order_id, 0, t('CyberSource: Subscription @subscription_id updated.', array('@subscription_id' => $subscription_id)), 'admin');
      return;
    }
    else {
      uc_order_comment_save($order->order_id, 0, t('<b>Attempt to update CyberSource subscription profile failed.</b><br /><b>Reason:</b> @code', array('@code' => $reply->paySubscriptionCreateReply->reasonCode)), 'admin');
      return t('Subscription update failed with code @code', array('@code' => $reply->paySubscriptionCreateReply->reasonCode));
    }
  }
  uc_order_comment_save($order->order_id, 0, t('CyberSource: Subscription @subscription_id update failed: invalid reply.', array('@subscription_id' => $subscription_id)), 'admin');
  return t('Subscription update failed due to an invalid reply.');
}

/**
 * List the profile information.
 */
function uc_recurring_cybersource_profile_form($form, $form_state, $rfid) {
  $fee = uc_recurring_fee_user_load($rfid);
  $order = uc_order_load($fee->order_id);
  $refs = array_keys($order->data['cc_txns']['references']);
  $subscription_id = end($refs);

  module_load_include('inc', 'uc_cybersource', 'uc_cybersource.soap');

  try {
    $soapClient = new CyberSourceSoapClient(_uc_recurring_cybersource_soap_wsdl_url(), array());
    $login = _uc_cybersource_soap_login_data();
    // Create the request with some meta data.
    $request = new stdClass();
    $request->merchantID = $login['merchant_id'];
    $request->merchantReferenceCode = $order->order_id;

    $paySubscriptionRetrieveService = new stdClass();
    $paySubscriptionRetrieveService->run = 'true';
    $request->paySubscriptionRetrieveService = $paySubscriptionRetrieveService;

    $recurringSubscriptionInfo = new stdClass();
    $recurringSubscriptionInfo->subscriptionID = $subscription_id;
    $request->recurringSubscriptionInfo = $recurringSubscriptionInfo;

    $reply = $soapClient->runTransaction($request);
  }
  catch (SoapFault $exception) {
    // Log and display errors if Ubercart is unable to connect via SOAP.
    watchdog('uc_cybersource', 'Unable to connect to CyberSource via SOAP.', array(), WATCHDOG_ERROR);
    drupal_set_message(t('We apologize for the delay, but we are unable to process your credit card at this time. Please <a href="!url">contact sales</a> to complete your order.', array('!url' => url('contact'))), 'error');
  }

  if (isset($reply->paySubscriptionRetrieveReply)) {
    $header = $rows = $row = array();
    foreach ($reply->paySubscriptionRetrieveReply as $key => $val) {
      $header[] = $key;
      $row[] = $val;
    }
    $rows[] = $row;

    $form['info'] = array(
      '#markup' => t('The profile details below were returned from CyberSource'),
    );
    $form['profile'] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
    );
    $form['return'] = array(
      '#markup' => l(t('Return'), 'admin/store/orders/recurring/view/fee/' . $rfid),
    );
  }
  else {
    drupal_set_message(t('Invalid response from SOAP server while retrieving subscription @subscription_id', array('@subscription_id' => $subscription_id)), 'error');
  }
  return $form;
}

/**
 * Get the WDSL URL used for SOAP requests
 */
function _uc_recurring_cybersource_soap_wsdl_url() {
  if (variable_get('uc_cybersource_server', 'test') == 'test') {
    return 'https://ics2wstest.ic3.com/commerce/1.x/transactionProcessor/CyberSourceTransaction_1.38.wsdl';
  }
  else {
    return 'https://ics2ws.ic3.com/commerce/1.x/transactionProcessor/CyberSourceTransaction_1.38.wsdl';
  }
}

/**
 * Set up a CyberSource customer profile
 * @param $order Order object
 */
function _uc_recurring_cybersource_create_profile($order) {
  module_load_include('inc', 'uc_cybersource', 'uc_cybersource.soap');
  try {
    $soapClient = new CyberSourceSoapClient(_uc_recurring_cybersource_soap_wsdl_url(), array());
    $login = _uc_cybersource_soap_login_data();
    // Create the request with some meta data.
    $request = new stdClass();
    $request->merchantID = $login['merchant_id'];
    $request->merchantReferenceCode = $order->order_id;

    $recurringSubscriptionInfo = new stdClass();
    $recurringSubscriptionInfo->amount = 0;
    $recurringSubscriptionInfo->frequency = 'on-demand';
    $request->recurringSubscriptionInfo = $recurringSubscriptionInfo;

    $paySubscriptionCreateService = new stdClass();
    $paySubscriptionCreateService->run = 'true';
    $request->paySubscriptionCreateService = $paySubscriptionCreateService;

    $purchaseTotals = new stdClass();
    $purchaseTotals->currency = variable_get('uc_cybersource_currency', 'usd');
    $request->purchaseTotals = $purchaseTotals;

    $request->billTo = _uc_recurring_cybersource_billto_obj($order);
    $request->card = _uc_recurring_cybersource_card_obj($order);

    $reply = $soapClient->runTransaction($request);
  }
  catch (SoapFault $exception) {
    // Log and display errors if Ubercart is unable to connect via SOAP.
    watchdog('uc_cybersource', 'Unable to connect to CyberSource via SOAP.', array(), WATCHDOG_ERROR);
    drupal_set_message(t('We apologize for the delay, but we are unable to process your credit card at this time. Please <a href="!url">contact sales</a> to complete your order.', array('!url' => url('contact'))), 'error');
  }

  if (isset($reply->paySubscriptionCreateReply)) {
    // If the create request was successful...
    if ($reply->paySubscriptionCreateReply->reasonCode == '100') {
      $id = $reply->paySubscriptionCreateReply->subscriptionID;

      // Save the subscription ID to the order's data array.
      $order->data = uc_credit_log_reference($order->order_id, $id, $order->payment_details['cc_number']);

      uc_order_comment_save($order->order_id, 0, t('<b>CyberSource profile created.</b><br /><b>Subscription ID:</b> @id', array('@id' => $id)), 'admin');
      return TRUE;
    }
    else {
      uc_order_comment_save($order->order_id, 0, t('<b>Attempt to create CyberSource profile failed.</b><br /><b>Reason:</b> @code', array('@code' => $reply->paySubscriptionCreateReply->reasonCode)), 'admin');
    }
  }
  return FALSE;
}

/**
 * Create a billTo SOAP object from an order
 * @param $order Order object
 */
function _uc_recurring_cybersource_billto_obj($order) {
  $billing_country = uc_get_country_data(array('country_id' => $order->billing_country));
  $billTo = new stdClass();
  $billTo->firstName = $order->billing_first_name;
  $billTo->lastName = $order->billing_last_name;
  $billTo->street1 = $order->billing_street1;
  if ($order->billing_street2) {
    $billTo->street2 = $order->billing_street2;
  }
  $billTo->city = $order->billing_city;
  $billTo->state = uc_get_zone_code($order->billing_zone);
  $billTo->postalCode = $order->billing_postal_code;
  $billTo->country = $billing_country[0]['country_iso_code_2'];
  if ($order->billing_phone) {
    $billTo->phoneNumber = $order->billing_phone;
  }
  $billTo->email = $order->primary_email;
  $billTo->customerID = $order->uid;
  return $billTo;
}

/**
 * Create a card SOAP object from an order
 * @param $order Order object
 */
function _uc_recurring_cybersource_card_obj($order) {
  $card = new stdClass();
  $card->accountNumber = $order->payment_details['cc_number'];
  $card->expirationMonth = $order->payment_details['cc_exp_month'];
  $card->expirationYear = $order->payment_details['cc_exp_year'];
  $card->cardType = _uc_cybersource_card_type($order->payment_details['cc_number']);
  if (variable_get('uc_credit_cvv_enabled', TRUE)) {
    $card->cvNumber = $order->payment_details['cc_cvv'];
  }
  return $card;
}
