<?php

/**
 * @file
 * Provides an Admin UI page for the Workflow Transition labels.
 *
 * Modify the workflow form items so specific workflow transitions
 * can have their own labels which the admin can describe relative
 * to the beginning and ending states.
 * Rather than showing the user a workflow box containing options like
 * "review required" as a state in the workflow, it could say "move to
 * the editing department for grammar review".
 *
 * AUTHOR
 * ------
 * David Kent Norman (http://deekayen.net/)
 *
 * Amazon Honor System donation:
 * http://zme.amazon.com/exec/varzea/pay/T2EOCSRRDQ9CL2
 *
 * Paypal donation:
 * https://www.paypal.com/us/cgi-bin/webscr?cmd=_xclick&business=paypal@deekayen.net&item_name=Drupal%20contribution&currency_code=USD&amount=20.00
 */

/**
 * Label edit form, where each fieldset represents a starting workflow state.
 *
 * Each contains the transitions with that starting workflow state.
 *
 * @return array
 *   Array of form items for editing labels on transitions.
 */
function workflow_admin_ui_labels_form($form, $form_state, $workflow, $op) {
  if (!is_object($workflow)) {
    drupal_set_message(t('Improper worklow ID provided.'), 'error');
    watchdog('workflow_named_transitions', 'Improper worklow ID provided.');
    drupal_goto('admin/config/workflow/workflow');
  }

  $headers = array(
    t('Transition from'),
    t('to'),
    t('label'),
  );

  $rows = array();
  $previous_from_sid = -1;
  // Get transitions, sorted by weight of the old state.
  $config_transitions = $workflow->getTransitions();
  foreach ($config_transitions as $transition) {
    $old_state = $transition->getOldState();
    $new_state = $transition->getNewState();
    $rows[] = array(
      'data' => array(
        array('data' => (($previous_from_sid != $transition->sid) ? $old_state->label() : '"')),
        array('data' => $new_state->label()),
        array(
          'data' => array(
            '#type' => 'textfield',
            '#value' => $transition->label(),
            '#size' => 60,
            '#maxlength' => 128,
            '#name' => 'label_' . $transition->tid,
            '#id' => 'label_' . $transition->tid,
          ),
        ),
      ),
    );
    $previous_from_sid = $transition->sid;
  }

  $form['transition_labels'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
  );

  // Save the transitions in the form to fetch upon submit.
  $form['#transitions'] = $config_transitions;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Automatic submission handler for the Transition labels form.
 *
 * @see workflow_admin_ui_labels_form()
 */
function workflow_admin_ui_labels_form_submit($form, &$form_state) {
  foreach ($form['#transitions'] as $config_transition) {
    $config_transition->label = trim($form_state['input']['label_' . $config_transition->tid]);
    $config_transition->save();
  }
  drupal_set_message(t('The transition labels have been saved.'));
}
