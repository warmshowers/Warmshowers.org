<?php

/**
 * @file
 * Node convert book.module include
 *
 * Performs necessary changes regarding book type conversions.
 * @TODO Fix for Drupal 7.
 */

/**
 * Implementation of node_convert_change().
 */
function book_node_convert_change($data, $op) {
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
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query("INSERT INTO {book} (nid, mlid, bid) VALUES (%d, %d, %d)", $node->nid, $book['mlid'], $book['bid']) */
      $id = db_insert('book')
  ->fields(array(
        'nid' => $node->nid,
        'mlid' => $book['mlid'],
        'bid' => $book['bid'],
      ))
  ->execute();
    }
  }
  elseif ($op == 'delete') {
    if ($data['node']->type == 'book') {
      menu_link_delete($data['node']->book['mlid']);
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('DELETE FROM {book} WHERE mlid = %d', $data['node']->book['mlid']) */
      db_delete('book')
  ->condition('mlid', $data['node']->book['mlid'])
  ->execute();
    }
  }
  elseif ($op == 'options') {
    $form = array();
    if ($data['dest_node_type'] == 'book') {
      $options = array();
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
    return $form;
  }
}
