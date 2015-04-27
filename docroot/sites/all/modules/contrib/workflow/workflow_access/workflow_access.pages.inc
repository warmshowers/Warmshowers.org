<?php
/**
 * @file
 * Provides pages for administrative UI.
 */

/**
 * Implements hook_form().
 *
 * Add a form to set the weight fo the access module.
 */
function workflow_access_priority_form($form, $form_state) {
  $form['workflow_access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow Access Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['workflow_access']['#tree'] = TRUE;
  $form['workflow_access']['workflow_access_priority'] = array(
    '#type' => 'weight',
    '#delta' => 10,
    '#title' => t('Workflow Access Priority'),
    '#default_value' => variable_get('workflow_access_priority', 0),
    '#description' => t('This sets the node access priority. Changing this
      setting can be dangerous. If there is any doubt, leave it at 0. 
      <a href="@url">Read the manual.</a>', array('@url' => url('https://api.drupal.org/api/drupal/modules!node!node.api.php/function/hook_node_access_records/7'))),
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Submit handler.
 */
function workflow_access_priority_form_submit($form, &$form_state) {
  variable_set('workflow_access_priority', $form_state['values']['workflow_access']['workflow_access_priority']);
  $form_state['redirect'] = 'admin/config/workflow/workflow';
}


/**
 * Implements hook_form().
 *
 * Add a "three dimensional" (state, role, permission type) configuration
 * interface to the workflow edit form.
 */
function workflow_access_form($form, $form_state, $workflow, $op) {
  if (!$workflow) {
    // Leave this page immediately.
    drupal_set_message(t('Unknown workflow'));
    drupal_goto('admin/config/workflow/workflow');
  }

  drupal_set_title(t('@name Access', array('@name' => $workflow->label()))); // No t() for Settings page.

  $form = array('#tree' => TRUE);

  $form['#wid'] = $workflow->wid;

  // A list of role names keyed by role ID, including the 'author' role.
  $roles = workflow_get_roles('participate in workflow');

  // Add a table for every workflow state.
  foreach ($workflow->getStates($all = TRUE) as $state) {
    if ($state->isCreationState()) {
      // No need to set perms on creation.
      continue;
    }
    $view = $update = $delete = array();
    $count = 0;
    foreach (workflow_access_get_workflow_access_by_sid($state->sid) as $access) {
      $count++;
      if ($access->grant_view) {
        $view[] = $access->rid;
      }
      if ($access->grant_update) {
        $update[] = $access->rid;
      }
      if ($access->grant_delete) {
        $delete[] = $access->rid;
      }
    }
    // Allow view grants by default for anonymous and authenticated users,
    // if no grants were set up earlier.
    if (!$count) {
      $view = array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID);
    }
    // @todo: better tables using a #theme function instead of direct #prefixing.
    $form[$state->sid] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($state->label()),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form[$state->sid]['view'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#default_value' => $view,
      '#title' => t('Roles who can view posts in this state'),
      '#prefix' => '<table width="100%" style="border: 0;"><tbody style="border: 0;"><tr><td>',
    );

    $form[$state->sid]['update'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#default_value' => $update,
      '#title' => t('Roles who can edit posts in this state'),
      '#prefix' => "</td><td>",
    );

    $form[$state->sid]['delete'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#default_value' => $delete,
      '#title' => t('Roles who can delete posts in this state'),
      '#prefix' => "</td><td>",
      '#suffix' => "</td></tr></tbody></table>",
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Stores permission settings for workflow states.
 */
function workflow_access_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $sid => $access) {
    // Ignore irrelevant keys.
    if (!is_numeric($sid)) {
      continue;
    }
    foreach ($access['view'] as $rid => $checked) {
      $data = array(
        'sid' => $sid,
        'rid' => $rid,
        'grant_view' => (!empty($checked)) ? (bool) $checked : 0,
        'grant_update' => (!empty($access['update'][$rid])) ? (bool) $access['update'][$rid] : 0,
        'grant_delete' => (!empty($access['delete'][$rid])) ? (bool) $access['delete'][$rid] : 0,
      );
      workflow_access_insert_workflow_access_by_sid($data);
    }

    // Update all nodes having same workflow state to reflect new settings.
    // just set a flag, which is working for both Workflow Field ánd Workflow Node.
    node_access_needs_rebuild(TRUE);
  }

  drupal_set_message(t('Workflow access permissions updated.'));
  $form_state['redirect'] = 'admin/config/workflow/workflow/' . $form['#wid'];
}
