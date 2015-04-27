<?php

/**
 * @file
 * Defines a Workflow formatter.
 * You won't find a DefaultFormatter, because:
 * - The 'default' formatter provided by the List module;
 * - The 'workflow' formatter is only representing the WorkflowDefault Widget.
 *
 * All hooks are wrapper functions for a D8-style WorkflowDefaultWidget object.
 */

/**
 * Implements hook_field_formatter_info().
 */
function workflowfield_field_formatter_info() {
  return WorkflowDefaultWidget::settings();
}

/**
 * Implements hook_field_formatter_view().
 *
 * Shows current State + Widget on a Node View page or a Workflow History tab.
 */
function workflowfield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode = LANGUAGE_NONE, $items = array(), $display = array()) {
  global $user; // @todo #2287057: OK?
  // @todo: Perhaps global user is not always the correct user.
  // E.g., on ScheduledTransition->execute()? But this function is mostly used in UI.

  $field_name = isset($field['field_name']) ? $field['field_name'] : '';
  $field_id = isset($field['id']) ? $field['id'] : 0;

  $current_sid = workflow_node_current_state($entity, $entity_type, $field_name);
  $current_state = workflow_state_load_single($current_sid);

  if ($field_name) {
    // First compose the current value with the normal formatter from list.module.
    $list_element = workflow_state_formatter($entity_type, $entity, $field, $instance, $current_sid);
  }
  elseif (!empty($field['settings']['widget']['current_status'])) {
    $list_element = workflow_state_formatter($entity_type, $entity, $field, $instance, $current_sid);
  }

  // Check permission, so that even with state change rights,
  // the form can be suppressed from the node view (#1893724).
  if (!user_access('show workflow state form')) {
    return $list_element;
  }
  if ($entity_type == 'comment') {
    // No Workflow form allowed on a comment display.
    // (Also, this avoids a lot of error messages.)
    return $list_element;
  }
  // Only build form if user has possible target state(s).
  if (!$current_state->showWidget($entity_type, $entity, $field_name, $user, FALSE)) {
    return $list_element;
  }

  // Add some data for the form.
  // Add the current value to the form.
  // Make sure the current value is before the form. (which has weight = 0.005)
  $list_element['#weight'] = 0;
  $element['workflow_current_state'] = $list_element;

  // Add the form/widget to the formatter, and include the nid in the form id,
  // to allow multiple forms per page (in listings, with hook_forms() ).
  // Ultimately, this is a wrapper for WorkflowDefaultWidget.
  $entity_id = entity_id($entity_type, $entity);
  $element += drupal_get_form(implode('_', array('workflow_transition_form', $entity_type, $entity_id, $field_id)), $field, $instance, $entity_type, $entity);

  return $element;
}
