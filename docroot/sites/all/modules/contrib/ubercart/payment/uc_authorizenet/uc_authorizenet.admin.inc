<?php

/**
 * @file
 * Includes page callbacks for administrative recurring fee operation pages.
 */


// Displays a form to update a subscriptions's CC info.
function uc_authorizenet_arb_admin_update_form($form_state, $rfid) {
  $order = new stdClass();
  $form = array();

  $fee = uc_recurring_fee_load('user', $rfid);

  $form['rfid'] = array(
    '#type' => 'value',
    '#value' => $rfid,
  );
  $form['description'] = array(
    '#value' => '<div>'. t('Subscription ID: @subscription_id', array('@subscription_id' => $fee['data'])) .'</div>',
  );

  $form['cc_data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credit card details'),
    '#theme' => 'uc_payment_method_credit_form',
    '#tree' => TRUE,
  );
  $form['cc_data'] += uc_payment_method_credit_form(array(), $order);
  unset($form['cc_data']['cc_policy']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#suffix' => l(t('Cancel'), 'admin/store/orders/recurring'),
  );

  return $form;
}

function uc_authorizenet_arb_admin_update_form_submit($form, &$form_state) {
  $fee = uc_recurring_fee_load('user', $form_state['values']['rfid']);

  $updates = array(
    'payment' => array(
      'creditCard' => array(
        'cardNumber' => $form_state['values']['cc_data']['cc_number'],
        'expirationDate' => $form_state['values']['cc_data']['cc_exp_year'] .'-'. $form_state['values']['cc_data']['cc_exp_month'],
      ),
    ),
  );

  $result = uc_authorizenet_arb_update($fee['data'], $updates, $fee['order_id']);

  // If the update was successful...
  if ($result) {
    drupal_set_message(t('Subscription data updated at Authorize.Net.'));
  }
  else {
    drupal_set_message(t('Subscription update failed. See order admin comments for more details.'), 'error');
  }

  $form_state['redirect'] = 'admin/store/orders/recurring';
}

// Displays a confirm form for cancelling a subscription.
function uc_authorizenet_arb_admin_cancel_form($form_state, $rfid) {
  $form['rfid'] = array(
    '#type' => 'value',
    '#value' => $rfid,
  );

  return confirm_form($form, t('Are you sure you wish to cancel this subscription?'), 'admin/store/orders/recurring', NULL, t('Confirm'), t('Cancel'));
}

function uc_authorizenet_arb_admin_cancel_form_submit($form, &$form_state) {
  $fee = uc_recurring_fee_load('user', $form_state['values']['rfid']);

  $result = uc_authorizenet_arb_cancel($fee['data'], $fee['order_id'], $fee);

  // If the cancellation was successful...
  if ($result) {
    drupal_set_message(t('Subscription cancelled through Authorize.Net.'));

    // Set the fee's recurring charges to 0.
    uc_recurring_fee_cancel($fee['rfid']);
  }
  else {
    drupal_set_message(t('Subscription cancellation failed. See order admin comments for more details.'), 'error');
  }

  $form_state['redirect'] = 'admin/store/orders/recurring';
}
