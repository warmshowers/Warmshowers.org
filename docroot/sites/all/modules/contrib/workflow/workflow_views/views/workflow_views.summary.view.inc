<?php

/**
 * @file
 * View for listing nodes by workflow state.
 */

$view = new view();
$view->name = 'workflow_summary';
$view->description = 'Show all content and their workflow state.';
$view->tag = 'workflow';
$view->base_table = 'node';
$view->human_name = 'Workflow dashboard';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Workflow';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'access workflow summary views';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '12';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'nid' => 'nid',
  'title' => 'title',
  'type' => 'type',
  'name' => 'name',
  'hid_1' => 'hid_1',
  'sid' => 'sid',
  'stamp' => 'stamp',
  'name_1' => 'name_1',
  'comment' => 'comment',
  'workflow_node' => 'workflow_node',
  'edit_comment' => 'workflow_node',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'nid' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'hid_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'sid' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'stamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'comment' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'workflow_node' => array(
    'align' => '',
    'separator' => '<br />',
    'empty_column' => 0,
  ),
  'edit_comment' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['sticky'] = TRUE;
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid_3']['id'] = 'uid_3';
$handler->display->display_options['relationships']['uid_3']['table'] = 'node';
$handler->display->display_options['relationships']['uid_3']['field'] = 'uid';
/* Relationship: Workflow current: Current user */
$handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
$handler->display->display_options['relationships']['uid_1']['table'] = 'workflow_node_current';
$handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
$handler->display->display_options['relationships']['uid_1']['label'] = 'Current user';
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Content: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['exclude'] = TRUE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid_3';
$handler->display->display_options['fields']['name']['label'] = 'Author';
/* Field: Workflow current: Workflow executed transition ID */
$handler->display->display_options['fields']['hid_1']['id'] = 'hid_1';
$handler->display->display_options['fields']['hid_1']['table'] = 'workflow_node_current';
$handler->display->display_options['fields']['hid_1']['field'] = 'hid';
$handler->display->display_options['fields']['hid_1']['label'] = 'hid';
$handler->display->display_options['fields']['hid_1']['exclude'] = TRUE;
/* Field: Workflow current: Current state */
$handler->display->display_options['fields']['sid']['id'] = 'sid';
$handler->display->display_options['fields']['sid']['table'] = 'workflow_node_current';
$handler->display->display_options['fields']['sid']['field'] = 'sid';
/* Field: Workflow current: Time */
$handler->display->display_options['fields']['stamp']['id'] = 'stamp';
$handler->display->display_options['fields']['stamp']['table'] = 'workflow_node_current';
$handler->display->display_options['fields']['stamp']['field'] = 'stamp';
$handler->display->display_options['fields']['stamp']['label'] = 'Since';
$handler->display->display_options['fields']['stamp']['date_format'] = 'raw time ago';
/* Field: User: Name */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'users';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
$handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
$handler->display->display_options['fields']['name_1']['label'] = 'By';
/* Field: Workflow current: Current comment */
$handler->display->display_options['fields']['comment']['id'] = 'comment';
$handler->display->display_options['fields']['comment']['table'] = 'workflow_node_current';
$handler->display->display_options['fields']['comment']['field'] = 'comment';
$handler->display->display_options['fields']['comment']['label'] = 'Comment';
/* Field: Content: Workflow link */
$handler->display->display_options['fields']['workflow_node']['id'] = 'workflow_node';
$handler->display->display_options['fields']['workflow_node']['table'] = 'views_entity_node';
$handler->display->display_options['fields']['workflow_node']['field'] = 'workflow_node';
$handler->display->display_options['fields']['workflow_node']['label'] = 'Actions';
$handler->display->display_options['fields']['workflow_node']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['workflow_node']['alter']['text'] = 'view history';
/* Field: Workflow current: Edit comment */
$handler->display->display_options['fields']['edit_comment']['id'] = 'edit_comment';
$handler->display->display_options['fields']['edit_comment']['table'] = 'workflow_node_current';
$handler->display->display_options['fields']['edit_comment']['field'] = 'edit_comment';
/* Sort criterion: Workflow current: Time */
$handler->display->display_options['sorts']['stamp']['id'] = 'stamp';
$handler->display->display_options['sorts']['stamp']['table'] = 'workflow_node_current';
$handler->display->display_options['sorts']['stamp']['field'] = 'stamp';
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['group'] = 1;
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Content Type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Workflow current: Current state */
$handler->display->display_options['filters']['sid']['id'] = 'sid';
$handler->display->display_options['filters']['sid']['table'] = 'workflow_node_current';
$handler->display->display_options['filters']['sid']['field'] = 'sid';
$handler->display->display_options['filters']['sid']['group'] = 1;
$handler->display->display_options['filters']['sid']['exposed'] = TRUE;
$handler->display->display_options['filters']['sid']['expose']['operator_id'] = 'sid_op';
$handler->display->display_options['filters']['sid']['expose']['label'] = 'Current state';
$handler->display->display_options['filters']['sid']['expose']['operator'] = 'sid_op';
$handler->display->display_options['filters']['sid']['expose']['identifier'] = 'sid';
$handler->display->display_options['filters']['sid']['expose']['remember'] = TRUE;
$handler->display->display_options['filters']['sid']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['sid']['expose']['workflow_reduce_wid'] = FALSE;

