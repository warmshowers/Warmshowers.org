<?php
/**
 * @file
 * API documentation for the node_convert module.
 */

/**
 * Provide implementation of hook_ctools_plugin_api().
 *
 * This will tell CTools which file contains the implementation of hook_node_convert_change().
 * The conversion hook will not execute without first defining the ctools hook.
 *
 * Note that your custom module may already include this hook if you use CTools export-ables, in which case
 * you should just add the relevant conditional inside the existing implementation.
 */
function hook_ctools_plugin_api($module, $api) {
  // Conversion behaviors.
  if ($module == 'node_convert' && $api == NODE_CONVERT_BEHAVIOR_PLUGIN) {
    return array(
      'version' => 1,
      'path' => drupal_get_path('module', 'custom') . '/includes/',
      'file' => "$module.$api.inc",
    );
  }

  return NULL;
}


/**
 * This is an example implementation for the hook. Preforms actions when converting a node based on it's type.
 *
 * @param $data
 *   An array containing information about the conversion process. The keys are
 *   - dest_node_type  The destination type of the node
 *   - node  The node object
 *   - Any other information passed by $op = 'options' or $op = 'options validate'
 * @param $op
 *   A string containing the operation which should be executed. These are the possible values
 *   - insert  Operations which should be run when the node is transferred to the new node type.
 *   Usually for transferring and adding new node information into the database.
 *   - delete  Operations which should be run after the node is transferred to the new node type.
 *   Usually for deleting unneeded information from the database after the transfer.
 *   - options  Configuration elements shown on the conversion form. Should return a FAPI array.
 *   - options validate  Validation check on the options elements.
 * @return
 *    Should return a FAPI array only when using the options operation.
 */
function hook_node_convert_change($data, $op) {
  // All of this is just an example.
  if ($op == 'insert') {
    if ($data['dest_node_type'] == 'book') {
      $book = array();
      $node = $data['node'];
      $book['link_path'] = 'node/' . $node->nid;
      $book['link_title'] = $node->title;
      $book['plid'] = 0;
      $book['menu_name'] = book_menu_name($node->nid);
      $mlid = menu_link_save($book);
      $book['bid'] = $data['hook_options']['bid'];
      if ($book['bid'] == 'self') {
        $book['bid'] = $node->nid;
      }
      $id = db_insert('book')
        ->fields(array(
          'nid' => $node->nid,
          'mlid' => $book['mlid'],
          'bid' => $book['bid'],
        ))
        ->execute();
    }
    if ($data['dest_node_type'] == 'forum') {
      $id = db_insert('forum')
        ->fields(array(
          'tid' => $data['hook_options']['forum'],
          'vid' => $data['node']->vid,
          'nid' => $data['node']->nid,
        ))
        ->execute();

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
    if ($data['node']->type == 'book') {
      menu_link_delete($data['node']->book['mlid']);
      db_delete('book')
        ->condition('mlid', $data['node']->book['mlid'])
        ->execute();
    }
    if ($data['node']->type == 'forum') {
      db_delete('forum')
        ->condition('nid', $data['node']->nid)
        ->execute();

      db_delete('taxonomy_term_node')
        ->condition('nid', $data['node']->nid)
        ->execute();
    }
  }
  elseif ($op == 'options') {
    $form = array();
    if ($data['dest_node_type'] == 'book') {
      foreach (book_get_books() as $book) {
        $options[$book['nid']] = $book['title'];
      }
      $options = array('self' => '<' . t('create a new book') . '>') + $options;
      $form['bid'] = array(
        '#type' => 'select',
        '#title' => t('Book'),
        '#options' => $options,
        '#description' => t('Your page will be a part of the selected book.'),
        '#attributes' => array('class' => 'book-title-select'),
      );
    }
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

/**
 * Allow modifying a node during conversion but before the final save.
 *
 * @param stdClass $node
 *   The node object.
 * @param array $hook_options
 *   Additional options passed to node_convert_node_convert().
 */
function hook_node_convert_presave($node, $hook_options = array()) {
  // Set the author to user 1.
  $node->uid = 1;
}
