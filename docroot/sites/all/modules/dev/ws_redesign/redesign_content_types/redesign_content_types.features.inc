<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function redesign_content_types_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function redesign_content_types_node_info() {
  $items = array(
    'block' => array(
      'name' => t('Block'),
      'module' => 'features',
      'description' => t('Intended to be deployed as a block'),
      'has_title' => '1',
      'title_label' => t('Block Title'),
      'has_body' => '1',
      'body_label' => t('Block Contents'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'sitebuilding' => array(
      'name' => t('Sitebuilding'),
      'module' => 'features',
      'description' => t('This is to be used for complex sitebuilding approaches, where special markup may be required, etc. It\'s not for ordinary users.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
