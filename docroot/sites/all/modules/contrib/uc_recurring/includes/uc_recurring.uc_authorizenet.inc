<?php

/**
 * @file
 * Uc recurring implementation for the test gateway module.
 */

/**
 * Implements hook_recurring_info().
 */
function uc_recurring_uc_authorizenet_recurring_info() {
  $items['authorizenet_cim'] = array(
    'name' => t('Authorize.net (CIM)'),
    'payment method' => 'credit',
    'module' => 'uc_recurring',
    'fee handler' => 'authorizenet_cim',
    'renew callback' => 'uc_recurring_authorizenet_cim_renew',
    'process callback' => 'uc_recurring_authorizenet_cim_process',
    'saved profile' => TRUE,
    'menu' => array(
      'charge' => UC_RECURRING_MENU_DEFAULT,
      'edit' => UC_RECURRING_MENU_DEFAULT,
      'update' => array(
        'title' => 'Update Account Details',
        'page arguments' => array('uc_recurring_authorizenet_cim_update_form'),
        'file' => 'includes/uc_recurring.uc_authorizenet.inc',
      ),
      'profile' => array(
        'title' => 'CIM Profile',
        'page arguments' => array('uc_recurring_authorizenet_cim_profile_form'),
        'access callback' => 'user_access',
        'access arguments' => array('administer recurring fees'),
        'file' => 'includes/uc_recurring.uc_authorizenet.inc',
      ),
      'cancel' => UC_RECURRING_MENU_DEFAULT,
    ), // Use the default user operation defined in uc_recurring.
  );

  if (variable_get('uc_authnet_cim_mode', 'disabled') != 'disabled') {
    $items['authorizenet'] = $items['authorizenet_cim'];
  }
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
function uc_recurring_authorizenet_cim_process($order, &$fee) {
  $fee->fee_handler = 'authorizenet_cim';
  if (variable_get('uc_authnet_cim_profile', FALSE) == FALSE) {
    $data = array(
      'txn_type' => UC_CREDIT_REFERENCE_SET,
    );
    return uc_authorizenet_charge($order->order_id, $order->total_amount, $data);
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
function uc_recurring_authorizenet_cim_renew($order, &$fee) {
  if (!empty($order->data['cc_txns']['references'])) {
    $refs = array_keys($order->data['cc_txns']['references']);
    $data = array(
      'txn_type' => UC_CREDIT_REFERENCE_TXN,
      'ref_id' => end($refs),
    );
    $result = uc_authorizenet_charge($order->order_id, $order->order_total, $data);

    if ($result['success'] == TRUE) {
      $result['data'] = (isset($result['data'])) ? $result['data'] : "";
      uc_payment_enter($order->order_id, $order->payment_method, $order->order_total, $fee->uid, $result['data'], $result['comment']);
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Create form for updating credit card details for recurring fee.
 */
function uc_recurring_authorizenet_cim_update_form($form, $form_state, $rfid) {
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
    $form['bill_to']['#theme'] = 'uc_recurring_authorizenet_cim_billto_form';
    
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
 * Implements update form validation for the authorizenet CIM gateway.
 */
function uc_recurring_authorizenet_cim_update_form_validate(&$form, &$form_state) {
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
 * Implements update form submit for the authorizenet CIM gateway.
 */
function uc_recurring_authorizenet_cim_update_form_submit(&$form, &$form_state) {
  $fee = uc_recurring_fee_user_load($form_state['values']['rfid']);
  $order = uc_order_load($fee->order_id);
  $last4old=substr($order->payment_details['cc_number'], -4);
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
  $profile_id = end($refs);
  if ($message = _uc_recurring_uc_authorizenet_cim_update_paymentprofile($order, $profile_id)) {
    drupal_set_message(t('Account update failed: @error', array('@error' => $message)), 'error');
    $form_state['redirect'] = FALSE;
  }
  else {
    drupal_set_message(t('Account updated'));
    uc_order_save($order);
    $last4new=substr($order->payment_details['cc_number'], -4);
    if ($last4new!=$last4old) {
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
function theme_uc_recurring_authorizenet_cim_billto_form($form) {
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
 * TODO: This function needs to be moved to the uc_authorizenet module and
 *       renamed to:
 *         uc_authorizenet_cim_update_paymentprofile($order, $ref_id)
 */
function _uc_recurring_uc_authorizenet_cim_update_paymentprofile($order, $ref_id) {
  $server = variable_get('uc_authnet_cim_mode', 'disabled');

  // Help build the request.
  $request = _uc_recurring_uc_authorizenet_cim_paymentprofile_update_request($order, $ref_id);

  // Request a profile from auth.net.
  $xml = _uc_authorizenet_xml_api_wrapper('updateCustomerPaymentProfileRequest', _uc_authorizenet_array_to_xml($request));

  // Parse the response.
  $response = _uc_authorizenet_cim_parse_response(uc_authorizenet_xml_api($server, $xml));
  if ($response['resultCode'] == 'Error') {
    uc_order_comment_save($order->order_id, 0, t('Authorize.Net: Update CIM profile failed.<br />@error - @text', array('@error' => $response['code'], '@text' => $response['text'])), 'admin');
    return $response['text'];
  }
  else {
    uc_order_comment_save($order->order_id, 0, t('Authorize.Net: CIM profile update - @id', array('@id' => $response['customerProfileId'])));
  }
}

/**
 * TODO: This function needs to be moved to the uc_authorizenet module and
 *       renamed to:
 *         _uc_authorizenet_cim_paymentprofile_update_request($order, $ref_id)
 */
function _uc_recurring_uc_authorizenet_cim_paymentprofile_update_request($order, $ref_id) {
  $profile = _uc_authorizenet_cim_profile_get($order, $ref_id);
  if ($profile['resultCode'] == 'Error') {
    return $profile;
  }
  else {
    $request = array(
      'refId' => substr($order->order_id . '-' . REQUEST_TIME, 0, 20),
      'customerProfileId' => $profile['customerProfileId'],
      'paymentProfile' => array(
        'billTo' => _uc_authorize_cim_xml_billto($order),
        'payment' => array(
          'creditCard' => array(
            'cardNumber' => $order->payment_details['cc_number'],
            'expirationDate' => $order->payment_details['cc_exp_year'] . '-' . str_pad($order->payment_details['cc_exp_month'], 2, '0', STR_PAD_LEFT),
          ),
        ),
        'customerPaymentProfileId' => $profile['customerPaymentProfileId'],
      ),
    );
    return $request;
  }
}

/**
 * List the CIM profile information.
 */
function uc_recurring_authorizenet_cim_profile_form($form, $form_state, $rfid) {
  $fee = uc_recurring_fee_user_load($rfid);
  $order = uc_order_load($fee->order_id);
  $refs = array_keys($order->data['cc_txns']['references']);
  $ref_id = end($refs);
  $profile = _uc_authorizenet_cim_profile_get($order, $ref_id);

  $header = array_keys($profile);
  $rows[] = array_values($profile);

  $form['info'] = array(
    '#markup' => t('The profile details below were return from Authorize.net'),
  );
  $form['profile'] = array(
    '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
  );
  $form['return'] = array(
    '#markup' => l(t('Return'), 'admin/store/orders/recurring/view/fee/' . $rfid),
  );

  return $form;
}
