<?php

/**
 * @file
 * Recurring payments menu items.
 */

/**
 * Displays the confirmation form for cancelling a recurring fee.
 */
function uc_recurring_user_cancel_form($form, $form_state, $rfid, $fee_handler) {
  $cancel_path = request_uri();

  $fee = uc_recurring_fee_user_load($rfid);
  if ($fee->fee_handler == $fee_handler) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $fee->uid,
    );
    $form['rfid'] = array(
      '#type' => 'value',
      '#value' => $rfid,
    );

    return confirm_form($form, t('Are you sure you want to cancel this recurring fee?'), $cancel_path, t('This action cannot be undone and may result in the termination of subscription services.'), t('Yes, I want to cancel the subscription'), t('No, I want to continue with the subscription'));
  }
  else {
    drupal_set_message(t('Invalid fee handler was given, try again from the operations for this <a href="@url">recurring fee</a>, if problem persists contact the site administrator.', array('@url' => url($cancel_path))), 'error');
  }
}

/**
 * Implements hook_submit() for the cancel recurring fee form().
 */
function uc_recurring_user_cancel_form_submit($form, &$form_state) {
  uc_recurring_fee_cancel($form_state['values']['rfid']);

  drupal_set_message(t('The recurring fee has been cancelled.'));

  $fee = uc_recurring_fee_user_load($form_state['values']['rfid']);
  $form_state['redirect'] = 'user/' . $fee->uid;
}

