<?php
/**
 * @file
* Administration interface for nodewords.module.
 */

/**
 * Show the confirmation form for the page meta tags delete operation.
 */
function nodewords_custom_pages_confirm_delete(&$form_state, $page) {
  $form['pid'] = array(
    '#type' => 'value',
    '#value' => $page->pid,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the meta tags for the page %name?', array('%name' => $page->name)),
    'admin/content/nodewords/meta-tags/custom',
    NULL,
    t('Delete')
  );
}

/**
 * Submit processor for the confirm-delete page.
 */
function nodewords_custom_pages_confirm_delete_submit($form, &$form_state) {
  nodewords_delete_tags(NODEWORDS_TYPE_PAGE, $form_state['values']['pid']);

  if (db_affected_rows()) {
    drupal_set_message(t('The configuration options have been saved.'), 'status');
  }

  $form_state['redirect'] = 'admin/content/nodewords/meta-tags/custom';
}

/**
 * Return the list of pages with custom meta tags settings.
 */
function nodewords_custom_pages_overview() {
  $enabled = array();
  $form = array('#tree' => TRUE);
  $pages = array();

  foreach (_nodewords_get_custom_pages_data() as $pid => $page) {
    $pages[$pid] = '';

    if ($page->enabled) {
      $enabled[] = $pid;
    }

    $form['name'][$pid] = array(
      '#value' => $page->name,
    );

    $form['path'][$pid] = array(
      '#type' => 'value',
      '#value' => $page->path,
    );

    $form['weight'][$pid] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $page->weight,
    );
  }

  $form['add_meta_tags'] = array(
    '#value' => t(' <a href="@add_metatags">Add meta tags</a> for a new page.', array('@add_metatags' => url('admin/content/nodewords/meta-tags/custom/add'))),
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );

  if (!empty($pages)) {
    $form['enabled'] = array(
      '#type' => 'checkboxes',
      '#options' => $pages,
      '#default_value' => $enabled,
      '#checkall' => count($pages) > 1 ? 'nodewords-pages-overview-enabled' : FALSE,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }

  return $form;
}

function nodewords_custom_pages_overview_submit($form, &$form_state) {
  $row = new stdClass();

  foreach ($form_state['values']['path'] as $pid => $value) {
    $row->pid = $pid;
    $row->path = $form_state['values']['path'][$pid];
    $row->weight = $form_state['values']['weight'][$pid];
    $row->enabled = $form_state['values']['enabled'][$pid];

    drupal_write_record('nodewords_custom', $row, 'pid');
    $done = TRUE;
  }

  if (isset($done)) {
    drupal_set_message(t('The configuration options have been saved.'));
  }
}

/**
 * Edit form for custom pages.
 */
function nodewords_custom_pages_edit($form_state, $page = NULL) {
  $form = array();

  if (!isset($page)) {
    $page = (object) array(
      'name' => t('Custom page'),
      'path' => '',
      'weight' => 0,
      'enabled' => 1,
      'tags' => array(),
    );
  }
  else {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $page->pid,
    );
  }

  $nodewords_form = nodewords_form(
    NODEWORDS_TYPE_PAGE,
    $page->tags,
    array(
      'fieldset' => FALSE,
    )
  );

  if (empty($nodewords_form)) {
    $form['warning'] = array(
      '#value' => t('You have not yet enabled any <a href="@url">meta tags for edit forms</a>.', array('@url' => url('admin/content/nodewords'))),
      '#prefix' => '<div class="messages warning">',
      '#suffix' => '</div>',
    );
    return $form;
  }
  else {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t("A name to better identify the page in the list of custom pages. The name must contains at least one alpha-numeric character."),
      '#default_value' => $page->name,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
    );

    $form['path'] = array(
      '#type' => 'textarea',
      '#title' => t('Path'),
      '#description' => t("Enter the Drupal paths associated with the page. The character <q>*</q> is a wildcard. Example paths are <em>blog</em> for the blog page and <em>blog/*</em> for every personal blog. The list must contain at least one path."),
      '#default_value' => $page->path,
      '#size' => 60,
      '#wysiwyg' => FALSE,
      '#required' => TRUE,
    );

    $form['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#description' => t('Pages with lower weight will be considered first. Only the first matching page will be used.'),
      '#delta' => 10,
      '#default_value' => $page->weight,
    );

    $form['enabled'] = array(
      '#type' => 'radios',
      '#options' => array(t('Disabled'), t('Enabled')),
      '#default_value' => $page->enabled,
    );

    $form['nodewords'] = $nodewords_form;
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 40,
    );
  }

  return $form;
}

