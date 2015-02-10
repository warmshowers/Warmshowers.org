<?php
/**
 * @file
 * Views 2 hooks and callback registries.
 */


/**
 * Implementation of hook_views_handlers().
 */
function uc_views_bulk_operations_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'uc_views_bulk_operations') . '/views',
    ),
    'handlers' => array(
      // filters
    ),
  );
}

/**
 * Conditionally add editablefields support.
 */
function uc_views_bulk_operations_views_tables_alter(&$tables) {
}

