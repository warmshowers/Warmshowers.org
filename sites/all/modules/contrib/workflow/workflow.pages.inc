<?php
// $Id: workflow.pages.inc,v 1.2 2008/10/29 03:13:55 jvandyk Exp $

/**
 * @file
 * Provide user interface for changing workflow state.
 */

/**
 * Menu callback. Display workflow summary of a node.
 */
function workflow_tab_page($node = NULL) {
  drupal_set_title(check_plain($node->title));
  $wid = workflow_get_workflow_for_type($node->type);
  $states_per_page = variable_get('workflow_states_per_page', 20);
  $result = db_query("SELECT sid, state FROM {workflow_states} WHERE status = 1 ORDER BY sid");
  while ($data = db_fetch_object($result)) {
    $states[$data->sid] = $data->state;
  }
  $deleted_states = array();
  $result = db_query("SELECT sid, state FROM {workflow_states} WHERE status = 0 ORDER BY sid");
  while ($data = db_fetch_object($result)) {
    $deleted_states[$data->sid] = $data->state;
  }
  $current = workflow_node_current_state($node);

  // theme_workflow_current_state() must run state through check_plain().
  $output = '<p>'. t('Current state: !state', array('!state' => theme('workflow_current_state', $states[$current]))) . "</p>\n";
  $output .= drupal_get_form('workflow_tab_form', $node, $wid, $states, $current);

  $result = pager_query("SELECT h.*, u.name FROM {workflow_node_history} h LEFT JOIN {users} u ON h.uid = u.uid WHERE nid = %d ORDER BY hid DESC", $states_per_page, 0, NULL, $node->nid);
  $rows = array();
  while ($history = db_fetch_object($result)) {
    if ($history->sid == $current && !isset($deleted_states[$history->sid]) && !isset($current_themed)) {
      // Theme the current state differently so it stands out.
      $state_name = theme('workflow_current_state', $states[$history->sid]);
      // Make a note that we have themed the current state; other times in the history
      // of this node where the node was in this state do not need to be specially themed.
      $current_themed = TRUE;
    }
    elseif (isset($deleted_states[$history->sid])) {
      // The state has been deleted, but we include it in the history.
      $state_name = theme('workflow_deleted_state', $deleted_states[$history->sid]);
      $footer_needed = TRUE;
    }
    else {
      // Regular state.
      $state_name = check_plain($states[$history->sid]);
    }

    if (isset($deleted_states[$history->old_sid])) {
      $old_state_name = theme('workflow_deleted_state', $deleted_states[$history->old_sid]);
      $footer_needed = TRUE;
    }
    else {
      $old_state_name = check_plain($states[$history->old_sid]);
    }
    $rows[] = theme('workflow_history_table_row', $history, $old_state_name, $state_name);
  }
  $output .= theme('workflow_history_table', $rows, !empty($footer_needed));
  $output .= theme('pager', $states_per_page);
  return $output;
}

/*
 * Theme one workflow history table row.
 */
function theme_workflow_history_table_row($history, $old_state_name, $state_name) {
  return array(
    format_date($history->stamp),
    $old_state_name,
    $state_name,
    theme('username', $history),
    filter_xss($history->comment, array('a', 'em', 'strong')),
  );
}

/*
 * Theme entire workflow history table.
 */
function theme_workflow_history_table($rows, $footer) {
  $output = theme('table', array(t('Date'), t('Old State'), t('New State'), t('By'), t('Comment')), $rows, array('class' => 'workflow_history'), t('Workflow History'));
  if ($footer) {
    $output .= t('*State is no longer available.');
  }
  return $output;
}

/**
 * Theme the current state in the workflow history table.
 */
function theme_workflow_current_state($state_name) {
  return '<strong>'. check_plain($state_name) .'</strong>';
}

/**
 * Theme a deleted state in the workflow history table.
 */
function theme_workflow_deleted_state($state_name) {
  return check_plain($state_name) .'*';
}

/**
 * Form builder. Allow workflow state change and scheduling from workflow tab.
 *
 * @param $node
 *   Node for which workflow information will be displayed.
 * @param $wid
 *   ID of workflow to display.
 * @param $states
 *   Array of states for the workflow.
 * @param $current
 *   Current workflow state of this node.
 * @return
 *   Form definition array.
 */
function workflow_tab_form($form_state, &$node, $wid, $states, $current) {
  $form['#tab'] = TRUE;
  $choices = workflow_field_choices($node);

  $min = $states[$current] == t('(creation)') ? 1 : 2;
  // Only build form if user has possible target state(s).
  if (count($choices) >= $min) {
    $wid = workflow_get_workflow_for_type($node->type);
    $workflow = workflow_load($wid);
    $form['#wf'] = $workflow;
    $name = check_plain($workflow->name);
    // See if scheduling information is present.
    if ($node->_workflow_scheduled_timestamp && $node->_workflow_scheduled_sid) {
      global $user;
      if (variable_get('configurable_timezones', 1) && $user->uid && strlen($user->timezone)) {
        $timezone = $user->timezone;
      }
      else {
        $timezone = variable_get('date_default_timezone', 0);
      }
      // The default value should be the upcoming sid.
      $current = $node->_workflow_scheduled_sid;
      $timestamp = $node->_workflow_scheduled_timestamp;
      $comment = $node->_workflow_scheduled_comment;
    }

    // Include the same form elements here that are included on a
    // regular node editing page. $form is modified by reference.
    workflow_node_form($form, $form_state, t('Change %s state', array('%s' => $name)), $name, $current, $choices, $timestamp, $comment);

    $form['node'] = array(
      '#type' => 'value',
      '#value' => $node,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit')
    );
  }
  return $form;
}

/**
 * Submit handler for the form on the workflow tab.
 *
 * @see workflow_tab_form
 */
function workflow_tab_form_submit($form, &$form_state) {
  // The entire node object was stashed in the form.
  $node = $form_state['values']['node'];
  $node->workflow = $form_state['values']['workflow'];
  $node->workflow_comment = $form_state['values']['workflow_comment'];
  $node->workflow_scheduled = $form_state['values']['workflow_scheduled'];
  $node->workflow_scheduled_date = $form_state['values']['workflow_scheduled_date'];
  $node->workflow_scheduled_hour = $form_state['values']['workflow_scheduled_hour'];

  // Call node_save() to make sure any handlers that use the
  // new workflow values will see them.
  node_save($node);

  $form_state['redirect'] = 'node/' . $node->nid;
}