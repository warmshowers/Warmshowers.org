<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ws_guest_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function ws_guest_profile_node_info() {
  $items = array(
    'hosting_request' => array(
      'name' => t('Hosting Request'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Comments'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'traveler_trip' => array(
      'name' => t('Traveler Trip'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Trip Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function ws_guest_profile_views_api() {
  return array(
    'api' => '2',
  );
}
