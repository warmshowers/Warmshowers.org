<?php

/**
 * @file
 * Conditional actions overview UI.
 */

/**
 * Display the administration page that lets you add and modify predicates.
 */
function ca_admin($groupby = 'trigger') {
  drupal_add_css(drupal_get_path('module', 'ca') .'/ca.css');

  // Load all the module defined predicates into a temporary array.
  $temp = module_invoke_all('ca_predicate');
  drupal_alter('ca_predicate', $temp);

  // Assign a default weight if need be.
  foreach ($temp as $key => $value) {
    $temp[$key]['#pid'] = $key;
    if (!isset($value['#weight'])) {
      $temp[$key]['#weight'] = 0;
    }
  }

  // Load and loop through all the database stored predicates.
  $result = db_query("SELECT * FROM {ca_predicates}");
  while ($predicate = db_fetch_array($result)) {
    // Prepare the database data into a predicate.
    $predicate = ca_prepare_db_predicate($predicate);

    // Overrides the module defined predicate if it's been modified by a user.
    $temp[$predicate['#pid']] = $predicate;
  }

  usort($temp, 'ca_weight_sort');

  // Copy the temporary array of predicates into a keyed array.
  $predicates = array();
  foreach ($temp as $predicate) {
    $predicates[$predicate['#pid']] = $predicate;
  }

  // Load up the trigger data so we can display them by title.
  $triggers = module_invoke_all('ca_trigger');

  // Build the header for the predicate tables based on the grouping type.
  if ($groupby == 'trigger') {
    $header = array(
      array('data' => t('Title'), 'class' => 'col-title'),
      t('Class'),
      t('Status'),
      t('Weight'),
      t('Operations'),
    );

    $table_class = 'ca-predicate-trigger';
    $table_label = '<strong>'. t('Trigger') .':</strong> ';
  }
  elseif ($groupby == 'class') {
    $header = array(
      array('data' => t('Title'), 'class' => 'col-title'),
      t('Trigger'),
      t('Status'),
      t('Weight'),
      t('Operations'),
    );

    $table_class = 'ca-predicate-class';
    $table_label = '<strong>'. t('Class') .':</strong> ';
  }

  $rows = array();

  foreach ($predicates as $key => $value) {
    // Build the basic operation links for each predicate.
    $ops = array(
      l(t('edit'), CA_UI_PATH .'/'. $key .'/edit'),
    );

    // Add the reset link if a module defined predicate has been modified.
    if (!is_numeric($key) && isset($value['#modified'])) {
      $ops[] = l(t('reset'), CA_UI_PATH .'/'. $key .'/reset');
    }

    // Add a delete link if displaying a custom predicate.
    if (is_numeric($key)) {
      $ops[] = l(t('delete'), CA_UI_PATH .'/'. $key .'/delete');
    }

    // Add the predicate's row to the table based on the grouping type.
    if ($groupby == 'trigger') {
      $tables[$triggers[$value['#trigger']]['#title']]['rows'][] = array(
        check_plain($value['#title']),
        strpos($value['#class'], 'custom') === 0 ? check_plain(substr($value['#class'], 7)) : $value['#class'],
        $value['#status'] == 0 ? t('Disabled') : t('Enabled'),
        array('data' => $value['#weight'], 'class' => 'ca-predicate-table-weight'),
        array('data' => implode(' ', $ops), 'class' => 'ca-predicate-table-ops'),
      );
    }
    elseif ($groupby == 'class') {
      $class = strpos($value['#class'], 'custom') === 0 ? check_plain(substr($value['#class'], 7)) : $value['#class'];

      $tables[$class]['rows'][] = array(
        check_plain($value['#title']),
        check_plain($triggers[$value['#trigger']]['#title']),
        $value['#status'] == 0 ? t('Disabled') : t('Enabled'),
        array('data' => $value['#weight'], 'class' => 'ca-predicate-table-weight'),
        array('data' => implode(' ', $ops), 'class' => 'ca-predicate-table-ops'),
      );
    }
  }

  // Put the tables in alphabetical order.
  ksort($tables);

  // Add the tables for each trigger to the output.
  $output = '';
  foreach ($tables as $key => $value) {
    // Accommodate empty class names.
    if (empty($key)) {
      $key = t('- None -');
    }

    $output .= theme('table', $header, $value['rows'], array('class' => $table_class), $table_label . check_plain($key));
  }

  $output .= l(t('Add a predicate'), CA_UI_PATH .'/add');

  return $output;
}

/**
 * Form to allow the creation and editing of conditional action predicates.
 */
