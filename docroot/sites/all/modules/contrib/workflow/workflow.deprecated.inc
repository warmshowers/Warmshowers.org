<?php

/**
 * @file
 * Contains contains per-class functions, that are deprecated.
 *
 * Usage: The new code can be tested, by removing this file-include from workflow.module.
 */

/**
 * Deprecated functions related to table {workflows}.
 *
 * These are replaced by methods of class Workflow.
 */

/**
 * Get all workflows.
 *
 * @deprecated: workflow_get_workflows --> workflow_load_multiple
 */
function workflow_get_workflows() {
  return workflow_load_multiple();
}

/**
 * Get a specific workflow, wid is a unique ID.
 *
 * @deprecated: workflow_get_workflows_by_wid --> workflow_load_single
 */
function workflow_get_workflows_by_wid($wid, $reset = FALSE) {
  return workflow_load_single($wid, $reset);
}

/**
 * Get a specific workflow, name is a unique ID.
 *
 * @deprecated: workflow_get_workflows_by_name --> workflow_load_by_name
 */
function workflow_get_workflows_by_name($name) {
  return workflow_load_by_name($name);
}

/**
 * Helper function, to get the label of a given workflow.
 *
 * @deprecated: workflow_get_wid_label --> workflow_label
 */
function workflow_get_wid_label($wid) {
  if (empty($wid)) {
    $output = t('No workflow');
  }
  elseif ($workflow = workflow_load_single($wid)) {
    $output = $workflow->label();
  }
  else {
    $output = t('Unknown workflow');
  }
  return $output;
}

/**
 * Return the ID of the creation state for this workflow.
 *
 * @param mixed $wid
 *   The ID of the workflow.
 *
 * @deprecated: workflow_get_creation_state_by_wid($wid) --> $workflow->getCreationSid().
 */
function workflow_get_creation_state_by_wid($wid) {
  $sid = 0;
  if ($workflow = workflow_load_single($wid)) {
    $sid = $workflow->getCreationSid();
  }
  return $sid;
}

/**
 * Return the ID of the creation state given a content type.
 *
 * @param string $type
 *   The type of the content.
 */
//function workflow_get_creation_state_by_type($type) {
//  $sid = FALSE;
//
//  if ($workflow = workflow_get_workflows_by_type($type, 'node')) {
//    $sid = $workflow->getCreationSid();
//  }
//  return $sid;
//}

/**
 * Given information, update or insert a new workflow. Returns data by ref. (like node_save).
 *
 * @deprecated: workflow_update_workflows() --> Workflow->save()
 */
//function workflow_update_workflows(&$data, $create_creation_state = TRUE) {
//  $data = (object) $data;
//  if (isset($data->tab_roles) && is_array($data->tab_roles)) {
//    $data->tab_roles = serialize($data->tab_roles);
//  }
//
//  if (isset($data->wid) && workflow_load_single($data->wid)) {
//    drupal_write_record('workflows', $data, 'wid');
//  }
//  else {
//    drupal_write_record('workflows', $data);
//    if ($create_creation_state) {
//      $state_data = array(
//        'wid' => $data->wid,
//        'state' => t(WORKFLOW_CREATION_STATE_NAME),
//        'sysid' => WORKFLOW_CREATION,
//        'weight' => WORKFLOW_CREATION_DEFAULT_WEIGHT,
//        );
//
//      workflow_update_workflow_states($state_data);
//      // @TODO consider adding state data to return here as part of workflow data structure.
//      // That way we could past structs and transitions around as a data object as a whole.
//      // Might make clone easier, but it might be a little hefty for our needs?
//    }
//  }
//}

/**
 * Given a wid, delete the workflow and its data.
 *
 * @deprecated: workflow_delete_workflows_by_wid() --> Workflow::delete().
 */
function workflow_delete_workflows_by_wid($wid) {
  $workflow = workflow_load_single($wid);
  $workflow->delete();
}

/**
 * Deprecated functions related to table {workflow_states}.
 *
 * These are replaced by methods of class WorkflowState.
 */

/**
 * Get all active states in the system.
 *
 * @return array
 *   A keyed array $id => $name, of all active states.
 *
 * @deprecated: workflow_get_workflow_states_all() --> Workflow::getOptions()
 */
