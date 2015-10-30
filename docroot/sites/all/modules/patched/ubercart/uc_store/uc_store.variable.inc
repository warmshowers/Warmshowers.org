<?php

/**
 * @file
 * Variable module hook implementations.
 */


/**
 * Implements hook_variable_group_info().
 */
function uc_store_variable_group_info() {
  $groups['uc_store'] = array(
    'title' => t('Ubercart store settings'),
    'access' => 'administer store',
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function uc_store_variable_info($options) {
  $variables['uc_field_first_name'] = array(
    'type' => 'string',
    'title' => t('First name', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('First name', array(), $options),
  );
  $variables['uc_field_last_name'] = array(
    'type' => 'string',
    'title' => t('Last name', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Last name', array(), $options),
  );
  $variables['uc_field_company'] = array(
    'type' => 'string',
    'title' => t('Company', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Company', array(), $options),
  );
  $variables['uc_field_street1'] = array(
    'type' => 'string',
    'title' => t('Street address 1', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Street address', array(), $options),
  );
  $variables['uc_field_street2'] = array(
    'type' => 'string',
    'title' => t('Street address 2', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => '',
  );
  $variables['uc_field_city'] = array(
    'type' => 'string',
    'title' => t('City', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Country', array(), $options),
  );
  $variables['uc_field_zone'] = array(
    'type' => 'string',
    'title' => t('State/Province', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('State/Province', array(), $options),
  );
  $variables['uc_field_postal_code'] = array(
    'type' => 'string',
    'title' => t('Postal code', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Postal code', array(), $options),
  );
  $variables['uc_field_country'] = array(
    'type' => 'string',
    'title' => t('Country', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Country', array(), $options),
  );
  $variables['uc_field_phone'] = array(
    'type' => 'string',
    'title' => t('Phone number', array(), $options),
    'description' => t('Ubercart address field label.', array(), $options),
    'group' => 'uc_store',
    'default' => t('Phone number', array(), $options),
  );
  return $variables;
}
