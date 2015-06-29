<?php
/**
 * @file
 * comments_review_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function comments_review_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'comments_review';
  $view->description = 'Comments Review (Admin)';
  $view->tag = 'admin';
  $view->base_table = 'comment';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 20;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Comment: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'comment';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'User';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Comment: Author */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'comment';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['name']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Comment: Updated date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'comment';
  $handler->display->display_options['fields']['timestamp']['field'] = 'changed';
  $handler->display->display_options['fields']['timestamp']['label'] = '';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
  /* Field: Comment: Title */
  $handler->display->display_options['fields']['subject']['id'] = 'subject';
  $handler->display->display_options['fields']['subject']['table'] = 'comment';
  $handler->display->display_options['fields']['subject']['field'] = 'subject';
  $handler->display->display_options['fields']['subject']['label'] = '';
  /* Field: Comment: Comment */
  $handler->display->display_options['fields']['comment']['id'] = 'comment';
  $handler->display->display_options['fields']['comment']['table'] = 'field_data_comment_body';
  $handler->display->display_options['fields']['comment']['field'] = 'comment_body';
  $handler->display->display_options['fields']['comment']['label'] = '';
  $handler->display->display_options['fields']['comment']['alter']['max_length'] = '500';
  $handler->display->display_options['fields']['comment']['alter']['trim'] = TRUE;
  /* Sort criterion: Comment: Updated date */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'comment';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'changed';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Filter criterion: Comment: Updated date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'comment';
  $handler->display->display_options['filters']['timestamp']['field'] = 'changed';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'review_block');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Comment: Updated date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'comment';
  $handler->display->display_options['filters']['timestamp']['field'] = 'changed';
  $handler->display->display_options['filters']['timestamp']['operator'] = '>=';
  $handler->display->display_options['filters']['timestamp']['value']['value'] = '-1 day';
  $handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['timestamp']['group'] = '0';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Comment: Post date';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
    2 => '2',
    3 => 0,
    1 => 0,
    27 => 0,
    26 => 0,
    15 => 0,
    24 => 0,
    8 => 0,
    19 => 0,
    23 => 0,
    18 => 0,
    25 => 0,
    22 => 0,
    20 => 0,
    12 => 0,
    7 => 0,
    17 => 0,
    9 => 0,
    6 => 0,
    13 => 0,
    11 => 0,
    16 => 0,
  );

  /* Display: Email version */
  $handler = $view->new_display('block', 'Email version', 'review_email');
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'timestamp' => 'name',
    'subject' => 'name',
    'comment' => 'comment',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'separator' => ' | ',
    ),
    'timestamp' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'subject' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'comment' => array(
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Comment: Updated date */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'comment';
  $handler->display->display_options['filters']['timestamp']['field'] = 'changed';
  $handler->display->display_options['filters']['timestamp']['operator'] = '>=';
  $handler->display->display_options['filters']['timestamp']['value']['value'] = '-1 day';
  $handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['timestamp']['group'] = '0';
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'review_page');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['path'] = 'admin/content/comments/review';
  $translatables['comments_review'] = array(
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
    t('Uid'),
    t('Block'),
    t('Comment: Post date'),
    t('Email version'),
    t('Page'),
  );
  $export['comments_review'] = $view;

  return $export;
}