function workflow_get_workflow_states_all() {
  // Get all states, only where active.
  return workflow_get_workflow_state_names($wid = 0, $grouped = FALSE, $all = FALSE);
}

/**
 * Menu access control callback. Determine access to Workflow tab.
 *
 * @deprecated workflow_node_tab_access() --> workflow_tab_access().
 */
function workflow_node_tab_access($node = NULL) {
  if ($node == NULL) {
    return FALSE;
  }
  return workflow_tab_access('node', $node);
}

/**
 * Get the states current user can move to for a given node.
 *
 * @param object $node
 *   The node to check.
 * @param bool $force
 *   A switch to enable access to all states (e.g., for Rules).
 * @param State $state
 *   The predetermined state object (v7.x-1.3: new parameter for Workflow Field).
 *
 * @return array
 *   Array of transitions.
 *
 * @deprecated workflow_field_choices() --> WorkflowState->getOptions()
 */
function workflow_field_choices($node, $force = FALSE, $state = NULL) {
  global $user; // Alert: In 7.x-2.4, getOptions() has a new $user parameter.

  $choices = array();

  if (!$node) {
    // If no node is given, no result. (e.g., on a Field settings page.)
    return $choices;
  }

  if ($state) {
    // This is used in Field API. A state object is already passed in.
  }
  else {
    // This is used in Node API.
    $field_name = ''; // An explicit var is needed.
    $current_sid = workflow_node_current_state($node, 'node', $field_name);
    $state = workflow_state_load_single($current_sid);
  }
  return $state->getOptions('node', $node, '', $user, $force);
}

/**
 * Determine if the Workflow Form must be shown.
 * If not, a formatter must be shown, since there are no valid options.
 *
 * @param mixed $sid
 *   the current state ID.
 * @param Workflow $workflow
 *   the workflow object (might be derived from $sid).
 * @param array $options
 *   an array with $id => $label options, as determined in WorkflowState->getOptions().
 *
 * @return bool $show_widget
 *   TRUE = a form must be shown; FALSE = no form, a formatter must be shown instead.
 *
 * @deprecated workflow_show_form() --> WorkflowState->showWidget()
 */
// function workflow_show_form($sid, $workflow, array $options) {
//   $state = workflow_state_load_single($sid);
//   return !$state->showWidget($options);
// }

/**
 * Validate target state and either execute a transition immediately or schedule
 * a transition to be executed later by cron.
 *
 * @param object $entity
 * @param string $new_sid
 *   An integer; the target state ID.
 * @param bool $force
 *   Allows bypassing permissions, primarily for Rules.
 * @param array $field
 *   The field structure for the operation.
 *
 * @deprecated: workflow_transition --> WorkflowDefaultWidget::submit()
 */
function workflow_transition($entity, $new_sid, $force = FALSE, $field = array()) {
  $entity_type = 'node'; // Entity support is in workflow_transition --> WorkflowDefaultWidget::submit()
  // @todo: do not use widget:submit directly, use workflow_entity_save instead.
  $widget = new WorkflowDefaultWidget($field, $instance = array(), $entity_type, $entity);
  $form = array();
  $form_state = array();
  $items = array();
  $items[0]['workflow'] = (array) $entity;
  $items[0]['workflow']['workflow_sid'] = $new_sid;
  $widget->submit($form, $form_state, $items, $force);
}

/**
 * Get all states in the system by content type.
 */
function workflow_get_workflow_states_by_type($type) {
  $query = "SELECT ws.sid, ws.wid, ws.state, ws.weight, ws.sysid "
    . "FROM {workflow_type_map} wtm "
    . "INNER JOIN {workflow_states} ws ON ws.wid = wtm.wid "
    . "WHERE wtm.type = :type AND ws.status = 1 "
    . "ORDER BY ws.weight, ws.sid ";
  $query_array = array(':type' => $type);
  $results = db_query($query, $query_array);
  return $results->fetchAll();
}

/**
 * Get all states in the system, with options to filter, only where a workflow exists.
 *
 * @deprecated: workflow_get_workflow_states() --> WorkflowState::getStates()
 * @deprecated: workflow_get_workflow_states_by_wid() --> WorkflowState::getStates()
 */
