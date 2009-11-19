<?php
// $Id: devel_generate.drush.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 *  Generate content, taxonomy, and users via drush framework.
 */

/**
 * Implementation of hook_drush_help().
 */
function devel_generate_drush_help($section) {
  switch ($section) {
    case 'drush:generate users':
      return dt('Usage: drush [options] generate users <number_users> <kill>. <kill> is optional, type "kill" if you want to delete the users first.');
    case 'drush:generate taxonomy':
      return dt('Usage: drush [options] generate tax <number_vocab> <number_terms> <kill>. <kill> is optional, type "kill" if you want to delete all taxonomy first.');
    case 'drush:generate content':
      return dt('Usage: drush [options] generate content <number_nodes> <number_comments>. <kill> is optional, type "kill" if you want to delete all content first.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function devel_generate_drush_command() {
  $items['generate users'] = array(
    'callback' => 'drush_generate_users',
    'description' => 'Creates users.'
  );
  $items['generate taxonomy'] = array(
    'callback' => 'drush_generate_tax',
    'description' => 'Creates taxonomy.'
  );
  $items['generate content'] = array(
    'callback' => 'drush_generate_nodescontent',
    'description' => 'Creates content.'
  );
  return $items;
}


/**
 * Command callback. Generate a number of users.
 */
function drush_generate_users($num_users = NULL, $kill = '') {
  if (drush_generate_is_number($num_users) == FALSE) {
    drush_error(t('No valid number given as argument.'));
  }
  $kill = ($kill == 'kill') ? $kill = '1' : $kill = '';
  drush_generate_include_devel();
  devel_create_users($num_users, $kill);
  drush_print(t('Generated @number users.', array('@number' => $num_users)));
}

/**
 * Command callback. Generate a number of taxonomy.
 */
function drush_generate_tax($num_vocab = NULL, $num_terms = NULL, $kill = '') {
  if (drush_generate_is_number($num_vocab) == FALSE) {
    drush_error(t('No valid vocab number given as argument.'));
  }
  if (drush_generate_is_number($num_terms) == FALSE) {
    drush_error(t('No valid term number given as argument.'));
  }
  $kill = ($kill == 'kill') ? $kill = '1' : $kill = '';
  drush_generate_include_devel();
  devel_generate_taxonomy_data($num_vocab, $num_terms, '12', $kill);
  drush_print(t('Generated @num_vocab vocabulary, @num_terms terms', array('@num_vocab' => $num_vocab, '@num_terms' => $num_terms)));
}

/**
 * Command callback. Generate a number of content.
 */
function drush_generate_nodescontent($num_nodes = NULL, $num_comments = NULL, $kill = '') {
  if (drush_generate_is_number($num_nodes) == FALSE) {
    drush_error(t('No valid node number given as argument.'));
  }
  if (drush_generate_is_number($num_comments) == FALSE) {
    drush_error(t('No valid comment number given as argument.'));
  }

  // Let's load user 1, seems to be need for creating comments.
  global $user;
  $user_one = user_load(array('uid' => '1'));
  $user = $user_one;
  drupal_save_session(FALSE);
  
  if ($kill == 'kill') $values['values']['kill_content'] = 1;
  $values['values']['title_length'] = '8';
  $values['values']['num_nodes'] = $num_nodes;
  $values['values']['num_comments'] = $num_comments;
  $values['values']['node_types'] = array('page' => 'page', 'article' => 'article');
  drush_generate_include_devel();
  devel_generate_content($values);
  drush_print(t('Generated @num_nodes nodes, @num_comments comments per node', array('@num_nodes' => $num_nodes, '@num_comments' => $num_comments)));
}

//////////////////////////////////////////////////////////////////////////////
// Helper functions

// verify if param is a number
function drush_generate_is_number($number) {
  if ($number == NULL) return FALSE;
  if (!is_numeric($number)) return FALSE;
  if ($number < 0) return FALSE;
  return TRUE;
}

// include devel_generate.inc
function drush_generate_include_devel() {
  $path = drupal_get_path('module', 'devel_generate');
  require_once($path .'/devel_generate.inc');
}