<?php
/**
 * @file
 * A view that overrides the programmed 'workflow history tab'.
 *
 * Usage:
 * - enable the view. it contains a Page and an Attachment.
 * -- the Page shows the current Node.
 * -- the Attachment shows the workflow history (transitions) beneath it.
 * - Go to the content type administration page. If the content type has a
 *   workflow field attached to it, it has a 'Workflow History' view mode.
 *
 * Modifications:
 * - modify the Contextual filter and the Path to make it work for Taxonomy terms, etc.
 * - Change the view mode as you like
 * - Change the history list.
 */

$view = new view();
$view->name = 'workflow_history_per_entity';
$view->description = 'Show history of (multiple) workflows per entity. Maintain the view mode \'workflow_tab\'.';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Workflow history per entity';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Workflow history per entity';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Workflow history: Workflow executed transition ID */
$handler->display->display_options['fields']['hid']['id'] = 'hid';
$handler->display->display_options['fields']['hid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['hid']['field'] = 'hid';
$handler->display->display_options['fields']['hid']['label'] = 'Transition ID';
$handler->display->display_options['fields']['hid']['exclude'] = TRUE;
/* Field: Workflow history: Entity_type */
$handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['label'] = 'Entity type';
$handler->display->display_options['fields']['entity_type']['exclude'] = TRUE;
/* Field: Workflow history: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = 'ID';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
/* Field: Workflow history: Revision_id */
$handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['label'] = 'Revision ID';
/* Field: Workflow history: Field_name */
$handler->display->display_options['fields']['field_name']['id'] = 'field_name';
$handler->display->display_options['fields']['field_name']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['field_name']['field'] = 'field_name';
$handler->display->display_options['fields']['field_name']['label'] = 'Field name';
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
/* Field: Workflow history: Time */
$handler->display->display_options['fields']['stamp']['id'] = 'stamp';
$handler->display->display_options['fields']['stamp']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['stamp']['field'] = 'stamp';
$handler->display->display_options['fields']['stamp']['date_format'] = 'medium';
/* Field: Workflow history: Previous user */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
/* Field: Workflow history: The old (previous) state in a transition. */
$handler->display->display_options['fields']['old_sid']['id'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['old_sid']['field'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['label'] = 'Previous state';
/* Field: Workflow history: New state */
$handler->display->display_options['fields']['sid']['id'] = 'sid';
$handler->display->display_options['fields']['sid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['sid']['field'] = 'sid';
/* Field: Workflow history: Comment */
$handler->display->display_options['fields']['comment']['id'] = 'comment';
$handler->display->display_options['fields']['comment']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['comment']['field'] = 'comment';
/* Field: Workflow history: Edit comment */
$handler->display->display_options['fields']['edit_comment']['id'] = 'edit_comment';
$handler->display->display_options['fields']['edit_comment']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['edit_comment']['field'] = 'edit_comment';
$handler->display->display_options['fields']['edit_comment']['label'] = 'Operations';
/* Sort criterion: Workflow history: Workflow executed transition ID */
$handler->display->display_options['sorts']['hid']['id'] = 'hid';
$handler->display->display_options['sorts']['hid']['table'] = 'workflow_node_history';
$handler->display->display_options['sorts']['hid']['field'] = 'hid';
$handler->display->display_options['sorts']['hid']['order'] = 'DESC';
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
$handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = '%1';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '1';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'entity';
$handler->display->display_options['row_options']['view_mode'] = 'workflow_tab';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['path'] = 'node/%/workflow';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Workflow';
$handler->display->display_options['menu']['description'] = 'Show transitions for this Node';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Attachment */
$handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Workflow history';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: Unfiltered text */
$handler->display->display_options['header']['area_text_custom']['id'] = 'area_text_custom';
$handler->display->display_options['header']['area_text_custom']['table'] = 'views';
$handler->display->display_options['header']['area_text_custom']['field'] = 'area_text_custom';
$handler->display->display_options['header']['area_text_custom']['label'] = '"Workflow history"';
$handler->display->display_options['header']['area_text_custom']['content'] = 'Workflow history';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Workflow history: Workflow executed transition ID */
$handler->display->display_options['fields']['hid']['id'] = 'hid';
$handler->display->display_options['fields']['hid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['hid']['field'] = 'hid';
$handler->display->display_options['fields']['hid']['label'] = 'Transition ID';
$handler->display->display_options['fields']['hid']['exclude'] = TRUE;
/* Field: Workflow history: Entity_type */
$handler->display->display_options['fields']['entity_type']['id'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['entity_type']['field'] = 'entity_type';
$handler->display->display_options['fields']['entity_type']['label'] = 'Entity type';
$handler->display->display_options['fields']['entity_type']['exclude'] = TRUE;
/* Field: Workflow history: Nid */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['label'] = 'ID';
$handler->display->display_options['fields']['nid']['exclude'] = TRUE;
/* Field: Workflow history: Revision_id */
$handler->display->display_options['fields']['revision_id']['id'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['revision_id']['field'] = 'revision_id';
$handler->display->display_options['fields']['revision_id']['label'] = 'Revision ID';
/* Field: Workflow history: Field_name */
$handler->display->display_options['fields']['field_name']['id'] = 'field_name';
$handler->display->display_options['fields']['field_name']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['field_name']['field'] = 'field_name';
$handler->display->display_options['fields']['field_name']['label'] = 'Field name';
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
/* Field: Workflow history: Time */
$handler->display->display_options['fields']['stamp']['id'] = 'stamp';
$handler->display->display_options['fields']['stamp']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['stamp']['field'] = 'stamp';
$handler->display->display_options['fields']['stamp']['label'] = 'Date';
$handler->display->display_options['fields']['stamp']['date_format'] = 'medium';
/* Field: Workflow history: The old (previous) state in a transition. */
$handler->display->display_options['fields']['old_sid']['id'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['old_sid']['field'] = 'old_sid';
$handler->display->display_options['fields']['old_sid']['label'] = 'Old state';
/* Field: Workflow history: New state */
$handler->display->display_options['fields']['sid']['id'] = 'sid';
$handler->display->display_options['fields']['sid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['sid']['field'] = 'sid';
/* Field: Workflow history: Previous user */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = 'By';
/* Field: Workflow history: Comment */
$handler->display->display_options['fields']['comment']['id'] = 'comment';
$handler->display->display_options['fields']['comment']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['comment']['field'] = 'comment';
/* Field: Workflow history: Edit comment */
$handler->display->display_options['fields']['edit_comment']['id'] = 'edit_comment';
$handler->display->display_options['fields']['edit_comment']['table'] = 'workflow_node_history';
$handler->display->display_options['fields']['edit_comment']['field'] = 'edit_comment';
$handler->display->display_options['fields']['edit_comment']['label'] = 'Operations';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Workflow history: Workflow executed transition ID */
$handler->display->display_options['sorts']['hid']['id'] = 'hid';
$handler->display->display_options['sorts']['hid']['table'] = 'workflow_node_history';
$handler->display->display_options['sorts']['hid']['field'] = 'hid';
$handler->display->display_options['sorts']['hid']['order'] = 'DESC';
$handler->display->display_options['displays'] = array(
  'page' => 'page',
  'default' => 0,
);
$handler->display->display_options['attachment_position'] = 'after';
$translatables['workflow_history_per_entity'] = array(
  t('Master'),
  t('Workflow history per entity'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Transition ID'),
  t('.'),
  t(','),
  t('Entity type'),
  t('ID'),
  t('Revision ID'),
  t('Field name'),
  t('Language'),
  t('Delta'),
  t('Time'),
  t('Previous user'),
  t('Previous state'),
  t('New state'),
  t('Comment'),
  t('Operations'),
  t('All'),
  t('Page'),
  t('%1'),
  t('Attachment'),
  t('Workflow history'),
  t('"Workflow history"'),
  t('Date'),
  t('Old state'),
  t('By'),
);