/**
 * Validate function for the meta tags edit page.
 */
function nodewords_custom_pages_edit_validate($form, &$form_state) {
  if (trim($form_state['values']['name']) == '') {
    form_set_error('name', t('The name contains only spaces, and other not printable characters.'));
  }

  if (($path = trim($form_state['values']['path'])) != '') {
    $bool = db_result(
      db_query_range("SELECT 1 FROM {nodewords_custom} WHERE path = '%s' AND pid <> %d",
        $path,
        isset($form_state['values']['pid']) ? $form_state['values']['pid'] : 0,
        0, 1
      )
    );

    if ($bool) {
      form_set_error('path', t('The path is already used.'));
    }
  }
  else {
    form_set_error('path', t('The path contains only spaces, and other not printable characters.'));
  }
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_custom_pages_edit_submit($form, &$form_state) {
  $form_state['values']['path'] = trim($form_state['values']['path']);

  $row = new stdClass();

  if (isset($form_state['values']['pid'])) {
    $row->pid = $form_state['values']['pid'];
  }

  $row->name = $form_state['values']['name'];
  $row->path = $form_state['values']['path'];
  $row->weight = $form_state['values']['weight'];
  $row->enabled = $form_state['values']['enabled'];

  drupal_write_record('nodewords_custom', $row, isset($row->pid) ? 'pid' : array());
  nodewords_save_tags(NODEWORDS_TYPE_PAGE, $row->pid, $form_state['values']['nodewords'], TRUE);

  drupal_set_message(t('The configuration options have been saved.'));

  $form_state['values']['pid'] = $row->pid;
  $form_state['redirect'] = 'admin/content/nodewords/meta-tags/custom';
}

/**
 * Menu callback: settings form.
 */
function nodewords_settings_form() {
  $edit_tags = variable_get('nodewords_edit', array());
  $form = array();
  $options = array();
  $tags_info = nodewords_get_possible_tags();

  if (!empty($tags_info)) {
    foreach ($tags_info as $name => $info) {
      $options[$name] = $info['label'];
    }

    uasort($options, 'strnatcmp');
  }

  $form['edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags to show on edit forms'),
    '#description' => t('Select the meta tags that appear on edit forms. Users with the "administer meta tags" permission can edit all meta tags, regardless of the follow.'),
    '#collapsible' => TRUE,
    '#group' => 'nodewords',
  );

  if (!empty($tags_info)) {
    $form['edit']['nodewords_edit'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $edit_tags,
      '#checkall' => TRUE,
    );
  }
  else {
    $form['edit']['nodewords_edit'] = array(
      '#type' => 'value',
      '#value' => $edit_tags,
    );
  }

  $form['metatags_creation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags creation options'),
    '#description' => t('These options change the way the meta tags are created.'),
    '#collapsible' => TRUE,
    '#group' => 'nodewords',
  );

  $form['metatags_creation']['nodewords_enable_user_metatags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the user profile meta tags'),
    '#default_value' => variable_get('nodewords_enable_user_metatags', TRUE),
  );

  $form['metatags_creation']['nodewords_list_repeat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Repeat meta tags for lists'),
    '#description' => t('Some search engines punish sites that use the same meta tags on different pages. Uncheck this option if you want to suppress the repetition of the same meta tags on pages that use the pager - if unchecked, Drupal will only display the meta tags for the first page and not for subsequent pages. If unsure, select this option.'),
    '#default_value' => variable_get('nodewords_list_repeat', FALSE),
  );

  $form['metatags_creation']['nodewords_use_frontpage_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use front page meta tags'),
    '#description' => t('Check this option if you want to use the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a> even if the <a href="@site-settings-url" title="Site information">default front page</a> specified is a view, panel or node - in this case, the meta tags specified for the view, panel or node will be ignored. If you want to use the meta tags of the view, panel or node instead, uncheck this option. If unsure, select this option and specify the meta tags you want on the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a>.', array('@front-page-url' => url('admin/content/nodewords/meta-tags/frontpage'), '@site-settings-url' => url('admin/settings/site-information'))),
    '#default_value' => variable_get('nodewords_use_frontpage_tags', TRUE),
  );

  $default_size = variable_get('nodewords_max_size', 350);

  $form['metatags_creation']['nodewords_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum meta tags length'),
    '#description' => t('The maximum length to use for the meta tags form fields.'),
    '#default_value' => $default_size,
    '#required' => TRUE,
    '#element_validate' => array('nodewords_max_size_validate'),
    '#size' => 6,
    '#maxlength' => 6,
  );

  if ($default_size < 350) {
    $form['metatags_creation']['nodewords_max_size']['#description'] .= ' ' . t('Google Search now uses the first 350 characters of the meta tag DESCRIPTION 350 characters, which is not the maximum length actually set for the meta tags. See <a href="@google-blog">Official Google Blog: Two new improvements to Google results pages</a> for more information.', array('@google-blog' => 'http://googleblog.blogspot.com/2009/03/two-new-improvements-to-google-results.html'));
  }

  $form['metatags_creation']['nodewords_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#description' => t('Enter the base URL that will be used for any URL generated by the module.'),
    '#default_value' => variable_get('nodewords_base_url', ''),
    '#element_validate' => array('nodewords_base_url_validate'),
    '#size' => 60,
  );

  $form['metatags_creation']['nodewords_use_path_alias'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use URL aliases'),
    '#description' => t('By default URLs output in meta tags, e.g. the Canonical URL, will use the internal Drupal path, e.g. "node/123" and "taxonomy/term/123".  Enabling this option will instead make the URLs use the URL alias if one exists.  This option works best when the <a href="@url">GlobalRedirect</a> module is also used.', array('@url' => 'http://drupal.org/project/globalredirect')),
    '#default_value' => variable_get('nodewords_use_path_alias', TRUE),
  );

  foreach ($tags_info as $name => $info) {
    $function = $info['callback'] . '_settings_form';
    $options = array();

    if (function_exists($function)) {
      $function($form, 'nodewords_settings_form', $options);
    }
  }

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 100;

  return $form;
}

