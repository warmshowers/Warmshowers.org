<?php
/**
 * @file
 * Contains admin/config/workflow/workflow/cleanup page.
 */

/**
 * The main cleanup page.
 */
function workflow_cleanup_form($form, $form_state) {
  $form = array();

  // Get all of the states, indexed by sid.
  $states = $orphans = $inactive = array();

  foreach ($states = workflow_state_load_multiple() as $state) {
    // Does the associated workflow exist?
    if (!$state->getWorkflow()) {
      $orphans[$state->sid] = $state->getName();
    }
    else {
      // Is the state still active?
      if (!$state->isActive()) {
        $inactive[$state->sid] = $state->getName();
      }
    }
  }

  $form['#workflow_states'] = $states;

  $form['no_workflow'] = array(
    '#type' => 'container',
    '#title' => t('Orphaned States'),
    '#description' => t('These states no longer belong to an existing workflow.'),
    '#tree' => TRUE,
  );

  foreach ($orphans as $sid => $name) {
    $form['no_workflow'][$sid]['check'] = array(
      '#type' => 'checkbox',
      '#return_value' => $sid,
    );

    $form['no_workflow'][$sid]['name'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($name),
    );
  }

  $form['inactive'] = array(
    '#type' => 'container',
    '#title' => t('Inactive (Deleted) States'),
    '#description' => t('These states belong to a workflow, but have been marked inactive (deleted).'),
    '#tree' => TRUE,
  );

  foreach ($inactive as $sid => $name) {
    $form['inactive'][$sid]['check'] = array(
      '#type' => 'checkbox',
      '#return_value' => $sid,
    );

    $form['inactive'][$sid]['name'] = array(
      '#type' => 'markup',
      '#markup' => check_plain($name),
    );

    $form['inactive'][$sid]['wf'] = array(
      '#type' => 'markup',
      '#markup' => (!empty($sid)) ? check_plain($states[$sid]->getWorkflow()->getName()) : '',
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Delete selected states'));

  return $form;
}

/**
 * Theme the main form.
 */
function theme_workflow_cleanup_form($variables) {
  $form = $variables['form'];
  $output = '';
  $header = array(t('Select'), t('State'));

  $rows = array();
  foreach (element_children($form['no_workflow']) as $sid) {
    $rows[] = array(
      drupal_render($form['no_workflow'][$sid]['check']),
      drupal_render($form['no_workflow'][$sid]['name']),
    );
  }

  $output .= '<h3>' . $form['no_workflow']['#title'] . '</h3>';
  $output .= '<div class="description">' . $form['no_workflow']['#description'] . '</div>';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => 'All states are fine',
    'attributes' => array('style' => 'width: auto;'),
  ));

  $header[] = t('Workflow');
  $rows = array();
  foreach (element_children($form['inactive']) as $sid) {
    $rows[] = array(
      drupal_render($form['inactive'][$sid]['check']),
      drupal_render($form['inactive'][$sid]['name']),
      drupal_render($form['inactive'][$sid]['wf']),
    );
  }

  $output .= '<h3>' . $form['inactive']['#title'] . '</h3>';
  $output .= '<div class="description">' . $form['inactive']['#description'] . '</div>';
  $output .= theme(
    'table',
    array(
      'header' => $header,
      'rows' => $rows,
      'empty' => 'All states are fine',
      'attributes' => array('style' => 'width: auto;'),
    )
  );

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Submission handler for main cleanup form.
 */
function workflow_cleanup_form_submit($form, $form_state) {
  $states = $form['#workflow_states'];
  foreach (array('no_workflow', 'inactive') as $section) {
    if (!isset($form_state['values'][$section])) {
      continue;
    }
    foreach ($form_state['values'][$section] as $sid => $data) {
      // FAPI returns either a 0 or the sid.
      if ($data['check']) {
        $state = $states[$sid];
        $state_name = $state->getName();
        // Delete any transitions this state is involved in.
        $trans_del = db_delete('workflow_transitions')->condition('target_sid', $sid)->execute();
        $trans_del += db_delete('workflow_transitions')->condition('sid', $sid)->execute();
        if ($trans_del) {
          drupal_set_message(t('@count transitions for the "@state" state have been deleted.',
            array('@state' => $state_name, '@count' => $trans_del)));
        }

        // Remove history records too.
        $hist_del = db_delete('workflow_node_history')->condition('sid', $sid)->execute();
        if ($hist_del) {
          drupal_set_message(t('@count history records for the "@state" state have been deleted.',
            array('@state' => $state_name, '@count' => $hist_del)));
        }

        // Go ahead and delete the state.
        db_delete('workflow_states')->condition('sid', $sid)->execute();
        drupal_set_message(t('The "@state" state has been deleted.',
          array('@state' => $state_name)));
      }
    }
  }
}
