<?php
// $Id: forum.node_convert.inc,v 1.1.2.2 2009/05/01 21:30:03 placinta Exp $

/**
 * @file
 * Node convert forum.module include
 *
 * Performs necessary changes regarding forum type conversions.
 */

/**
 * Implementation of node_convert_change().
 */
function forum_node_convert_change($data, $op) {
  if ($op == 'insert') {
    if ($data['dest_node_type'] == 'forum') {
      db_query('INSERT INTO {forum} (tid, vid, nid) VALUES (%d, %d, %d)', $data['hook_options']['forum'], $data['node']->vid, $data['node']->nid);
      db_query('INSERT INTO {term_node} (tid, vid, nid) VALUES (%d, %d, %d)', $data['hook_options']['forum'], $data['node']->vid, $data['node']->nid);
    }
  }
  elseif ($op == 'delete') {
    if ($data['node']->type == 'forum') {
      db_query('DELETE FROM {forum} WHERE nid = %d', $data['node']->nid);
      db_query('DELETE FROM {term_node} WHERE nid = %d', $data['node']->nid);
    }
  }
  elseif ($op == 'options') {
    $form = array();
    if ($data['dest_node_type'] == 'forum') {
      $vid = variable_get('forum_nav_vocabulary', '');
      $form['forum'] = taxonomy_form($vid);
      $form['forum']['#weight'] = 7;
      $form['forum']['#required'] = TRUE;
      $form['forum']['#options'][''] = t('- Please choose -');
    }
    return $form;
  }
  elseif ($op == 'options validate') {
    $form_state = $data['form_state'];
    if ($data['dest_node_type'] == 'forum') {
      $containers = variable_get('forum_containers', array());
      $term = $form_state['values']['hook_options']['forum'];
      if (in_array($term, $containers)) {
        $term = taxonomy_get_term($term);
        form_set_error('hook_options][forum', t('The item %forum is only a container for forums. Please select one of the forums below it.', array('%forum' => $term->name)));
      }
    }
  }
}