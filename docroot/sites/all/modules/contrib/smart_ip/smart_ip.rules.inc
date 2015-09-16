<?php

/**
 * @file
 * Rules integration for the Smart IP module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_condition_info().
 */
function smart_ip_rules_condition_info() {
  $items = array();
  $items['smart_ip_condition_in_country'] = array(
    'label' => t('IP address is in country'),
    'group' => t('Smart IP'),
    'parameter' => array(
      'country' => array(
        'type' => 'list<text>',
        'label' => t('Country'),
        'description' => t('The countries to check for.'),
        'options list' => 'smart_ip_country_list',
      ),
    ),
  );
  $items['smart_ip_condition_in_region'] = array(
    'label' => t('IP address is in region'),
    'group' => t('Smart IP'),
    'parameter' => array(
      'regid' => array(
        'type' => 'text',
        'label' => t('Region ID'),
        'description' => t('The region ID to check for.'), 
      ),
    ),
  );
  $items['smart_ip_condition_in_region_name'] = array(
    'label' => t('IP address is in region name'),
    'group' => t('Smart IP'),
    'parameter' => array(
      'regname' => array(
        'type' => 'text',
        'label' => t('Region Name'),
        'description' => t('The region name to check for.'), 
      ),
    ),
  );	
  $items['smart_ip_condition_in_city'] = array(
    'label' => t('IP address is in city'),
    'group' => t('Smart IP'),
    'parameter' => array(
      'city' => array(
        'type' => 'text',
        'label' => t('City'),
        'description' => t('The city to check for.'), 
      ),
    ),
  ); 
  return $items;
}

/**
 * Returns a list of countires from locale module.
 */
function smart_ip_country_list() {
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  return country_get_list();
}

/**
 * Condition implementation for IP address in country.
 */
function smart_ip_condition_in_country($countries) {
  $location = smart_ip_get_current_visitor_location_data();
  if (isset($location['country_code']) && isset($countries[$location['country_code']])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Condition implementation for IP address in region.
 */
function smart_ip_condition_in_region($regid) {
  $location = smart_ip_get_current_visitor_location_data();
  if ($location['region_code'] == $regid) {
	  return TRUE;
  }
  return FALSE;
}

/**
 * Condition implementation for IP address in region name.
 */
function smart_ip_condition_in_region_name($regname) {
  $location = smart_ip_get_current_visitor_location_data();
  if ($location['region'] == $regname) {
	  return TRUE;
  }
  return FALSE;
}

/**
 * Condition implementation for IP address in city.
 */
function smart_ip_condition_in_city($city) {
  $location = smart_ip_get_current_visitor_location_data();
  if ($location['city'] == $city) {
    return TRUE;
  }
  return FALSE;
}

/**
 * @}
 */