function ca_predicate_meta_form($form_state, $pid) {
  $predicate = array();

  // Load the predicate if an ID is passed in.
  if (!empty($pid) && $pid !== 0) {
    $predicate = ca_load_predicate($pid);

    // Fail out if we didn't find a predicate for that ID.
    if (empty($predicate)) {
      drupal_set_message(t('That predicate does not exist.'), 'error');
      drupal_goto(CA_UI_PATH);
    }

    drupal_set_title($predicate['#title']);
  }

  $form['predicate_pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  $form['predicate_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Enter a title used for display on the overview tables.'),
    '#default_value' => $pid ? $predicate['#title'] : '',
    '#required' => TRUE,
  );

  // Create an array of triggers for the select box.
  $triggers = module_invoke_all('ca_trigger');
  foreach ($triggers as $key => $value) {
    $options[$value['#category']][$key] = $value['#title'];
  }

  $form['predicate_trigger'] = array(
    '#type' => 'select',
    '#title' => t('Trigger'),
    '#description' => t('Select the trigger for this predicate.<br />Cannot be modified if the predicate has conditions or actions.'),
    '#options' => $options,
    '#default_value' => $pid ? $predicate['#trigger'] : '',
    '#disabled' => empty($predicate['#conditions']) && empty($predicate['#actions']) ? FALSE : TRUE,
    '#required' => TRUE,
  );

  $form['predicate_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('Enter a description that summarizes the use and intent of the predicate.'),
    '#default_value' => $pid ?
                        (isset($predicate['#description']) ? $predicate['#description'] : '') :
                        '',
  );

  // Accommodate the mandatory custom prefix for predicate classes.
  $class = isset($predicate['#class']) ? $predicate['#class'] : '';
  if (strpos($class, 'custom') === 0) {
    $class = ltrim(substr($class, 6), ':');
  }

  if (is_numeric($pid)) {
    $form['predicate_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Class'),
      '#description' => t('Classes let you categorize your predicates based on the type of functionality they provide.'),
      '#field_prefix' => 'custom:',
      '#default_value' => $class,
    );
  }
  else {
    $form['predicate_class'] = array(
      '#type' => 'value',
      '#value' => $class,
    );
  }

  $form['predicate_status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled predicates will not be processed when their trigger is pulled.'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => $pid ? $predicate['#status'] : 1,
  );

  $form['predicate_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('Predicates will be sorted by weight and processed sequentially.'),
    '#default_value' => ($pid && isset($predicate['#weight'])) ? $predicate['#weight'] : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save predicate'),
    '#suffix' => l(t('Cancel'), CA_UI_PATH),
  );

  return $form;
}

/**
 * @see ca_predicate_meta_form()
 */
function ca_predicate_meta_form_submit($form, &$form_state) {
  $form_state['redirect'] = CA_UI_PATH;
  $save = FALSE;

  // Load the original predicate, if any.
  $predicate = array();
  if ($form_state['values']['predicate_pid'] !== 0) {
    $predicate = ca_load_predicate($form_state['values']['predicate_pid']);
    $predicate['#pid'] = $form_state['values']['predicate_pid'];
  }

  // Setup a list of fields to check for and apply changes.
  $fields = array('title', 'trigger', 'description', 'class', 'status', 'weight');

  // Compare the values from the form submission with what is already set.
  foreach ($fields as $field) {
    if (!isset($predicate['#'. $field]) || $form_state['values']['predicate_'. $field] != $predicate['#'. $field]) {
      $predicate['#'. $field] = $form_state['values']['predicate_'. $field];
      $save = TRUE;
    }
  }

  // Add empty conditions and actions arrays if this is a new predicate.
  if (empty($predicate['#pid'])) {
    $predicate['#pid'] = variable_get('ca_predicates_pid', 1);
    variable_set('ca_predicates_pid', $predicate['#pid'] + 1);
    $predicate['#conditions'] = array();
    $predicate['#actions'] = array();

    // For new predicates, redirect to the conditions tab.
    $form_state['redirect'] = CA_UI_PATH .'/'. $predicate['#pid'] .'/edit/conditions';
  }

  // Check to see if any changes were made and save if necessary.
  if ($save) {
    ca_save_predicate($predicate);
  }

  drupal_set_message(t('Predicate meta data saved.'));
}

/**
 * Form to reset a modified module defined predicate to its original state.
 */
function ca_predicate_delete_form($form_state, $pid) {
  $predicate = ca_load_predicate($pid);

  // Fail if we received an invalid predicate ID.
  if (empty($predicate)) {
    drupal_set_message(t('That predicate does not exist.'), 'error');
    drupal_goto(CA_UI_PATH);
  }

  $form['predicate_pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  $form['predicate_title'] = array(
    '#type' => 'value',
    '#value' => $predicate['#title'],
  );

  $description = '<p><strong>'. check_plain($predicate['#title']) .'</strong><br />'
               . check_plain($predicate['#description']) .'</p><p>'
                . t('This action cannot be undone.') .'</p>';

  return confirm_form($form, t('Are you sure you want to !op this predicate?', array('!op' => is_numeric($pid) ? t('delete') : t('reset'))), CA_UI_PATH, $description);
}

/**
 * @see ca_predicate_delete_form()
 */
function ca_predicate_delete_form_submit($form, &$form_state) {
  ca_delete_predicate($form_state['values']['predicate_pid']);

  drupal_set_message(t('Predicate %title !op.', array('%title' => $form_state['values']['predicate_title'], '!op' => is_numeric($form_state['values']['predicate_pid']) ? t('deleted') : t('reset'))));

  $form_state['redirect'] = CA_UI_PATH;
}

