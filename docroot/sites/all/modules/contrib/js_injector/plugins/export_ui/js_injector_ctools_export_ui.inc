<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'js_injector_rule',  // As defined in hook_schema().
  'access' => 'administer js_injector',  // define a permission users must have to access these pages.

  // define the menu item.
  'menu' => array(
    'menu item' => 'js-injector',
    'menu prefix' => 'admin/config/development',
    'menu title' => 'JS injector',
    'menu description' => 'Settings for JS injector rules. Allows for the insertion of arbitrary JavaScript on pages.',
  ),

  // define user interface texts.
  'title singular' => t('rule'),
  'title plural' => t('rules'),
  'title singular proper' => t('JS injector rule'),
  'title plural proper' => t('JS injector rules'),

  // define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'js_injector_ctools_export_ui_form',
    'validate' => 'js_injector_ctools_export_ui_form_validate',
    'submit' => 'js_injector_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the preset add/edit form.
 */
function js_injector_ctools_export_ui_form(&$form, &$form_state) {
  $rule = $form_state['item'];

  // Alter the name field (this is set by the default export_ui class).
  $form['info']['name']['#title'] = t('Friendly name');
  $form['info']['name']['#description'] = t('This is the unique ID for this rule. Only alphanumeric characters, spaces, underscores and dashes are allowed.');

  // Alter the admin_description field (this is set by the default export_ui
  // class).
  $form['info']['admin_description']['#title'] = t('Description');
  $form['info']['admin_description']['#type'] = 'textfield';
  $form['info']['admin_description']['#description'] = t('The human readable description of this rule.');
  $form['info']['admin_description']['#required'] = TRUE;

  $form['js'] = array(
    '#type' => 'textarea',
    '#title' => t('JS code'),
    '#description' => t('The actual JavaScript code goes in here. There is no need to insert the &lt;script&gt; tags.'),
    '#rows' => 10,
    '#default_value' => $rule->js,
    '#required' => TRUE,
  );

  // placement options fieldset
  $form['placement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Placement options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['placement']['position'] = array(
    '#type' => 'select',
    '#title' => 'Position of the javascipt',
    '#description' => t('Where in the HTML will the JavaScript be placed.'),
    '#options' => array(
      'header' => t('Header'),
      'footer' => t('Footer'),
    ),
    '#default_value' => $rule->position,
  );
  $form['placement']['preprocess'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preprocess JS'),
    '#description' => t('If the JavaScript is preprocessed, and JavaScript aggregation is enabled, the script file will be aggregated. Warning - this means you will require a theme cache clear in order to regenerate new aggregate files.'),
    '#default_value' => $rule->preprocess,
  );
  $form['placement']['inline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inline JS'),
    '#description' => t('The JavaScript file can also be inline on the page.'),
    '#default_value' => $rule->inline,
    '#states' => array(
      'visible' => array(
        'input[name="preprocess"]' => array('checked' => FALSE),
      ),
    ),
  );

  // page visibility fieldset. Code ported from googleanalytics.module
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $title = t('Pages');
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  $options = array(
    0 => t('Every page except the listed pages'),
    1 => t('The listed pages only'),
  );
  if (module_exists('php') && user_access('use PHP for settings')) {
    $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  $form['pages']['page_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking to specific pages'),
    '#options' => $options,
    '#default_value' => $rule->page_visibility,
  );
  $form['pages']['page_visibility_pages'] = array(
    '#type' => 'textarea',
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $rule->page_visibility_pages,
    '#description' => $description,
    '#rows' => 10,
  );
}

/**
 * Validation handler for the preset edit form.
 */
function js_injector_ctools_export_ui_form_validate($form, &$form_state) {
  // if preprocess is ticked, then ensure inline is not (they conflict)
  if ($form_state['values']['preprocess'] == 1) {
    $form_state['values']['inline'] = 0;
  }
  // Ensure the name field only contains URL safe characters.
  if (preg_match('/[^A-Za-z0-9_\-\ ]+/', $form_state['values']['name'])) {
    form_set_error('name', t('Illegal characters found in the friendly name, please remove them.'));
  }
}

/**
 * Submit handler for the preset edit form.
 */
function js_injector_ctools_export_ui_form_submit($form, &$form_state) {
  // not used
}