/**
 * Function to validate the base URL.
 */
function nodewords_base_url_validate($element, &$form_state) {
  if (!empty($element['#value']) && !($base_url = rtrim($element['#value'], '/'))) {
    form_error($element, t('Invalid base URL.'));
  }
  elseif (!empty($base_url) && !valid_url($base_url, TRUE)) {
    form_error($element, t('Invalid base URL.'));
  }
}

/**
 * Function to validate the regular expression.
 */
function nodewords_filter_regex_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $value = trim($element['#value']);

    if (empty($value)) {
      form_error($element, t('The regular expression contains only spaces, and other not printable characters.'));
    }
  }
}

/**
 * Function to validate the value entered for the maximum meta tags length.
 */
function nodewords_max_size_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $value = trim($element['#value']);

    if (empty($value) || (!empty($value) && (!is_numeric($value) || $value <= 0))) {
      form_error($element, t('The maximum length must be a number greater than zero.'));
    }
  }
}

/**
 * Front page settings form.
 */
function nodewords_tags_form(&$form_state, $type = NODEWORDS_TYPE_DEFAULT, $id = 0) {
  $form = array();
  $form['#nodewords_type'] = $type;
  $form['#nodewords_id'] = $id;

  $form['nodewords'] = nodewords_form(
    $type,
    nodewords_load_tags($type, $id),
    array(
      'fieldset' => FALSE,
    )
  );

  if (empty($form['nodewords'])) {
    $form['warning'] = array(
      '#value' => t('You have not yet enabled any <a href="@url">meta tags for edit forms</a>.', array('@url' => url('admin/content/nodewords'))),
      '#prefix' => '<div class="messages warning">',
      '#suffix' => '</div>',
    );
  }
  else {
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#submit' => array('nodewords_tags_form_submit'),
      '#weight' => 40,
    );

    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('nodewords_tags_form_reset'),
      '#weight' => 42,
    );
  }

  return $form;
}

