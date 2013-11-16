<?php
// $Id: workflow.views_default.inc,v 1.2 2008/09/18 03:18:55 jvandyk Exp $

/**
 * @file
 * View for listing nodes by workflow state.
 */

function workflow_views_default_views() {
  $view = new view;
  $view->name = 'workflow_summary';
  $view->description = 'See which posts are in which workflow state.';
  $view->tag = 'workflow';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'sid' => array(
      'label' => 'Current state',
      'exclude' => 0,
      'id' => 'sid',
      'table' => 'workflow_node',
      'field' => 'sid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'type' => array(
      'label' => 'Type',
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'sid' => array(
      'operator' => 'in',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'sid_op',
        'identifier' => 'sid',
        'label' => 'Current State',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'sid',
      'table' => 'workflow_node',
      'field' => 'sid',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'type_op',
        'identifier' => 'type',
        'label' => 'Content Type',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access workflow summary views',
  ));
  $handler->override_option('title', 'Workflow summary');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'state' => 'state',
      'title' => 'title',
    ),
    'info' => array(
      'state' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => 'state',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'workflow/summary');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'Summary',
    'weight' => '-1',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Workflow summary',
    'weight' => '0',
  ));

  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'workflow_pending';
  $view->description = 'Shows upcoming state changes.';
  $view->tag = 'workflow';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'sid' => array(
      'label' => 'Current state',
      'exclude' => 0,
      'id' => 'sid',
      'table' => 'workflow_node',
      'field' => 'sid',
      'relationship' => 'none',
    ),
    'sid_1' => array(
      'label' => 'Scheduled state',
      'exclude' => 0,
      'id' => 'sid_1',
      'table' => 'workflow_scheduled_transition',
      'field' => 'sid',
      'relationship' => 'none',
    ),
    'scheduled' => array(
      'label' => 'Scheduled time',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'scheduled',
      'table' => 'workflow_scheduled_transition',
      'field' => 'scheduled',
      'relationship' => 'none',
    ),
    'comment' => array(
      'label' => 'Scheduled comment',
      'exclude' => 0,
      'id' => 'comment',
      'table' => 'workflow_scheduled_transition',
      'field' => 'comment',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'scheduled' => array(
      'order' => 'ASC',
      'granularity' => 'second',
      'id' => 'scheduled',
      'table' => 'workflow_scheduled_transition',
      'field' => 'scheduled',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'scheduled' => array(
      'operator' => '>=',
      'value' => array(
        'type' => '',
        'value' => '-1 hour',
        'min' => '-1 hour',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'scheduled',
      'table' => 'workflow_scheduled_transition',
      'field' => 'scheduled',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access workflow summary views',
  ));
  $handler->override_option('title', 'Workflow Pending Changes');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'workflow/pending');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Pending',
    'weight' => '0',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;
  return $views;
}