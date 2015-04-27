<?php

/**
 * @file
 * Contains workflow\includes\Entity\WorkflowTransition.
 *
 * Implements (scheduled/executed) state transitions on entities.
 */

/**
 * Implements an actual Transition.
 *
 * If a transition is executed, the new state is saved in the Field or {workflow_node}.
 * If a transition is saved, it is saved in table {workflow_history_node}
 */
class WorkflowTransition extends Entity {
  // Field data.
  public $entity_type;
  public $field_name = '';
  public $language = LANGUAGE_NONE;
  public $delta = 0;
  // Entity data.
  public $revision_id;
  public $entity_id; // Use WorkflowTransition->getEntity() to fetch this.
  public $nid; // @todo D8: remove $nid, use $entity_id. (requires conversion of Views displays.)
  // Transition data.
  public $old_sid = 0;
  public $new_sid = 0;
  public $sid = 0; // @todo D8: remove $sid, use $new_sid. (requires conversion of Views displays.)
  public $uid = 0; // Use WorkflowTransition->getUser() to fetch this.
  public $stamp;
  public $comment = '';
  // Cached data, from $this->entity_id and $this->uid.
  protected $entity = NULL; // Use WorkflowTransition->getEntity() to fetch this.
  protected $user = NULL; // Use WorkflowTransition->getUser() to fetch this.
  // Extra data.
  protected $is_scheduled = NULL;
  protected $is_executed = NULL;
  protected $force = NULL;

  /**
   * Entity class functions.
   */

  /**
   * Creates a new entity.
   *
   * @param string $entity_type
   *   The entity type of the attached $entity.
   * @param string $entityType
   *   The entity type of this Entity subclass.
   *
   * @see entity_create()
   *
   * No arguments passed, when loading from DB.
   * All arguments must be passed, when creating an object programmatically.
   * One argument $entity may be passed, only to directly call delete() afterwards.
   */
  public function __construct(array $values = array(), $entityType = 'WorkflowTransition') {
    // Please be aware that $entity_type and $entityType are different things!
    parent::__construct($values = array(), $entityType);

    // This transition is not scheduled,
    $this->is_scheduled = FALSE; // This transition is not scheduled,
    $this->is_executed = NULL;   // But we do not know if it is executed, yet.

    // Fill the 'new' fields correctly. @todo D8: rename these fields in db table.
    $this->entity_id = $this->nid;
    $this->new_sid = $this->sid;
  }

  /**
   * Helper function for __construct. Used for all children of WorkflowTransition (aka WorkflowScheduledTransition)
   */
  public function setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $uid, $stamp, $comment) {

    // Normally, the values are passed in an array, and set in parent::__construct, but we do it ourselves.
    // (But there is no objection to do it there.)

    $this->entity_type = (!$entity_type) ? $this->entity_type : $entity_type;
    $this->field_name = (!$field_name) ? $this->field_name : $field_name;

    // If constructor is called with new() and arguments.
    // Load the supplied entity.
    if ($entity && !$entity_type) {
      // Not all parameters are passed programmatically.
      drupal_set_message(t('Wrong call to new Workflow*Transition()'), 'error');
    }
    elseif ($entity) {
      $this->setEntity($entity_type, $entity);
    }

    if (!$entity && !$old_sid && !$new_sid) {
      // If constructor is called without arguments, e.g., loading from db.
    }
    elseif ($entity && $old_sid) {
      // Caveat: upon entity_delete, $new_sid is '0'.
      // If constructor is called with new() and arguments.
      $this->old_sid = $old_sid;
      $this->sid = $new_sid;

      $this->uid = $uid;
      $this->stamp = $stamp;
      $this->comment = $comment;

      // Set language. Multi-language is not supported for Workflow Node.
      $this->language = _workflow_metadata_workflow_get_properties($entity, array(), 'langcode', $entity_type, $field_name);
    }
    elseif (!$old_sid) {
      // Not all parameters are passed programmatically.
      drupal_set_message(
        t('Wrong call to constructor Workflow*Transition(@old_sid to @new_sid)', array('@old_sid' => $old_sid, '@new_sid' => $new_sid)),
        'error');
    }

    // Fill the 'new' fields correctly. @todo D8: rename these fields in db table.
    $this->entity_id = $this->nid;
    $this->new_sid = $this->sid;
  }

  protected function defaultLabel() {
    // @todo; Should return title of WorkflowConfigTransition. Make it a superclass??
    // return $this->title;
    return '';
  }

