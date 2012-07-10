<?php

/**
 * @file
 * colorbox.views.inc
 */

/**
 * Implementation of hook_views_data().
 */
function colorbox_views_data() {

  $data['colorbox']['table']['group'] = t('Colorbox');

  $data['colorbox']['table']['join'] = array(
    '#global' => array(),
  );

  $data['colorbox']['colorbox'] = array(
    'group' => t('Colorbox'),
    'field' => array(
      'title' => t('Colorbox trigger'),
      'help' => t('Provide custom text or link.'),
      'handler' => 'colorbox_handler_field_colorbox',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function colorbox_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'colorbox') .'/views',
    ),
    'handlers' => array(
      'colorbox_handler_field_colorbox' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
