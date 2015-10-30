<?php

/**
 * @file
 * USPS administration menu items.
 */

/**
 * Configures USPS settings.
 *
 * @see uc_usps_admin_settings_validate()
 * @see uc_usps_admin_settings_submit()
 * @ingroup forms
 */
function uc_usps_admin_settings($form, &$form_state) {

  // Put fieldsets into vertical tabs
  $form['usps-settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'uc_usps') . '/uc_usps.admin.js',
      ),
    ),
  );

  // Container for credential forms
  $form['uc_usps_credentials'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Credentials'),
    '#description'   => t('Account number and authorization information.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'usps-settings',
  );

  $form['uc_usps_credentials']['uc_usps_user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('USPS user ID'),
    '#description' => t('To acquire or locate your user ID, refer to the <a href="!url">USPS documentation</a>.', array('!url' => 'http://drupal.org/node/1308256')),
    '#default_value' => variable_get('uc_usps_user_id', ''),
  );

  $form['domestic'] = array(
    '#type' => 'fieldset',
    '#title' => t('USPS Domestic'),
    '#description' => t('Set the conditions that will return a USPS quote.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group'         => 'usps-settings',
  );

  $form['domestic']['uc_usps_online_rates'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display USPS "online" rates'),
    '#default_value' => variable_get('uc_usps_online_rates', FALSE),
    '#description' => t('Show your customer standard USPS rates (default) or discounted "online" rates.  Online rates apply only if you, the merchant, pay for and print out postage from the USPS <a href="https://sss-web.usps.com/cns/landing.do">Click-N-Ship</a> web site.'),
  );

  $form['domestic']['uc_usps_env_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('USPS envelope services'),
    '#default_value' => variable_get('uc_usps_env_services', array_keys(_uc_usps_env_services())),
    '#options' => _uc_usps_env_services(),
    '#description' => t('Select the USPS services that are available to customers. Be sure to include the services that the Postal Service agrees are available to you.'),
  );

  $form['domestic']['uc_usps_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('USPS parcel services'),
    '#default_value' => variable_get('uc_usps_services', array_keys(_uc_usps_services())),
    '#options' => _uc_usps_services(),
    '#description' => t('Select the USPS services that are available to customers. Be sure to include the services that the Postal Service agrees are available to you.'),
  );

  $form['international'] = array(
    '#type' => 'fieldset',
    '#title' => t('USPS International'),
    '#description' => t('Set the conditions that will return a USPS International quote.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group'         => 'usps-settings',
  );

  $form['international']['uc_usps_intl_env_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('USPS international envelope services'),
    '#default_value' => variable_get('uc_usps_intl_env_services', array_keys(_uc_usps_intl_env_services())),
    '#options' => _uc_usps_intl_env_services(),
    '#description' => t('Select the USPS services that are available to customers. Be sure to include the services that the Postal Service agrees are available to you.'),
  );

  $form['international']['uc_usps_intl_services'] = array(
    '#type' => 'checkboxes',
    '#title' => t('USPS international parcel services'),
    '#default_value' => variable_get('uc_usps_intl_services', array_keys(_uc_usps_intl_services())),
    '#options' => _uc_usps_intl_services(),
    '#description' => t('Select the USPS services that are available to customers. Be sure to include the services that the Postal Service agrees are available to you.'),
  );

  // Container for quote options
  $form['uc_usps_quote_options'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Quote options'),
    '#description'   => t('Preferences that affect computation of quote.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'usps-settings',
  );

  $form['uc_usps_quote_options']['uc_usps_all_in_one'] = array(
    '#type' => 'radios',
    '#title' => t('Product packages'),
    '#default_value' => variable_get('uc_usps_all_in_one', 1),
    '#options' => array(
      0 => t('Each product in its own package'),
      1 => t('All products in one package'),
    ),
    '#description' => t('Indicate whether each product is quoted as shipping separately or all in one package. Orders with one kind of product will still use the package quantity to determine the number of packages needed, however.'),
  );

  // Insurance
  $form['uc_usps_quote_options']['uc_usps_insurance'] = array(
    '#type' => 'checkbox',
    '#title' => t('Package insurance'),
    '#default_value' => variable_get('uc_usps_insurance', FALSE),
    '#description' => t('When enabled, the quotes presented to the customer will include the cost of insurance for the full sales price of all products in the order.'),
    '#disabled' => TRUE,
  );

  // Delivery Confirmation
  $form['uc_usps_quote_options']['uc_usps_delivery_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delivery confirmation'),
    '#default_value' => variable_get('uc_usps_delivery_confirmation', FALSE),
    '#description' => t('When enabled, the quotes presented to the customer will include the cost of delivery confirmation for all packages in the order.'),
    '#disabled' => TRUE,
  );

  // Signature Confirmation
  $form['uc_usps_quote_options']['uc_usps_signature_confirmation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Signature confirmation'),
    '#default_value' => variable_get('uc_usps_signature_confirmation', FALSE),
    '#description' => t('When enabled, the quotes presented to the customer will include the cost of signature confirmation for all packages in the order.'),
    '#disabled' => TRUE,
  );

  // Container for markup forms
  $form['uc_usps_markups'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Markups'),
    '#description'   => t('Modifiers to the shipping weight and quoted rate.'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#group'         => 'usps-settings',
  );

  $form['uc_usps_markups']['uc_usps_rate_markup_type'] = array(
    '#type' => 'select',
    '#title' => t('Rate markup type'),
    '#default_value' => variable_get('uc_usps_rate_markup_type', 'percentage'),
    '#options' => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'currency' => t('Addition (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
    ),
  );
  $form['uc_usps_markups']['uc_usps_rate_markup'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping rate markup'),
    '#default_value' => variable_get('uc_usps_rate_markup', '0'),
    '#description' => t('Markup shipping rate quote by dollar amount, percentage, or multiplier.'),
  );

  // Form to select type of weight markup
  $form['uc_usps_markups']['uc_usps_weight_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Weight markup type'),
    '#default_value' => variable_get('uc_usps_weight_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'mass'       => t('Addition (!mass)', array('!mass' => '#')),
    ),
    '#disabled' => TRUE,
  );

  // Form to select weight markup amount
  $form['uc_usps_markups']['uc_usps_weight_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shipping weight markup'),
    '#default_value' => variable_get('uc_usps_weight_markup', '0'),
    '#description'   => t('Markup shipping weight on a per-package basis before quote, by weight amount, percentage, or multiplier.'),
    '#disabled' => TRUE,
  );

  // Taken from system_settings_form(). Only, don't use its submit handler.
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/store/settings/quotes'),
  );

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }

  return $form;
}

