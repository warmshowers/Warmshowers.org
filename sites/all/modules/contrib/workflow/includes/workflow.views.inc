<?php
// $Id: workflow.views.inc,v 1.1 2008/09/18 02:49:25 jvandyk Exp $
/**
 * @file
 * Provide views data and handler information for workflow.module.
 */

/**
 * @defgroup views_workflow_module workflow.module handlers
 *
 * Includes the 'workflow_node' and 'workflow_state' tables, but generates a
 * pseudo-table for each separate workflow that's been set up.
 * @{
 */

/**
 * Implementation of hook_views_data().
 */
function workflow_views_data() {
  $data['workflow_states']['table']['group']  = t('Workflow');
  // Explain how this table joins to others.
  $data['workflow_states']['table']['join'] = array(
    'node' => array(
      'field' => 'sid',
      'left_table' => 'workflow_node',
      'left_field' => 'sid',
    ),
  );

  // state weight
  $data['workflow_states']['weight'] = array(
    'title' => t('State weight'), // Appears in views UI.
    'help' => t('The weight of the current workflow state that the node is in.'),
    'sort' => array(
      'handler' => 'views_handler_sort_numeric',
    ),
  );

  // state
  $data['workflow_states']['state'] = array(
    'title' => t('Current state name'), // Appears in views UI.
    'help' => t('The readable name of the workflow state that the node is in. (Less efficient, use only when click-sorting by state name.)'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
  );

  $data['workflow_node']['table']['group']  = t('Workflow');
  // Explain how this table joins to others.
  $data['workflow_node']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
    ),
  );

  // state
  $data['workflow_node']['sid'] = array(
    'title' => t('Current state'), // Appears in views UI.
    'help' => t('The current workflow state that the node is in.'),
    'field' => array(
      'handler' => 'workflow_views_handler_field_sid',
      'click sortable' => TRUE,
     ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'workflow_views_handler_filter_sid',
      'numeric' => TRUE,
    ),
  );

  $data['workflow_scheduled_transition']['table']['group']  = t('Workflow');
  // Explain how this table joins to others.
  $data['workflow_scheduled_transition']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
    ),
  );

  // state
  $data['workflow_scheduled_transition']['sid'] = array(
    'title' => t('Scheduled state'), // Appears in views UI.
    'help' => t('The current workflow state that the node is in.'),
    'field' => array(
      'handler' => 'workflow_views_handler_field_sid',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'workflow_views_handler_filter_sid',
      'numeric' => TRUE,
    ),
  );

  // time
  $data['workflow_scheduled_transition']['scheduled'] = array(
    'title' => t('Scheduled time'), // Appears in views UI.
    'help' => t('The time at which the node will change workflow states.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'numeric' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // comment
  $data['workflow_scheduled_transition']['comment'] = array(
    'title' => t('Scheduled comment'), // Appears in views UI.
    'help' => t('A comment describing why the node was scheduled for state transition.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'numeric' => TRUE,
    ),
  );

  $data['workflow_node_history']['table']['group']  = t('Workflow');
  // Explain how this table joins to others.
  $data['workflow_node_history']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
    ),
  );

  // state
  $data['workflow_node_history']['sid'] = array(
    'title' => t('Previous state'), // Appears in views UI.
    'help' => t('A workflow state that the node was in previously.'),
    'field' => array(
      'handler' => 'workflow_views_handler_field_sid',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'workflow_views_handler_filter_sid',
      'numeric' => TRUE,
    ),
  );

  // time
  $data['workflow_node_history']['stamp'] = array(
    'title' => t('Previous time'), // Appears in views UI.
    'help' => t('The time at which the node moved from one state to another.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'numeric' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // comment
  $data['workflow_node_history']['comment'] = array(
    'title' => t('Previous comment'), // Appears in views UI.
    'help' => t('A comment describing why the node was moved from one state to another in the past.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'numeric' => TRUE,
    ),
  );

  return $data;
}

/**
 * @}
 */

/**
 * Implementation of hook_views_handlers().
 */
function workflow_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'workflow') . '/includes',
    ),
    'handlers' => array(
      // argument handlers
      'workflow_views_handler_argument_state' => array(
        'parent' => 'views_handler_argument_node_type',
      ),
      // field handlers
      'workflow_views_handler_field_sid' => array(
        'parent' => 'views_handler_field_node',
      ),
      // filter handlers
      'workflow_views_handler_filter_sid' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      // relationship handlers
      // sort handlers
    ),
  );
}