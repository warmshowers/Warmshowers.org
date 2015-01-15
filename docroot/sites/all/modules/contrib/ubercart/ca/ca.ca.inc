<?php

/**
 * @file
 * This file includes some generic conditions and actions.
 */

/*******************************************************************************
 * Conditional Actions Hooks
 ******************************************************************************/

/**
 * Implements hook_ca_entity().
 *
 * Demonstrates defining a data object that may be passed in to ca_pull_trigger
 * and mapped to a predicate's arguments.
 */
function ca_ca_entity() {
  $entities = array();

  $entities['user'] = array(
    '#title' => t('Drupal user'),
    '#type' => 'object',
    '#load' => 'user_load',
    '#save' => 'user_save',
  );
  $entities['node'] = array(
    '#title' => t('Node'),
    '#type' => 'object',
    '#load' => 'node_load',
    '#save' => 'node_save',
  );
  $entities['arguments'] = array(
    '#title' => t('Trigger arguments'),
    '#type' => 'array',
  );

  return $entities;
}

/**
 * Implements hook_ca_condition().
 */
function ca_ca_condition() {
  $conditions = array();

  $conditions['ca_condition_date'] = array(
    '#title' => t('Check the current date'),
    '#description' => t('Used to determine if the action should be performed on the current date.'),
    '#category' => t('Drupal'),
    '#callback' => 'ca_condition_date',
    '#arguments' => array(),
  );

  $conditions['node_field_comparison'] = array(
    '#title' => t('Compare a node field value'),
    '#description' => t('Returns TRUE if the node field selected below compares to the value entered as specified by the operator.'),
    '#category' => t('Node'),
    '#callback' => 'ca_condition_node_field_comparison',
    '#arguments' => array(
      'node' => array('#entity' => 'node'),
    ),
  );

  $conditions['ca_condition_custom_php'] = array(
    '#title' => t('Execute custom PHP code'),
    '#description' => t('Returns whatever your custom PHP code returns.'),
    '#category' => t('System'),
    '#callback' => 'ca_condition_custom_php',
    '#arguments' => array(
      'arguments' => array('#entity' => 'arguments', '#title' => t('Arguments')),
    ),
  );

  $conditions['ca_condition_user_roles'] = array(
    '#title' => t("Check the user's roles"),
    '#category' => t('User'),
    '#description' => t('Returns TRUE if the user roles match your settings.'),
    '#callback' => 'ca_condition_user_roles',
    '#arguments' => array(
      'account' => array('#entity' => 'user', '#title' => t('User')),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_ca_action().
 *
 * Demonstrates defining an action for predicates to use; primarily specifies a
 * callback function to perform the action and an array that specifies arguments
 * and their data types.
 */
function ca_ca_action() {
  $actions['ca_drupal_set_message'] = array(
    '#title' => t('Display a message to the user'),
    '#category' => t('Drupal'),
    '#callback' => 'ca_action_drupal_set_message',
    '#arguments' => array(),
  );

  $actions['ca_action_custom_php'] = array(
    '#title' => t('Execute custom PHP code'),
    '#category' => t('System'),
    '#callback' => 'ca_action_custom_php',
    '#arguments' => array(
      'arguments' => array('#entity' => 'arguments', '#title' => t('Arguments')),
    ),
  );

  return $actions;
}

/**
 * Check that the current time is within a specified range.
 *
 * @see ca_condition_date_form()
 */
function ca_condition_date($settings) {
  $set_date_start = mktime(0, 0, 0, $settings['date']['month'], $settings['date']['day'], $settings['date']['year']);
  $set_date_end = mktime(23, 59, 59, $settings['date']['month'], $settings['date']['day'], $settings['date']['year']);

  $curr_time = time();

  switch ($settings['operator']) {
    case 'before':
      return $curr_time < $set_date_start;
    case 'only':
      return ($set_date_start < $curr_time) && ($curr_time < $set_date_end);
    case 'after':
      return $set_date_end < $curr_time;
    default:
      return FALSE;
  }
}

/**
 * @see ca_condition_date()
 */
function ca_condition_date_form($form_state, $settings = array()) {
  $form = array();

  $form['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Comparison'),
    '#options' => array(
      'before' => t('Before'),
      'only' => t('Only'),
      'after' => t('After'),
    ),
    '#default_value' => $settings['operator'],
    '#description' => t('Example: "The current date is before the date below."'),
  );

  $form['date'] = array(
    '#type' => 'date',
    '#title' => t('Date'),
    '#default_value' => $settings['date'],
    '#description' => t('When the predicate is evaluated, the current date is compared to this date.'),
  );

  return $form;
}

/**
 * Compare a node field to a specified value using an operator list.
 *
 * @see ca_condition_node_field_comparison_form()
 */
function ca_condition_node_field_comparison($node, $settings) {
  // Convert the node to an array.
  $node_array = (array) $node;

  // Store the value of the field we're comparing.
  $field_value = $node_array[$settings['field']];

  // Return the result based on the operator.
  switch ($settings['operator']) {
    case 'equal':
      if ($field_value == $settings['value']) {
        return TRUE;
      }
      break;

    case 'not':
      if ($field_value != $settings['value']) {
        return TRUE;
      }
      break;

    case 'greater':
      if ($field_value > $settings['value']) {
        return TRUE;
      }
      break;

    case 'less':
      if ($field_value < $settings['value']) {
        return TRUE;
      }
      break;

    case 'greater_equal':
      if ($field_value >= $settings['value']) {
        return TRUE;
      }
      break;

    case 'less_equal':
      if ($field_value <= $settings['value']) {
        return TRUE;
      }
      break;

    case 'begins':
      if (strpos($field_value, $settings['value']) === 0) {
        return TRUE;
      }
      break;

    case 'ends':
      if (substr($field_value, -1 * strlen($settings['value'])) === $settings['value']) {
        return TRUE;
      }
      break;

    case 'contains':
      if (strpos($field_value, $settings['value']) !== FALSE) {
        return TRUE;
      }
      break;

    case 'yes':
      if ($field_value) {
        return TRUE;
      }
      break;

    case 'no':
      if (!$field_value) {
        return TRUE;
      }
      break;
  }

  return FALSE;
}

/**
 * @see ca_condition_node_field_comparision()
 */
function ca_condition_node_field_comparison_form($form_state, $settings = array()) {
  $form = array();

  // Define the fields this works for.
  $options = array(
    t('Core node fields') => array(
      'nid' => t('Node ID'),
      'vid' => t('Node revision ID'),
      'type' => t('Node type'),
      'title' => t('Title'),
      'uid' => t("Author's user ID"),
      'status' => t('Node is published?'),
      'promote' => t('Node is promoted?'),
      'sticky' => t('Node is sticky?'),
    ),
  );

  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Node field'),
    '#options' => $options,
    '#default_value' => $settings['field'],
  );

  // Define the operators for the fields.
  $options = array(
    t('Simple comparison') => array(
      'equal' => t('is equal to'),
      'not' => t('is not equal to'),
      'greater' => t('is greater than'),
      'less' => t('is less than'),
      'greater_equal' => t('is greater than or equal to'),
      'less_equal' => t('is less than or equal to'),
    ),
    t('Text matching') => array(
      'begins' => t('begins with'),
      'contains' => t('contains'),
      'ends' => t('ends with'),
    ),
    t('Yes/No') => array(
      'yes' => t('yes'),
      'no' => t('no'),
    ),
  );

  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#description' => t('Please note that not every operator makes sense for every field.'),
    '#options' => $options,
    '#default_value' => $settings['operator'],
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Comparison value'),
    '#description' => t('You do not need to specify a value if your operator is in the Yes/No category.'),
    '#default_value' => $settings['value'],
  );

  return $form;
}

