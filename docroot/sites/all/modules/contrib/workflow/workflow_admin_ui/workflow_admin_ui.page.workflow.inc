<?php

/**
 * @file
 * Provides an Admin UI page for the Workflow Properties.
 */

/**
 * Menu callback. Edit a workflow's properties.
 *
 * @param Workflow $worflow
 *   The workflow object.
 *
 * @return array
 *   HTML form and permissions table.
 */
function workflow_admin_ui_edit_form($form, &$form_state, $workflow = NULL, $op) {
  $noyes = array(t('No'), t('Yes'));

  $form = array();

  $form['workflow'] = array(
    '#type' => 'value',
    '#value' => $workflow,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('workflow_admin_ui_edit_form_submit'),
    '#validate' => array('workflow_admin_ui_edit_form_validate'),
    '#weight' => 15,
  );
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($workflow->label) ? $workflow->label : $workflow->name,
    '#description' => t('The human-readable name of this content type.'),
    '#required' => TRUE,
    '#maxlength' => '254',
    '#size' => 30,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $workflow->getName(),
    '#maxlength' => '254', // $todo D8 : '#maxlength' => 32,
    '#required' => TRUE,
    '#disabled' => !empty($workflow->locked),
    '#machine_name' => array(
      'exists' => 'workflow_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this content type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  // All other settings are only for workflow_node.
  if (!module_exists('workflownode')) {
    return $form; // <==== exit !
  }

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow form settings'),
  );

  $form['basic']['options'] = array(
    '#type' => 'select',
    '#title' => t('How to show the available states'),
    '#required' => FALSE,
    '#default_value' => isset($workflow->options['options']) ? $workflow->options['options'] : 'radios',
    // '#multiple' => TRUE / FALSE,
    '#options' => array(
      // These options are taken from options.module
      'select' => 'Select list',
      'radios' => 'Radio buttons',
      // This option does not work properly on Comment Add form.
      'buttons' => 'Action buttons',
    ),
    '#description' => t("The Widget shows all available states. Decide which
      is the best way to show them."
    ),
  );
  $form['basic']['name_as_title'] = array(
    '#type' => 'radios',
    '#options' => $noyes,
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Use the workflow name as the title of the workflow form?'),
    '#default_value' => isset($workflow->options['name_as_title']) ? $workflow->options['name_as_title'] : 0,
    '#description' => t('The workflow section of the editing form is in its own
      fieldset. Checking the box will add the workflow name as the title of workflow section of the editing form.'),
  );

  $form['schedule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scheduling for Workflow'),
  );

  $form['schedule']['schedule'] = array(
    '#type' => 'radios',
    '#options' => $noyes,
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Allow scheduling of workflow transitions?'),
    '#default_value' => isset($workflow->options['schedule']) ? $workflow->options['schedule'] : 1,
    '#description' => t('Workflow transitions may be scheduled to a moment in the future.
      Soon after the desired moment, the transition is executed by Cron.'),
  );

  $form['schedule']['schedule_timezone'] = array(
    '#type' => 'radios',
    '#options' => $noyes,
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Show a timezone when scheduling a transition?'),
    '#default_value' => isset($workflow->options['schedule_timezone']) ? $workflow->options['schedule_timezone'] : 1,
  );

  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment for Workflow Log'),
  );

  $form['comment']['comment_log_node'] = array(
    '#type' => 'select',
    '#required' => FALSE,
    '#options' => array(
      // Use 0/1/2 to stay compatible with previous checkbox.
      0 => t('hidden'),
      1 => t('optional'),
      2 => t('required'),
    ),
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Show a comment field in the workflow section of the editing form?'),
    '#default_value' => isset($workflow->options['comment_log_node']) ? $workflow->options['comment_log_node'] : 0,
    '#description' => t(
      'On the node editing form, a Comment form can be shown so that the person
      making the state change can record reasons for doing so. The comment is
      then included in the node\'s workflow history.'
    ),
  );

  $form['comment']['comment_log_tab'] = array(
    '#type' => 'select',
    '#required' => FALSE,
    '#options' => array(
      // Use 0/1/2 to stay compatible with previous checkbox.
      0 => t('hidden'),
      1 => t('optional'),
      2 => t('required'),
    ),
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Show a comment field in the workflow section of the workflow tab form?'),
    '#default_value' => isset($workflow->options['comment_log_tab']) ? $workflow->options['comment_log_tab'] : 0,
    '#description' => t(
      'On the workflow tab, a Comment form can be shown so that the person
      making the state change can record reasons for doing so. The comment
      is then included in the node\'s workflow history.'
    ),
  );

  $form['watchdog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Watchdog Logging for Workflow'),
  );

  $form['watchdog']['watchdog_log'] = array(
    '#type' => 'radios',
    '#options' => $noyes,
    '#attributes' => array('class' => array('container-inline')),
    '#title' => t('Log informational watchdog messages when a transition is executed (state of a node is changed)?'),
    '#default_value' => isset($workflow->options['watchdog_log']) ? $workflow->options['watchdog_log'] : 0,
    '#description' => t('Optionally log transition state changes to watchdog.'),
  );

  $form['tab'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow tab permissions'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tab']['history_tab_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the workflow history, and show it on a separate tab.'),
    '#required' => FALSE,
    '#default_value' => isset($workflow->options['history_tab_show']) ? $workflow->options['history_tab_show'] : 1,
    '#description' => t("Every state change is recorded in table
      {workflow_node_history}. If checked and user has proper permission, a
      tab 'Workflow' is shown on the entity view page, which gives access to
      the History of the workflow."),
  );
  $form['tab']['tab_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => workflow_get_roles(),
    '#default_value' => array_keys($workflow->tab_roles),
    '#description' => t('Select any roles that should have access to the workflow tab on nodes that have a workflow.'),
  );

  return $form;
}

