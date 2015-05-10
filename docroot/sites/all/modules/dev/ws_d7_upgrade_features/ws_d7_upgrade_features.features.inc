<?php
/**
 * @file
 * ws_d7_upgrade_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ws_d7_upgrade_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
