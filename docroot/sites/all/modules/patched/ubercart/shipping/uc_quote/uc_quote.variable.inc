<?php

/**
 * @file
 * Variable module hook implementations.
 */


/**
 * Implements hook_variable_group_info().
 */
function uc_quote_variable_group_info() {
  $groups['uc_quote'] = array(
    'title' => t('Ubercart shipping quote settings'),
    'access' => 'administer store',
    'path' => array('admin/store/settings/quotes'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function uc_quote_variable_info($options) {
  $variables['uc_quote_err_msg'] = array(
    'type' => 'text',
    'title' => t('Shipping quote error message', array(), $options),
    'description' => t('The error message shown to a user when a problem is encountered while generating a shipping quote.', array(), $options),
    'group' => 'uc_quote',
    'default' => t("There were problems getting a shipping quote. Please verify the delivery and product information and try again.\nIf this does not resolve the issue, please call in to complete your order.", array(), $options),
  );
  $variables['uc_quote_pane_description'] = array(
    'type' => 'text',
    'title' => t('Shipping quote pane description', array(), $options),
    'description' => t('Message to indicate to users that a shipping quote can be updated automatically on address change, or manually by pressing a button.', array(), $options),
    'group' => 'uc_quote',
    'default' => t('Shipping quotes are generated automatically when you enter your address and may be updated manually with the button below.', array(), $options),
  );
  return $variables;
}
