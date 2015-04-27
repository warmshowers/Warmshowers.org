<?php

/**
 * @file
 * Provides an Admin UI page for the Workflow States.
 */

/**
 * Menu callback.
 *
 * Creates the main workflow page, which gives an overview
 * of workflows and workflow states.
 * Replaced by http://drupal.org/node/1367530.
 */
function workflow_admin_ui_states_form($form, &$form_state, $workflow, $op) {
  // drupal_set_title(t('Workflow: @label', array('@label' => $workflow->label())));
  $form = array();
  $form['#tree'] = TRUE;
  $form['workflow'] = array(
    '#type' => 'value',
    '#value' => $workflow,
  );

  // Build select options for reassigning states.
  // We put a blank state first for validation.
  $state_list = array('' => ' ');
  $state_list += workflow_get_workflow_state_names($workflow->wid, $grouped = FALSE, $all = FALSE);
  // Is this the last state available?
  $form['#last_mohican'] = count($state_list) == 2;

  // Get the state objects as array ($sid => WorkflowState).
  $states = $workflow->getStates($all = TRUE);
  // Create a dummy object for new state item. It must NOT be saved to DB.
  $maxweight = $minweight = -50;
  // $wid = $workflow->wid;
  $dummy = $workflow->createState('', FALSE);
  $dummy->weight = $minweight;
  $states[$dummy->sid] = $dummy; // Although the index is 0, the state is appended at the end of the list.

  foreach ($states as $state) {
    // Allow modules to insert operations per state.
    $links = module_invoke_all('workflow_operations', 'state', $workflow, $state);

    // Make it impossible to reassign to the same state that is disabled.
    if ($state->isCreationState() || !$state->sid || !$state->isActive()) {
      $current_state = array();
      $current_state_list = array();
    }
    else {
      $current_state = array($state->sid => $state_list[$state->sid]);
      $current_state_list = array_diff($state_list, $current_state);
    }

    $form['states'][$state->sid] = array(
      'state' => array(
        '#type' => 'textfield',
        '#size' => 30,
        '#maxlength' => 255,
        '#default_value' => check_plain($state->label()),
      ),

      'name' => array(
        '#type' => 'machine_name',
        '#size' => 30,
        '#maxlength' => 255,
        '#required' => FALSE,
        // '#disabled' => !empty($name), // If needed this would disable updating machine name, once set.
        '#default_value' => $state->getName(),
        '#machine_name' => array(
          'exists' => 'workflow_admin_ui_states_validate_state_machine_name',
          'source' => array('states', $state->sid, 'state'),
          'replace_pattern' => '[^a-z0-9_()]+', // Added '()' characters from exclusion list since creation state has it.
        ),
      ),

      'weight' => array(
        '#type' => 'weight',
        '#delta' => 20,
        '#default_value' => $state->weight,
        '#title-display' => 'invisible',
        '#attributes' => array('class' => array('state-weight')),
      ),

      'status' => array(
        '#type' => 'checkbox',
        '#default_value' => $state->isActive(),
      ),

      // Save the original status for the validation handler.
      'orig_status' => array(
        '#type' => 'value',
        '#value' => $state->isActive(),
      ),

      'reassign' => array(
        '#type' => 'select',
        '#options' => $current_state_list,
      ),

      'count' => array(
        '#type' => 'value',
        '#value' => $state->count(),
      ),

      'ops' => array(
        '#type' => 'markup',
        '#markup' => theme('links', array('links' => $links)),
      ),
    );

    // Don't let the creation state change weight or status or name.
    if ($state->isCreationState()) {
      $form['states'][$state->sid]['weight']['#value'] = $minweight;
      $form['states'][$state->sid]['sysid']['#value'] = 1;
      $form['states'][$state->sid]['state']['#disabled'] = TRUE;
      $form['states'][$state->sid]['name']['#disabled'] = TRUE;
      $form['states'][$state->sid]['status']['#disabled'] = TRUE;
      $form['states'][$state->sid]['reassign']['#type'] = 'hidden';
      $form['states'][$state->sid]['reassign']['#disabled'] = TRUE;
    }
    // New state and disabled states cannot be reassigned.
    if (!$state->sid || !$state->isActive()) {
      $form['states'][$state->sid]['reassign']['#type'] = 'hidden';
      $form['states'][$state->sid]['reassign']['#disabled'] = TRUE;
    }
    // Disabled states cannot be renamed (and is a visual clue, too.).
    if (!$state->isActive()) {
      $form['states'][$state->sid]['state']['#disabled'] = TRUE;
    }
    // Set a proper weight to the new state.
    $maxweight = max($maxweight, $state->weight);
    if (!$state->sid) {
      $form['states'][$state->sid]['weight']['#default_value'] = $maxweight + 1;
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));

  return $form;
}

