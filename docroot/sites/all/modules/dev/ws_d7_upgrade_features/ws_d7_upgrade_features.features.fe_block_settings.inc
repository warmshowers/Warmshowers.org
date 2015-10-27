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

  return $export;
}
