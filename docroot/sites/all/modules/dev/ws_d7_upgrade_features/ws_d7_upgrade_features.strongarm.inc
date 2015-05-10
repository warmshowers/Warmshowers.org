<?php
/**
 * @file
 * ws_d7_upgrade_features.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ws_d7_upgrade_features_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'search_active_modules';
  $strongarm->value = array(
    'node' => 'node',
    'user' => 'user',
    'wsuser' => 'wsuser',
  );
  $export['search_active_modules'] = $strongarm;

  return $export;
}
