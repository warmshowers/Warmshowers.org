<?php

/**
 * @file
 * Exposes faq module's database tables to views.
 */

/**
 * Implements hook_views_data().
 */
function faq_views_data() {
  // faq_questions table.
  $data['faq_questions']['table']['group'] = t('FAQ');
  $data['faq_questions']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['faq_questions']['question'] = array(
    'title' => t('FAQ short question'),
    'help' => t('The short question text for FAQ nodes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['faq_questions']['detailed_question'] = array(
    'title' => t('FAQ detailed question'),
    'help' => t('The long question text for FAQ nodes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  // faq_weights table.
  $data['faq_weights']['table']['group'] = t('FAQ');
  $data['faq_weights']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );
  $data['faq_weights']['tid'] = array(
    'title' => t('FAQ category tid'),
    'help' => t('The tid of the FAQ node.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
      'zero is null' => FALSE,
    ),
    'filter' => array(
      'title' => t('Tid'),
      'handler' => 'views_handler_filter_numeric',
      'hierarchy table' => 'term_hierarchy',
      'numeric' => TRUE,
      'skip base' => 'term_data',
      'allow empty' => TRUE,
      'zero is null' => FALSE,
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('category'),
      'base' => 'term_node',
      'base field' => 'tid',
      'skip base' => 'term_data',
    ),
  );
  $data['faq_weights']['category'] = array(
    'title' => t('FAQ category'),
    'help' => t('The term of the FAQ node.'),
    'real field' => 'tid',
    'field' => array(
      'handler' => 'views_handler_field_term_node_tid',
      'click sortable' => TRUE,
      'zero is null' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_term_node_tid',
      'name table' => 'term_data',
      'name field' => 'name',
      'empty field name' => t('Uncategorized'),
      'numeric' => TRUE,
      'skip base' => 'term_data',
      'zero is null' => TRUE,
    ),
    'filter' => array(
      'title' => t('Term'),
      'handler' => 'views_handler_filter_term_node_tid',
      'hierarchy table' => 'term_hierarchy',
      'numeric' => TRUE,
      'skip base' => 'term_data',
      'allow empty' => TRUE,
      'zero is null' => FALSE,
    ),
  );
  $data['faq_weights']['weight'] = array(
    'title' => t('FAQ weight'),
    'help' => t('The weight of the FAQ node.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

