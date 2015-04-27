<?php

/**
 * @file
 * Contains workflow\includes\Entity\WorkflowScheduledTransition.
 */

/**
 * Implements a scheduled transition, as shown on Workflow form.
 */
class WorkflowScheduledTransition extends WorkflowTransition {
  // Scheduled timestamp of state change.
  public $scheduled;

  /**
   * Constructor.
   */
  public function __construct(array $values = array(), $entityType = 'WorkflowScheduledTransition') {
    // Please be aware that $entity_type and $entityType are different things!
    parent::__construct($values, $entityType);

    $this->is_scheduled = TRUE;
    $this->is_executed = FALSE;
  }

  public function setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $uid, $scheduled, $comment) {
    // A scheduled transition does not have a timestamp, yet.
    $stamp = 0;
    parent::setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $uid, $stamp, $comment);

    // Set the scheduled timestamp of state change.
    $this->scheduled = $scheduled;
  }

  /**
   * Given a node, get all scheduled transitions for it.
   *
   * @param string $entity_type
   * @param int $entity_id
   * @param string $field_name
   *   Optional.
   *
   * @return array
   *   An array of WorkflowScheduledTransitions.
   *
   * @deprecated: workflow_get_workflow_scheduled_transition_by_nid() --> WorkflowScheduledTransition::load()
   */
  public static function load($entity_type, $entity_id, $field_name = '', $limit = NULL) {
    if (!$entity_id) {
      return array();
    }

    $query = db_select('workflow_scheduled_transition', 'wst');
    $query->fields('wst');
    $query->condition('entity_type', $entity_type, '=');
    $query->condition('nid', $entity_id, '=');
    if ($field_name !== NULL) {
      $query->condition('field_name', $field_name, '=');
    }
    $query->orderBy('scheduled', 'ASC');
    $query->addTag('workflow_scheduled_transition');
    if ($limit) {
      $query->range(0, $limit);
    }
    $result = $query->execute()->fetchAll(PDO::FETCH_CLASS, 'WorkflowScheduledTransition');

    return $result;
  }

  /**
   * Given a timeframe, get all scheduled transitions.
   *
   * @deprecated: workflow_get_workflow_scheduled_transition_by_between() --> WorkflowScheduledTransition::loadBetween()
   */
  public static function loadBetween($start = 0, $end = 0) {
    $query = db_select('workflow_scheduled_transition', 'wst');
    $query->fields('wst');
    $query->orderBy('scheduled', 'ASC');
    $query->addTag('workflow_scheduled_transition');

    if ($start) {
      $query->condition('scheduled', $start, '>');
    }
    if ($end) {
      $query->condition('scheduled', $end, '<');
    }

    $result = $query->execute()->fetchAll(PDO::FETCH_CLASS, 'WorkflowScheduledTransition');
    return $result;
  }

  /**
   * Save a scheduled transition. If the transition is executed, save in history.
   */
  public function save() {
    // If executed, save in history.
    if ($this->is_executed) {
      // Be careful, we are not a WorkflowScheduleTransition anymore!
      $this->entityType = 'WorkflowTransition';
      $this->setUp();

      return parent::save(); // <--- exit !!
    }

    // Since we do not have an entity_id here, we cannot use entity_delete.
    // @todo: Add an 'entity id' to WorkflowScheduledTransition entity class.
    // $result = parent::save();

    // Avoid duplicate entries.
    $clone = clone $this;
    $clone->delete();
    // Save (insert or update) a record to the database based upon the schema.
    drupal_write_record('workflow_scheduled_transition', $this);

    // Create user message.
    if ($state = $this->getNewState()) {
      $entity_type = $this->entity_type;
      $entity = $this->getEntity();
      $message = '%entity_title scheduled for state change to %state_name on %scheduled_date';
      $args = array(
        '@entity_type' => $entity_type,
        '%entity_title' => entity_label($entity_type, $entity),
        '%state_name' => entity_label('WorkflowState', $state),
        '%scheduled_date' => format_date($this->scheduled),
      );
      $uri = entity_uri($entity_type, $entity);
      watchdog('workflow', $message, $args, WATCHDOG_NOTICE, l('view', $uri['path'] . '/workflow'));
      drupal_set_message(t($message, $args));
    }
  }

  /**
   * Given a node, delete transitions for it.
   *
   * @deprecated: workflow_delete_workflow_scheduled_transition_by_nid() --> WorkflowScheduledTransition::delete()
   */
  public function delete() {
    // Support translated Workflow Field workflows by including the language.
    db_delete($this->entityInfo['base table'])
        ->condition('entity_type', $this->entity_type)
        ->condition('nid', $this->entity_id)
        ->condition('field_name', $this->field_name)
        ->condition('language', $this->language)
        ->execute();
  }

  /**
   * Property functions.
   */

  /**
   * If a scheduled transition has no comment, a default comment is added before executing it.
   */
  public function addDefaultComment() {
    $this->comment = t('Scheduled by user @uid.', array('@uid' => $this->uid));
  }

}
