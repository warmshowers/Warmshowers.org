<?php

/**
 * @file
 * Node convert forum.module include
 *
 * Performs necessary changes regarding forum type conversions.
 *
 * @TODO Fix for Drupal 7.
 */

/**
 * Implementation of node_convert_change().
 */
function forum_node_convert_change($data, $op) {
  if ($op == 'insert') {
    if ($data['dest_node_type'] == 'forum') {
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('INSERT INTO {forum} (tid, vid, nid) VALUES (%d, %d, %d)', $data['hook_options']['forum'], $data['node']->vid, $data['node']->nid) */
      $id = db_insert('forum')
  ->fields(array(
        'tid' => $data['hook_options']['forum'],
        'vid' => $data['node']->vid,
        'nid' => $data['node']->nid,
      ))
  ->execute();
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('INSERT INTO {taxonomy_term_node} (tid, vid, nid) VALUES (%d, %d, %d)', $data['hook_options']['forum'], $data['node']->vid, $data['node']->nid) */
      $id = db_insert('taxonomy_term_node')
  ->fields(array(
        'tid' => $data['hook_options']['forum'],
        'vid' => $data['node']->vid,
        'nid' => $data['node']->nid,
      ))
  ->execute();
    }
  }
  elseif ($op == 'delete') {
    if ($data['node']->type == 'forum') {
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('DELETE FROM {forum} WHERE nid = %d', $data['node']->nid) */
      db_delete('forum')
  ->condition('nid', $data['node']->nid)
  ->execute();
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('DELETE FROM {taxonomy_term_node} WHERE nid = %d', $data['node']->nid) */
      db_delete('taxonomy_term_node')
  ->condition('nid', $data['node']->nid)
  ->execute();
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
        $term = taxonomy_term_load($term);
        form_set_error('hook_options][forum', t('The item %forum is only a container for forums. Please select one of the forums below it.', array('%forum' => $term->name)));
      }
    }
  }
}
