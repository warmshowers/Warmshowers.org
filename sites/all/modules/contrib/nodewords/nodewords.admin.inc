<?php
// $Id: nodewords.admin.inc 552 2009-10-26 21:29:47Z rfay $

/**
 * @file
 * Assign META tags to nodes, vocabularies, terms and pages.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/*****************************************************************************
 * Menu callbacks / form builders, submit/validate functions.
 ****************************************************************************/

/**
 * Show the confirmation form for the page meta tags delete operation.
 */
function nodewords_pages_confirm_delete(&$form_state, $page) {
  $form['path'] = array(
    '#type' => 'value',
    '#value' => $page->path,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the meta tags for %path?', array('%path' => $page->path)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/nodewords/global/other',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 *
 */
function nodewords_pages_confirm_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    nodewords_delete_tags('page', $form_state['values']['path']);
  }

  if (db_affected_rows()) {
    drupal_set_message(t('The configuration options have been saved.'), 'status');
  }

  $form_state['redirect'] = 'admin/content/nodewords/global/other';
}

/**
 * Return the list of pages with custom meta tags settings.
 */
function nodewords_pages_overview() {
  $enabled = array();
  $form = array('#tree' => TRUE);
  $pages = array();

  foreach (_nodewords_get_pages_data() as $pid => $page) {
    $pages[$pid] = '';

    if ($page->enabled) {
      $enabled[] = $pid;
    }

    $form['path'][$pid] = array(
      '#type' => 'hidden',
      '#value' => $page->path,
      '#prefix' => check_plain($page->path),
    );

    $form['weight'][$pid] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => $page->weight,
    );

    $form['edit'][$pid] = array(
      '#value' => l(t('edit'), "admin/content/nodewords/global/other/edit/$page->pid"),
    );
    $form['delete'][$pid] = array(
      '#value' => l(t('delete'), "admin/content/nodewords/global/other/delete/$page->pid"),
    );
  }

  $form['enabled'] = array(
    '#type' => 'checkboxes',
    '#options' => $pages,
    '#default_value' => $enabled,
    '#checkall' =>'nodewords-pages-overview-enabled',
  );

  if (count($pages)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save')
    );
  }

  return $form;
}

function nodewords_pages_overview_submit($form, &$form_state) {
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
    nodewords_cache_clear();
  }
}

/**
 *
 */
