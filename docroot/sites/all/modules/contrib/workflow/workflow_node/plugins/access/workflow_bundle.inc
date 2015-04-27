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
  'title' => t('Workflow: bundle'),
  'description' => t('Controls access by workflow bundle'),
  'callback' => 'workflow_bundle_ctools_access_check',
  'default' => array('workflow_bundle' => 0),
  'settings form' => 'workflow_bundle_ctools_settings',
  'summary' => 'workflow_bundle_ctools_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Custom callback defined by 'callback' in the $plugin array.
 *
 * Check for access.
 */
function workflow_bundle_ctools_access_check($conf, $context) {
  // For some unknown reason $context may not be set. We just want to be sure.
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }

  // If the node's content type is not part of the selected workflow access to
  // the pane is denied.
  $type = $context->data->type;
  $workflow = workflow_get_workflow_type_map_by_type($type);
  if (isset($workflow->wid) && $conf['workflow_bundle'] == $workflow->wid) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Settings form for the 'workflow bundle' access plugin.
 */
function workflow_bundle_ctools_settings($form, &$form_state, $conf) {
  $options = array();
  $workflows = workflow_get_workflows();
  foreach ($workflows as $value) {
    $options[$value->wid] = $value->name;
  }

  $form['settings']['workflow_bundle'] = array(
    '#title' => t('Select workflow'),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t('The pane will only be visible for nodes of content types that belong to this workflow'),
    '#default_value' => $conf['workflow_bundle'],
  );
  return $form;
}

/**
 * Provide a summary description based upon the workflow bundle.
 */
function workflow_bundle_ctools_summary($conf, $context) {
  $workflow = workflow_get_workflows_by_wid($conf['workflow_bundle']);
  return t('Nodes that have workflow "@workflow"', array('@workflow' => $workflow->name));
}