/**
 * Build a form for adding and editing actions on a predicate.
 *
 * @param $form_state
 *   Used by FAPI; the current state of the form as it processes.
 * @param $pid
 *   The ID of the predicate whose actions are being modified.
 * @param $title
 *   TRUE or FALSE to specify whether or not to set the predicate's title to be
 *     the title of the page.
 * @return
 *   The form array for the actions form.
 *
 * @ingroup forms
 * @see
 *   ca_actions_form_add_action_submit()
 *   ca_actions_form_add_action_submit()
 *   ca_actions_form_save_changes_submit()
 */
function ca_actions_form($form_state, $pid, $title = TRUE) {
  // Locate the specified predicate.
  $predicate = ca_load_predicate($pid);

  // Return an empty form if the predicate does not exist.
  if (empty($predicate)) {
    return array();
  }

  // Include the JS for conditional actions forms.
  drupal_add_js(drupal_get_path('module', 'ca') .'/ca.js');

  // Display the title if appropriate.
  if ($title) {
    drupal_set_title($predicate['#title']);
  }

  // Load up any actions that could possibly be on this predicate.
  $action_data = ca_load_action();

  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  $form['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Actions'),
    '#description' => t('These actions will be performed in order when this predicate passes the conditions evaluation.'),
    '#tree' => TRUE,
  );

  // Loop through the actions and add them to the form if valid.
  $i = 0;

  foreach ($predicate['#actions'] as $key => $action) {
    // Add it to the form if the action's callback exists.
    $callback = isset($action_data[$action['#name']]['#callback']) ? $action_data[$action['#name']]['#callback'] : '';

    if (function_exists($callback)) {
      $form['actions'][$i] = array(
        '#type' => 'fieldset',
        '#title' => t('Action: @title', array('@title' => $action_data[$action['#name']]['#title'])),
        '#description' => isset($action_data[$action['#name']]['#description']) ?  $action_data[$action['#name']]['#description'] : '',
        '#collapsible' => TRUE,
        '#collapsed' => isset($_SESSION['ca_action_focus']) && $i == $_SESSION['ca_action_focus'] ? FALSE : TRUE,
      );

      $form['actions'][$i]['name'] = array(
        '#type' => 'value',
        '#value' => $action['#name'],
      );

      $form['actions'][$i]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => $action['#title'],
      );

      $form['actions'][$i]['argument_map'] = array(
        '#type' => 'fieldset',
        '#title' => t('Arguments'),
        '#description' => t('Some triggers pass in multiple options for arguments related to a particular action, so you must specify which options to use in the fields below.'),
        '#collapsible' => TRUE,
      );

      // Setup a variable to decide if we can collapse the arguments fieldset.
      $collapsed = TRUE;

      foreach ($action_data[$action['#name']]['#arguments'] as $key => $value) {
        // Load the available arguments for each entity as received by the
        // trigger when it was pulled.
        $options = ca_load_trigger_arguments($predicate['#trigger'], $value['#entity']);

        // If we have more than one option for any argument, do not collapse.
        if (count($options) > 1) {
          $collapsed = FALSE;
        }

        $form['actions'][$i]['argument_map'][$key] = array(
          '#type' => 'select',
          '#title' => check_plain($value['#title']),
          '#options' => $options,
          '#default_value' => $action['#argument_map'][$key],
        );
      }

      $form['actions'][$i]['argument_map']['#collapsed'] = $collapsed;

      // Add the action's form elements if any exist.
      $callback .= '_form';

      if (function_exists($callback)) {
        // Load the trigger data.
        $trigger_data = ca_load_trigger($predicate['#trigger']);

        $form['actions'][$i]['settings'] = $callback(array(), $action['#settings'], $trigger_data['#arguments']);
      }

      $form['actions'][$i]['remove'] = array(
        '#type' => 'submit',
        '#value' => t('Remove this action'),
        '#name' => 'ca_remove_action_'. $i,
        '#submit' => array('ca_actions_form_remove_action_submit'),
        '#attributes' => array('class' => 'ca-remove-confirm'),
      );

      $i++;
    }
  }

  // Unset the session variable used to focus on the new action.
  unset($_SESSION['ca_action_focus']);

  $form['actions']['add_action'] = array(
    '#type' => 'select',
    '#title' => t('Available actions'),
    '#options' => ca_load_trigger_actions($predicate['#trigger']),
  );

  $form['actions']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add action'),
    '#submit' => array('ca_actions_form_add_action_submit'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#submit' => array('ca_actions_form_save_changes_submit'),
  );

  filter_configure_form($form);

  return $form;
}

/**
 * Remove action submit handler.
 *
 * @see ca_actions_form()
 */
function ca_actions_form_remove_action_submit($form, &$form_state) {
  // Save the existing actions to preserve any changes.
  ca_actions_form_update_actions($form_state['values']['pid'], $form_state['values']['actions']);

  // Remove the appropriate action from the predicate and save it.
  ca_remove_action($form_state['values']['pid'], $form_state['clicked_button']['#parents'][1]);

  drupal_set_message(t('Action removed.'));
}

/**
 * Add action submit handler.
 *
 * @see ca_actions_form()
 */
