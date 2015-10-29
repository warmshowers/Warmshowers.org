<?php
/**
 * @file
 * Views inc file for disable breadcrumbs module.
 */

/**
 * Implements hook_views_data().
 */
function disable_breadcrumbs_views_data() {
  $data['disable_breadcrumbs'] = array(
    'table' => array(
      'group' => t('Disable breadcrumbs'),
      'join' => array(
        'node' => array(
          'table' => 'disable_breadcrumbs',
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    ),
    'nid' => array(
      'title' => t('Node ID'),
      'description' => t('The node ID (nid) referenced in the disable breadcrumbs table'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'relationship' => array(
        'base' => 'node',
        'field' => 'nid',
        'label' => 'content',
        'help' => t('Bring in data about the disable breadcrumbs status of the node.'),
      ),
    ),
    'disable_breadcrumb' => array(
      'title' => t('Disable breadcrumb status'),
      'help' => t('Whether the breadcrumb for this node is disabled or not.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );

  return $data;
}
