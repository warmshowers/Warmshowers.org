<?php
/**
 * @file
 * ws_d7_upgrade_features.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function ws_d7_upgrade_features_filter_default_formats() {
  $formats = array();

  // Exported format: markdown.
  $formats[5] = array(
    'format' => 5,
    'name' => 'markdown',
    'cache' => 1,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_markdown' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_autop' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -45,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