function ca_actions_form_add_action_submit($form, &$form_state) {
  // Save the existing actions to preserve any changes.
  $predicate = ca_actions_form_update_actions($form_state['values']['pid'], $form_state['values']['actions']);

  // Store the presumed key of the new action.
  $_SESSION['ca_action_focus'] = count($predicate['#actions']);

  // Add the specified action to the predicate.
  ca_add_action($form_state['values']['pid'], $form_state['values']['actions']['add_action']);

  drupal_set_message(t('Action added.'));
}

/**
 * Save changes submit handler for the actions form.
 *
 * @see ca_actions_form()
 */
function ca_actions_form_save_changes_submit($form, &$form_state) {
  // Save the existing actions to preserve any changes.
  ca_actions_form_update_actions($form_state['values']['pid'], $form_state['values']['actions']);

  drupal_set_message(t('Actions saved.'));
}

/**
 * Updates a predicate's actions based on the values from an actions form.
 *
 * @param $pid
 *   The ID of the predicate whose actions should be updated.
 * @param $data
 *   The actions values from the form state that should be used to update the
 *     predicate; normally $form_state['values']['actions'].
 * @return
 *   An array representing the full, updated predicate.
 */
function ca_actions_form_update_actions($pid, $data) {
  $actions = array();

  // Unset top level components we don't want to get in the way.
  unset($data['add_action'], $data['add']);

  // Loop through the actions from the form and add them to our temporary array.
  foreach ((array) $data as $key => $value) {
    $actions[] = array(
      '#name' => $value['name'],
      '#title' => $value['title'],
      '#argument_map' => $value['argument_map'],
      '#settings' => empty($value['settings']) ? array() : $value['settings'],
    );
  }

  // Load the predicate as it is now.
  $predicate = ca_load_predicate($pid);

  // Update the actions and save the result.
  $predicate['#actions'] = $actions;
  ca_save_predicate($predicate);

  return $predicate;
}

/**
 * Build a form for adding and editing conditions on a predicate.
 *
 * @ingroup forms
 * @see
 *   ca_conditions_form_add_condition_group_submit()
 *   ca_conditions_form_remove_condition_group_submit()
 *   ca_conditions_form_add_condition_submit()
 *   ca_conditions_form_remove_condition_submit()
 *   ca_conditions_form_save_changes_submit()
 */
function ca_conditions_form($form_state, $pid, $title = TRUE) {
  // Locate the specified predicate.
  $predicate = ca_load_predicate($pid);

  // Return an empty form if the predicate does not exist.
  if (empty($predicate)) {
    return array();
  }

  // Include the JS for conditional actions forms.
  drupal_add_js(drupal_get_path('module', 'ca') .'/ca.js');

  // Display the title if appropriate.
  if ($title) {
    drupal_set_title($predicate['#title']);
  }

  // Initialize the conditions available for this predicate.
  ca_load_trigger_conditions($predicate['#trigger']);

  // Add the predicate ID to the form array.
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  // If the predicate conditions array isn't already a container of containers,
  // make it so.  i.e. group => array('group' => array('cond', 'cond'))
  if (empty($predicate['#conditions'])) {
    $predicate['#conditions'] = ca_new_conditions();
  }
  else {
    $key = array_shift(array_keys($predicate['#conditions']['#conditions']));

    if (empty($predicate['#conditions']['#conditions'][$key]['#operator'])) {
      $predicate['#conditions'] = array(
        '#operator' => 'AND',
        '#conditions' => array(
          $predicate['#conditions'],
        ),
      );
    }
  }

  // Recurse through the predicate's conditions to build the form.
  $form['conditions'] = _ca_conditions_form_tree($predicate['#conditions'], $predicate['#trigger']);
  $form['conditions'] = $form['conditions'][0];

  // Always remove the top level condition group controls.
  unset($form['conditions']['condition']);
  unset($form['conditions']['add_condition']);
  unset($form['conditions']['remove_condition_group']);

  // If there are more than one top level condition groups...
  if (count($form['conditions']['conditions']) > 1) {
    // Update the top level fieldset.
    $form['conditions']['#title'] = t('Condition groups');
    $form['conditions']['#collapsible'] = FALSE;

    // Adjust the titles of the operator options to be more explicit.
    $form['conditions']['operator']['#options'] = array(
      'AND' => t('AND. If all of these condition groups are TRUE.'),
      'OR' => t('OR. If any of these condition groups are TRUE.'),
    );
  }
  else {
    // Otherwise remove the top level fieldset and operator.
    unset($form['conditions']['#type']);
    unset($form['conditions']['operator']);

    // Adjust the second level conditions group to not collapse.
    $form['conditions']['conditions'][0]['#collapsible'] = FALSE;
  }

  // Set the conditions array to a tree so the submitted form values mirror the
  // structure of a predicate's #conditions array.
  $form['conditions']['#tree'] = TRUE;

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#submit' => array('ca_conditions_form_save_changes_submit'),
  );

  $form['add_condition_group'] = array(
    '#type' => 'submit',
    '#value' => t('Add condition group'),
    '#submit' => array('ca_conditions_form_add_condition_group_submit'),
  );

  filter_configure_form($form);

  return $form;
}

/**
 * Recursively add logical groups to the conditions form as fieldsets.
 *
 * @param $condition
 *   An array consisting of an #operator and another array of #conditions, or
 *   a condition array with data pertaining to its callback function.
 * @param $trigger
 *   The trigger name and data concerning the arguments that are passed in to
 *   the condition.
 * @return
 *   A form array representing the tree of conditions.
 */
