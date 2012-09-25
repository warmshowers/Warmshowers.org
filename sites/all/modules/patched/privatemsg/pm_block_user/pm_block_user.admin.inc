<?php

/**
 * @file
 * Administration menu callbacks for pm_block_user.module.
 */


/**
 * Menu callback for blocked user settings.
 */
function pm_block_user_settings(&$form_state) {
  drupal_add_css(drupal_get_path('module', 'pm_block_user') .'/pm_block_user.css');
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
    '#ahah' => array(
      'path' => 'messages/block/js',
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
    // @todo: add whitelist options/default_value here.
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
    '#value' => t('Remove'),
    '#attributes' => array('class' => 'remove-action'),
    '#prefix' => '<div id="remove-rule-button">',
    '#suffix' => '<label for="edit-remove">' . t('Remove rule') . '</label></div>',
    '#ahah' => array(
      'path' => 'messages/block/js',
      'wrapper' => 'block-actions',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
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

/**
 * Takes an array of settings and filters out anything that's un-needed.
 *
 * Leaving only settings to be saved.
 *
 * @param $settings
 *   The array of settings to filter.
 * @return
 *   Array of settings, ready to be stored in the database.
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
function pm_block_user_js() {
  // See: http://drupal.org/node/331941 for the philosophy of Drupal AHAH.
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $output_form = $form['block_actions'];
  unset($output_form['#prefix'], $output_form['#suffix']); // Prevent duplicate wrappers.
  $output = theme('status_messages') . drupal_render($output_form);

  // Hack to make behaviours attach to new HTML controls (delete buttons in
  // this case).
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array('status' => TRUE, 'data' => $output));
}


/**
 * Theme the user actions form.
 *
 * @ingroup themeable
 */
function theme_pm_block_user_actions($form) {
  $rows = array();
  $headers = array(
    t('If the author has the role'),
    t('And the recipient has the role'),
    t('Action'),
    t('Enabled'),
    '',
  );
  $form_data = element_children($form);

  foreach ($form_data as $key) {
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['author'])),
        array('data' => drupal_render($form[$key]['recipient'])),
        array('data' => drupal_render($form[$key]['action'])),
        array('data' => drupal_render($form[$key]['enabled'])),
        array('data' => drupal_render($form[$key]['remove'])),
      ),
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  // If there are no rows, output some instructions for the user.
  if (empty($form_data)) {
    $rows[] = array(
      array(
        'data' => t("No rules have been added. All users may block private messages from each other. To limit which users may be blocked, click 'Add new rule'."),
        'colspan' => '5',
      ),
    );
  }

  $output = theme('table', $headers, $rows);
  $output .= drupal_render($form);

  return $output;
}