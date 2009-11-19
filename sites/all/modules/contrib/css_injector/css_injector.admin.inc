<?php
// $Id: css_injector.admin.inc,v 1.1.2.6 2008/08/10 01:56:58 add1sun Exp $

/**
 * Page callback for CSS Injector's main admin page.
 */
function css_injector_admin_form() {
  $rules = _css_injector_load_rule(NULL, TRUE);
  $path = drupal_get_path('module', 'css_injector') .'/';
  $form = array();
  $form['#tree'] = TRUE;
  foreach ($rules as $rule) {
    $form['rules'][$rule['crid']]['#rule'] = $rule;
    $form['rules'][$rule['crid']]['edit'] = array(
      '#type' => 'image_button',
      '#title' => t('Edit rule'),
      '#src' => $path .'text-editor.png',
      '#submit' => array('css_injector_admin_edit_button'),
      '#crid' => $rule['crid'],
    );
    $form['rules'][$rule['crid']]['delete'] = array(
      '#type' => 'image_button',
      '#title' => t('Delete rule'),
      '#src' => $path .'edit-delete.png',
      '#submit' => array('css_injector_admin_delete_button'),
      '#crid' => $rule['crid'],
    );
  }
  return $form;
}

/**
 * Edit button callback for the CSS rule listing form.
 */
function css_injector_admin_edit_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/settings/css_injector/edit/'. $crid;
}

/**
 * Delete button callback for the CSS rule listing form.
 * Redirects the user to the confirmation form.
 */
function css_injector_admin_delete_button($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $crid = $button['#crid'];
  $form_state['redirect'] = 'admin/settings/css_injector/delete/'. $crid;
}

/**
 * Theme function for the CSS Injector admin overview form.
 */
function theme_css_injector_admin_form(&$form) {
  $headers = array(t('Title'), t('File path'), t('Actions'));
  $rows = array();
  if (!empty($form['rules'])) {
    foreach (element_children($form['rules']) as $crid) {
      $row = array();
      $rule = $form['rules'][$crid]['#rule'];
      $row[] = check_plain($rule['title']);
      $row[] = check_plain($rule['file_path']);
      $row[] = drupal_render($form['rules'][$crid]);
      $rows[] = $row;
    }
  }

  $link = l(t('Create a new rule'), 'admin/settings/css_injector/add');
  $row = array();
  if (empty($rows)) {
    $row[] = array(
      'data' => t('No CSS injection rules have been set up yet. !url.', array('!url' => $link)),
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

  $output .= theme('table', $headers, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Constructor for the CSS rule edit form.
 */
function css_injector_edit($form_state, $crid = NULL) {
  if (isset($crid)) {
    $rule = _css_injector_load_rule($crid, TRUE);
    $form['crid'] = array(
      '#type' => 'value',
      '#value' => $crid,
    );
    $path = file_create_path($rule['file_path']);
    if (file_exists($path)) {
      $rule['css_text'] = file_get_contents($path);
      $form['file_path'] = array(
        '#type' => 'value',
        '#value' => $rule['file_path'],
      );
    }
    else {
      $rule['css_text'] = '';
    }
  }
  else {
    $rule = array(
      'title' => '',
      'rule_type' => 0,
      'rule_conditions' => '',
      'media' => 'all',
      'preprocess' => 1,
      'css_text' => '',
    );
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $rule['title'],
    '#required' => TRUE,
  );

  $form['css_text'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS code'),
    '#rows' => 10,
    '#default_value' => $rule['css_text'],
    '#required' => TRUE,
  );

  // Shamelessly ripped from block.module. Who doesn't use this snippet
  // of code, really?
  $access = user_access('use PHP for block visibility');
  if ($rule['rule_type'] == 2 && !$access) {
    $form['conditional'] = array();
    $form['conditional']['rule_type'] = array('#type' => 'value', '#value' => 2);
    $form['conditional']['rule_conditions'] = array('#type' => 'value', '#value' => $rule['rule_conitions']);
  }
  else {
    $options = array(t('Add on every page except the listed pages.'), t('Add on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['conditional']['rule_type'] = array(
      '#type' => 'radios',
      '#title' => t('Add the CSS on specific pages'),
      '#options' => $options,
      '#default_value' => $rule['rule_type'],
    );
    $form['conditional']['rule_conditions'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $rule['rule_conditions'],
      '#description' => $description,
    );
  }

  $form['media'] = array(
    '#type' => 'select',
    '#title' => t('Media'),
    '#description' => t(''),
    '#options' => array('all' => t('All'), 'screen' => t('Screen'), 'print' => t('Print')),
    '#default_value' => $rule['media'],
  );

  $form['preprocess'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preprocess CSS'),
    '#description' => t(''),
    '#default_value' => $rule['preprocess'],
  );

  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('css_injector_edit_save'),
  );

  if (!empty($rule['crid'])) {
    $form['buttons']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('css_injector_admin_delete_button'),
      '#crid' => $rule['crid'],
    );
  }

  return $form;
}

/**
 * Validation callback for the CSS rule edit form.
 */
function css_injector_edit_validate($form, &$form_state) {
  $rule = $form_state['values'];
  $directory = file_directory_path();
  file_check_directory($directory, 1);
  $form_state['rule'] = $rule;
}

/**
 * Submit button callback for the CSS rule edit form.
 */
function css_injector_edit_save($form, &$form_state) {
  $rule = $form_state['rule'];

  if (empty($rule['crid'])) {
    drupal_write_record('css_injector_rule', $rule);
    $directory = file_directory_path();
    $rule['file_path'] = file_create_path($directory .'/css_injector_'. $rule['crid'] .'.css');
  }
  drupal_write_record('css_injector_rule', $rule, array('crid'));

  file_save_data($rule['css_text'], file_create_path($rule['file_path']), FILE_EXISTS_REPLACE);
  _css_injector_load_rule(NULL, TRUE);

  drupal_set_message('Your CSS injection rule was saved.');
  $form_state['redirect'] = 'admin/settings/css_injector';
}


/**
 * Menu callback -- ask for confirmation of rule deletion.
 */
function css_injector_delete_confirm(&$form_state, $crid) {
  $form['crid'] = array(
    '#type' => 'value',
    '#value' => $crid,
  );

  $rule = _css_injector_load_rule($crid);
  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $rule['title'])),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/settings/css_injector',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function css_injector_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _css_injector_delete_rule($form_state['values']['crid']);
  }

  $form_state['redirect'] = 'admin/settings/css_injector';
  return;
}
