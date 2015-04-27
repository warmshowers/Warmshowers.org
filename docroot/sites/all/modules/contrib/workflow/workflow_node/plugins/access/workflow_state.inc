<?php
/**
 * @file
 * Describes a CTools Access plugin.
 *
 * Used to select whether or not a panel (variant) is displayed based
 * upon the current workflow and/or workflow state of the current node.
 * @see https://drupal.org/node/2187731
 */

/**
 * Defines the Plugin.
 *
 * Plugins are described by creating a $plugin array which will
 * be used by the system that includes the file.
 */
$plugin = array(
  'title' => t('Workflow: state'),
  'description' => t('Controls access by workflow bundle'),
  'callback' => 'workflow_state_ctools_access_check',
  'default' => array('workflow_state' => 0),
  'settings form' => 'workflow_state_ctools_settings',
  'summary' => 'workflow_state_ctools_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Custom callback defined by 'callback' in the $plugin array.
 *
 * Check for access.
 */
function workflow_state_ctools_access_check($conf, $context) {
  // For some unknown reason $context may not be set. We just want to be sure.
  if (empty($context) || empty($context->data) || empty($context->data->workflow)) {
    return FALSE;
  }

  // If the node's content type is not part of the selected workflow access to
  // the pane is denied.
  $workflow_state = $context->data->workflow;
  if ($conf['workflow_state'] == $workflow_state) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Settings form for the 'workflow state' access plugin.
 */
function workflow_state_ctools_settings($form, &$form_state, $conf) {
  $options = array();
  $workflows = workflow_get_workflows();
  foreach ($workflows as $workflow) {
    $options[$workflow->name] = array();
    $states = workflow_get_workflow_states_by_wid($workflow->wid);
    foreach ($states as $state) {
      $options[$workflow->name][$state->sid] = $state->state;
    }
  }

  $form['settings']['workflow_state'] = array(
    '#title' => t('Select workflow state'),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t('The pane will only be visible for nodes that are in this workflow state.'),
    '#default_value' => $conf['workflow_state'],
  );
  return $form;
}

/**
 * Provide a summary description based upon the workflow state.
 */
function workflow_state_ctools_summary($conf, $context) {
  $state = workflow_get_workflow_states_by_sid($conf['workflow_state']);
  return t('Nodes that have the workflow state "@state" in workflow "@workflow"', array(
      '@state' => $state->state,
      '@workflow' => $state->name,
    )
  );
}
