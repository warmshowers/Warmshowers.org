<?php

/**
 * @file
 * Views data and handlers for the views_watchdog module.
 */


/**
 * @defgroup views_watchdog_module views_watchdog.module handlers
 *
 * @{
 */


/**
 * Implements of hook_views_data().
 */
function views_watchdog_views_data() {

  $data = array();
  $data['watchdog']['table'] = array(
    'base' => array(
      'field' => 'wid',
      'title' => t('Watchdog entry'),
      'help' => t('Watchdog entries are a list of recorded system events.'),
      'weight' => 25,
    ),
    'group' => t('Watchdog'),
    'join' => array(
      'users' => array(
        'field' => 'uid',
        'left_field' => 'uid',
      ),
    ),
  );
  // Table field: watchdog.wid
  $data['watchdog']['wid'] = array(
    'title' => t('Wid'),
    'help' => t('Unique watchdog event ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'wid',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.uid
  $data['watchdog']['uid'] = array(
    'title' => t('User'),
    'help' => t('The ID of the user who triggered the event.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => 'Watchdog - Users',
    ),
  );
  // Table field: watchdog.type
  $data['watchdog']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of log message, for example "user" or "page not found".'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'type',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_watchdog_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.message
  $data['watchdog']['message'] = array(
    'title' => t('Message'),
    'help' => t('Text of log message.'),
    'field' => array(
      'handler' => 'views_handler_field_watchdog_message',
      'additional fields' => array('wid', 'variables'),
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'message',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_watchdog_message',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.variables
  $data['watchdog']['variables'] = array(
    'title' => t('Variables'),
    'help' => t('Serialized array of variables that match the message string and that is passed into the t() function.'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'variables',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.severity
  $data['watchdog']['severity'] = array(
    'title' => t('Severity'),
    'help' => t('The severity level of the event; ranges from 0 (Emergency) to 7 (Debug).'),
    'field' => array(
      'handler' => 'views_handler_field_watchdog_severity',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'severity',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_watchdog_severity',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.link
  $data['watchdog']['link'] = array(
    'title' => t('Link'),
    'help' => t('Link to view the result of the event.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'link',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.location
  $data['watchdog']['location'] = array(
    'title' => t('Location'),
    'help' => t('URL of the origin of the event.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'location',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.referer
  $data['watchdog']['referer'] = array(
    'title' => t('Referrer'),
    'help' => t('URL of referring page.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'referer',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.hostname
  $data['watchdog']['hostname'] = array(
    'title' => t('Hostname'),
    'help' => t('Hostname of the user who triggered the event.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'hostname',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Table field: watchdog.timestamp
  $data['watchdog']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Unix timestamp of when event occurred.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'name field' => 'timestamp',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  return $data;
}


/**
 * Implements of hook_views_plugins
 */
function views_watchdog_views_plugins() {

  $plugins = array(
    'module' => 'views_watchdog',
    'row' => array(
      'watchdog_rss' => array(
        'title' => t('Watchdog entry'),
        'help' => t('Display the watchdog entry.'),
        'handler' => 'views_plugin_row_watchdog_rss',
        'path' => drupal_get_path('module', 'views_watchdog') .'/views/plugins',
        'theme' => 'views_view_row_rss',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'views') .'/theme',
        'uses options' => TRUE,
        'type' => 'feed',
        'base' => array('watchdog'),
        'help topic' => 'style-watchdog-rss',
      ),
    ),
    'style' => array(
      'watchdog_table' => array(
        'title' => t('Watchdog table'),
        'help' => t('Displays watchdog entries as highlighted rows in a table.'),
        'handler' => 'views_plugin_style_watchdog_table',
        'path' => drupal_get_path('module', 'views_watchdog') .'/views/plugins',
        'theme' => 'views_view_watchdog_table',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'views_watchdog') .'/views/theme',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'parent' => 'table',
        'base' => array('watchdog'),
        'help topic' => 'style-watchdog-table',
      ),
    ),
  );

  return $plugins;
}


/**
 * @}
 */