/**
 * Validation handler for uc_usps_admin_settings form.
 *
 * @see uc_usps_admin_settings()
 * @see uc_usps_admin_settings_submit()
 */
function uc_usps_admin_settings_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['uc_usps_rate_markup'])) {
    form_set_error('uc_usps_rate_markup', t('Rate markup must be a numeric value.'));
  }
  if (!is_numeric($form_state['values']['uc_usps_weight_markup'])) {
    form_set_error('uc_usps_weight_markup', t('Weight markup must be a numeric value.'));
  }
}

/**
 * Submit handler for uc_usps_admin_settings form.
 *
 * @see uc_usps_admin_settings()
 * @see uc_usps_admin_settings_validate()
 */
function uc_usps_admin_settings_submit($form, &$form_state) {
  $fields = array(
    'uc_usps_user_id',
    'uc_usps_online_rates',
    'uc_usps_env_services',
    'uc_usps_services',
    'uc_usps_intl_env_services',
    'uc_usps_intl_services',
    'uc_usps_rate_markup_type',
    'uc_usps_rate_markup',
    'uc_usps_weight_markup_type',
    'uc_usps_weight_markup',
    'uc_usps_all_in_one',
    'uc_usps_insurance',
    'uc_usps_delivery_confirmation',
    'uc_usps_signature_confirmation',
  );

  foreach ($fields as $key) {
    $value = $form_state['values'][$key];

    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

  drupal_set_message(t('The configuration options have been saved.'));

  cache_clear_all();
  drupal_theme_rebuild();
}
