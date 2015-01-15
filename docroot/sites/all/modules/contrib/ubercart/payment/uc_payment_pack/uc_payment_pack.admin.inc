<?php

/**
 * @file
 * Payment pack administration menu items.
 */

/**
 * Receives a check for an order and put in a clear date.
 *
 * @see uc_payment_pack_receive_check_form_validate()
 * @see uc_payment_pack_receive_check_form_submit()
 * @see theme_uc_payment_pack_receive_check_form()
 * @ingroup forms
 */
function uc_payment_pack_receive_check_form($form_state, $order) {
  $balance = uc_payment_balance($order);
  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );
  $options = array(
    'sign' => FALSE,
    'thou' => FALSE,
    'dec' => '.',
  );
  $form['balance'] = array('#value' => uc_price($balance, $context));
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['check_exists'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check has already cleared.'),
    '#attributes' => array('onclick' => 'receive_check_toggle(this.checked);'),
  );
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => uc_price($balance, $context, $options),
    '#size' => 10,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );
  $form['comment'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment'),
    '#description' => t('Any notes about the check, like type or check number.'),
    '#size' => 64,
    '#maxlength' => 256,
  );
  $form['clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Expected clear date'),
    '#collapsible' => FALSE,
  );
  $form['clear']['clear_month'] = uc_select_month(NULL, format_date(time(), 'custom', 'n'));
  $form['clear']['clear_day'] = uc_select_day(NULL, format_date(time(), 'custom', 'j'));
  $form['clear']['clear_year'] = uc_select_year(NULL, format_date(time(), 'custom', 'Y'), format_date(time(), 'custom', 'Y'), format_date(time(), 'custom', 'Y') + 1);
  foreach (array('clear_month', 'clear_day', 'clear_year') as $key) {
    $form['clear'][$key]['#prefix'] = '<div style="float: left; margin-right: 1em;">';
    $form['clear'][$key]['#suffix'] = '</div>';
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Receive check'),
  );

  return $form;
}

/**
 * Themes the receive check form.
 *
 * @see uc_payment_pack_receive_check_form()
 * @see uc_payment_pack_receive_check_form_submit()
 * @ingroup themeable
 */
function theme_uc_payment_pack_receive_check_form($form) {
  drupal_add_js(drupal_get_path('module', 'uc_payment') .'/uc_payment.js');

  $output = '<p>'. t('Use the form to enter the check into the payments system and set the expected clear date.') .'</p>';
  $output .= '<p><strong>'. t('Order balance:') .'</strong> '. drupal_render($form['balance']) .'</p>';

  $output .= drupal_render($form);

  return $output;
}

/**
 * Form validation for uc_payment_pack_receive_check_form().
 *
 * @see uc_payment_pack_receive_check_form()
 * @see uc_payment_pack_receive_check_form_submit()
 */
function uc_payment_pack_receive_check_form_validate($form, &$form_state) {
  if (!$form_state['values']['check_exists'] && !is_numeric($form_state['values']['amount'])) {
    form_set_error('amount', t('The amount must be a number.'));
  }
}

/**
 * Submit handler for uc_payment_pack_receive_check_form().
 *
 * @see uc_payment_pack_receive_check_form()
 * @see uc_payment_pack_receive_check_form_validate()
 */
function uc_payment_pack_receive_check_form_submit($form, &$form_state) {
  global $user;

  uc_payment_enter($form_state['values']['order_id'], 'check', $form_state['values']['amount'], $user->uid, '', $form_state['values']['comment']);

  db_query("INSERT INTO {uc_payment_check} (order_id, clear_date) VALUES (%d, %d)",
    $form_state['values']['order_id'], mktime(12, 0, 0, $form_state['values']['clear_month'],
    $form_state['values']['clear_day'], $form_state['values']['clear_year']));

  drupal_set_message(t('Check received, expected clear date of @date.', array('@date' => uc_date_format($form_state['values']['clear_month'], $form_state['values']['clear_day'], $form_state['values']['clear_year']))));

  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'];
}
