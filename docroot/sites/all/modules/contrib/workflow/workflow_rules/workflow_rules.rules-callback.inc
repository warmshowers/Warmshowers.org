<?php
/**
 * @file
 * Callback implementations for Rules integration for the Workflow module.
 *
 * These callbacks must be included in the module file.
 * It is not sufficient to have them in the rules_info file.
 */

/**
 * Helper function to parse a token "node:<field_name>".
 *
 * @param string $token
 *
 * @return string $field_name
 *
 * This is a poorman's effort to convert a token into a field name.
 */
function _workflow_rules_token_replace($field_name) {
  // Get the part after the first domain indicator.
  $list_parts = explode(':', $field_name, 2);
  $field_name = end($list_parts);

  $field_name = str_replace('-', '_', $field_name);
  return $field_name;
}

/**
 * Condition callback: gather all workflow states, to show in list_options.
 */
function _workflow_rules_workflow_get_options($data) {
  // This is a poorman's effort to convert a token into a field name.
  $field_name = isset($data->settings['field:select']) ? _workflow_rules_token_replace($data->settings['field:select']) : '';
  $field = _workflow_info_field($field_name, NULL);

  $options['ANY'] = 'ANY state';
  $options += workflow_get_workflow_state_names($field['settings']['wid'], $grouped = TRUE);
  return $options;
}

/**
 * Condition implementation helper function: check given state.
 *
 * @param mixed $sid
 *   A State ID, to compare with the given list of allowed State ID's.
 * @param array $sids
 *   A list of allowed State ID's.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function _workflow_rules_workflow_check_given_state($sid, array $sids) {
  return in_array('ANY', $sids) || in_array($sid, $sids);
}

/**
 * Condition implementation: check state transition..
 *
 * Only for Workflow Node! Workflow Field can use default Rules condition.
 *
 * @param object $node
 *   The node with the new values. Other entity types are not supported.
 * @param array $old_sids
 *   An array of old sids to meet the condition.
 * @param array $new_sids
 *   An array of new sids to meet the condition.
 * @param array $condition
 *   A RulesCondition->settings array.
 */
function _workflow_rules_node_check_transition($node, array $old_sids, array $new_sids, array $condition) {
  $last_transition = workflow_transition_load_single('node', $node->nid, '');
  $old_sid = $last_transition->old_sid;
  $new_sid = $last_transition->new_sid;

  return
    _workflow_rules_workflow_check_given_state($old_sid, $old_sids) &&
    _workflow_rules_workflow_check_given_state($new_sid, $new_sids);
}

/**
 * Condition implementation: check current state for Workflow Node API.
 *
 * Only for Workflow Node! Workflow Field can use default Rules condition.
 *
 * @param object $node
 *   The node with the new values. Other entity types are not supported.
 * @param array $sids
 *   An array of State IDs to meet the condition.
 */
function _workflow_rules_node_check_state($node, array $sids) {
  $field_name = ''; // An explicit var is needed.
  $sid = workflow_node_current_state($node, 'node', $field_name);
  return _workflow_rules_workflow_check_given_state($sid, $sids);
}

/**
 * Condition implementation: check previous state.
 *
 * Only for Workflow Node! Workflow Field can use default Rules condition.
 */
function _workflow_rules_node_check_previous_state($node, $sids) {
  $last_transition = workflow_transition_load_single('node', $node->nid, '');
  $sid = $last_transition->old_sid;
  return _workflow_rules_workflow_check_given_state($sid, $sids);
}

/**
 * Action implementation: set current state, ignoring current user permission.
 *
 * For both Workflow Node and Workflow Field.
 */
function _workflow_rules_set_state(array $parameters, RulesAction $action) {
  global $user;

  // Warning: keep this action in line between Workflow Field and Workflow Node.
  // "$parameters['node']" is for backwards compatibility: can be any entity_type.
  $entity = $parameters['node']->value(); // $parameters['entity'] is an EntityDrupalWrapper.
  $entity_type = $parameters['node']->type();

  // This is a poorman's effort to convert a token into a field name.
  $field_name = isset($parameters['settings']['field:select']) ? _workflow_rules_token_replace($parameters['settings']['field:select']) : '';
  $old_sid = workflow_node_current_state($entity, $entity_type, $field_name);

  // Select the last state on the list.
  $new_sid = array_pop($parameters['workflow_state']);
  $comment = $parameters['workflow_comment'];

  $transition = new WorkflowTransition();
  $transition->setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $user->uid, REQUEST_TIME, $comment);
  // Force this transition, to ignore the limitations on the current user's permissions.
  $transition->force(TRUE);

  // Execute the transition. It may bounce, due to extra checks.
  // @todo: use $new_sid = $transition->execute() without generating infinite loops.
  // Below method does not recalc tokens on automatic_entity_label.
  $new_sid = workflow_execute_transition($entity_type, $entity, $field_name, $transition);
}