/**
 * Validate the workflow edit/add form.
 */
function workflow_admin_ui_edit_form_validate($form, &$form_state) {
  $workflow = $form_state['values']['workflow'];
  $name = $form_state['values']['name'];

  // Make sure workflow name is not numeric.
  if (ctype_digit($name)) {
    form_set_error('name', t('Please choose a non-numeric name for your workflow.',
      array('%name' => $name)));
  }

  // Make sure workflow name is not a duplicate.
  foreach (workflow_load_multiple() as $stored_workflow) {
    if ($name == check_plain($stored_workflow->getName()) && $workflow->wid != $stored_workflow->wid) {
      form_set_error('name', t('A workflow with the name %name already exists. Please enter another name for this workflow.',
        array('%name' => $name)));
      break;
    }
  }
}

/**
 * Submit handler for the workflow editing form.
 *
 * @see workflow_edit_form()
 * @todo: this is only valid for Node API, not for Field API.
 *        Field API has 'Field settings'.
 */
function workflow_admin_ui_edit_form_submit($form, &$form_state) {
  $workflow = $form_state['values']['workflow'];
  $insert = !empty($workflow->is_new);

  $workflow->name = trim($form_state['values']['name']);
  $workflow->label = trim($form_state['values']['label']);
  // For workflow_field, all is in the field settings.
  // All other settings are only for workflow_node.
  if (module_exists('workflownode')) {
    $workflow->tab_roles = array_filter($form_state['values']['tab_roles']);
    $workflow->options = array(
      'name_as_title' => $form_state['values']['name_as_title'],
      'options' => $form_state['values']['options'],
      'schedule' => $form_state['values']['schedule'],
      'schedule_timezone' => $form_state['values']['schedule_timezone'],
      'comment_log_node' => $form_state['values']['comment_log_node'],
      'comment_log_tab' => $form_state['values']['comment_log_tab'],
      'watchdog_log' => $form_state['values']['watchdog_log'],
      'history_tab_show' => $form_state['values']['history_tab_show'],
    );
  }

  $workflow->save();
  if ($insert) {
    $args = array(
      '%name' => $workflow->getName(),
      '@url' => url('admin/config/workflow/workflow/edit/' . $workflow->wid),
    );
    watchdog('workflow', 'Created workflow %name', $args);
    drupal_set_message(t('The workflow %name was created. Please maintain the states and transitions.', $args), 'status');
  }
  else {
    drupal_set_message(t('The workflow was updated.'));
  }
  // This redirect is needed, when changing the workflow name, with name in URL.
  // Also for cloning a workflow.
  $form_state['redirect'] = 'admin/config/workflow/workflow/manage/' . $workflow->wid;
}
