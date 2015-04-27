<?php

/**
 * @file
 * Provides the views data and handlers for nodeblock.module.
 */

/**
 * Implements hook_views_data().
 */
function nodeblock_views_data() {
  $data = array();
  // Define the {nodeblock} table.
  $data['nodeblock']['table']['group'] = t('Node Block');
  // We can use this as a base table for Views.
  $data['nodeblock']['table']['base'] = array(
    'field' => 'nodeblock',
    'title' => t('Node Block'),
    'help' => t('Node Block turns nodes into blocks.'),
  );
  // Define the joins.
  $data['nodeblock']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );
  // Define the fields.
  $data['nodeblock']['block_title'] = array(
    'title' => t('Block title'),
    'help' => t('The block title for this node.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a block title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a title.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['nodeblock']['enabled'] = array(
    'title' => t('Block available'),
    'help' => t('Whether the node is available as block.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'yes-no' => array(t('Available'), t('Unavailable')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Available as node block'),
      'type' => 'yes-no',
    ),
  );
  return $data;
}
