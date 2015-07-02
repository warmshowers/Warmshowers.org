<?php
/**
 * @file
 * system_logs_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function system_logs_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'system_logs';
  $view->description = 'Views to display system logs.';
  $view->tag = '';
  $view->base_table = 'watchdog';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => 3,
    6 => 6,
    13 => 13,
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 200;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'message' => 'message',
    'timestamp' => 'timestamp',
    'type' => 'type',
    'name' => 'name',
    'hostname' => 'hostname',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'message' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'type' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'hostname' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* Relationship: Watchdog: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'watchdog';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'User';
  /* Field: Watchdog: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'watchdog';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  /* Field: Watchdog: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'watchdog';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Username';
  /* Field: Watchdog: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'watchdog';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '1200';
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['message']['watchdog_message_format'] = 1;
  $handler->display->display_options['fields']['message']['watchdog_message_link'] = 1;
  /* Field: Watchdog: Hostname */
  $handler->display->display_options['fields']['hostname']['id'] = 'hostname';
  $handler->display->display_options['fields']['hostname']['table'] = 'watchdog';
  $handler->display->display_options['fields']['hostname']['field'] = 'hostname';
  $handler->display->display_options['fields']['hostname']['label'] = 'IP Address';
  /* Filter criterion: Watchdog: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'watchdog';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'contains';
  $handler->display->display_options['filters']['message']['group'] = '0';
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Anything in Message Content';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  /* Filter criterion: Watchdog: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'watchdog';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'access denied' => 'access denied',
    'action' => 'action',
    'actions' => 'actions',
    'browscap' => 'browscap',
    'browser' => 'browser',
    'comment_notify' => 'comment_notify',
    'content' => 'content',
    'cron' => 'cron',
    'fboauth' => 'fboauth',
    'form' => 'form',
    'geocode' => 'geocode',
    'googleanalytics' => 'googleanalytics',
    'i18nstrings' => 'i18nstrings',
    'imageapi' => 'imageapi',
    'imagecache' => 'imagecache',
    'l10n_update' => 'l10n_update',
    'locale' => 'locale',
    'mail' => 'mail',
    'mandrill_incomin' => 'mandrill_incomin',
    'page not found' => 'page not found',
    'php' => 'php',
    'privatemsg' => 'privatemsg',
    'role_delay' => 'role_delay',
    'rules' => 'rules',
    'search' => 'search',
    'security' => 'security',
    'uc_order' => 'uc_order',
    'uc_payment' => 'uc_payment',
    'uc_recurring' => 'uc_recurring',
    'uc_stripe' => 'uc_stripe',
    'user' => 'user',
    'vbo' => 'vbo',
    'wsuser' => 'wsuser',
    'wsuser_pester' => 'wsuser_pester',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Message Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['reduce'] = TRUE;
  /* Filter criterion: Watchdog: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'watchdog';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = '0';
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  /* Filter criterion: Watchdog: Timestamp */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'watchdog';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['operator'] = 'between';
  $handler->display->display_options['filters']['timestamp']['group'] = '0';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Timestamp (like YYYY-MM-DD or "-5 days")';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = '0';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: Watchdog: Hostname */
  $handler->display->display_options['filters']['hostname']['id'] = 'hostname';
  $handler->display->display_options['filters']['hostname']['table'] = 'watchdog';
  $handler->display->display_options['filters']['hostname']['field'] = 'hostname';
  $handler->display->display_options['filters']['hostname']['operator'] = 'starts';
  $handler->display->display_options['filters']['hostname']['group'] = '0';
  $handler->display->display_options['filters']['hostname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['hostname']['expose']['operator_id'] = 'hostname_op';
  $handler->display->display_options['filters']['hostname']['expose']['label'] = 'IP Address';
  $handler->display->display_options['filters']['hostname']['expose']['operator'] = 'hostname_op';
  $handler->display->display_options['filters']['hostname']['expose']['identifier'] = 'hostname';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'latest_logs');
  $handler->display->display_options['path'] = 'admin/reports/logs_filterable';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Recent logs (filterable!)';
  $handler->display->display_options['menu']['weight'] = '-2';
  $translatables['system_logs'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('User'),
    t('Timestamp'),
    t('Type'),
    t('Username'),
    t('Message'),
    t('IP Address'),
    t('Anything in Message Content'),
    t('Message Type'),
    t('Severity'),
    t('Timestamp (like YYYY-MM-DD or "-5 days")'),
    t('Page'),
  );
  $export['system_logs'] = $view;

  return $export;
}
