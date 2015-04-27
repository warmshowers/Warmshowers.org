<?php
/**
 * @file
 * Contains 'workflow_revert' page.
 */

/**
 * Menu callback to do the revert function.
 *
 * @todo: add support for Field in workflow_revert.
 */
function workflow_revert_form($form, $form_state, $entity_type, $id, $field_name, $previous_sid = NULL) {
  $entity = entity_load_single($entity_type, $id);
  $uri = entity_uri($entity_type, $entity);
  $return_uri = $uri['path'] . '/workflow';

  if (drupal_valid_token($_GET['token'], 'workflow_revert ' . $previous_sid)) {
    $state = workflow_state_load_single($previous_sid);

    $args['#previous_sid'] = $previous_sid;
    $args['#entity_id'] = $id;
    $args['#entity'] = $entity;
    $args['#entity_type'] = $entity_type;
    $args['#field_name'] = $field_name;
    $question = t('Are you sure you want to revert %title to the "@state" state?', array(
      '@state' => $state->label(),
      '%title' => entity_label($entity_type, $entity),
      )
    );
    return confirm_form($args,
      $question,
      $return_uri,
      t('The workflow state will be changed.')
    );
  }
  else {
    watchdog('workflow_revert', 'Invalid token', array(), WATCHDOG_ERROR);
    drupal_set_message(t('Invalid token. Your information has been recorded.'), 'error');
    drupal_goto($return_uri);
  }
}

/**
 * Submit callback function.
 *
 * The function is magically called, due to its name <form>_submit.
 */
function workflow_revert_form_submit($form, $form_state) {
  global $user;

  $previous_sid = $form['#previous_sid'];
  // $id = $form['#entity_id'];
  $entity = $form['#entity'];
  $entity_type = $form['#entity_type'];
  $field_name = $form['#field_name'];

  $comment = t('State reverted.');

  $uri = entity_uri($entity_type, $entity);
  $return_uri = $uri['path'];

  // If Rules is available, signal the reversion.
  if (module_exists('rules')) {
    rules_invoke_event('workflow_state_reverted', $entity);
  }

  $current_sid = workflow_node_current_state($entity, $entity_type, $field_name);
  $transition = new WorkflowTransition();
  $transition->setValues($entity_type, $entity, $field_name, $current_sid, $previous_sid, $user->uid, REQUEST_TIME, $comment);

  // Force the transition because it's probably not valid.
  $new_sid = workflow_execute_transition($entity_type, $entity, $field_name, $transition, TRUE);
  ($previous_sid == $new_sid) ? drupal_set_message($comment) : drupal_set_message(t('State could not be reverted.'), 'warning');  

  drupal_goto($return_uri . '/workflow');
}
