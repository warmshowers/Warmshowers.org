<?php
/**
 * @file
 * Hooks provided by the workflow_admin_ui module.
 */

/**
 * Implements hook_workflow_operations().
 *
 * @param string $op
 *   'top_actions': Allow modules to insert their own front page action links.
 *   'operations': Allow modules to insert their own workflow operations.
 *   'state':  Allow modules to insert state operations.
 * @param Workflow $workflow
 *   The current workflow object.
 * @param WorkflowState $state
 *   The current state object.
 */
function hook_workflow_operations($op, object $workflow, object $state) {
  switch ($op) {
    case 'top_actions':
      $actions = array();
      // The workflow_admin_ui module creates links to add a new state,
      // and reach each workflow.
      // Your module may add to these actions.
      return $actions;

    case 'operations':
      $actions = array();
      // The workflow_admin_ui module creates links to add a new state,
      // edit the workflow, and delete the workflow.
      // Your module may add to these actions.
      return $actions;

    case 'workflow':
      $actions = array();
      // Allow modules to insert their own workflow operations.
      return $actions;

    case 'state':
      $ops = array();
      // The workflow_admin_ui module does not use this.
      // Your module may add operations.
      return $ops;
  }
}
