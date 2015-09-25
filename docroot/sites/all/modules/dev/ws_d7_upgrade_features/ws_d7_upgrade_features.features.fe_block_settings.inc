<?php
/**
 * @file
 * ws_d7_upgrade_features.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function ws_d7_upgrade_features_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['lang_dropdown-language'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'language',
    'module' => 'lang_dropdown',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'warmshowers_zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'warmshowers_zen',
        'weight' => 0,
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => 0,
      ),
    ),
    'title' => '<none>',
    'visibility' => 0,
  );

  return $export;
}
