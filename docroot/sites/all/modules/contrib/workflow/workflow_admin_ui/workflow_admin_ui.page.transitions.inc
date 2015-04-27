<?php

/**
 * @file
 * Provides an Admin UI page for the Workflow Transitions.
 */

/**
 * Menu callback. Edit a workflow's transitions.
 *
 * @param array $transitions from values.
 *   Transitions, for example:
 *     18 => array(
 *       20 => array(
 *         'author' => 1,
 *         1        => 0,
 *         2        => 1,
 *       )
 *     )
 *   means the transition from state 18 to state 20 can be executed by
 *   the node author or a user in role 2. The $transitions array should
 *   contain ALL transitions for the workflow.
 * @param Workflow $workflow
 *   The Workflow object.
 *
 * @return array
 *   HTML form and permissions table.
 */
function workflow_admin_ui_transitions_form($form, &$form_state, $workflow, $op) {
  // Make sure we have a workflow.
  if ($workflow) {
    $form = array();
    $form['workflow'] = array(
      '#type' => 'value',
      '#value' => $workflow,
    );
    $form['transitions'] = _workflow_admin_ui_transition_grid_form($form, $form_state, $workflow);
    $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

    return $form;
  }
}

/**
 * Form builder. Build the grid of transitions for defining a workflow.
 *
 * Some special situations exist:
 * - it is not allowed to go to the '(creation)' state.
 * - all roles are permitted to use the 'stay on this state' transition.
 *   So, this is hidden from the user.
 *
 * @param Workflow $workflow
 *   The Workflow object.
 */
function _workflow_admin_ui_transition_grid_form($form, &$form_state, $workflow) {
  $form = array('#tree' => TRUE);

  $states = $workflow->getStates($all = 'CREATION');
  if (!$states) {
    $form['error'] = array(
      '#type' => 'markup',
      '#value' => t('There are no states defined for this workflow.'),
    );
    return $form;
  }

  $roles = workflow_get_roles();
  foreach ($states as $state1) {
    $from = $state1->sid;
    foreach ($states as $state2) {
      // Don't allow transition TO '(creation)'.
      if (!$state2->isCreationState()) {
        $to = $state2->sid;
        $stay_on_this_state = ($to == $from);

        // Generate checkboxes for each transition.
        foreach ($roles as $rid => $role_name) {
          $checked = $stay_on_this_state;
          $config_transitions = $workflow->getTransitionsBySidTargetSid($from, $to);
          if ($config_transition = reset($config_transitions)) {
            $checked |= $config_transition->isAllowed(array($rid));
          }
          $form[$from][$to][$rid] = array(
            '#type' => $stay_on_this_state ? 'hidden' : 'checkbox',
            '#title' => check_plain($role_name),
            '#default_value' => $checked,
            '#disabled' => $stay_on_this_state,
          );
        }
      }
    }
  }
  return $form;
}

/**
 * Theme the workflow editing form.
 *
 * @see workflow_edit_form()
 */
function theme_workflow_admin_ui_transitions_form($variables) {
  $output = '';
  $form = $variables['form'];

  $workflow = $form['workflow']['#value'];

  if ($workflow) {
    drupal_set_title(t('Edit workflow %name transitions', array('%name' => $workflow->getName())), PASS_THROUGH);

    $states = $workflow->getStates($all = 'CREATION');
    if ($states) {
      $roles = workflow_get_roles();

      $header = array(array('data' => t('From / To') . ' &nbsp;' . WORKFLOW_ADMIN_UI_ARROW));
      $rows = array();
      foreach ($states as $state) {
        $label = check_plain($state->label());
        // Don't allow transition TO (creation).
        if (!$state->isCreationState()) {
          $header[] = array('data' => $label);
        }
        $row = array(array('data' => $label));
        foreach ($states as $nested_state) {
          // Don't allow transition TO (creation).
          if ($nested_state->isCreationState()) {
            continue;
          }
          if (TRUE || $nested_state != $state) {
            // Render checkboxes for each transition.
            $from = $state->sid;
            $to = $nested_state->sid;
            $cell = '';
            foreach ($roles as $rid => $role_name) {
              $cell .= drupal_render($form['transitions'][$from][$to][$rid]);
            }
            $row[] = array('data' => $cell);
          }
          else {
            $row[] = array('data' => '');
          }
        }
        $rows[] = $row;
      }
      $output .= theme('table', array('header' => $header, 'rows' => $rows));
    }
    else {
      $output = t('There are no states defined for this workflow.');
    }

    $output .= drupal_render_children($form);
    return $output;
  }
}

/**
 * Validate the workflow editing form.
 *
 * @see workflow_edit_form()
 */
function workflow_admin_ui_transitions_form_validate($form, $form_state) {
  $workflow = $form_state['values']['workflow'];
  $wid = $workflow->wid;

  // Make sure 'author' is checked for (creation) -> [something].
  $creation_state = $workflow->getCreationState();
  $creation_sid = $creation_state->sid;
  if (isset($form_state['values']['transitions'][$creation_sid]) && is_array($form_state['values']['transitions'][$creation_sid])) {
    foreach ($form_state['values']['transitions'][$creation_sid] as $roles) {
      if ($roles[WORKFLOW_ROLE_AUTHOR_RID]) {
        $author_has_permission = TRUE;
        break;
      }
    }
  }
  $state_count = db_query('SELECT COUNT(sid) FROM {workflow_states} WHERE wid = :wid', array(':wid' => $wid))->fetchField();
  if (empty($author_has_permission) && $state_count > 1) {
    form_set_error('transitions', t('Please give the author permission to go from %creation to at least one state!',
      array('%creation' => $creation_state->label())));
  }
}

/**
 * Submit handler for the workflow editing form.
 *
 * @see workflow_edit_form()
 */
function workflow_admin_ui_transitions_form_submit($form, &$form_state) {
  $workflow = $form['workflow']['#value'];
  $wid = $workflow->wid;

  if (isset($form_state['values']['transitions'])) {
    $transitions = $form_state['values']['transitions'];

    // Empty string is sometimes passed in instead of an array.
    if (!$transitions) {
      return;
    }
    foreach ($transitions as $from => $to_data) {
      foreach ($to_data as $to => $role_data) {
        $roles = array();
        foreach ($role_data as $role => $can_do) {
          if ($can_do) {
            $roles += array($role => $role);
          }
        }
        if (count($roles)) {
          $config_transition = $workflow->createTransition($from, $to);
          $config_transition->roles = $roles;
          $config_transition->save();
        }
        else {
          foreach ($workflow->getTransitionsBySidTargetSid($from, $to, 'ALL') as $config_transition) {
            $config_transition->delete();
          }
        }
      }
    }
  }

  drupal_set_message(t('The workflow was updated.'));
  // $form_state['redirect'] = 'admin/config/workflow/workflow';
}
