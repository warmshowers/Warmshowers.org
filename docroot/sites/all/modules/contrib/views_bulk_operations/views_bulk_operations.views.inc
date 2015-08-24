<?php

function views_bulk_operations_views_plugins() {
  return array(
    'style' => array(
      'bulk' => array(
        'title' => t('Bulk Operations'),
        'help' => t('Displays rows with checkmarks for bulk actions.'),
        'handler' => 'views_bulk_operations_plugin_style',
        'parent' => 'table',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
}
