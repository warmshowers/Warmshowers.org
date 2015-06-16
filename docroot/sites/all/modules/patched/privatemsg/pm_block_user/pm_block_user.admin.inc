<?php

/**
 * @file
 * Administration menu callbacks for pm_block_user.module.
 */

/**
 * Builds row of sending, receiving roles and actions that go with them.
 *
 * @param $details
 *   Details of the row: default values and the unique row number (delta).
 * @param $blacklist
 *   When the functionality has been added, this will allow building actions
 *   based on a whitelist or blacklist. The current code only covers the use
 *   case of a blacklist, where blocking everyone is allowed by default and
 *   rules are exceptions to that. Conversely, a whitelist will disallow
 *   blocking by default and rules will configure roles that are allowed to
 *   block.
 *
 * @return
 *   Part of a form with controls for sending, receiving and actions.
 */
function _pm_block_user_actions_form($details, $blacklist = TRUE) {
  $form = array(
    '#tree' => TRUE,
  );
  $delta = $details['delta'];
  // FALSE by default, or if the user has checked the 'Enabled' check box for
  // this row.
  $row_disabled = (isset($details['enabled']) ? !$details['enabled'] : FALSE);

  $form['author'] = array(
    '#type' => 'select',
    '#options' => user_roles(TRUE),
    '#default_value' => (isset($details['author']) ? $details['author'] : DRUPAL_AUTHENTICATED_RID),
    '#disabled' => $row_disabled,
  );
  $form['recipient'] = array(
    '#type' => 'select',
    '#options' => user_roles(TRUE),
    '#default_value' => (isset($details['recipient']) ? $details['recipient'] : DRUPAL_AUTHENTICATED_RID),
    '#disabled' => $row_disabled,
  );

  // Provide different action radios if we're using a whitelist or a blacklist.
  if ($blacklist) {
    $options = array(
      PM_BLOCK_USER_DISALLOW_BLOCKING => t('Disallow blocking author'),
      PM_BLOCK_USER_DISALLOW_SENDING => t('Disallow sending message'),
    );
    $default_value = (isset($details['action']) ? $details['action'] : PM_BLOCK_USER_DISALLOW_BLOCKING);
  }
  else {
    // TODO: add whitelist options/default_value here.
  }
  $form['action']  = array(
    '#type' => 'radios',
    '#options' => $options,
    '#disabled' => $row_disabled,
    '#default_value' => $default_value,
  );

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#default_value' => (isset($details['enabled']) ? $details['enabled'] : TRUE),
  );
  $form['remove'] = array(
    '#type' => 'submit',
    '#submit' => array('pm_block_user_remove_submit'),
    '#value' => t('Remove_' . $delta),
    '#attributes' => array('class' => array('remove-action')),
    '#prefix' => '<div id="remove-rule-button">',
    '#suffix' => '<label for="edit-remove">' . t('Remove rule') . '</label></div>',
    '#ajax' => array(
      'callback' => 'pm_block_user_js',
      'wrapper' => 'block-actions',
    ),
  );

  return $form;
}

/**
 * Takes an array of settings and filters out anything that's un-needed.
 * Leaving only settings to be saved.
 *
 * @param $settings
 *   The array of settings to filter.
 *
 * @return
 *   Array of settings, ready to be stored in the database.
 *
 * @see pm_block_user_settings_submit()
 */
function _pm_block_user_settings_filter($settings) {
  // Add-in the names of any settings to be saved into the array below.
  $save_keys = array('author', 'recipient', 'action', 'enabled');
  $matching = array();
  // Run through each of the keys we want to save, creating a new array.
  // It's not possible to simply check for unwanted values and unset() them as
  // the array is multi-dimensional.
  foreach ($save_keys as $save_key) {
    if (isset($settings[$save_key])) {
      $matching[$save_key] = $settings[$save_key];
    }
  }
  if (count($matching) > 0) {
    return $matching;
  }
  else {
    return array_map('_pm_block_user_settings_filter', $settings);
  }
}

/**
 * Menu callback for AHAH handling.
 */
function pm_block_user_js($form, $form_state) {
  return drupal_render($form['block_actions']);
}

/**
 * Submit handler for 'More' button, adds a new action.
 *
 * @see pm_block_user_remove_submit()
 */
function pm_block_user_more_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  // Get the submitted actions, then put them into a special area of
  // the $form_state.
  $submitted_values = $form_state['values'];
  // Add an empty action.
  $submitted_values['block_actions'][] = array();
  $form_state['pm_block_user'] = $submitted_values;
  // Rebuild the form by passing our $form_state through the
  // pm_block_user_settings() builder function.
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for 'Remove' button, removes an action.
 *
 * @see pm_block_user_more_submit()
 */
function pm_block_user_remove_submit($form, &$form_state) {
  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $submitted_values = $form_state['values'];
  // Remove the requested action.
  $delta = $form_state['clicked_button']['#parents'][1];
  unset($submitted_values['block_actions'][$delta]);
  $form_state['pm_block_user'] = $submitted_values;
  $form_state['rebuild'] = TRUE;
}

/**
 * Menu callback for blocked user settings.
 */
function pm_block_user_settings($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'pm_block_user') . '/pm_block_user.css');
  // Need to cache form for AHAH, so it can be rebuilt from cache later.
  $form = array(
    '#cache' => TRUE,
  );

  // Container for just the actions, used for AHAH.
  $form['block_actions'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="block-actions">',
    '#suffix' => '</div>',
    '#theme' => 'pm_block_user_actions',
  );

  // Should we populate the form with data from $form_state or the database?
  if (!isset($form_state['pm_block_user']['block_actions'])) {
    $block_actions = variable_get('pm_block_user_actions', array());
  }
  else {
    $block_actions = $form_state['pm_block_user']['block_actions'];
  }
  // Work through each rule, adding it as a new element in
  // $form['block_actions'] ready to be themed later.
  foreach ($block_actions as $delta => $details) {
    $details['delta'] = $delta;
    $form['block_actions'][$delta] = _pm_block_user_actions_form($details);
  }

  // The magic AHAH callback button that adds more rows.
  $form['pm_block_actions_more'] = array(
    '#type' => 'submit',
    '#value' => t('More'),
    '#weight' => 1,
    '#prefix' => '<div id="add-rule-button">',
    '#suffix' => '<label for="edit-pm-block-actions-more">' . t('Add new rule') . '</label></div>',
    '#submit' => array('pm_block_user_more_submit'),
    '#ajax' => array(
      'callback' => 'pm_block_user_js',
      'wrapper' => 'block-actions',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['submit_form'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for admin form.
 */
function pm_block_user_settings_submit($form, &$form_state) {
  // We don't want it to submit when we're adding/removing actions.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    // If the form's 'block_actions' aren't set, the user has deleted all the
    // rows in the table, so we save an empty array to stop errors in the form
    // builder.
    if (isset($form_state['values']['block_actions'])) {
      variable_set('pm_block_user_actions', _pm_block_user_settings_filter($form_state['values']['block_actions']));
    }
    else {
      variable_set('pm_block_user_actions', array());
    }
    drupal_set_message(t('The configuration options have been saved.'));
  }
}