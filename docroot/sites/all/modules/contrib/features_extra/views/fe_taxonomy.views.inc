<?php

/**
 * @file
 * Provide views data and handlers for fe_taxonomy.module
 */

/**
 * Implementation of hook_views_data()
 */
function fe_taxonomy_views_data() {
  // Table
  $data['fe_taxonomy_vocabulary']['table']['group']  = t('FE Taxonomy');
//  $data['fe_taxonomy_vocabulary']['table']['base'] = array(
//    'field' => 'vid',
//    'title' => t('fe_taxonomy_vocabulary'),
//    'help' => t('The table saves machine name of vocabularies.'),
//  );
  $data['fe_taxonomy_vocabulary']['table']['join'] = array(
    'vocabulary' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'term_data' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node' => array(
      'left_table' => 'term_data',
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // Fields
  $data['fe_taxonomy_vocabulary']['machine_name'] = array(
    'title' => t('Machine name'),
    'help' => t('Machine name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

//  $data['fe_taxonomy_vocabulary_x']['table']['group']  = t('[features extra] vocabulary');

//  $data['term_node']['tid'] = array(
//    'title' => t('Term ID'),
//    'help' => t('The taxonomy term ID'),
//    'field' => array(
//      'title' => t('All terms'),
//      'help' => t('Display all taxonomy terms associated with a node from specified vocabularies.'),
//      'handler' => 'views_handler_field_term_node_tid',
//      'skip base' => 'term_data',
//    ),
//    'argument' => array(
//      'handler' => 'views_handler_argument_term_node_tid',
//      'name table' => 'term_data',
//      'name field' => 'name',
//      'empty field name' => t('Uncategorized'),
//      'numeric' => TRUE,
//      'skip base' => 'term_data',
//    ),
//    'filter' => array(
//      'title' => t('Term'),
//      'handler' => 'views_handler_filter_term_node_tid',
//      'hierarchy table' => 'term_hierarchy',
//      'numeric' => TRUE,
//      'skip base' => 'term_data',
//      'allow empty' => TRUE,
//    ),
//  );

  return $data;
}
