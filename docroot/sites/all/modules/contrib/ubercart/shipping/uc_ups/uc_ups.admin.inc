<?php

/**
 * @file
 * UPS administration menu items.
 */

/**
 * UPS Online Tool settings.
 *
 * Records UPS account information necessary to use the service. Allows testing
 * or production mode. Configures which UPS services are quoted to customers.
 *
 * @see uc_ups_admin_settings_validate()
 * @ingroup forms
 */
function uc_ups_admin_settings() {
  // Container for credential forms
  $form['uc_ups_credentials'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Credentials'),
    '#description'   => t('Account number and authorization information.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['uc_ups_credentials']['uc_ups_access_license'] = array(
    '#type' => 'textfield',
    '#title' => t('UPS OnLine Tools XML Access Key'),
    '#default_value' => variable_get('uc_ups_access_license', ''),
    '#required' => TRUE,
  );
  $form['uc_ups_credentials']['uc_ups_shipper_number'] = array(
    '#type' => 'textfield',
    '#title' => t('UPS Shipper #'),
    '#description' => t('The 6-character string identifying your UPS account as a shipper.'),
    '#default_value' => variable_get('uc_ups_shipper_number', ''),
    '#required' => TRUE,
  );
  $form['uc_ups_credentials']['uc_ups_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('UPS.com user ID'),
    '#default_value' => variable_get('uc_ups_user_id', ''),
    '#required' => TRUE,
  );
  $form['uc_ups_credentials']['uc_ups_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => variable_get('uc_ups_password', ''),
  );
  $form['uc_ups_credentials']['uc_ups_connection_address'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#description' => t('Quotes and shipments requested in Testing mode will not be picked up or charged to your account.'),
    '#options' => array('https://wwwcie.ups.com/ups.app/xml/' => t('Testing'),
      'https://onlinetools.ups.com/ups.app/xml/' => t('Production'),
    ),
    '#default_value' => variable_get('uc_ups_connection_address', 'https://wwwcie.ups.com/ups.app/xml/'),
  );

  $form['services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Service options'),
    '#description' => t('Set the conditions that will return a UPS quote.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['services']['uc_ups_conditions'] = array(
    '#value' => '<p>'. l(t('Set the conditions that will return a UPS quote.'), CA_UI_PATH .'/uc_ups_get_quote/edit/conditions') .'</p>',
  );

  $form['services']['uc_ups_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('UPS services'),
    '#default_value' => variable_get('uc_ups_services', _uc_ups_service_list()),
    '#options' => _uc_ups_service_list(),
    '#description' => t('Select the UPS services that are available to customers.'),
  );

  // Container for quote options
  $form['uc_ups_quote_options'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Quote options'),
    '#description'   => t('Preferences that affect computation of quote.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['uc_ups_quote_options']['uc_ups_all_in_one'] = array(
    '#type' => 'radios',
    '#title' => t('Product packages'),
    '#default_value' => variable_get('uc_ups_all_in_one', 1),
    '#options' => array(
      0 => t('Each in its own package'),
      1 => t('All in one'),
    ),
    '#description' => t('Indicate whether each product is quoted as shipping separately or all in one package. Orders with one kind of product will still use the package quantity to determine the number of packages needed, however.'),
  );

  // Form to select package types
  $form['uc_ups_quote_options']['uc_ups_pkg_type'] = array(
    '#type' => 'select',
    '#title' => t('Default Package Type'),
    '#default_value' => variable_get('uc_ups_pkg_type', _uc_ups_pkg_types()),
    '#options' => _uc_ups_pkg_types(),
    '#description' => t('Type of packaging to be used.  May be overridden on a per-product basis via the product node edit form.'),
  );

  $form['uc_ups_quote_options']['uc_ups_classification'] = array(
    '#type' => 'select',
    '#title' => t('UPS Customer classification'),
    '#options' => array(
      '01' => t('Wholesale'),
      '03' => t('Occasional'),
      '04' => t('Retail'),
    ),
    '#default_value' => variable_get('uc_ups_classification', '04'),
    '#description' => t('The kind of customer you are to UPS. For daily pickups the default is wholesale; for customer counter pickups the default is retail; for other pickups the default is occasional.'),
  );

  $form['uc_ups_quote_options']['uc_ups_negotiated_rates'] = array(
    '#type' => 'radios',
    '#title' => t('Negotiated rates'),
    '#default_value' => variable_get('uc_ups_negotiated_rates', 0),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#description' => t('Is your UPS account receiving negotiated rates on shipments?'),
  );

  // Form to select pickup type
  $form['uc_ups_quote_options']['uc_ups_pickup_type'] = array(
    '#type' => 'select',
    '#title' => t('Pickup type'),
    '#options' => array(
      '01' => 'Daily Pickup',
      '03' => 'Customer Counter',
      '06' => 'One Time Pickup',
      '07' => 'On Call Air',
      '11' => 'Suggested Retail Rates',
      '19' => 'Letter Center',
      '20' => 'Air Service Center',
    ),
    '#default_value' => variable_get('uc_ups_pickup_type', '01'),
  );

  $form['uc_ups_quote_options']['uc_ups_residential_quotes'] = array(
    '#type' => 'radios',
    '#title' => t('Assume UPS shipping quotes will be delivered to'),
    '#default_value' => variable_get('uc_ups_residential_quotes', 0),
    '#options' => array(
      0 => t('Business locations'),
      1 => t('Residential locations (extra fees)'),
    ),
  );

  $form['uc_ups_quote_options']['uc_ups_unit_system'] = array(
    '#type' => 'select',
    '#title' => t('System of measurement'),
    '#default_value' => variable_get('uc_ups_unit_system', variable_get('uc_length_unit', 'in')),
    '#options' => array(
      'in' => t('British'),
      'cm' => t('Metric'),
    ),
    '#description' => t('Choose the standard system of measurement for your country.'),
  );

  $form['uc_ups_quote_options']['uc_ups_insurance'] = array(
    '#type' => 'checkbox',
    '#title' => t('Package insurance'),
    '#default_value' => variable_get('uc_ups_insurance', TRUE),
    '#description' => t('When enabled, the quotes presented to the customer will include the cost of insurance for the full sales price of all products in the order.'),
  );

  // Container for markup forms
  $form['uc_ups_markups'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Markups'),
    '#description'   => t('Modifiers to the shipping weight and quoted rate.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  // Form to select type of rate markup
  $form['uc_ups_markups']['uc_ups_markup_type'] = array(
    '#type' => 'select',
    '#title' => t('Markup type'),
    '#default_value' => variable_get('uc_ups_markup_type', 'percentage'),
    '#options' => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'currency' => t('Addition (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
    ),
  );

  // Form to select rate markup amount
  $form['uc_ups_markups']['uc_ups_markup'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping rate markup'),
    '#default_value' => variable_get('uc_ups_markup', '0'),
    '#description' => t('Markup shipping rate quote by currency amount, percentage, or multiplier.'),
  );

  // Container for label printing
  $form['uc_ups_labels'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Label Printing'),
    '#description'   => t('Preferences for UPS Shipping Label Printing.  Additional permissions from UPS are required to use this feature.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $period = drupal_map_assoc(array(86400, 302400, 604800, 1209600, 2419200, 0), 'format_interval');
  $period[0] = t('Forever');

  // Form to select how long labels stay on server
  $form['uc_ups_labels']['uc_ups_label_lifetime'] = array(
    '#type'          => 'select',
    '#title'         => t('Label lifetime'),
    '#default_value' => variable_get('uc_ups_label_lifetime', 0),
    '#options'       => $period,
    '#description'   => t('Controls how long labels are stored on the server before being automatically deleted. Cron must be enabled for automatic deletion. Default is never delete the labels, keep them forever.'),
  );

  $form['#validate'][] = 'uc_ups_admin_settings_validate';

  return system_settings_form($form);
}

/**
 * Validation handler for uc_ups_admin_settings.
 *
 * Requires password only if it hasn't been set.
 *
 * @see uc_ups_admin_settings()
 */
function uc_ups_admin_settings_validate($form, &$form_state) {
  $old_password = variable_get('uc_ups_password', '');
  if (!$form_state['values']['uc_ups_password']) {
    if ($old_password) {
      form_set_value($form['uc_ups_password'], $old_password, $form_state);
    }
    else {
      form_set_error('uc_ups_password', t('Password field is required.'));
    }
  }

  if (!is_numeric($form_state['values']['uc_ups_markup'])) {
    form_set_error('uc_ups_markup', t('Rate markup must be a numeric value.'));
  }
}