function nodewords_pages_edit($form_state, $page = NULL) {
  if (!isset($page)) {
    $page = new stdClass();
    $page->path = '';
    $page->weight = 0;
    $page->enabled = 1;
    $page->tags = array();
  }
  else {
    $form['pid'] = array(
      '#type' => 'value',
      '#value' => $page->pid,
    );
  }

  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('Enter the Drupal path of the page. The '*' character is a wildcard. Example paths are <em>blog</em> for the blog page and <em>blog/*</em> for every personal blog.'),
    '#default_value' => $page->path,
    '#size' => 60,
    '#maxlength' => 255,
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
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $page->enabled,
  );

  $form['nodewords'] = nodewords_form(
    'page',
    $page->tags,
    array(
      'page:administrative' => TRUE,
      'fieldset' => FALSE,
    )
  );

  if (empty($form['nodewords'])) {
    $message = (count(array_filter(variable_get('nodewords_edit', array()))) == 0 ?
      t('No meta tags have been enabled for editing; enable them on the <a href="@settings_page">settings page</a>.', array('@settings_page' => url('admin/content/nodewords/settings'))) :
      t('There are no modules that define meta tags.')
    );

    $form['nodewords'] = array(
      '#type' => 'fieldset',
    );

    $form['nodewords']['message'] = array(
      '#value' => $message,
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }
  else {
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
function nodewords_pages_edit_validate($form, &$form_state) {
  if ($path = trim($form_state['values']['path'])) {
    if ($path[0] == '/') {
      form_set_error('path', t('The path must not start with a slash.'));
      return;
    }

    $bool = db_result(
      db_query("SELECT 1 FROM {nodewords_custom}
        WHERE path = '%s' AND pid <> %d", $path,
        isset($form_state['values']['pid']) ? $form_state['values']['pid'] : 0
      )
    );

    if ($bool) {
      form_set_error('path', t('The path has been already used.'));
    }
  }
  else {
    form_set_error('path', t('The path must not contain only spaces.'));
  }
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_pages_edit_submit($form, &$form_state) {
  $form_state['values']['path'] = trim($form_state['values']['path']);

  nodewords_save_tags('page', $form_state['values']['path'], $form_state['values']['nodewords'], TRUE);
  drupal_write_record('nodewords_custom', $form_state['values'], isset($form_state['values']['pid']) ? 'pid' : NULL);

  nodewords_cache_clear();
  drupal_set_message(t('The configuration options have been saved.'));
  $form_state['redirect'] = 'admin/content/nodewords/global/other';
}

/**
 * Menu callback: settings form.
 */
function nodewords_settings_form() {
  $edit_options = array();
  $edit_tags = variable_get('nodewords_edit', array());
  $head_options = array();
  $head_tags = variable_get('nodewords_head', array());
  $tags_info = nodewords_get_possible_tags();

  foreach ($tags_info as $name => $info) {
    $edit_options[$name] = $info['widget:label'];
    $head_options[$name] = $info['widget:label'];
  }

  uksort($edit_options, 'strnatcmp');
  uksort($head_options, 'strnatcmp');

  $form['edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tags to show on edit forms'),
    '#description' => t('Select the meta tags you want to be able to edit on the edit page of nodes, terms and vocabularies.'),
    '#collapsible' => TRUE,
    '#collapsed' => count(array_filter($edit_tags)) != 0,
  );

  $form['edit']['nodewords_edit'] = array(
    '#type' => 'checkboxes',
    '#options' => $edit_options,
    '#default_value' => $edit_tags,
    '#checkall' => TRUE,
  );

  $form['head'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tags to output in HTML'),
    '#description' => t('Select the meta tags you want to appear in the HEAD section of the HTML pages.'),
    '#collapsible' => TRUE,
    '#collapsed' => count(array_filter($head_tags)) != 0,
  );

  $form['head']['nodewords_head'] = array(
    '#type' => 'checkboxes',
    '#options' => $head_options,
    '#default_value' => $head_tags,
    '#checkall' => TRUE,
  );

  if (function_exists('taxonomy_get_vocabularies')) {
    $options = array();

    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $options[$vocabulary->vid] = check_plain($vocabulary->name);
    }

    if ($count = count($options)) {
      $form['taxonomy'] = array(
        '#type' => 'fieldset',
        '#title' => t('Taxonomy'),
        '#collapsible' => TRUE,
      );
      $form['taxonomy']['nodewords_keyword_vids'] = array(
        '#type' => $count > 10 ? 'select' : 'checkboxes',
        '#title' => t('Auto-keywords vocabularies'),
        '#description' => t('Select the vocabularies which contain terms you want to add to the keywords meta tag for nodes. The terms of these vocabularies are added before the global keywords but after the page-specific keywords.'),
        '#default_value' => variable_get('nodewords_keyword_vids', array()),
        '#options' => $options,
        '#multiple' => TRUE,
      );

      if ($count > 10) {
        $form['taxonomy']['nodewords_keyword_vids']['#multiple'] = TRUE;
      }
      elseif ($count > 2) {
        // Add support for Check All if the checkboxes are more than two.
        $form['taxonomy']['nodewords_keyword_vids']['#checkall'] = TRUE;
      }
    }
  }

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
  );

  $form['advanced']['nodewords_enable_user_metatags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the user profile meta tags'),
    '#default_value' => variable_get('nodewords_enable_user_metatags', TRUE),
  );

  $form['advanced']['nodewords_list_repeat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Repeat meta tags for lists'),
    '#description' => t('Some search engines punish sites that use the same meta tags on different pages. Uncheck this option if you want to suppress the repetition of the same meta tags on pages that use the pager - if unchecked, Drupal will only display the meta tags for the first page and not for subsequent pages. If unsure, select this option.'),
    '#default_value' => variable_get('nodewords_list_repeat', FALSE),
  );

  $form['advanced']['nodewords_use_frontpage_tags'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use front page meta tags'),
    '#description' => t('Check this option if you want to use the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a> even if the <a href="@site-settings-url" title="Site information">default front page</a> specified is a view, panel or node - in this case, the meta tags specified for the view, panel or node will be ignored. If you want to use the meta tags of the view, panel or node instead, uncheck this option. If unsure, select this option and specify the meta tags you want on the <a href="@front-page-url" title="Meta tags for front page">meta tags for the front page</a>.', array('@front-page-url' => url('admin/content/nodewords/global/frontpage'), '@site-settings-url' => url('admin/settings/site-information'))),
    '#default_value' => variable_get('nodewords_use_frontpage_tags', TRUE),
  );

  $form['advanced']['nodewords_collapse_fieldset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show the meta tags fieldset as collapsed'),
    '#description' => t('Check this option if you want to always show the meta tags fieldset as collapsed.'),
    '#default_value' => variable_get('nodewords_collapse_fieldset', FALSE),
  );

  $form['advanced']['nodewords_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum meta tags lenght'),
    '#description' => t('The maximum lenght to use for the meta tags form fields.'),
    '#default_value' => variable_get('nodewords_max_size', 255),
    '#size' => 6,
    '#maxlength' => 6,
  );

  foreach ($tags_info as $name => $info) {
    $function = $info['tag:function:prefix'] . '_settings_form';
    $options = array(
      'parameters' => !empty($info['tag:function:parameters']) ? $info['tag:function:parameters'] : array(),
    );

    if (function_exists($function)) {
      $function($form, 'advanced_settings', $options);
    }
  }

  return system_settings_form($form);
}

