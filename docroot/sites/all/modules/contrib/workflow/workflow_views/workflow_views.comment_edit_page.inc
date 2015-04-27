<?php

/**
 * @file
 * Displays a text area populated with the selected workflow log comment and allows the user to modify and save it.
 */

function workflow_views_comment_edit_form($form, $form_state, $workflow_state_transition_record) {
  $form = array();
  $form['hid'] = array(
    '#type' => 'value',
    '#value' => $workflow_state_transition_record->hid,
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $workflow_state_transition_record->nid,
  );
  $form['workflow']['workflow_comment'] = array(
    '#type' => 'textarea',
    '#title' => t('Comment'),
    '#description' => t('Modify this workflow state transition comment and press submit.'),
    '#default_value' => $workflow_state_transition_record->comment,
    '#rows' => 2,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Submit handler for the workflow transition comment edit form.
 *
 * @see workflow_extensions_workflow_comment_edit_form()
 */
function workflow_views_comment_edit_form_submit($form, &$form_state) {

  $hid = $form_state['values']['hid'];
  $comment = $form_state['values']['workflow_comment'];

  db_update('workflow_node_history')
    ->fields(array('comment' => $comment))
    ->condition('hid', $hid)
    ->execute();

  // Whatever is set here, is overriden by the "?destination=..." parameter, if present
  // $nid = $form_state['values']['nid'];
  // $form_state['redirect'] = module_exists('views') && views_get_view('workflow_history')
  //   ? "workflow-history/$nid"
  //   : (workflow_node_tab_access(node_load($nid)) ? "node/$nid/workflow" : "node/$nid");
}