/* Display: Summary */
$handler = $view->new_display('page', 'Summary', 'page_1');
$handler->display->display_options['display_description'] = 'A list of all executed transitions';
$handler->display->display_options['path'] = 'workflow/summary';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'Summary';
$handler->display->display_options['menu']['weight'] = '-1';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$handler->display->display_options['tab_options']['type'] = 'normal';
$handler->display->display_options['tab_options']['title'] = 'Workflow';
$handler->display->display_options['tab_options']['description'] = 'Your workflow dashboard';
$handler->display->display_options['tab_options']['weight'] = '0';

/* Display: My content */
$handler = $view->new_display('page', 'My content', 'page_3');
$handler->display->display_options['display_description'] = 'Show content created by current user.';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Author uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'node';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
$handler->display->display_options['path'] = 'workflow/my';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'My content';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Pending */
$handler = $view->new_display('page', 'Pending', 'page_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Workflow pending state changes';
$handler->display->display_options['display_description'] = 'Scheduled transitions';
$handler->display->display_options['defaults']['relationships'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
$handler->display->display_options['fields']['nid_1']['table'] = 'node';
$handler->display->display_options['fields']['nid_1']['field'] = 'nid';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Workflow scheduled: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
/* Field: Workflow scheduled: Entity_type */
$handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['label'] = 'Entity type';
$handler->display->display_options['fields']['entity_type']['exclude'] = TRUE;
/* Field: Content: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['label'] = 'Entity bundle';
$handler->display->display_options['fields']['type']['exclude'] = TRUE;
/* Field: Workflow scheduled: Field_name */
$handler->display->display_options['fields']['field_name']['id'] = 'field_name';
$handler->display->display_options['fields']['field_name']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['field_name']['field'] = 'field_name';
$handler->display->display_options['fields']['field_name']['label'] = 'Field name';
$handler->display->display_options['fields']['field_name']['exclude'] = TRUE;
/* Field: Workflow scheduled: Language */
$handler->display->display_options['fields']['language']['id'] = 'language';
$handler->display->display_options['fields']['language']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['language']['field'] = 'language';
$handler->display->display_options['fields']['language']['exclude'] = TRUE;
/* Field: Workflow scheduled: Scheduled time */
$handler->display->display_options['fields']['scheduled']['id'] = 'scheduled';
$handler->display->display_options['fields']['scheduled']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['scheduled']['field'] = 'scheduled';
$handler->display->display_options['fields']['scheduled']['date_format'] = 'short';
/* Field: Workflow scheduled: Current state */
$handler->display->display_options['fields']['old_sid']['id'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['old_sid']['field'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['label'] = 'From';
/* Field: Workflow scheduled: Scheduled state */
$handler->display->display_options['fields']['sid']['id'] = 'sid';
$handler->display->display_options['fields']['sid']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['sid']['field'] = 'sid';
$handler->display->display_options['fields']['sid']['label'] = 'To';
/* Field: Workflow scheduled: Scheduling user */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = 'By';
/* Field: Workflow scheduled: Scheduled comment */
$handler->display->display_options['fields']['comment']['id'] = 'comment';
$handler->display->display_options['fields']['comment']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['fields']['comment']['field'] = 'comment';
$handler->display->display_options['fields']['comment']['label'] = 'Comment';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Workflow scheduled: Scheduled time */
$handler->display->display_options['sorts']['scheduled']['id'] = 'scheduled';
$handler->display->display_options['sorts']['scheduled']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['sorts']['scheduled']['field'] = 'scheduled';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Workflow scheduled: Scheduled time */
$handler->display->display_options['filters']['scheduled']['id'] = 'scheduled';
$handler->display->display_options['filters']['scheduled']['table'] = 'workflow_scheduled_transition';
$handler->display->display_options['filters']['scheduled']['field'] = 'scheduled';
$handler->display->display_options['filters']['scheduled']['operator'] = '>=';
$handler->display->display_options['filters']['scheduled']['value']['value'] = '-1 hour';
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Content type';
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['path'] = 'workflow/pending';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Pending';
$handler->display->display_options['menu']['description'] = 'Scheduled transitions';
$handler->display->display_options['menu']['weight'] = '2';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: History */
$handler = $view->new_display('page', 'History', 'page_4');
$handler->display->display_options['display_description'] = 'A list of all state changes.';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Content: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['exclude'] = TRUE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'uid_3';
$handler->display->display_options['fields']['name']['label'] = 'Author';
/* Field: Workflow history: Nid */
$handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
$handler->display->display_options['fields']['nid_1']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['nid_1']['field'] = 'nid';
$handler->display->display_options['fields']['nid_1']['exclude'] = TRUE;
/* Field: Workflow history: Revision_id */
$handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['label'] = 'Revision ID';
$handler->display->display_options['fields']['revision_id']['exclude'] = TRUE;
/* Field: Workflow history: Entity_type */
$handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['label'] = 'Entity type';
$handler->display->display_options['fields']['entity_type']['exclude'] = TRUE;
/* Field: Workflow history: Field_name */
$handler->display->display_options['fields']['field_name']['id'] = 'field_name';
$handler->display->display_options['fields']['field_name']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['field_name']['field'] = 'field_name';
$handler->display->display_options['fields']['field_name']['label'] = 'Field name';
$handler->display->display_options['fields']['field_name']['exclude'] = TRUE;
/* Field: Workflow history: Language */
$handler->display->display_options['fields']['language']['id'] = 'language';
$handler->display->display_options['fields']['language']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['language']['field'] = 'language';
$handler->display->display_options['fields']['language']['exclude'] = TRUE;
/* Field: Workflow history: Delta */
$handler->display->display_options['fields']['delta']['id'] = 'delta';
$handler->display->display_options['fields']['delta']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['delta']['field'] = 'delta';
$handler->display->display_options['fields']['delta']['exclude'] = TRUE;
/* Field: Workflow history: Workflow executed transition ID */
$handler->display->display_options['fields']['hid']['id'] = 'hid';
$handler->display->display_options['fields']['hid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['hid']['field'] = 'hid';
$handler->display->display_options['fields']['hid']['label'] = 'hid';
$handler->display->display_options['fields']['hid']['exclude'] = TRUE;
/* Field: Workflow history: Old state */
$handler->display->display_options['fields']['old_sid']['id'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['old_sid']['field'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['label'] = 'From';
/* Field: Workflow history: New state */
$handler->display->display_options['fields']['sid_1']['id'] = 'sid_1';
$handler->display->display_options['fields']['sid_1']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['sid_1']['field'] = 'sid';
$handler->display->display_options['fields']['sid_1']['label'] = 'To';
/* Field: Workflow history: Time */
$handler->display->display_options['fields']['stamp_1']['id'] = 'stamp_1';
$handler->display->display_options['fields']['stamp_1']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['stamp_1']['field'] = 'stamp';
$handler->display->display_options['fields']['stamp_1']['label'] = 'Since';
$handler->display->display_options['fields']['stamp_1']['date_format'] = 'short';
/* Field: Workflow history: Previous user */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = 'By';
/* Field: Workflow history: Comment */
$handler->display->display_options['fields']['comment_1']['id'] = 'comment_1';
$handler->display->display_options['fields']['comment_1']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['comment_1']['field'] = 'comment';
$handler->display->display_options['path'] = 'workflow/history';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'History';
$handler->display->display_options['menu']['description'] = 'A list of all state transitions.';
$handler->display->display_options['menu']['weight'] = '3';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;
$translatables['workflow_summary'] = array(
  t('Defaults'),
  t('Workflow'),
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
  t('author'),
  t('Current user'),
  t('Nid'),
  t('Title'),
  t('Type'),
  t('Author'),
  t('hid'),
  t('.'),
  t(','),
  t('Current state'),
  t('Since'),
  t('By'),
  t('Comment'),
  t('Actions'),
  t('view history'),
  t('Edit comment'),
  t('Content Type'),
  t('Summary'),
  t('A list of all executed transitions'),
  t('My content'),
  t('Show content created by current user.'),
  t('All'),
  t('Pending'),
  t('Workflow pending state changes'),
  t('Scheduled transitions'),
  t('Entity type'),
  t('Entity bundle'),
  t('Field name'),
  t('Language'),
  t('Scheduled time'),
  t('From'),
  t('To'),
  t('Content type'),
  t('History'),
  t('A list of all state changes.'),
  t('Revision ID'),
  t('Delta'),
);
