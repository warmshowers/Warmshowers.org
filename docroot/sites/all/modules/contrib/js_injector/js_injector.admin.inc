<?php
/**
 * @file js_injector.admin.inc
 * Administrative interface for JS Injector.
 */

/**
 * Form builder function for JS Injector's main admin page.
 */
function js_injector_admin_form($form, &$form_state) {
  $rules = _js_injector_load_rule(NULL, TRUE);
  $path = drupal_get_path('module', 'js_injector') . '/';
  $form = array();
  $form['#tree'] = TRUE;

  // TODO: Turn these 'submit' buttons back into image_buttons when
  // http://drupal.org/node/873070 lands. Currently you can't use
  // #submit with an image_button.
  foreach ($rules as $rule) {
    $form['rules'][$rule['crid']]['#rule'] = $rule;
    $form['rules'][$rule['crid']]['edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit rule'),
      '#name' => 'edit' . $rule['crid'],
      '#submit' => array('js_injector_admin_edit_button'),
      '#crid' => $rule['crid'],
    );
    $form['rules'][$rule['crid']]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete rule'),
      '#name' => 'delete' . $rule['crid'],
      '#submit' => array('js_injector_admin_delete_button'),
      '#crid' => $rule['crid'],
    );
  }
  return $form;
}

/**
 * Edit button callback for the JS rule listing form.
 */
function js_injector_admin_edit_button($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/config/development/js-injector/edit/' . $crid;
}

/**
 * Delete button callback for the JS rule listing form.
 * Redirects the user to the confirmation form.
 */
function js_injector_admin_delete_button($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/config/development/js-injector/delete/' . $crid;
}

/**
 * Theme function for the JS Injector admin overview form.
 */
function theme_js_injector_admin_form($variables) {
  $form = $variables['form'];
  $headers = array(t('Title'), t('Location'), t('Actions'));
  $rows = array();
  if (!empty($form['rules'])) {
    foreach (element_children($form['rules']) as $crid) {
      $row = array();
      $rule = $form['rules'][$crid]['#rule'];
      $row[] = check_plain($rule['title']);
      $row[] = check_plain(_js_injector_rule_path($rule['crid']));
      $row[] = drupal_render($form['rules'][$crid]);
      $rows[] = $row;
    }
  }

  $link = l(t('Create a new rule'), 'admin/config/development/js-injector/add');
  $row = array();
  if (empty($rows)) {
    $row[] = array(
      'data' => t('No JS injection rules have been set up yet. !url.', array('!url' => $link)),
      'colspan' => 3,
    );
  }
  else {
    $row[] = array(
      'data' => t('!url.', array('!url' => $link)),
      'colspan' => 3,
    );
  }
  $rows[] = $row;

  $output = theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form builder function for the JS rule edit form.
 */
function js_injector_edit($form, $form_state, $crid = NULL) {
  if (isset($crid)) {
    $rule = _js_injector_load_rule($crid, TRUE);

    $path = _js_injector_rule_path($rule['crid']);
    if (file_exists($path)) {
      $rule['js_text'] = file_get_contents($path);
    }
    else {
      $rule['js_text'] = '';
    }
  }
  else {
    $rule = array(
      'title' => '',
      'rule_type' => JS_INJECTOR_PAGES_NOTLISTED,
      'rule_conditions' => '',
      'media' => 'all',
      'preprocess' => 1,
      'js_text' => '',
    );
  }

  if (isset($crid)) {
    $form['crid'] = array(
      '#type' => 'value',
      '#value' => $crid,
    );
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $rule['title'],
    '#required' => TRUE,
  );

  $form['js_text'] = array(
    '#type' => 'textarea',
    '#title' => t('JS code'),
    '#rows' => 10,
    '#default_value' => $rule['js_text'],
    '#required' => TRUE,
  );

  // Shamelessly ripped from block.module. Who doesn't use this snippet
  // of code, really?
  $php_access = (user_access('use PHP for settings') && module_exists('php'));
  $options = array(
    JS_INJECTOR_PAGES_NOTLISTED => t('Add on every page except the listed pages.'),
    JS_INJECTOR_PAGES_LISTED => t('Add on only the listed pages.')
  );
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if ($php_access) {
    $options[JS_INJECTOR_PHP] = t('Add if the following PHP code outputs a nonzero value (PHP-mode, experts only).');
    $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  $form['conditional']['rule_type'] = array(
    '#type' => 'radios',
    '#title' => t('Add the JS on specific pages'),
    '#options' => $options,
    '#default_value' => $rule['rule_type'],
  );
  $form['conditional']['rule_conditions'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => $rule['rule_conditions'],
    '#description' => $description,
  );


  $form['media'] = array(
    '#type' => 'select',
    '#title' => t('Media'),
    '#options' => array('all' => t('All'), 'screen' => t('Screen'), 'print' => t('Print')),
    '#default_value' => $rule['media'],
  );

  $form['preprocess'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preprocess JS'),
    '#default_value' => $rule['preprocess'],
  );

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('js_injector_edit_save'),
  );
  $form['buttons']['save_and_continue'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Continue Editing'),
    '#submit' => array('js_injector_edit_save_and_continue'),
  );

  if (!empty($rule['crid'])) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#crid' => $rule['crid'],
      '#submit' => array('js_injector_admin_delete_button'),
    );
  }

  return $form;
}

/**
 * Validation callback for the JS rule edit form.
 */
function js_injector_edit_validate($form, &$form_state) {
  $directory = drupal_realpath('public://js_injector');
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    form_error($form, t('The directory %directory is not writable', array('%directory' => $directory)));
  }
}

/**
 * Submit button callback for the JS rule edit form.
 */
function js_injector_edit_save($form, &$form_state) {
  $rule = $form_state['values'];
  $crid = !empty($form_state['values']['crid']) ? $form_state['values']['crid'] : NULL;
  drupal_write_record('js_injector_rule', $rule, empty($crid) ? array() : 'crid');
  $form_state['values']['rule'] = $rule;
  file_unmanaged_save_data($rule['js_text'], _js_injector_rule_uri($rule['crid']), FILE_EXISTS_REPLACE);
  _js_injector_load_rule(NULL, TRUE);

  drupal_set_message(t('Your JS injection rule %title was saved.', array('%title' => $rule['title'])));
  $form_state['redirect'] = 'admin/config/development/js-injector';
}

/**
 * Save and continue callback for the JS rule edit form.
 */
function js_injector_edit_save_and_continue($form, &$form_state) {
  js_injector_edit_save($form, $form_state);
  $form_state['redirect'] = 'admin/config/development/js-injector/edit/' . $form_state['values']['rule']['crid'];
}


/**
 * Menu callback -- ask for confirmation of rule deletion.
 */
function js_injector_delete_confirm($form, &$form_state, $crid) {
  $form['crid'] = array(
    '#type' => 'value',
    '#value' => $crid,
  );

  $rule = _js_injector_load_rule($crid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $rule['title'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/development/js-injector',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function js_injector_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _js_injector_delete_rule($form_state['values']['crid']);
  }

  $form_state['redirect'] = 'admin/config/development/js-injector';
  return;
}