/**
 * Display a message to the user.
 *
 * @see ca_action_drupal_set_message_form()
 */
function ca_action_drupal_set_message($settings) {
  // Filter the text using the default format.
  $message = check_markup($settings['message_text'], FILTER_FORMAT_DEFAULT, FALSE);

  // Return if there's nothing to display.
  if (empty($message) || empty($settings['message_text'])) {
    return;
  }

  // Make sure we have a valid message type.
  if ($settings['message_type'] == 'error') {
    $type = $settings['message_type'];
  }
  else {
    $type = 'status';
  }

  // Output the message using the Drupal message API.
  drupal_set_message($message, $type);
}

/**
 * @see ca_action_drupal_set_message()
 */
function ca_action_drupal_set_message_form($form_state, $settings = array()) {
  $form = array();

  $form['message_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Message text'),
    '#default_value' => $settings['message_text'],
  );

  $tips = _filter_tips(variable_get('filter_default_format', 1), FALSE);
  $form['format']['guidelines'] = array(
    '#title' => t('Formatting guidelines'),
    '#value' => theme('filter_tips', $tips, FALSE, theme('filter_tips_more_info')),
  );

  $form['message_type'] = array(
    '#type' => 'radios',
    '#title' => t('Message type'),
    '#options' => array(
      'status' => t('Status'),
      'error' => t('Error'),
    ),
    '#default_value' => isset($settings['message_type']) ? $settings['message_type'] : 'status',
  );

  return $form;
}

