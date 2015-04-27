<?php

/**
 * @file
 * Contains workflow\includes\Entity\WorkflowTransitionController.
 *
 * Controller class for WorkflowTransition and WorkflowScheduledTransition.
 */

/**
 * Implements a controller class for WorkflowTransition.
 *
 * The 'true' controller class is 'Workflow'.
 */
class WorkflowTransitionController extends EntityAPIController {

  /**
   * Overrides DrupalDefaultEntityController::cacheGet().
   *
   * Override default function, due to core issue #1572466.
   */
  protected function cacheGet($ids, $conditions = array()) {
    // Load any available entities from the internal cache.
    if ($ids === FALSE && !$conditions) {
      return $this->entityCache;
    }
    return parent::cacheGet($ids, $conditions);
  }

  /**
   * Insert (no update) a transition.
   *
   * @deprecated workflow_insert_workflow_node_history() --> WorkflowTransition::save()
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    // Check for no transition.
    if ($entity->old_sid == $entity->new_sid) {
      if (!$entity->comment) {
        // Write comment into history though.
        return;
      }
    }

    // Make sure we haven't already inserted history for this update.
    $last_history = workflow_transition_load_single($entity->entity_type, $entity->entity_id, $entity->field_name, $entity->language);
    if ($last_history &&
        $last_history->stamp == REQUEST_TIME &&
        $last_history->new_sid == $entity->new_sid) {
      return;
    }
    else {
      unset($entity->hid);
      $entity->stamp = REQUEST_TIME;

      return parent::save($entity, $transaction);
    }
  }
}
