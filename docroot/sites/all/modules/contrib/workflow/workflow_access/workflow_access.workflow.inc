<?php

/**
 * @file
 * Provides node access permissions based on workflow states.
 */

/**
 * Implements hook_workflow().
 *
 * Update grants when a node changes workflow state.
 * This is already called when node_save is called.
 */
function workflow_access_workflow($op, $id, $new_sid, $entity, $force, $entity_type = '', $field_name = '', $transition = NULL) {
  // ALERT:
  // This is a tricky spot when called on node_insert as part of the transition from create to state1.
  // node_save invokes this function as a hook before calling node_access_acquire_grants.
  // But when it calls node_access_acquire_grants later, it does so without deleting the access
  // set when calling workflow_node_insert because it is an insert and no prior grants are expected.
  // This leads to a SQL error of duplicate grants which causes a rollback of all changes.
  // Unfortunately, setting access rights isn't the only thing we're doing on node_insert so we
  // can't skip the whole thing. So we need to fix it further downstream in order to get this to work.
  // Here we don't want to run this in the case of (and ONLY in the case of) a brand new node.
  // Node access grants will be run as part of node_save's own granting after this.
  //
  // NOTE: Any module that sets node access rights on insert will hit this situation.
  //
  switch ($op) {
    case 'transition post':
      // This is only used for Workflow Node.
      // In some other location, we make sure that 'transition post' is only
      // called for Workflow Node, never for Workflow Field. We do not need
      // that hook, since you can use the core event 'node_presave' and 'node_save'.
      // Node_access functions are only for nodes with a state change and a valid new state.
      if ($entity_type == 'node' && !isset($entity->is_new)) {
        node_access_acquire_grants($entity);
      }
      break;
  }
}
