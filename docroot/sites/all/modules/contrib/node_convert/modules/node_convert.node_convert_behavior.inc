<?php
/**
 * @file
 * Declare API compatibility on behalf of certain modules, in case they are missing.
 */

$provided_module_behaviors = node_convert_provided_module_behaviors();
$node_convert_api_name = NODE_CONVERT_BEHAVIOR_PLUGIN;
$path = DRUPAL_ROOT . '/' . drupal_get_path('module', 'node_convert') . '/modules/';

foreach ($provided_module_behaviors as $module) {
  if (!function_exists("{$module}_node_convert_change")) {
    $include_file = "{$path}$module.$node_convert_api_name.inc";
    require_once $include_file;
  }
}