/**
 * Evaluate a custom PHP condition.
 *
 * @see ca_condition_custom_php_form()
 */
function ca_condition_custom_php($arguments, $settings) {
  return _ca_custom_php_eval($settings['php'], $arguments);
}

/**
 * @see ca_condition_custom_php()
 */
function ca_condition_custom_php_form($form_state, $settings = array(), $arguments = array()) {
  $form['variables'] = _ca_custom_php_variables_table($arguments);

  $form['php'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom PHP'),
    '#description' => t('Enter the custom PHP to be evaluated when this condition is executed. Should not include &lt;?php ?> delimiters.'),
    '#default_value' => isset($settings['php']) ? $settings['php'] : '',
    '#wysiwyg' => FALSE,
  );

  return $form;
}

/**
 * Check a user's roles.
 *
 * @see ca_condition_user_roles_form()
 */
function ca_condition_user_roles($account, $settings) {
  $settings['roles'] = array_filter($settings['roles']);

  if ($settings['operator'] == 'AND') {
    foreach ($settings['roles'] as $key) {
      if (!isset($account->roles[$key])) {
        return FALSE;
      }
    }
    return TRUE;
  }
  else {
    foreach ($settings['roles'] as $key) {
      if (isset($account->roles[$key])) {
        return TRUE;
      }
    }
    return FALSE;
  }
}

/**
 * @see ca_condition_user_roles()
 */
function ca_condition_user_roles_form($form_state, $settings = array()) {
  $form['operator'] = array(
    '#type' => 'radios',
    '#title' => t('Operator'),
    '#description' => t('If you specify <em>AND</em> and want to check a custom role, remember to specify <em>authenticated user</em> where applicable.'),
    '#options' => array(
      'OR' => t('OR: If the user has any of these roles.'),
      'AND' => t('AND: If the user has all of these roles.'),
    ),
    '#default_value' => isset($settings['operator']) ? $settings['operator'] : 'OR',
  );
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#options' => user_roles(),
    '#default_value' => isset($settings['roles']) ? $settings['roles'] : array(),
  );

  return $form;
}

/**
 * Perform a custom PHP action.
 *
 * @see ca_action_custom_php_form()
 */
function ca_action_custom_php($arguments, $settings) {
  _ca_custom_php_eval($settings['php'], $arguments);
}

/**
 * @see ca_action_custom_php()
 */
function ca_action_custom_php_form($form_state, $settings = array(), $arguments = array()) {
  $form['variables'] = _ca_custom_php_variables_table($arguments);

  $form['php'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom PHP'),
    '#description' => t('Enter the custom PHP to be evaluated when this action is executed.  Should not include &lt;?php ?> delimiters.'),
    '#default_value' => isset($settings['php']) ? $settings['php'] : '',
    '#wysiwyg' => FALSE,
  );

  return $form;
}

/**
 * Return a fieldset used to display available variables for custom PHP
 * conditions and actions.
 */
function _ca_custom_php_variables_table($arguments) {
  $rows = array();

  $header = array(t('Variable'), t('Type'), t('Description'));

  // Translate the arguments into descriptive rows.
  foreach ($arguments as $key => $value) {
    $rows[] = array(check_plain('$'. $key), check_plain($value['#entity']), $value['#title']);
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No variables available.'), 'colspan' => 3));
  }

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Available PHP variables'),
    '#description' => t('You may use these variables in your custom PHP.')
                    . theme('table', $header, $rows),
    '#collapsible' => TRUE,
  );

  return $fieldset;
}

/**
 * Evaluate a custom PHP condition or action with trigger arguments available
 * as variables.
 */
function _ca_custom_php_eval($php, $arguments) {
  $argument_data = array();

  // Convert the arguments to an array of data that we can extract.
  foreach ($arguments as $key => $value) {
    $argument_data[$key] = $value['#data'];
  }

  extract($argument_data);
  return eval($php);
}

/**
 * Build an email settings form.
 */
function ca_build_email_form($form_state, $settings, $token_filters) {
  $form = array();

  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => $settings['from'],
    '#description' => t('The "From" address.'),
    '#required' => TRUE,
  );
  $form['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => $settings['addresses'],
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
    '#wysiwyg' => FALSE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#wysiwyg' => FALSE,
  );

  // We add the #is_format element to allow us to locate and configure the filters.
  $form['format'] = filter_form($settings['format']) + array('#is_format' => TRUE);

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the recipients field, the subject, and the message body.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach ($token_filters as $name) {
    $form['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }

  return $form;
}
