<?php

/**
 * @file
 * Hooks provided by the workflow module.
 */

/**
 * Implements hook_workflow().
 */
function workflownode_workflow($op, $id, $new_sid, $entity, $force, $entity_type = '', $field_name = '', $transition = NULL) {
  switch ($op) {
    case 'transition permitted':
      return TRUE;

    case 'transition pre':
      break;

    case 'transition post':
      break;

    case 'transition delete':
      // A transition is deleted. Only the first parameter is used.
      // $tid = $id;
      break;

    case 'state delete':
      // A state is deleted. Only the first parameter is used.
      // $current_sid = $id;
      break;

    case 'workflow delete':
      // A workflow is deleted. Only the first parameter is used.
      $wid = $id;
      workflow_delete_workflow_type_map_by_wid($wid);
      break;
  }
}