function _ca_conditions_form_tree($condition, $trigger) {
  // We need an index so condition group fieldsets have unique keys.
  static $index = 0;
  $form = array();

  // If we're dealing with a conditions group and not a single condition.
  if (isset($condition['#operator']) && is_array($condition['#conditions'])) {
    $level = $index;

    $form[$level] = array(
      '#type' => 'fieldset',
      '#title' => t('Conditions group'),
      '#collapsible' => TRUE,
    );

    // Add an operator radio select for the group.
    $form[$level]['operator'] = array(
      '#type' => 'radios',
      '#title' => t('Operator'),
      '#options' => array(
        'AND' => t('AND. If all of these conditions are TRUE.'),
        'OR' => t('OR. If any of these conditions are TRUE.'),
      ),
      '#default_value' => $condition['#operator'],
    );

    $form[$level]['conditions'] = array();

    // For each condition in #conditions, add a condition fieldset to the form.
    foreach ($condition['#conditions'] as $sub_condition) {
      $result = _ca_conditions_form_tree($sub_condition, $trigger);

      if (is_array($result)) {
        $form[$level]['conditions'] = array_merge($form[$level]['conditions'], $result);
      }
    }

    $form[$level]['condition'] = array(
      '#type' => 'select',
      '#title' => t('Available conditions'),
      '#options' => ca_load_trigger_conditions(),
    );
    $form[$level]['add_condition'] = array(
      '#type' => 'submit',
      '#value' => t('Add condition'),
      '#submit' => array('ca_conditions_form_add_condition_submit'),
      '#name' => 'add_condition_'. $level,
    );
    $form[$level]['remove_condition_group'] = array(
      '#type' => 'submit',
      '#value' => t('Remove group'),
      '#submit' => array('ca_conditions_form_remove_condition_group_submit'),
      '#attributes' => array('class' => 'ca-remove-confirm'),
      '#name' => 'remove_condition_group_'. $level,
    );

    $index++;

    return $form;
  }
  elseif (!empty($condition)) {
    // Load the data for the conditions as defined by modules.
    $condition_data = ca_load_condition();

    // Otherwise add a fieldset for the individual condition.
    $form[0] = array(
      '#type' => 'fieldset',
      '#title' => t('Condition: @title', array('@title' => $condition_data[$condition['#name']]['#title'])),
      '#description' => t('@description', array('@description' => $condition_data[$condition['#name']]['#description'])),
      '#collapsible' => TRUE,
      '#collapsed' => isset($condition['#expanded']) ? FALSE : TRUE,
    );

    // Add form elements to the fieldset to adjust the condition's settings.
    $form[0] += _ca_conditions_form_condition($condition, $trigger);

    return $form;
  }
}

/**
 * Add a single condition's fieldset to the conditions form.
 *
 * @param $condition
 *   The condition data array.
 * @param $trigger
 *   The trigger name and data concerning the arguments that are passed in to
 *   the condition.
 * @return
 *   A form array representing the condition.
 */
function _ca_conditions_form_condition($condition, $trigger) {
  static $identifier = 0;

  // Load the data for the conditions as defined by modules.
  $condition_data = ca_load_condition();

  // Condition name is a hard reference to the condition and so not editable.
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $condition['#name'],
  );

  // The title for this particular instance of this condition.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $condition['#title'],
  );

  $form['argument_map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Arguments'),
    '#description' => t('Some triggers pass in multiple options for arguments related to a particular condition, so you must specify which options to use in the fields below.'),
    '#collapsible' => TRUE,
  );

  // Setup a variable to decide if we can collapse the arguments fieldset.
  $collapsed = TRUE;

  // Cast to an array to accommodate conditions that need no arguments.
  foreach ((array) $condition_data[$condition['#name']]['#arguments'] as $key => $value) {
    // Load the available arguments for each entity as received by the
    // trigger when it was pulled.
    $options = ca_load_trigger_arguments($trigger, $value['#entity']);

    // If we have more than one option for any argument, do not collapse.
    if (count($options) > 1) {
      $collapsed = FALSE;
    }

    $form['argument_map'][$key] = array(
      '#type' => 'select',
      '#title' => check_plain($value['#title']),
      '#options' => $options,
      '#default_value' => $condition['#argument_map'][$key],
    );
  }

  $form['argument_map']['#collapsed'] = $collapsed;

  // Whether or not to negate the result of this condition.
  $form['settings']['negate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Negate this condition.'),
    '#description' => t('Return FALSE if the condition is TRUE and vice versa.'),
    '#default_value' => $condition['#settings']['negate'],
  );

  // Get the callback for the condition we're displaying.
  $callback = $condition_data[$condition['#name']]['#callback'] .'_form';

  if (function_exists($callback)) {
   // Load the trigger data.
   $trigger_data = ca_load_trigger($trigger);

  // Add the condition's form elements to the fieldset.
    $form['settings'] += $callback(array(), $condition['#settings'], $trigger_data['#arguments']);
  }

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove this condition'),
    '#submit' => array('ca_conditions_form_remove_condition_submit'),
    '#attributes' => array('class' => 'ca-remove-confirm'),
    '#name' => 'remove_condition_group_'. $identifier++,
  );

  return $form;
}

