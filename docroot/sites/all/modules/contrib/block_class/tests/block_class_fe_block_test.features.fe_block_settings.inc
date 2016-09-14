<?php
/**
 * @file
 * block_class_fe_block_test.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function block_class_fe_block_test_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['user-online'] = array(
    'cache' => -1,
    'css_class' => 'fe_block-class1 fe_block-class2 fe_block-class3',
    'custom' => 0,
    'delta' => 'online',
    'module' => 'user',
    'node_types' => array(),
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'bartik' => array(
        'region' => 'content',
        'status' => 1,
        'theme' => 'bartik',
        'weight' => -7,
      ),
    ),
    'title' => 'Block Class Test Who\'s Online with FE Block',
    'visibility' => 1,
  );

  return $export;
}
