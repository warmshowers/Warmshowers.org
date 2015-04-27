<?php

/**
 * @file
 * Provides an Admin UI page for the Workflow Permissions.
 */

/**
 * View workflow permissions by role.
 *
 * @param Workflow $workflow
 *   The workflow object.
 */
function workflow_admin_ui_view_permissions_form($workflow, $op) {
  // If we don't have a workflow at this point, go back to admin page.
  if (!$workflow) {
    drupal_goto('admin/config/workflow/workflow');
  }

  $all = array();
  $roles = workflow_get_roles();
  foreach ($roles as $rid => $value) {
    $all[$rid]['name'] = $value;
  }

  $transitions = $workflow->getTransitions();
  foreach ($transitions as $transition) {
    foreach ($transition->roles as $rid) {
      $old_state = $transition->getOldState();
      $new_state = $transition->getNewState();
      $all[$rid]['transitions'][] = array(check_plain(t($old_state->label())), WORKFLOW_ADMIN_UI_ARROW, check_plain(t($new_state->label())));
    }
  }

  $header = array(t('From'), '', t('To'));
  $output = '';

  // @todo: we should theme out the html here.
  foreach ($all as $rid => $value) {
    $role_name = !empty($value['name']) ? $value['name'] : t('deleted role !rid', array('!rid' => $rid));
    $output .= '<h3>' . t('%role may do these transitions:', array('%role' => $role_name)) . '</h3>';
    if (!empty($value['transitions'])) {
      $output .= theme('table', array('header' => $header, 'rows' => $value['transitions'])) . '<p></p>';
    }
    else {
      $output .= '<table><tbody><tr class="odd"><td>' . t('None') . '</td><td></tr></tbody></table><p></p>';
    }
  }

  return $output;
}