function workflow_get_workflow_states($options = array()) {
  // Build the basic query.
  $query = db_select('workflow_states', 'ws');
  $query->leftJoin('workflows', 'w', 'w.wid = ws.wid');
  $query->fields('ws');
  $query->addField('w', 'wid');
  $query->addField('w', 'name');

  // Spin through the options and add conditions.
  foreach ($options as $column => $value) {
    $query->condition('ws.' . $column, $value);
  }

  // Set the sorting order.
  $query->orderBy('ws.wid');
  $query->orderBy('ws.weight');

  // Just for grins, add a tag that might result in modifications.
  $query->addTag('workflow_states');

  // Give them the answer.
  return $query->execute()->fetchAllAssoc('sid');
}

/**
 * Get all states in the system, with options to filter, only where a workflow exists.
 *
 * @deprecated: workflow_get_workflow_states_by_wid --> Workflow->getOptions
 */
function workflow_get_workflow_states_by_wid($wid, $options = array()) {
  $options['wid'] = $wid;
  return workflow_get_workflow_states($options);
}

/**
 * Given a sid, return a workflow. Sids are a unique id.
 *
 * @deprecated: workflow_get_workflow_by_sid --> workflow_state_load_single
 */
function workflow_get_workflow_by_sid($sid) {
  return db_query("SELECT w.wid, w.name, w.tab_roles, w.options FROM {workflow_states} s
    INNER JOIN {workflows} w ON w.wid=s.wid WHERE sid = :sid ",
    array(':sid' => $sid))->fetchObject();
}

/**
 * Given a sid, return a state. Sids are a unique id.
 *
 * @deprecated: workflow_get_workflow_states_by_sid --> workflow_state_load_single
 */
function workflow_get_workflow_states_by_sid($sid, $options = array()) {
  static $sids = array();
  if (!isset($sids[$sid])) {
    $states = workflow_get_workflow_states(array('sid' => $sid));
    $sids[$sid] = reset($states);
  }
  return $sids[$sid];
}

/**
 * Given a sid, return all other states in that workflow.
 *
 * @deprecated: replaced by WorkflowState::getStates($sid)
 */
function workflow_get_other_states_by_sid($sid) {
  $query = "SELECT sid, state "
    . "FROM {workflow_states} "
    . "WHERE wid = (SELECT wid FROM {workflow_states} WHERE sid = :sid AND status = 1 AND sysid = 0) ";
  return db_query($query, array(':sid' => $sid))->fetchAllKeyed();
}

/**
 * Given a wid and state, return a state. Wids / states are a unique id.
 */
function workflow_get_workflow_states_by_wid_state($wid, $state) {
  $options = array(
    'state' => $state,
    'wid' => $wid,
  );
  return workflow_get_workflow_states($options);
}

/**
 * Given a sid, delete the state and all associated data.
 *
 * @deprecated: workflow_delete_workflow_states_by_sid($sid, $new_sid, $true_delete) --> WorkflowState->delete()
 */
function workflow_delete_workflow_states_by_sid($sid, $new_sid = FALSE, $true_delete = FALSE) {
  if ($state = workflow_state_load_single($sid)) {
    $state->delete($new_sid, $true_delete);
  }
}

/**
 * Save (update/insert) a Workflow State into table {workflow_states}.
 *
 * @deprecated: workflow_update_workflow_states() --> WorkflowState->save()
 */
function workflow_update_workflow_states(&$data) {
  $data = (object) $data;
  if (!isset($data->sysid)) {
    $data->sysid = 0;
  }
  if (!isset($data->status)) {
    $data->status = 1;
  }
  if (isset($data->sid) && workflow_state_load_single($data->sid)) {
    drupal_write_record('workflow_states', $data, 'sid');
  }
  else {
    drupal_write_record('workflow_states', $data);
  }
}

/**
 * Functions related to table workflow_transitions.
 */

/**
 * Given a wid get the transitions.
 *
 * @deprecated: workflow_get_workflow_transitions_by_wid() ==> Workflow->loadTransitions()
 */