/**
 * Submit handler for button to add a condition group.
 *
 * @see ca_conditions_form()
 */
function ca_conditions_form_add_condition_group_submit($form, &$form_state) {
  // Save the existing conditions to preserve any changes.
  ca_conditions_form_update_conditions($form_state['values']['pid'], $form_state['values']['conditions']);

  // Add the condition group to the predicate.
  ca_add_condition_group($form_state['values']['pid']);

  // Display a confirmation message.
  drupal_set_message(t('Condition group added.'));
}

/**
 * Submit handler for button to remove a condition grou.
 *
 * @see ca_conditions_form()
 */
function ca_conditions_form_remove_condition_group_submit($form, &$form_state) {
  $group_key = FALSE;

  // Loop through the array keys of the conditions group.
  foreach (array_keys($form['conditions']['conditions']) as $key => $value) {
    // If we find the key we're looking for...
    if (is_numeric($value) && $value == $form_state['clicked_button']['#array_parents'][2]) {
      // Store its position as the new group key once the conditions are saved.
      // This is necessary because the save function will truncate any holes so
      // the keys are in numerical order starting with 0.
      $group_key = $key;
    }
  }

  // Save the existing conditions to preserve any changes.
  ca_conditions_form_update_conditions($form_state['values']['pid'], $form_state['values']['conditions']);

  // Fail if we didn't find a new group key for some reason.
  if ($group_key === FALSE) {
    drupal_set_message(t('An error occurred when trying to add the condition.  Please try again.'), 'error');
  }
  else {
    // Remove the condition group from the predicate.
    ca_remove_condition_group($form_state['values']['pid'], $group_key);

    // Display a confirmation message.
    drupal_set_message(t('Condition group removed.'));
  }
}

/**
 * Submit handler for button to add a condition to a condition group.
 *
 * @see ca_conditions_form()
 */
function ca_conditions_form_add_condition_submit($form, &$form_state) {
  $group_key = FALSE;

  // Loop through the array keys of the conditions group.
  foreach (array_keys($form['conditions']['conditions']) as $key => $value) {
    // If we find the key we're looking for...
    if (is_numeric($value) && $value == $form_state['clicked_button']['#array_parents'][2]) {
      // Store its position as the new group key once the conditions are saved.
      // This is necessary because the save function will truncate any holes so
      // the keys are in numerical order starting with 0.
      $group_key = $key;
    }
  }

  // Save the existing conditions to preserve any changes.
  ca_conditions_form_update_conditions($form_state['values']['pid'], $form_state['values']['conditions']);

  // Fail if we didn't find a new group key for some reason.
  if ($group_key === FALSE) {
    drupal_set_message(t('An error occurred when trying to add the condition.  Please try again.'), 'error');
  }
  else {
    // Otherwise add the condition to the specified group.
    $name = $form_state['values']['conditions']['conditions'][$group_key]['condition'];

    ca_add_condition($form_state['values']['pid'], $name, $group_key);

    $condition = ca_load_condition($name);

    drupal_set_message(t('%title condition added.', array('%title' => $condition['#title'])));
  }
}

/**
 * Submit handler for button to remove a condition from a condition group.
 *
 * @see ca_conditions_form()
 */
function ca_conditions_form_remove_condition_submit($form, &$form_state) {
  $group_key = FALSE;
  $cond_key = FALSE;

  // Loop through the array keys of the conditions group.
  foreach (array_keys($form['conditions']['conditions']) as $key => $value) {
    // If we find the key we're looking for...
    if (is_numeric($value) && $value == $form_state['clicked_button']['#array_parents'][2]) {
      // Store its position as the new group key once the conditions are saved.
      // This is necessary because the save function will truncate any holes so
      // the keys are in numerical order starting with 0.
      $group_key = $key;
    }
  }

  // Loop through the array keys of the conditions in the group.
  foreach (array_keys($form['conditions']['conditions'][$form_state['clicked_button']['#array_parents'][2]]['conditions']) as $key => $value) {
    // If we find the key we're looking for...
    if (is_numeric($value) && $value == $form_state['clicked_button']['#array_parents'][4]) {
      // Store its position as the new condition key once the conditions are
      // saved. This is necessary because the save function will truncate any
      // holes so the keys are in numerical order starting with 0.
      $cond_key = $key;
    }
  }

  // Save the existing conditions to preserve any changes.
  ca_conditions_form_update_conditions($form_state['values']['pid'], $form_state['values']['conditions']);

  // Fail if we didn't find a new group key for some reason.
  if ($group_key === FALSE || $cond_key === FALSE) {
    drupal_set_message(t('An error occurred when trying to remove the condition.  Please try again.'), 'error');
  }
  else {
    // Otherwise remove the condition from the specified group.
    ca_remove_condition($form_state['values']['pid'], $group_key, $cond_key);

    drupal_set_message(t('Condition removed.'));
  }
}

/**
 * Save changes submit handler for the conditions form.
 *
 * @see ca_conditions_form()
 */
