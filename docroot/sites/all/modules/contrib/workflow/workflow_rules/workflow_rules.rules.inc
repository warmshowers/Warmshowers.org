<?php
/**
 * @file
 * Rules integration for the Workflow module.
 *
 * Contains _info() hooks.
 * Callbacks are implemented in file workflow.rules.inc.
 */

/*
 * Include the Condition and Actions for Nodes and Entity.
 * They are in separate files, but must be kept in sync.
 * They contain separate logic for the 'conventional' Workflow Node API
 * and the 'new' Workfow Field API.
 */
module_load_include('inc', 'workflow_rules', 'workflow_rules.node');
module_load_include('inc', 'workflow_rules', 'workflow_rules.field');

/**
 * Implements hook_rules_event_info().
 *
 * @todo: add support for any entity type in hook_rules_event_info.
 */
function workflow_rules_rules_event_info() {
  $events = array();
  if (module_exists('workflownode')) {
    $events += _workflownode_rules_event_info();
  }
  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function workflow_rules_rules_condition_info() {
  $conditions = array();
  if (module_exists('workflownode')) {
    $conditions += _workflownode_rules_condition_info();
  }
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function workflow_rules_rules_action_info() {
  $actions = array();
  if (module_exists('workflownode')) {
    $actions += _workflownode_rules_action_info();
  }
  if (module_exists('workflowfield')) {
    $actions += _workflowfield_rules_action_info();
  }
  return $actions;
}
