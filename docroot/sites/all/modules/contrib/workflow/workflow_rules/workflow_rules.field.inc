<?php
/**
 * @file
 * Rules integration for the Workflow module with Entity API.
 */

if (module_exists('rules')) {
  module_load_include('inc', 'workflow', 'workflow_rules.rules-callback');
}

/**
 * Implements subfunction of hook_rules_condition_info().
 *
 * When using "node:" and "node:unchanged", there is no need to create more
 * conditions to check transitions.
 */
// function workflowfield_rules_condition_info() {
// }

/**
 * Implements subfunction of hook_rules_action_info().
 */
function _workflowfield_rules_action_info() {
  $actions = array();

  // Warning: keep this action in line between Workflow Field and Workflow Node.
  $actions['workflowfield_field_set_state'] = array(
    'group' => t('Workflow'),
    'label' => t('Set a Workflow state (with a comment)'),
    'parameter' => array(
      // "parameter['node']" is for backwards compatibility: can be any entity_type.
      'node' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The entity to set the current workflow state of.'),
        // 'save' => TRUE,
      ),
      'field' => array(
        'type' => WORKFLOWFIELD_PROPERTY_TYPE,
        'label' => t('Workflow field to set'),
        'description' => t('The workflow field to set.'),
        'restriction' => 'selector',
        // 'allow null' => TRUE,
      ),
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
