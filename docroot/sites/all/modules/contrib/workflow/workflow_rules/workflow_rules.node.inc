<?php
/**
 * @file
 * Rules integration for the Workflow module with Node API.
 */

/**
 * Implements subfunction of hook_rules_event_info().
 */
function _workflownode_rules_event_info() {
  $label = t('updated content');
  // Add variables for the 'node' type.
  $node_variables = rules_events_node_variables($label, TRUE);

  $events = array(
    'workflow_state_changed' => array(
      'group' => t('Workflow'),
      'label' => t('Workflow state has changed'),
      'variables' => $node_variables,
    ),
    'workflow_comment_added' => array(
      'group' => t('Workflow'),
      'label' => t('Workflow comment was added, but state did not change'),
      'variables' => $node_variables,
    ),
  );
  return $events;
}

/**
 * Implements subfunction of hook_rules_condition_info().
 */
function _workflownode_rules_condition_info() {
  return array(
    'workflow_check_transition' => array(
      'group' => t('Workflow'),
      'label' => t('Content makes a specific transition'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node whose workflow state is being checked.'),
        ),
        'old_state' => array(
          'type' => 'list<integer>',
          'label' => t('Old workflow state'),
          'options list' => '_workflow_rules_workflow_get_options',
          'description' => t('The workflow state moved from.'),
        ),
        'new_state' => array(
          'type' => 'list<integer>',
          'label' => t('New workflow state'),
          'options list' => '_workflow_rules_workflow_get_options',
          'description' => t('The workflow state moved to.'),
        ),
      ),
      'base' => '_workflow_rules_node_check_transition',
      'callbacks' => array(
        'execute' => '_workflow_rules_node_check_transition',
      ),
    ),
    'workflow_check_state' => array(
      'group' => t('Workflow'),
      'label' => t('Content has a workflow state'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node to compare the current workflow state of.'),
        ),
        'workflow_state' => array(
          'type' => 'list<integer>',
          'label' => t('Compare workflow state'),
          'options list' => '_workflow_rules_workflow_get_options',
          'description' => t('The possible workflow states to compare against.'),
        ),
      ),
      'base' => '_workflow_rules_node_check_state',
      'callbacks' => array(
        'execute' => '_workflow_rules_node_check_state',
      ),
    ),
    'workflow_check_previous_state' => array(
      'group' => t('Workflow'),
      'label' => t('Content has a previous workflow state'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node to compare the previous workflow state of.'),
        ),
        'workflow_state' => array(
          'type' => 'list<integer>',
          'label' => t('Compare workflow state'),
          'options list' => '_workflow_rules_workflow_get_options',
          'description' => t('The possible workflow states to compare against.'),
        ),
      ),
      'base' => '_workflow_rules_node_check_previous_state',
      'callbacks' => array(
        'execute' => '_workflow_rules_node_check_previous_state',
      ),
    ),
  );
}

/**
 * Implements subfunction of hook_rules_action_info().
 */
function _workflownode_rules_action_info() {
  $actions = array();

  // Warning: keep this action in line between Workflow Field and Workflow Node.
  $actions['workflow_rules_set_state'] = array(
    'group' => t('Workflow'),
    'label' => t('Set a Workflow state (with a comment)'),
    'parameter' => array(
      'node' => array(
        'type' => 'entity',
        'label' => t('Node'),
        'description' => t('The node to set the current workflow state of.'),
        // 'save' => TRUE,
      ),
/*
      //'field' => array(
      //  'type' => 'text', // WORKFLOWFIELD_PROPERTY_TYPE,
      //  'label' => t('Workflow field to set'),
      //  'description' => t('The workflow field to set.'),
      //  'restriction' => 'selector',
      //  'allow null' => TRUE,
      //),
 */
      'workflow_state' => array(
        'type' => 'list<integer>',
        'label' => t('New workflow state'),
        'options list' => '_workflow_rules_workflow_get_options',
        'description' => t('The workflow state to set (select only one).'),
      ),
      'workflow_comment' => array(
        'type' => 'text',
        'label' => t('Workflow Comment'),
        'description' => t('The workflow comment to set.'),
        'optional' => TRUE,
      ),
    ),
    'named parameter' => TRUE,
    'base' => '_workflow_rules_set_state',
    'callbacks' => array(
      'execute' => '_workflow_rules_set_state',
    ),
  );
  return $actions;
}
