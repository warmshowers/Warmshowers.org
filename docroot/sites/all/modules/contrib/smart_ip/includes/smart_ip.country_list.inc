<?php

/**
 * @file
 * Provides a list of countries based on ISO standards.
 */

/**
 * Get an array of all country code => country name pairs.
 *
 * Get an array of all country code => country name pairs as laid out
 * in ISO 3166-1 alpha-2.
 * @return
 *   An array of all country code => country name pairs.
 */
function country_get_predefined_list() {
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();

  // Allow other modules to modify country list via hook_smart_ip_country_predefined_list_alter()
  drupal_alter('smart_ip_country_predefined_list', $countries);

  return $countries;
}