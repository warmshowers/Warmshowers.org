<?php
/**
 * @file
 * Hooks provided by the workflow module.
 */

/**
 * Implements hook_workflow().
 *
 * NOTE: This hook may reside in the implementing module
 * or in a module.workflow.inc file.
 *
 * @param string $op
 *   The current workflow operation.
 *   E.g., 'transition permitted', 'transition pre' or 'transition post'.
 * @param mixed $id
 *   The ID of the current state/transition/workflow.
 * @param mixed $new_sid
 *   The state ID of the new state.
 * @param object $entity
 *   The entity whose workflow state is changing.
 * @param bool $force
 *   The caller indicated that the transition should be forced. (bool).
 *   This is only available on the "pre" and "post" calls.
 * @param string $entity_type
 *   The entity_type of the entity whose workflow state is changing.
 * @param string $field_name
 *   The name of the Workflow Field. Empty in case of Workflow Node.
 *   This is used when saving a state change of a Workflow Field.
 * @param object $transition
 *   The transition, that contains all of the above.
 *   @todo D8: remove all other parameters.
 *
 * @return mixed
 *   Only 'transition permitted' expects a boolean result.
 */
function hook_workflow($op, $id, $new_sid, $entity, $force, $entity_type = '', $field_name = '', $transition = NULL, $user = NULL) {
  switch ($op) {
    case 'transition permitted':
      // This is called in the following situations:
      // 1. when building a workflow widget with list of available transitions;
      // 2. when executing a transition, just before the 'transition pre';
      // 3. when showing a 'revert state' link in a Views display.
      // Your module's implementation may return FALSE here and disallow
      // the execution, or avoid the presentation of the new State.
      // This may be user-dependent.
      // As of 7.x-2.3, better use hook_workflow_permitted_state_transitions_alter() in option 1.
      // For options 2 and 3, the 'transition pre' gives an alternative.
      return TRUE;

    case 'transition pre':
      // The workflow module does nothing during this operation.
      // Implement this hook if you need to change/do something BEFORE anything
      // is saved to the database.
      // If you return FALSE here, you will veto the transition.
      break;

    case 'transition post':
      // This is called by Workflow Node during update of the state, directly
      // after updating {workflow_node}. Workflow Field does not call this,
      // since you can call a hook_entity_* event after saving the entity.
      // @see https://api.drupal.org/api/drupal/includes%21module.inc/group/hooks/7
      break;

    case 'transition delete':
      // A transition is deleted. Only the first parameter is used.
      // $tid = $id;
      break;

    case 'state delete':
      // A state is deleted. Only the first parameter is used.
      // $sid = $id;
      break;

    case 'workflow delete':
      // A workflow is deleted. Only the first parameter is used.
      // $wid = $id;
      break;
  }
}

/**
 * Implements hook_workflow_history_alter().
 *
 * Allow other modules to add Operations to the most recent history change.
 * E.g., Workflow Revert implements an 'undo' operation.
 *
 * @param array $variables
 *   The current workflow history information as an array.
 *   'old_sid' - The state ID of the previous state.
 *   'old_state_name' - The state name of the previous state.
 *   'sid' - The state ID of the current state.
 *   'state_name' - The state name of the current state.
 *   'history' - The row from the workflow_node_history table.
 *   'transition' - a WorkflowTransition object, containing all of the above.
 */
function hook_workflow_history_alter(array &$variables) {
  // The Workflow module does nothing with this hook.
  // For an example implementation, see the Workflow Revert add-on.
  $options = array();
  $path = '<front>';

  // If you want to add additional data, such as an operation link,
  // place it in the 'extra' value.
  $variables['extra'] = l(t('My new operation: go to frontpage'), $path, $options);
}

/**
 * Implements hook_workflow_comment_alter().
 *
 * Allow other modules to change the user comment when saving a state change.
 *
 * @param string $comment
 *   The comment of the current state transition.
 * @param array $context
 *   'transition' - The current transition itself.
 */
function hook_workflow_comment_alter(&$comment, array &$context) {
  $transition = $context->transition;
  $comment = $transition->uid . 'says: ' . $comment;
}

/**
 * Implements hook_workflow_permitted_state_transitions_alter().
 *
 * @param array $transitions
 *  An array of allowed transitions from the current state (as provided in
 *  $context). They are already filtered by the settings in Admin UI.
 * @param array $context
 *  An array of relevant objects. Currently:
 *    $context = array(
 *      'entity_type' => $entity_type,
 *      'entity' => $entity,
 *      'field_name' => $field_name,
 *      'force' => $force,
 *      'workflow' => $workflow,
 *      'state' => $current_state,
 *      'user' => $user,
 *      'user_roles' => $roles, // @todo: can be removed in D8, since $user is in.
 *    );
 *
 * This hook allows you to add custom filtering of allowed target states, add
 * new custom states, change labels, etc.
 * It is invoked in WorkflowState::getOptions().
 */
function hook_workflow_permitted_state_transitions_alter(array &$transitions, array $context) {
  // This example creates a new custom target state.
  $values = array(
    // Fixed values for new transition.
    'wid' => $context['workflow']->wid,
    'sid' => $context['state']->sid,

    // Custom values for new transition.
    // The ID must be an integer, due to db-table constraints.
    'target_sid' => '998',
    'label' => 'go to my new fantasy state',
  );
  $new_transition = new WorkflowConfigTransition($values);

  $transitions[] = $new_transition;
}

/**
 * Examples of hooks, that can be used to alter the Workflow Form.
 */

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 *
 * Use this hook to alter the form.
 * It is only suited if you only use View Page or Workflow Tab.
 * If you change the state on the Node Form (Edit modus), you need the hook
 * hook_form_alter(). See below for more info.
 */
function workflowfield_form_workflow_transition_form_alter(&$form, &$form_state, $form_id) {

  // Get the Entity.
  $entity = $form['workflow']['workflow_entity']['#value'];
  $entity_type = $form['workflow']['workflow_entity_type']['#value'];

  // Use the complicated form, which is suited for all Entity types.
  // For nodes only: $entity_type = 'node'; $entity_bundle = $entity->type;
  list(, , $entity_bundle) = entity_extract_ids($entity_type, $entity);

  // Get the current State ID.
  $sid = workflow_node_current_state($entity, $entity_type, $field_name = NULL);
  // Get the State object, if needed.
  $state = workflow_state_load($sid);

  // Change the form, depending on the state ID.
  // In the upcoming version 7.x-2.4, States should have a machine_name, too.
  if ($entity_type == 'node' && $entity_bundle == 'MY_NODE_TYPE') {
    switch ($state->sid) {
      case '2':
        // Change form element, form validate and form submit for state '2'.
        break;

      case '3':
        // Change form element, form validate and form submit for state '3'.
        break;
    }
  }

}

/**
 * Implements hook_form_alter().
 *
 * Use this hook to alter the form on a Node Form, Comment Form (Edit page).
 */
function workflowfield_form_alter(&$form, $form_state, $form_id) {

  // Get the Entity.
  $entity = $form['#entity'];
  $entity_type = $form['#entity_type'];
  // Use the complicated form, which is suited for all Entity Types.
  list(, , $entity_bundle) = entity_extract_ids($entity_type, $entity);

  // Discover if this is the correct form.
  // ...
  // Get the current state and act upon it.
  // .. copy code from the hook above.
}
