<?php
/**
 * @file
 * d7_theme_upgrade.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function d7_theme_upgrade_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
