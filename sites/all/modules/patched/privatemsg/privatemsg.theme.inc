<?php

/**
 * @file
 * Theme functions for privatemsg.
 */

/**
 * @defgroup theming Theming documentation
 *
 * It is possible to theme many aspect of privatemsg with theme functions.
 *
 * For the thread list, so called theme patterns are used to allow flexible
 * theming of the table and its columns (including columns added by other
 * modules).
 *
 * Columns are defined with hook_privatemsg_header_info(). The default weight
 * and if they are enabled or not can by set by default but can be overriden in
 * the admin user interface.
 *
 * Additionally, a theme function needs to be defined that defaults to the name
 * 'privatemsg_list_field__$key', where $key is the name of the header/column.
 *
 * Every theme function will receive the $thread array as single argument, which
 * is everything that has been returned by the query built in
 * privatemsg_sql_list(). The theme function should return either a string or a
 * theme_table() compatible array.
 *
 * To change what information is available in that array, it is possible to
 * either implement hook_privatemsg_sql_list_alter() or alter $form['#data'] of
 * privatemsg_list(). See privatemsg_filter_form_privatemsg_list_alter() for an
 * example.
 *
 * To override an already existing theme function, use the following structure:
 * themename_privatemsg_list_field_fieldname. It is possible to change the
 * header definition with hook_privatemsg_header_info_alter() but that is often
 * not necessary.
 */

/**
 * @addtogroup theming
 * @{
 */

/**
 * Theme the participants field.
 */
function phptemplate_privatemsg_list_field__participants($thread) {
  $participants = _privatemsg_generate_user_array($thread['participants'], -4);
  $field = array();
  $field['data'] = _privatemsg_format_participants($participants, 3, TRUE);
  $field['class'] = 'privatemsg-list-participants';
  return $field;
}

/**
 * Theme the subject of the thread.
 */
function phptemplate_privatemsg_list_field__subject($thread) {
  $field = array();
  $options = array();
  $is_new = '';
  if (!empty($thread['is_new'])) {
    $is_new = theme_mark(MARK_NEW);
    $options['fragment'] = 'new';
  }
  $field['data'] = l($thread['subject'], privatemsg_get_dynamic_url_prefix() . '/view/' . $thread['thread_id'], $options) . $is_new;
  $field['class'] = 'privatemsg-list-subject';
  return $field;
}

/**
 * Theme the replies field.
 */
function phptemplate_privatemsg_list_field__count($thread) {
  $field = array();
  $field['data'] = $thread['count'];
  $options = array();
  if (!empty($thread['is_new']) && $thread['is_new'] < $thread['count']) {
    $options['fragment'] = 'new';
    $field['data'] .= '<br />' . l((format_plural($thread['is_new'], '(1 new)', '(@count new)')), privatemsg_get_dynamic_url_prefix() . '/view/' . $thread['thread_id'], $options);
  }
  $field['class'] = 'privatemsg-list-count';
  return $field;
}

/**
 * Theme the last updated column.
 */
function phptemplate_privatemsg_list_field__last_updated($thread) {
  $field = array();
  $field['data'] = format_date($thread['last_updated'], 'small');
  $field['class'] = 'privatemsg-list-date';
  return $field;
}

/**
 * Theme the thread started column.
 */
function phptemplate_privatemsg_list_field__thread_started($thread) {
  $field = array();
  $field['data'] = format_date($thread['thread_started'], 'small');
  $field['class'] = 'privatemsg-list-date-started';
  return $field;
}

/**
 * Theme to display the privatemsg list.
 *
 * This theme builds a table with paging based on the data which has been built
 * by the header and field theme patterns.
 */