//  protected function defaultUri() {
//    return array('path' => 'admin/config/workflow/workflow/transitions/' . $this->wid);
//  }

  /**
   * CRUD functions.
   */

  /**
   * Given a node, get all transitions for it.
   *
   * Since this may return a lot of data, a limit is included to allow for only one result.
   *
   * @param string $entity_type
   * @param int $entity_id
   * @param string $field_name
   *   Optional. Can be NULL, if you want to load any field.
   *
   * @return array
   *   An array of WorkflowTransitions.
   */
  public static function loadMultiple($entity_type, array $entity_ids, $field_name = '', $limit = NULL, $langcode = '') {
    $query = db_select('workflow_node_history', 'h');
    $query->condition('h.entity_type', $entity_type);
    if ($entity_ids) {
      $query->condition('h.nid', $entity_ids);
    }
    if ($field_name !== NULL) {
      // If we do not know/care for the field_name, fetch all history.
      // E.g., in workflow.tokens.
      $query->condition('h.field_name', $field_name);
    }
    // Add selection on language.
    // Workflow Node: only has 'und'.
    // Workflow Field: untranslated field have 'und'.
    // Workflow Field: translated fields may be specified.
    if ($langcode) {
      $query->condition('h.language', $langcode);
    }

    $query->fields('h');
    // The timestamp is only granular to the second; on a busy site, we need the id.
    // $query->orderBy('h.stamp', 'DESC');
    $query->orderBy('h.hid', 'DESC');
    if ($limit) {
      $query->range(0, $limit);
    }
    $result = $query->execute()->fetchAll(PDO::FETCH_CLASS, 'WorkflowTransition');

    return $result;
  }

  /**
   * Property functions.
   */

  /**
   * Verifies if the given transition is allowed.
   *
   * - In settings;
   * - In permissions;
   * - By permission hooks, implemented by other modules.
   *
   * @return bool
   *   TRUE if OK, else FALSE.
   *
   *   Having both $roles AND $user seems redundant, but $roles have been
   *   tampered with, even though they belong to the $user.
   *
   * @see WorkflowConfigTransition::isAllowed()
   */
  protected function isAllowed($roles, $user, $force) {
    if ($force || ($user->uid == 1)) {
      return TRUE;
    }

    // Check allow-ability of state change if user is not superuser (might be cron).
    // Get the WorkflowConfigTransition.
    // @todo: some day, WorkflowConfigTransition can be a parent of WorkflowTransition.
    $workflow = $this->getWorkflow();
    $config_transitions = $workflow->getTransitionsBySidTargetSid($this->old_sid, $this->new_sid);
    $config_transition = reset($config_transitions);
    if (!$config_transition || !$config_transition->isAllowed($roles)) {
      $t_args = array(
        '%old_sid' => $this->old_sid,
        '%new_sid' => $this->new_sid,
      );
      watchdog('workflow', 'Attempt to go to nonexistent transition (from %old_sid to %new_sid)', $t_args, WATCHDOG_ERROR);
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Execute a transition (change state of a node).
   *
   * @param bool $force
   *   If set to TRUE, workflow permissions will be ignored.
   *
   * @return int
   *   New state ID. If execution failed, old state ID is returned,
   *
   * @deprecated: workflow_execute_transition() --> WorkflowTransition::execute().
   */
  public function execute($force = FALSE) {
    $user = $this->getUser();
    $old_sid = $this->old_sid;
    $new_sid = $this->new_sid;

    // Load the entity, if not already loaded.
    // This also sets the (empty) $revision_id in Scheduled Transitions.
    $entity = $this->getEntity();
    // Only after getEntity(), the following are surely set.
    $entity_type = $this->entity_type;
    $entity_id = $this->entity_id;
    $field_name = $this->field_name;


    // Make sure $force is set in the transition, too.
    if ($force) {
      $this->force($force);
    }

    // Store the transition, so it can be easily fetched later on.
    // Store in an array, to prepare for multiple workflow_fields per entity.
    // This is a.o. used in hook_entity_update to trigger 'transition post'.
    $entity->workflow_transitions[$field_name] = $this;

    // Prepare an array of arguments for error messages.
    $args = array(
      '%user' => isset($user->name) ? $user->name : '',
      '%old' => $old_sid,
      '%new' => $new_sid,
    );

    if (!$this->getOldState()) {
      drupal_set_message($message = t('You tried to set a Workflow State, but
        the entity is not relevant. Please contact your system administrator.'),
        'error');
      $message = 'Setting a non-relevant Entity from state %old to %new';
      $uri = entity_uri($entity_type, $entity);
      watchdog('workflow', $message, $args, WATCHDOG_ERROR, l('view', $uri['path']));
      return $old_sid;
    }

    // Check if the state has changed.
    $state_changed = ($old_sid != $new_sid);

    // If so, check the permissions.
    if ($state_changed) {
      // State has changed. Do some checks upfront.

      if (!$force) {
        // Make sure this transition is allowed by workflow module Admin UI.
        $roles = array_keys($user->roles);
        $roles = array_merge(array(WORKFLOW_ROLE_AUTHOR_RID), $roles);
        if (!$this->isAllowed($roles, $user, $force)) {
          watchdog('workflow', 'User %user not allowed to go from state %old to %new', $args, WATCHDOG_NOTICE);
          // If incorrect, quit.
          return $old_sid;
        }
      }

      if (!$force) {
        // Make sure this transition is allowed by custom module.
        // @todo D8: remove, or replace by 'transition pre'. See WorkflowState::getOptions().
        // @todo D8: replace all parameters that are inlcuded in $transition.
        $permitted = module_invoke_all('workflow', 'transition permitted', $old_sid, $new_sid, $entity, $force, $entity_type, $field_name, $this, $user);
        // Stop if a module says so.
        if (in_array(FALSE, $permitted, TRUE)) {
          watchdog('workflow', 'Transition vetoed by module.');
          return $old_sid;
        }
      }

      // Make sure this transition is valid and allowed for the current user.
      // Invoke a callback indicating a transition is about to occur.
      // Modules may veto the transition by returning FALSE.
      // (Even if $force is TRUE, but they shouldn't do that.)
      $permitted = module_invoke_all('workflow', 'transition pre', $old_sid, $new_sid, $entity, $force, $entity_type, $field_name, $this);
      // Stop if a module says so.
      if (in_array(FALSE, $permitted, TRUE)) {
        watchdog('workflow', 'Transition vetoed by module.');
        return $old_sid;
      }

    }
    elseif ($this->comment) {
      // No need to ask permission for adding comments.
      // Since you should not add actions to a 'transition pre' event, there is
      // no need to invoke the event.
    }
    else {
      // There is no state change, and no comment.
      // We may need to clean up something.
    }

    // The transition is allowed. Let other modules modify the comment.
    // @todo D8: remove all but last items from $context.
    $context = array(
      'node' => $entity,
      'sid' => $new_sid,
      'old_sid' => $old_sid,
      'uid' => $user->uid,
      'transition' => $this,
    );
    drupal_alter('workflow_comment', $this->comment, $context);

    // Now, change the database.

    // Log the new state in {workflow_node}.
    if (!$field_name) {
      if ($state_changed || $this->comment) {
        // If the node does not have an existing 'workflow' property,
        // save the $old_sid there, so it can be logged.
        if (!isset($entity->workflow)) { // This is a workflow_node sid.
          $entity->workflow = $old_sid;  // This is a workflow_node sid.
        }

        // Change the state for {workflow_node}.
        // The equivalent for Field API is in WorkflowDefaultWidget::submit.
        $data = array(
          'nid' => $entity_id,
          'sid' => $new_sid,
          'uid' => (isset($entity->workflow_uid) ? $entity->workflow_uid : $user->uid),
          'stamp' => REQUEST_TIME,
        );
        workflow_update_workflow_node($data);

        $entity->workflow = $new_sid;  // This is a workflow_node sid.
      }
    }
    else {
      // This is a Workflow Field.
      // Until now, adding code here (instead of in workflow_execute_transition() )
      // doesn't work, creating an endless loop.
/*
      if ($state_changed || $this->comment) {
        // Do a separate update to update the field (Workflow Field API)
        // This will call hook_field_update() and WorkflowFieldDefaultWidget::submit().
        // $entity->{$field_name}[$this->language] = array();
        // $entity->{$field_name}[$this->language][0]['workflow']['workflow_sid'] = $new_sid;
        // $entity->{$field_name}[$this->language][0]['workflow']['workflow_comment'] = $this->comment;
        $entity->{$field_name}[$this->language][0]['transition'] = $this;

        // Save the entity, but not through entity_save(),
        // since this will check permissions again and trigger rules.
        // @TODO: replace below by a workflow_field setter callback.
        // The transition was successfully executed, or else a message was raised.
//        entity_save($entity_type, $entity);
        // or
//        field_attach_update($entity_type, $entity);

        // Reset the entity cache after update.
        entity_get_controller($entity_type)->resetCache(array($entity_id));

        $new_sid = workflow_node_current_state($entity, $entity_type, $field_name);
      }
 */
    }

    $this->is_executed = TRUE;

    if ($state_changed || $this->comment) {

      // Log the transition in {workflow_node_history}.
      $this->save();

      // Register state change with watchdog.
      if ($state_changed) {
        $workflow = $this->getWorkflow();
        // Get the workflow_settings, unified for workflow_node and workflow_field.
        // @todo D8: move settings back to Workflow (like workflownode currently is).
        // @todo D8: to move settings back, grep for "workflow->options" and "field['settings']".
        $field = _workflow_info_field($field_name, $workflow);

        if (($new_state = $this->getNewState()) && !empty($field['settings']['watchdog_log'])) {
          $entity_type_info = entity_get_info($entity_type);
          $message = ($this->isScheduled()) ? 'Scheduled state change of @type %label to %state_name executed' : 'State of @type %label set to %state_name';
          $args = array(
            '@type' => $entity_type_info['label'],
            '%label' => entity_label($entity_type, $entity),
            '%state_name' => check_plain(t($new_state->label())),
          );
          $uri = entity_uri($entity_type, $entity);
          watchdog('workflow', $message, $args, WATCHDOG_NOTICE, l('view', $uri['path']));
        }
      }

      // Remove any scheduled state transitions.
      foreach (WorkflowScheduledTransition::load($entity_type, $entity_id, $field_name) as $scheduled_transition) {
        $scheduled_transition->delete();
      }

      // Notify modules that transition has occurred.
      // Action triggers should take place in response to this callback, not the 'transaction pre'.
      if (!$field_name) {
        // Now that workflow data is saved, reset stuff to avoid problems
        // when Rules etc want to resave the data.
        // Remember, this is only for nodes, and node_save() is not necessarily performed.
        unset($entity->workflow_comment);
        module_invoke_all('workflow', 'transition post', $old_sid, $new_sid, $entity, $force, $entity_type, $field_name, $this);
        entity_get_controller('node')->resetCache(array($entity->nid)); // from entity_load(), node_save();
      }
      else {
        // module_invoke_all('workflow', 'transition post', $old_sid, $new_sid, $entity, $force, $entity_type, $field_name, $this);
        // We have a problem here with Rules, Trigger, etc. when invoking
        // 'transition post': the entity has not been saved, yet. we are still
        // IN the transition, not AFTER. Alternatives:
        // 1. Save the field here explicitely, using field_attach_save;
        // 2. Move the invoke to another place: hook_entity_insert(), hook_entity_update();
        // 3. Rely on the entity hooks. This works for Rules, not for Trigger.
        // --> We choose option 2:
        // - First, $entity->workflow_transitions[] is set for easy re-fetching.
        // - Then, post_execute() is invoked via workflowfield_entity_insert(), _update().
      }
    }

    return $new_sid;
  }

  /**
   * Invokes 'transition post'.
   *
   * Add the possibility to invoke the hook from elsewhere.
   */
  public function post_execute($force = FALSE) {
    $old_sid = $this->old_sid;
    $new_sid = $this->new_sid;
    $entity = $this->getEntity(); // Entity may not be loaded, yet.
    $entity_type = $this->entity_type;
    // $entity_id = $this->entity_id;
    $field_name = $this->field_name;

    $state_changed = ($old_sid != $new_sid);
    if ($state_changed || $this->comment) {
      module_invoke_all('workflow', 'transition post', $old_sid, $new_sid, $entity, $force, $entity_type, $field_name, $this);
    }
  }


  /**
   * Get the Transitions $workflow.
   *
   * @return object
   *   The workflow for this Transition.
   */
  public function getWorkflow() {
    $state = workflow_state_load_single($this->new_sid);
    $workflow = workflow_load($state->wid);
    return $workflow;
  }

  /**
   * Get the Transitions $entity.
   *
   * @return object
   *   The entity, that is added to the Transition.
   */
  public function getEntity() {
    if (empty($this->entity)) {
      $entity_type = $this->entity_type;
      $entity_id = $this->entity_id;
      $entity = entity_load_single($entity_type, $entity_id);

      // Set the entity cache.
      $this->entity = $entity;

      // Make sure the vid of Entity and Transition are equal.
      // Especially for Scheduled Transition, that do not have this set, yet,
      // or may have an outdated revision ID.
      $info = entity_get_info($entity_type);
      $revision_key = $info['entity keys']['revision'];
      $this->revision_id = (isset($entity->{$revision_key})) ? $entity->{$revision_key} : NULL;
    }

    return $this->entity;
  }

  /**
   * Set the Transitions $entity.
   *
   * @param string $entity_type
   *   The entity type of the entity.
   * @param mixed $entity
   *   The Entity ID or the Entity object, to add to the Transition.
   *
   * @return object $entity
   *   The Entity, that is added to the Transition.
   */
  public function setEntity($entity_type, $entity) {
    if (!is_object($entity)) {
      $entity_id = $entity;
      // Use node API or Entity API to load the object first.
      $entity = entity_load_single($entity_type, $entity_id);
    }
    $this->entity = $entity;
    $this->entity_type = $entity_type;
    list($this->entity_id, $this->revision_id,) = entity_extract_ids($entity_type, $entity);

    // For backwards compatibility, set nid.
    $this->nid = $this->entity_id;

    return $this->entity;
  }

  public function getUser() {
    if (!isset($this->user) || ($this->user->uid != $this->uid)) {
      $this->user = user_load($this->uid);
    }
    return $this->user;
  }

  /**
   * Functions, common to the WorkflowTransitions.
   */
  public function getOldState() {
    return workflow_state_load_single($this->old_sid);
  }
  public function getNewState() {
    return workflow_state_load_single($this->new_sid);
  }

  /**
   * Returns if this is a Scheduled Transition.
   */
  public function isScheduled() {
    return $this->is_scheduled;
  }
  public function schedule($schedule = TRUE) {
    return $this->is_scheduled = $schedule;
  }

  public function isExecuted() {
    return $this->is_executed;
  }

  /**
   * A transition may be forced skipping checks.
   */
  public function isForced() {
    return (bool) $this->force;
  }
  public function force($force = TRUE) {
    return $this->force = $force;
  }

}
