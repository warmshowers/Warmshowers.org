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

  $export['lang_dropdown-language'] = array(
    'cache' => -1,
    'css_class' => '',
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
    ),
    'title' => '<none>',
    'visibility' => 0,
  );

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

  $export['views-46685cf431d761262ba5bcdf6cecb08a'] = array(
    'cache' => -1,
    'css_class' => '',
    'custom' => 0,
    'delta' => '46685cf431d761262ba5bcdf6cecb08a',
    'module' => 'views',
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

  return $export;
}