/**
 * Submission function for the meta tags edit page (reset button).
 */
function nodewords_tags_form_reset($form, &$form_state) {
  nodewords_delete_tags($form['#nodewords_type'], $form['#nodewords_id']);
  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_tags_form_submit($form, &$form_state) {
  nodewords_save_tags($form['#nodewords_type'], $form['#nodewords_id'], $form_state['values']['nodewords'], TRUE);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Render the list of pages with meta tags.
 */
function theme_nodewords_custom_pages_overview($form) {
  $has_pages = isset($form['name']) && is_array($form['name']);
  $rows = array();

  if ($has_pages) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['name'][$key]);

      $form['weight'][$key]['#attributes']['class'] = 'page-weight';

      $row[] = drupal_render($form['weight'][$key]);
      $row[] = drupal_render($form['enabled'][$key]);
      $row[] = l(t('edit'), "admin/content/nodewords/meta-tags/custom/$key/edit");
      $row[] = l(t('delete'), "admin/content/nodewords/meta-tags/custom/$key/delete");

      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are currently no meta tags defined.'),
        'colspan' => '5',
      )
    );
  }

  $header[] = t('Name');
  $header[] = t('Weight');

  $header[] = array(
    'data' => t('Enabled'),
    'class' => count($rows) > 1 ? 'nodewords-pages-overview-enabled' : '',
  );

  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '2',
  );

  drupal_add_tabledrag('nodewords', 'order', 'sibling', 'page-weight');

  return theme('table', $header, $rows, array('id' => 'nodewords')) . drupal_render($form);
}

/**
 * Add the fieldset for the generation settings to the form passed as argument.
 *
 * @param &$form
 *  The form to which add the fieldset. If the fieldset has been already added,
 *  then it will not be added.
 */
function nodewords_add_generation_fieldset(&$form) {
  if (!isset($form['generation'])) {
    $form['generation'] = array(
    );
  }
}


/**
 * Internal functions
 */


/**
 *
 */
function _nodewords_mass_delete_batch_process($ids, $type, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($ids);
    $context['sandbox']['ids'] = $ids;
  }

  $count = min(5, count($context['sandbox']['ids']));

  if (!isset($context['results']['count'])) {
    $context['results']['count'] = 0;
  }

  for ($i = 1; $i <= $count; $i++) {
    $id = array_shift($context['sandbox']['ids']);

    db_query("DELETE FROM {nodewords} WHERE type = %d AND id = %d", $type, $id);

    if ($type == NODEWORDS_TYPE_PAGE) {
      db_query("DELETE FROM {nodewords_custom} WHERE pid = %d", $type, $id);
    }

    $context['results']['count']++;
    $context['sandbox']['progress']++;
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 *
 */
function _nodewords_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');

    $message = format_plural(
      $results['count'],
      '1 item successfully processed.',
      '@count items successfully processed.'
    );

    drupal_set_message($message);
  }
}