function workflow_get_workflow_transitions_by_wid($wid) {
  static $transitions;
  if (!isset($transitions[$wid])) {
    $query = 'SELECT t.tid, t.sid, t.target_sid, t.roles, s1.wid '
      . 'FROM {workflow_transitions} t '
      . 'INNER JOIN {workflow_states} s1 ON t.sid=s1.sid '
      . 'INNER JOIN {workflow_states} s2 ON t.target_sid=s2.sid '
      . 'WHERE s1.wid = :wid AND s2.wid = :wid';
    $transitions[$wid] = db_query('SELECT t.*, s1.wid FROM {workflow_transitions} AS t INNER JOIN {workflow_states} AS s1 ON t.sid=s1.sid INNER JOIN {workflow_states} AS s2 ON t.target_sid=s2.sid WHERE s1.wid = :wid AND s2.wid = :wid',
      array(':wid' => $wid))->fetchAll();
  }
  return $transitions[$wid];
}

/**
 * Given a tid, get the transition. It is a unique object, only one return.
 */
function workflow_get_workflow_transitions_by_tid($tid) {
  static $transitions;
  if (!isset($transitions[$tid])) {
    $transitions[$tid] = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE tid = :tid',
      array(':tid' => $tid))->fetchObject();
  }
  return $transitions[$tid];
}

/**
 * Given a sid, get the transition.
 *
 * @deprecated: workflow_get_workflow_transitions_by_sid --> workflow_state_load_single
 */
//function workflow_get_workflow_transitions_by_sid($sid) {
//  static $transitions;
//  if (!isset($transitions[$sid])) {
//    $transitions[$sid] = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE sid = :sid',
//      array(':sid' => $sid))->fetchAll();
//  }
//  return $transitions[$sid];
//}

/**
 * Given a target_sid, get the transition.
 *
 * @deprecated: workflow_get_workflow_transitions_by_sid --> Workflow::getTransitionsByTargetSid
 */
//function workflow_get_workflow_transitions_by_target_sid($target_sid) {
//  static $transitions;
//  if (!isset($transitions[$target_sid])) {
//    $transitions[$target_sid] = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE target_sid = :target_sid',
//      array(':target_sid' => $target_sid))->fetchAll();
//  }
//  return $transitions[$target_sid];
//}

/**
 * Given a sid get any transition involved.
 *
 * @deprecated
 */
//function workflow_get_workflow_transitions_by_sid_involved($sid) {
//  $results = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE sid = :sid OR target_sid = :sid', array(':sid' => $sid));
//  return $results->fetchAll();
//}

/**
 * Given a role string get any transition involved.
 *
 * @deprecated
 */
//function workflow_get_workflow_transitions_by_roles($roles) {
//  $results = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE roles LIKE :roles', array(':roles' => $roles));
//  return $results->fetchAll();
//}

/**
 * Given a sid and target_sid, get the transition. This will be unique.
 *
 * @deprecated: workflow_get_workflow_transitions_by_sid_target_sid ==> $workflow->getTransitionsBySidTargetSid
 */
//function workflow_get_workflow_transitions_by_sid_target_sid($sid, $target_sid) {
//  $results = db_query('SELECT tid, sid, target_sid, roles FROM {workflow_transitions} WHERE sid = :sid AND target_sid = :target_sid', array(':sid' => $sid, ':target_sid' => $target_sid));
//  return $results->fetchObject();
//}

/**
 * Given a tid, delete the transition.
 *
 * @deprecated: workflow_delete_workflow_transitions_by_tid ==> entity_delete($transition)
 */
function workflow_delete_workflow_transitions_by_tid($tid) {
  // Notify any interested modules before we delete, in case data is needed.
  module_invoke_all('workflow', 'transition delete', $tid, NULL, NULL, FALSE);

  return db_delete('workflow_transitions')->condition('tid', $tid)->execute();
}

/**
 * Given a sid and target_sid, get the transition. This will be unique.
 */
function workflow_delete_workflow_transitions_by_roles($roles) {
  // NOTE: This allows us to send notifications out.
  foreach (workflow_get_workflow_transitions_by_roles($roles) as $transition) {
    workflow_delete_workflow_transitions_by_tid($transition->tid);
  }
}

