<?php
// $Id: book.node_convert.inc,v 1.1.2.2 2009/05/01 21:30:03 placinta Exp $

/**
 * @file
 * Node convert book.module include
 *
 * Performs necessary changes regarding book type conversions.
 */

/**
 * Implementation of node_convert_change().
 */
function book_node_convert_change($data, $op) {
  if ($op == 'insert') {
    if ($data['dest_node_type'] == 'book') {
      $book = array();
      $node = $data['node'];
      $book['link_path'] = 'node/'. $node->nid;
      $book['link_title'] = $node->title;
      $book['plid'] = 0;
      $book['menu_name'] = book_menu_name($node->nid);
      $mlid = menu_link_save($book);
      $book['bid'] = $data['hook_options']['bid'];
      if ($book['bid'] == 'self') $book['bid'] = $node->nid;
      db_query("INSERT INTO {book} (nid, mlid, bid) VALUES (%d, %d, %d)", $node->nid, $book['mlid'], $book['bid']);
    }
  }
  elseif ($op == 'delete') {
    if ($data['node']->type == 'book') {
      menu_link_delete($data['node']->book['mlid']);
      db_query('DELETE FROM {book} WHERE mlid = %d', $data['node']->book['mlid']);
    }
  }
  elseif ($op == 'options') {
    $form = array();
    if ($data['dest_node_type'] == 'book') {
      $options = array();
      foreach (book_get_books() as $book) {
        $options[$book['nid']] = $book['title'];
      }
      $options = array('self' => '<'. t('create a new book') .'>') + $options;
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