function ca_conditions_form_save_changes_submit($form, &$form_state) {
  // Save the existing conditions to preserve any changes.
  ca_conditions_form_update_conditions($form_state['values']['pid'], $form_state['values']['conditions']);

  drupal_set_message(t('Conditions saved.'));
}

/**
 * Update a predicate's conditions based on the values from a conditions form.
 *
 * @param $pid
 *   The ID of the predicate whose conditions should be updated.
 * @param $data
 *   The conditions values from the form state that should be used to update the
 *     predicate; normally $form_state['values']['conditions'].
 * @return
 *   An array representing the full, updated predicate.
 */
function ca_conditions_form_update_conditions($pid, $data) {
  // Build the new conditions array from scratch.
  $conditions = ca_new_conditions();

  // Override the top level operator if need be.
  if (isset($data['operator'])) {
    $conditions['#operator'] = $data['operator'];
  }

  // Use a variable to track the group array key so we can "reset" the keys.
  $group = 0;

  // Loop through each second level condition group.
  foreach ($data['conditions'] as $key => $value) {
    // Save the operator setting.
    $conditions['#conditions'][$group]['#operator'] = $value['operator'];
    $conditions['#conditions'][$group]['#conditions'] = array();

    // If conditions exist...
    if (is_array($value['conditions']) && count($value['conditions']) > 0) {
      // Use a variable to track the condition array key as for groups.
      $condition = 0;

      // Loop through the conditions in this group.
      foreach ($value['conditions'] as $cond_key => $cond_value) {
        // Save the condition's settings.
        $conditions['#conditions'][$group]['#conditions'][$condition] = array(
          '#name' => $cond_value['name'],
          '#title' => $cond_value['title'],
          '#argument_map' => $cond_value['argument_map'],
          '#settings' => $cond_value['settings'],
        );

        $condition++;
      }
    }

    $group++;
  }

  // Load the predicate as it is now.
  $predicate = ca_load_predicate($pid);

  // Update the actions and save the result.
  $predicate['#conditions'] = $conditions;
  ca_save_predicate($predicate);

  return $predicate;
}

/**
 * Landing page for converting Workflow-ng configurations.
 *
 * From this page, the user can begin the batch processing of any Workflow-ng
 * configurations in the site's database into Conditional Actions predicates.
 *
 * @ingroup forms
 * @see ca_conversion_form_submit()
 */
function ca_conversion_form() {
  $form = array();

  $form['help'] = array(
    '#value' => '<div>'. t('Use this form during the update process from Ubercart 1.0 to 2.0 to convert your Workflow-ng configurations to Conditional Actions predicates.  Once your configurations are converted, you should complete your uninstallation of Workflow-ng.') .'</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Convert configurations'),
  );

  return $form;
}

/**
 * @see ca_conversion_form_submit()
 */
function ca_conversion_form_submit($form, &$form_state) {
  // Convert workflow-ng configurations in a batch process because there may be
  // a lot of them.
  $batch = array(
    'operations' => array(
      array('_ca_convert_configurations', array()),
    ),
    'finished' => '_ca_conversion_finished',
    'title' => t('Converting Workflow-ng configurations'),
    'init_message' => t('Beginning conversion...'),
    'progress_message' => t('@percentage% converted'),
    'file' => drupal_get_path('module', 'ca') . '/ca.admin.inc',
  );

  batch_set($batch);
}

/**
 * Batch API callback for Workflow-ng configuration conversion.
 */
function _ca_convert_configurations(&$context) {
  global $user;

  if (db_table_exists('workflow_ng_cfgs')) {
    if (!isset($context['sandbox']['progress'])) {
      // Initialize batch data.
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['current_cfg'] = '';
      $context['sandbox']['max'] = db_result(db_query("SELECT COUNT(name) FROM {workflow_ng_cfgs} WHERE altered = 1"));
    }

    $limit = 25;

    $result = db_query_range("SELECT name, data FROM {workflow_ng_cfgs} WHERE altered = 1 AND name > '%s' ORDER BY name", $context['sandbox']['current_cfg'], 0, $limit);
    $predicates = array();

    while ($configuration_row = db_fetch_object($result)) {
      $predicate = array('#pid' => $configuration_row->name);
      $conditions = array();
      $actions = array();
      if ($configuration = unserialize($configuration_row->data)) {
        $predicate['#class'] = $configuration['#module'];
        $predicate['#title'] = $configuration['#label'];

        // Convert event names to corresponding triggers.
        switch ($configuration['#event']) {
          case 'order_status_update':
            $trigger = 'uc_order_status_update';
            break;
          case 'payment_entered':
            $trigger = 'uc_payment_entered';
            break;
          case 'checkout_complete':
            $trigger = 'uc_checkout_complete';
            break;
          default:
            $trigger = $configuration['#event'];
            break;
        }

        // In UC 1.x, taxes had an event for each tax rate, using the same
        // action. In UC 2.x, they use the same trigger, but have separate
        // actions.
        if (strpos($trigger, 'calculate_tax_') === 0) {
          $tax_id = substr($trigger, 14);
          $trigger = 'calculate_taxes';
        }

        $predicate['#trigger'] = $trigger;
        $predicate['#weight'] = $configuration['#weight'];
        $predicate['#status'] = $configuration['#active'];

        // Numeric keys in $configuration could be for conditions or actions.
        // Take each and categorize them to add them to the predicate later.
        for ($i = 0; isset($configuration[$i]); $i++) {
          if ($configuration[$i]['#type'] == 'action') {
            $action = _ca_convert_actions($configuration[$i], $tax_id);
            if ($action) {
              $actions[] = $action;
            }
          }
          else {
            $condition = _ca_convert_conditions($configuration[$i]);
            if ($condition) {
              $conditions[] = $condition;
            }
          }
        }

        // Conditions are optional. If there are no actions, there's no reason
        // to make a predicate.
        if ($conditions) {
          $predicate['#conditions'] = array(
            '#operator' => 'AND',
            '#conditions' => $conditions,
          );
        }
        if ($actions) {
          $predicate['#actions'] = $actions;
          $predicate['#uid'] = $user->uid;
          ca_save_predicate($predicate);
        }
      }

      // Store some result for post-processing in the finished callback.
      $context['results'][] = $configuration_row->name;

      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['sandbox']['current_cfg'] = $configuration_row->name;
      $context['message'] = t('Now processing %cfg', array('%cfg' => $configuration_row->name));
    }

    // Inform the batch engine that we are not finished,
    // and provide an estimation of the completion level we reached.
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }
}