/**
 * Given data, insert or update a workflow_transitions.
 *
 * @deprecated: workflow_update_workflow_transitions() ==> entity_save('WorkflowConfigTransition', $transition)
 */
//function workflow_update_workflow_transitions(&$data) {
//  $data = (object) $data;
//  $transition = workflow_get_workflow_transitions_by_sid_target_sid($data->sid, $data->target_sid);
//  if ($transition) {
//    $roles = explode(',', $transition->roles);
//    foreach (explode(',', $data->roles) as $role) {
//      if (array_search($role, $roles) === FALSE) {
//        $roles[] = $role;
//      }
//    }
//    $transition->roles = implode(',', $roles);
//    drupal_write_record('workflow_transitions', $transition, 'tid');
//    $data = $transition;
//  }
//  else {
//    drupal_write_record('workflow_transitions', $data);
//  }
//}

/**
 * Given a tid and new roles, update them.
 *
 * @todo - this should be refactored out, and the update made a full actual update.
 */
//function workflow_update_workflow_transitions_roles($tid, $roles) {
//  return db_update('workflow_transitions')->fields(array('roles' => implode(',', $roles)))->condition('tid', $tid, '=')->execute();
//}

/**
 * Get allowable transitions for a given workflow state.
 *
 * Typical use:
 * "global $user;
 * "$possible = workflow_allowable_transitions($sid, 'to', $user->roles);
 *
 * If the state ID corresponded to the state named "Draft", $possible now
 * contains the states that the current user may move to from the Draft state.
 *
 * @param $sid
 *   The ID of the state in question.
 * @param $dir
 *   The direction of the transition: 'to' or 'from' the state denoted by $sid.
 *   When set to 'to' all the allowable states that may be moved to are
 *   returned; when set to 'from' all the allowable states that may move to the
 *   current state are returned.
 * @param mixed $roles
 *   Array of ints (and possibly the string 'author') representing the user's
 *   roles. If the string 'ALL' is passed (instead of an array) the role
 *   constraint is ignored (this is the default for backwards compatibility).
 *
 * @return array
 *   Associative array of ($sid => $state_name), excluding current state.
 *
 * @deprecated: workflow_allowable_transitions() => Workflow::getTransitionsBySid()
 */
//function workflow_allowable_transitions($sid, $dir = 'to', $roles = 'ALL') {
//  $transitions = array();
//
//  // Main query from transitions table.
//  $query = db_select('workflow_transitions', 't')
//    ->fields('t', array('tid'));
//
//  if ($dir == 'to') {
//    $query->innerJoin('workflow_states', 's', 's.sid = t.target_sid');
//    $query->addField('t', 'target_sid', 'state_id');
//    $query->condition('t.sid', $sid);
//  }
//  else {
//    $query->innerJoin('workflow_states', 's', 's.sid = t.sid');
//    $query->addField('t', 'sid', 'state_id');
//    $query->condition('t.target_sid', $sid);
//  }
//
//  $query->addField('s', 'state', 'state_name');
//  $query->addField('s', 'weight', 'state_weight');
//  $query->addField('s', 'sysid');
//  $query->condition('s.status', 1);
//
//  // Now let's get the current state.
//  $query2 = db_select('workflow_states', 's');
//  $query2->addField('s', 'sid', 'tid');
//  $query2->addField('s', 'sid', 'state_id');
//  $query2->addField('s', 'state', 'state_name');
//  $query2->addField('s', 'weight', 'state_weight');
//  $query2->addField('s', 'sysid');
//  $query2->condition('s.status', 1);
//  $query2->condition('s.sid', $sid);
//
//  $query2->orderBy('state_weight');
//  $query2->orderBy('state_id');
//
//  // Add the union of the two queries.
//  $query->union($query2, 'UNION');
//
//  $results = $query->execute();
//
//  foreach ($results as $transition) {
//    if ($roles == 'ALL'  // Superuser.
//      || $sid == $transition->state_id // Include current state for same-state transitions.
//      || workflow_transition_allowed($transition->tid, $roles)) {
//      $transitions[] = $transition;
//      // $transitions[$transition->tid] = $transition; //@todo
//    }
//  }
//  return $transitions;
//}