function theme_privatemsg_list($form) {
  $has_posts = !empty($form['#data']);

  drupal_add_css(drupal_get_path('module', 'privatemsg') .'/styles/privatemsg-list.css');

  // Get the headers.
  $headers = privatemsg_get_headers(TRUE);
  $themed_rows = array();
  // Check if there is atleast a single thread.
  if ($has_posts) {
    foreach ($form['#data'] as $thread_id => $data) {
      // Theme the row.
      $row = _privatemsg_list_thread($data);
      $data = array();
      // Render the checkbox.
      $data[] = array('data' => drupal_render($form['threads'][$thread_id]), 'class' => 'privatemsg-list-select');

      // Store the #rows data in the same order as the header is,
      // the key property of the header refers to the field that belongs to it.
      foreach ($headers as $key => $header) {
        $data[] = isset($row['data'][$key]) ? $row['data'][$key] : '';
      }
      // Replace the data
      $row['data'] = $data;
      $themed_rows[] = $row;
    }
    // Add select all checkox to header array.
    array_unshift($headers, theme('table_select_header_cell'));
  }
  else {
    // Display a message if now messages are available.
    $themed_rows[] = array(array('data' => t('No messages available.'), 'colspan' => count($headers)));
  }

  // Remove any data in header that we don't need anymore.
  foreach ($headers as $id => $header) {
    unset($headers[$id]['key']);
    unset($headers[$id]['#weight']);
  }

  // Theme the table, pass all generated information to the table theme function.
  $form['list'] = array('#value' => theme('table', $headers, $themed_rows, array('class' => 'privatemsg-list')), '#weight' => 5);
  return drupal_render($form);
}

/**
 * Theme a block which displays the number of new messages a user has.
 */
function theme_privatemsg_new_block($count) {
  if ($count == 0) {
    $text = t('Click here to go to your messages.');
  }
  else {
    $text = format_plural($count, 'You have a new message! Click here to read it.',
                        'You have @count new messages! Click here to read them.',
                        array('@count' => $count));
    }

  return l($text, privatemsg_get_dynamic_url_prefix(), array('attributes' => array('id' => 'privatemsg-new-link')));
}

/**
 * Used to theme and display user recipients.
 *
 * Wrapper for theme_username() with a few additional options.
 */
function theme_privatemsg_username($recipient, $options) {
  if (!isset($recipient->uid)) {
    $recipient->uid = $recipient->recipient;
  }
  if (!empty($options['plain'])) {
    $name = $recipient->name;
    if (!empty($options['unique'])) {
      $name .= ' [user]';
    }
    return $name;
  }
  else {
    return theme('username', $recipient);
  }
}

/**
 * Output the admin settings display fields and weight settings as a
 * drag and drop sortable table.
 */
function theme_privatemsg_admin_settings_display_fields($element = array()) {
  $header = array(
    array('data' => t('Field'), 'class' => 'field'),
    array('data' => t('Enable'), 'class' => 'enable'),
    array('data' => t('Weight'), 'class' => 'weight'),
  );
  $rows = array();
  foreach (element_children($element['privatemsg_display_fields']) as $child) {
    $row = array();

    // Title.
    $row[] = array('data' => $element['privatemsg_display_fields'][$child]['#title'], 'class' => 'field');
    unset($element['privatemsg_display_fields'][$child]['#title']);

    // Enable checkbox.
    $row[] = array('data' => drupal_render($element['privatemsg_display_fields'][$child]), 'class' => 'enable');

    // Weight selector.
    unset($element['privatemsg_display_fields_weights'][$child]['#title']);
    $element['privatemsg_display_fields_weights'][$child]['#attributes']['class'] = 'privatemsg-display-fields-weight';
    $row[] = array(
      'data' => drupal_render($element['privatemsg_display_fields_weights'][$child]),
      'class' => 'weight',
    );

    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  if (!empty($rows)) {
    drupal_add_tabledrag('privatemsg-list-display-fields', 'order', 'sibling', 'privatemsg-display-fields-weight');
    return theme('table', $header, $rows, array('id' => 'privatemsg-list-display-fields')) . drupal_render($element);
  }
  else {
    return drupal_render($element);
  }
}

/**
 * @}
 */
