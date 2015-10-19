<?php
/**
 * @file
 * ws_cron_features.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ws_cron_features_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ultimate_cron" && $api == "ultimate_cron") {
    return array("version" => "3");
  }
}
