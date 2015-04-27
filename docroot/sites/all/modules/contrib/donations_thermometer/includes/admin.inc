<?php
/**
 * @file
 */
function donations_thermometer_admin() {

  $form['donations_thermometer_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Target'),
    '#default_value' => variable_get('donations_thermometer_target', 100),
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("The target."),
    '#required' => TRUE,
  );
  $form['donations_thermometer_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Current Amount'),
    '#default_value' => variable_get('donations_thermometer_amount', 50),
    '#size' => 9,
    '#maxlength' => 10,
    '#description' => t("The amount."),
    '#required' => TRUE,
  );
  $form['donations_thermometer_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#default_value' => variable_get('donations_thermometer_currency', '$'),
    '#options' => array(
      '$' => '$',
      '&pound;' => 'Pound',
      '&yen;' => 'Yen',
      '&euro;' => 'Euro',
      '' => t('None'),
    ),
    '#description' => t("Choose a currency(if applicable) to be used with your amounts")
  );
  $form['donations_thermometer_size'] = array(
    '#type' => 'select',
    '#title' => t('Size'),
    '#default_value' => variable_get('donations_thermometer_size', 'Large'),
    '#options' => array(
      'Large' => 'Large',
      'Small' => 'Small',
    ),
    '#description' => t("Choose what size to display the thermometer.")
  );

  return system_settings_form($form);
}

function donations_thermometer_admin_validate($form, &$form_state) {
  $amount = $form_state['values']['donations_thermometer_amount'];
  $total = $form_state['values']['donations_thermometer_target'];

  if (!is_numeric($amount)) {
    form_set_error('donations_thermometer_$amount', t('You must select a number for the current amount.'));
  }
  else if ($amount < 0) {
    form_set_error('donations_thermometer_$amount', t('Current Amount must be positive.'));
  }

  if (!is_numeric($total)) {
    form_set_error('donations_thermometer_$total', t('You must select a number for the target amount.'));
  }
  else if ($total <= 0) {
    form_set_error('donations_thermometer_$total', t('Target Amount must be positive.'));
  }

  if ($amount > $total) {
    form_set_error('donations_thermometer_$amount', t('Current Amount must be equal to or less than the target amount.'));
  }
}