/**
 * Helper function for converting Ubercart's Workflow-ng conditions.
 */
function _ca_convert_conditions($condition_tree) {
  static $condition_data;
  $ca_condition = array();

  if (!isset($condition_data)) {
    $condition_data = module_invoke_all('ca_condition');
  }

  // Handle multiple conditions recursively.
  if ($condition_tree['#type'] == 'AND' || $condition_tree['#type'] == 'OR') {
    $ca_condition['#operator'] = $condition_tree['#type'];
    $ca_condition['#conditions'] = array();
    for ($i = 0; isset($condition_tree[$i]); $i++) {
      $condition = _ca_convert_conditions($condition_tree[$i]);
      if ($condition) {
        $ca_condition['#conditions'][] = $condition;
      }
    }
  }
  elseif ($condition_tree['#type'] == 'condition') {
    // Some conditions were renamed but check the same things.
    switch ($condition_tree['#name']) {
      case 'uc_order_condition_order_total':
        $ca_condition['#name'] = 'uc_order_condition_total';
        break;
      case 'uc_payment_condition_balance':
        $ca_condition['#name'] = 'uc_payment_condition_order_balance';
        break;
      case 'workflow_ng_condition_custom_php':
        $ca_condition['#name'] = 'ca_condition_custom_php';
        break;
      case 'workflow_ng_condition_user_hasrole':
        $ca_condition['#name'] = 'ca_condition_user_roles';
        $condition_tree['#settings']['operator'] = $condition_tree['#settings']['operation'];
        unset($condition_tree['#settings']['operation']);
        break;
      default:
        $ca_condition['#name'] = $condition_tree['#name'];
        break;
    }

    if (isset($condition_tree['#label'])) {
      $ca_condition['#title'] = $condition_tree['#label'];
    }
    else {
      $ca_condition['#title'] = $condition_data[$ca_condition['#name']]['#title'];
    }

    $ca_condition['#argument_map'] = $condition_tree['#argument map'];
    foreach ($ca_condition['#argument_map'] as $key => $value) {
      if ($key == 'user') {
        $key = 'account';
      }
      if ($value == 'user') {
        $value = 'account';
      }
      $ca_condition['#argument_map'][$key] = $value;
    }

    $ca_condition['#settings'] = $condition_tree['#settings'];
    if (isset($condition_tree['#negate'])) {
      $ca_condition['#settings']['negate'] = $condition_tree['#negate'];
    }
  }

  return $ca_condition;
}

/**
 * Helper function for converting Ubercart's Workflow-ng actions.
 */
function _ca_convert_actions($wf_action, $tax_id = NULL) {
  static $action_data;
  $action = $wf_action;

  if (!isset($action_data)) {
    $action_data = module_invoke_all('ca_action');
  }

  // Some actions were renamed, but do the same things.
  switch ($action['#name']) {
    case 'uc_order_action_update_status':
      $action['#name'] = 'uc_order_update_status';
      break;
    case 'uc_taxes_action_apply_tax':
      $action['#name'] = 'uc_taxes_action_apply_tax_'. $tax_id;
      break;
    case 'workflow_ng_action_custom_php':
      $action['#name'] = 'ca_action_custom_php';
      break;
  }

  if (isset($action['#label'])) {
    $action['#title'] = $action['#label'];
    unset($action['#label']);
  }
  else {
    $action['#title'] = $action_data[$action['#name']]['#title'];
  }

  return $action;
}

/**
 * Batch finalization function to hide the conversion tab.
 */
function _ca_conversion_finished() {
  // Set a variable to hide the conversion tab.
  variable_set('ca_show_conversion_tab', FALSE);

  // Display a message and redirect to the overview page.
  drupal_set_message('Your Workflow-ng configurations have been converted.  Please verify the results in your Conditional Actions predicates below.');

  drupal_goto(CA_UI_PATH);
}
