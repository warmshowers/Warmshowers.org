<?php

/**
 * @file
 * VBO action to modify entity values (properties and fields).
 */

/**
 * Implements hook_action_info().
 *
 * Registers custom VBO actions as Drupal actions.
 */
function workflow_vbo_next_action_info() {
  return array(
    'workflow_vbo_next_state_action' => array(
      'type' => 'entity',
      'label' => t('Change workflow state of post to next state'),
      'configurable' => FALSE,
      'triggers' => array('any'),
    ),
  );
}

/**
 * Implements a Drupal action. Move a node to the next state in the workflow.
 */
function workflow_vbo_next_state_action($entity, array $context) {
  global $user;

  if (!isset($entity)) {
    $entity = $context['node'];
  }
  // In 'after saving new content', the node is already saved. Avoid sencond insert.
  // Todo: clone?
  unset($entity->is_new);

  // Get the entity type and numeric ID.
  $entity_type = ($vbo_action = isset($context['entity_type'])) ?
    // In a VBO Action.
    $context['entity_type'] :
    // In an Advanced Action. 
    str_replace(array('_insert', '_update' , '_delete'), '', $context['hook']);
  list($entity_id, , ) = entity_extract_ids($entity_type, $entity);

  if (!$entity_id) {
    watchdog('workflow_vbo', 'Unable to get current entity ID - entity is not yet saved.');
    return;
  }

  // Get the current State Id. Also, $field_name will be set magically, by reference.
  $field_name = NULL;
  $current_sid = workflow_node_current_state($entity, $entity_type, $field_name);
  if (!$current_sid) {
    watchdog('workflow_vbo', 'Unable to get current workflow state of entity %id.',
      array('%id' => $entity_id));
    return;
  }

  // Get the node's new State Id (which is the next available state).
  $current_state = workflow_state_load_single($current_sid);
  $options = $current_state->getOptions($entity_type, $entity, $field_name, $user, FALSE);
  $new_sid = $current_sid;
  $flag = $current_state->isCreationState();
  foreach ($options as $sid => $name) {
    if ($flag) {
      $new_sid = $sid;
      break;
    }
    if ($sid == $current_sid) {
      $flag = TRUE;
    }
  }

  $force = FALSE;
  // Get the Comment. It is empty.
  $comment = '';

  // Fire the transition.
  $transition = new WorkflowTransition();
  $transition->setValues($entity_type, $entity, $field_name, $current_sid, $new_sid, $user->uid, REQUEST_TIME, $comment);
  workflow_execute_transition($entity_type, $entity, $field_name, $transition, $force);
}