/**
 * See if a transition is allowed for a given role.
 *
 * @param int $tid
 *   A transition ID.
 * @param mixed $role
 *   A single role (int or string 'author') or array of roles.
 *
 * @return bool
 *   TRUE if the role is allowed to do the transition.
 *
 * @deprecated: workflow_transition_allowed => WorkflowConfigTransition::isAllowed
 */
function workflow_transition_allowed($tid, $role = NULL) {
  $config_transitions = entity_load('WorkflowConfigTransition', array($tid));
  $config_transition = reset($config_transitions);

  if ($role) {
    if (!is_array($role)) {
      $role = array($role);
    }
    $allowed = $config_transition->roles;
    return array_intersect($role, $allowed) == TRUE;
  }
}


/**
 * Deprecated functions related to table {workflow_scheduled_transition}.
 *
 * These are replaced by methods of class WorkflowScheduledTransition.
 */

/**
 * Given a node, get all scheduled transitions for it.
 *
 * @deprecated: workflow_get_workflow_scheduled_transition_by_nid() --> WorkflowScheduledTransition::load()
 */
function workflow_get_workflow_scheduled_transition_by_nid($nid) {
  return WorkflowScheduledTransition::load('node', $nid);
}

/**
 * Given a timeframe, get all scheduled transitions.
 *
 * @deprecated: workflow_get_workflow_scheduled_transition_by_between() --> WorkflowScheduledTransition::loadBetween()
 */
function workflow_get_workflow_scheduled_transition_by_between($start = 0, $end = REQUEST_TIME) {
  return WorkflowScheduledTransition::loadBetween($start, $end);
}

/**
 * Insert a new scheduled transition. Only one transition at a time (for now).
 *
 * @deprecated: workflow_insert_workflow_scheduled_transition() --> WorkflowScheduledTransition::save()
 */
function workflow_insert_workflow_scheduled_transition($data) {
  $data = (object) $data;
  workflow_delete_workflow_scheduled_transition_by_nid($data->nid);
  drupal_write_record('workflow_scheduled_transition', $data);
}

/**
 * Given a node, delete transitions for it.
 *
 * @deprecated: workflow_delete_workflow_scheduled_transition_by_nid() --> WorkflowScheduledTransition::delete()
 */
// function workflow_delete_workflow_scheduled_transition_by_nid($nid) {
//   return WorkflowScheduledTransition::deleteById('node', $nid);
// }

/**
 * Deprecated functions related to table {workflow_node}.
 */

/**
 * Given nid, update the new stamp. This probably can be refactored. Called by workflow_execute_transition().
 *
 * @deprecated: this is micro-optimalisation.
 */
function workflow_update_workflow_node_stamp($nid, $new_stamp) {
  return db_update('workflow_node')->fields(array('stamp' => $new_stamp))->condition('nid', $nid, '=')->execute();
}

/**
 * Deprecated functions related to table {workflow_node_history}.
 */

/**
 * Get most recent transition for a node.
 *
 * @deprecated: workflow_get_recent_node_history() --> workflow_transition_load_single()
 */
function workflow_get_recent_node_history($nid) {
  $field_name = '';
  return workflow_transition_load_single('node', $nid, $field_name);
}

/**
 * Get all recorded history for a node id.
 *
 * Since this may return a lot of data, a limit is included to allow for only one result.
 */
function workflow_get_workflow_node_history_by_nid($nid, $limit = NULL) {
  $field_name = '';
  return workflow_transition_load_multiple('node', array($nid), $field_name, $limit);
}

/**
 * Given data, insert a new history. Always insert.
 *
 * @deprecated: workflow_insert_workflow_node_history() --> WorkflowTransition::save()
 */
function workflow_insert_workflow_node_history($data) {
  $data = (object) $data;
  if (isset($data->hid)) {
    unset($data->hid);
  }

  // Check for no transition.
  if ($data->old_sid == $data->sid) {
    // Make sure we haven't already inserted history for this update.
    $last_history = workflow_get_workflow_node_history_by_nid($data->nid, 1);
    if (isset($last_history) && $last_history->stamp == REQUEST_TIME) {
      return;
    }
  }
  drupal_write_record('workflow_node_history', $data);
}