/**
 * Front page settings form.
 */
function nodewords_tags_form(&$form_state, $type = 'default', $id = '') {
  $form['#nodewords_type'] = $type;
  $form['#nodewords_id'] = $id;

  $form['nodewords'] = nodewords_form(
    $type,
    nodewords_load_tags($type, $id),
    array(
      'page:administrative' => TRUE,
      'fieldset' => FALSE,
    )
  );

  if (empty($form['nodewords'])) {
    $message = (count(array_filter(variable_get('nodewords_edit', array()))) == 0 ?
      t('No meta tags have been enabled for editing; enable them on the <a href="@settings_page">settings page</a>.', array('@settings_page' => url('admin/content/nodewords/settings'))) :
      t('There are no modules that define meta tags.')
    );

    $form['nodewords'] = array(
      '#type' => 'fieldset',
    );

    $form['nodewords']['message'] = array(
      '#value' => $message,
      '#prefix' => '<div>',
      '#suffix' => '</div>',
    );
  }
  else {
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
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

/*****************************************************************************
 * Theme functions.
 ****************************************************************************/

/**
 * Render the list of pages with meta tags.
 */
function theme_nodewords_pages_overview($form) {
  $has_pages = isset($form['path']) && is_array($form['path']);
  $rows = array();

  if ($has_pages) {
    foreach (element_children($form['path']) as $key) {
      $row = array();
      $row[] = drupal_render($form['path'][$key]);

      $form['weight'][$key]['#attributes']['class'] = 'page-weight';

      $row[] = drupal_render($form['weight'][$key]);
      $row[] = drupal_render($form['enabled'][$key]);
      $row[] = drupal_render($form['edit'][$key]);
      $row[] = drupal_render($form['delete'][$key]);

      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no meta tags currently defined.'),
        'colspan' => '4'
      )
    );
  }

  $header[] = t('Drupal path');
  $header[] = t('Weight');
  $header[] = array(
    'data' => t('Enabled'),
    'class' => 'nodewords-pages-overview-enabled',
  );
  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '2',
  );

  drupal_add_tabledrag('nodewords', 'order', 'sibling', 'page-weight');

  return theme('table', $header, $rows, array('id' => 'nodewords')) .
    drupal_render($form);
}

/**
 * @} End of "addtogroup nodewords" .
 */
