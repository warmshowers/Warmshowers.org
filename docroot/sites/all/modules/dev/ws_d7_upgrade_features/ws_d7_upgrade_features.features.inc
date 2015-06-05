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

/**
 * Implements hook_views_api().
 */
function ws_d7_upgrade_features_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function ws_d7_upgrade_features_node_info() {
  $items = array(
    'trust_referral' => array(
      'name' => t('Feedback'),
      'base' => 'node_content',
      'description' => t('This is a way for one member to leave feedback for another.  If you\'ve stayed with someone as host, or hosted someone who\'s a good guest, please put a reference for them here. It will show up on their member page.  And your story will be visible in the "all references area" too. '),
      'has_title' => '1',
      'title_label' => t('Your Experience'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
