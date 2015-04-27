<?php
/**
 * @file
 * Describes Workflow content pane for Panels.
 */

/**
 * Defines the Plugin.
 *
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Workflow'),
  'description' => t('Workflow states.'),
  'required context' => new ctools_context_required(t('Form'), 'node_form'),
  'category' => t('Form'),
);

/**
 * Implements pane render callback.
 */
function workflownode_node_form_workflow_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('node_form');
  $block->title = t('Workflow');
  $block->delta = 'workflow';

  if (isset($context->form)) {
    if (isset($context->form['workflow'])) {
      $block->content['workflow'] = $context->form['workflow'];
      // Set access to false on the original rather than removing so that
      // vertical tabs doesn't clone it. I think this is due to references.
      unset($context->form['workflow']);
    }
  }
  else {
    $block->content = t('Workflow.');
  }

  return $block;
}

/**
 * Returns the administrative title for a type.
 */
function workflownode_node_form_workflow_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" node form workflow', array('@s' => $context->identifier));
}

/**
 * Implements pane edit callback.
 */
function workflownode_node_form_workflow_content_type_edit_form($form, &$form_state) {
  // Provide a blank form so we have a place to have context setting.
  return $form;
}