function theme_workflow_admin_ui_states_form($variables) {
  $form = $variables['form'];
  $output = '';
  $table_id = 'workflow_admin_ui_states';

  $table = array(
    'rows' => array(),
    'header' => array(
      t('State'),
      t('Weight'),
      t('Active'),
      t('Reassign'),
      t('Count'),
      array('data' => t('Operations'), 'class' => 'state-ops'),
    ),
    'attributes' => array('id' => $table_id, 'style' => 'width: auto;'),
  );

  // The output needs to have the action links at the top.
  $output .= drupal_render($form['action-links']);

  // Iterate over each element in our $form['states'] array.
  foreach (element_children($form['states']) as $id) {
    // We are now ready to add each element of our $form data to the rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $table['rows'][] = array(
      'data' => array(
        // Add our 'name' column.
        // array('data' => drupal_render($form['states'][$id]['state']), 'class' => 'state-name'),
        array('data' => drupal_render($form['states'][$id]['state']) . drupal_render($form['states'][$id]['name']), 'class' => 'state-name'),
        // Add our 'weight' column.
        drupal_render($form['states'][$id]['weight']),
        // Add our 'status' column.
        array('data' => drupal_render($form['states'][$id]['status']), 'class' => 'state-status'),
        // Add our 'reassign' column.
        array('data' => drupal_render($form['states'][$id]['reassign']), 'class' => 'state-reassign'),
        // Add our 'count' column.
        array('data' => $form['states'][$id]['count']['#value'], 'class' => 'state-count'),
        // Add our 'operations' column.
        array('data' => drupal_render($form['states'][$id]['ops']), 'class' => 'state-ops'),
      ),
      // To support the tabledrag behavior, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => array('draggable'),
    );
  }

  $output .= theme('table', $table);

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js goodness onto our page.
  //
  // For a basic sortable table, we need to pass it:
  // - the $table_id of our <table> element,
  // - the $action to be performed on our form items ('order'),
  // - a string describing where $action should be applied ('siblings'),
  // - and the class of the element containing our 'weight' element.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'state-weight');

  return $output;
}

/**
 * Validation handler for the state form.
 */
function workflow_admin_ui_states_form_validate($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['states'] as $sid => $item) {
    // Reload $state from db, in case the states were changed by anyone else. And it is just as fast.
    $state = workflow_state_load_single($sid);

    // Does user want to deactivate the state (reassign current nodes)?
    if ($sid > 0 && $item['status'] == 0 && $state->isActive()) {
      $args = array('%state' => $state->getName()); // check_plain() is run by t().

      // Does that state have nodes in it?
      if ($item['count'] > 0 && empty($item['reassign'])) {
        if ($form['#last_mohican']) {
          $message = 'Since you are deleting the last available workflow state
            in this workflow, all content items which are in that state will have their
            workflow state removed.';
          drupal_set_message(t($message, $args), 'warning');
        }
        else {
          $message = 'The %state state has content; you must reassign the content to another state.';
          form_set_error("states'][$sid]['reassign'", t($message, $args));
        }
      }
      if (module_exists('workflowfield')) {
        // @todo: Reassign states for Workflow Field.
        $message = 'Deactivating state %state for does not reassign Fields of type Workflow with this status.';
        drupal_set_message(t($message, $args), 'warning');
      }
    }
  }
}

/**
 * Submission handler for the state form.
 */
function workflow_admin_ui_states_form_submit($form, &$form_state) {
  // Get the workflow id, then save it for the next round.
  $workflow = $form_state['values']['workflow'];
  $wid = $workflow->wid;

  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['states'] as $sid => $item) {
    $item['sid'] = $sid;
    $item['wid'] = $wid;

    // Is there not a new state name?
    if (empty($item['state'])) {
      // No new state entered, so skip it.
      continue;
    }

    // Reload $state from db, in case the states were changed by anyone else. And it is just as fast.
    $state = ($sid) ? workflow_state_load_single($sid) : $workflow->createState('');

    // Does user want to deactivate the state (reassign current nodes)?
    if ($sid > 0 && $item['status'] == 0 && $state->isActive()) {
      $new_sid = $item['reassign'];
      $new_state = workflow_state_load_single($new_sid);

      $args = array(
        '%workflow' => $workflow->getName(), // check_plain() is run by t().
        '%old_state' => $state->getName(),
        '%new_state' => isset($new_state) ? $new_state->getName() : '',
      );

      if ($item['count'] > 0) {
        if ($form['#last_mohican']) {
          $new_sid = NULL; // Do not reassign to new state.
          $message = 'Removing workflow states from content in the %workflow.';
          drupal_set_message(t($message, $args));
        }
        else {
          // Prepare the state delete function.
          $message = 'Reassigning content from %old_state to %new_state.';
          drupal_set_message(t($message, $args));
        }
      }
      // Delete the old state without orphaning nodes, move them to the new state.
      $state->deactivate($new_sid);

      $message = 'Deactivated workflow state %old_state in %workflow.';
      watchdog('workflow', $message, $args);
      drupal_set_message(t($message, $args));
    }

    $state->state = $item['state'];
    $state->name = $item['name'];
    $state->status = $item['status'];
    $state->weight = $item['weight'];
    $state->save();
  }

  drupal_set_message(t('The workflow was updated.'));
  // $form_state['redirect'] = 'admin/config/workflow/workflow';
}

/**
 * Validate duplicate machine names. Function registered in 'name' form element.
 */
function workflow_admin_ui_states_validate_state_machine_name($name, $element, $form_state) {
  // @todo: Should $name be checked against DB?
  $state_names = array();
  foreach ($form_state['values']['states'] as $sid => $item) {
    $state_names[] = $item['name'];
  }

  $state_names = array_map('strtolower', $state_names);
  $result = array_unique(array_diff_assoc($state_names, array_unique($state_names)));

  if (in_array($name, $result)) {
    return TRUE;
  }
  return FALSE;
}
