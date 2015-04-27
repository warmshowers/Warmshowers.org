<?php
/**
 * @file
 * Provide rules for workflows via hook_workflow.
 */

/**
 * Implements hook_workflow().
 *
 * Invokes events, as defined in hook_rules_event_info().
 *
 * @param string $op
 *   The current workflow operation: 'transition pre' or 'transition post'.
 * @param int $old_sid
 *   The state ID of the current state.
 * @param int $new_sid
 *   The state ID of the new state.
 * @param object $entity
 *   The entity whose workflow state is changing.
 * @param bool $force
 */
function workflow_rules_workflow($op, $old_sid, $new_sid, $entity, $force = FALSE, $entity_type = '', $field_name = '', $transition = NULL) {
  switch ($op) {
    case 'transition post':
      // Rules are updated only after a transition of a Workflow Node status.
      // When using Workflow Field, this hook is not called. Use default Rules
      // data instead.
      if ($old_sid == $new_sid) {
        rules_invoke_event('workflow_comment_added', $entity, $entity_type, $old_sid, $new_sid);
      }
      else {
        rules_invoke_event('workflow_state_changed', $entity, $entity_type, $old_sid, $new_sid);
      }
      break;

    default:
      break;
  }
}
