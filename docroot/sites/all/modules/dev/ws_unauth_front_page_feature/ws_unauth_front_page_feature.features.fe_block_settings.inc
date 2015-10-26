<?php
/**
 * @file
 * ws_unauth_front_page_feature.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function ws_unauth_front_page_feature_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['nodeblock-15521'] = array(
    'cache' => 1,
    'css_class' => 'block-mobileapps',
    'custom' => 0,
    'delta' => 15521,
    'module' => 'nodeblock',
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
    ),
    'title' => '<none>',
    'visibility' => 0,
  );

  $export['nodeblock-15526'] = array(
    'cache' => 1,
    'css_class' => 'block-howitworks',
    'custom' => 0,
    'delta' => 15526,
    'module' => 'nodeblock',
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
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
