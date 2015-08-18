<?php
/**
 * @file
 * Extensions to Rules UI.
 */

// Make Rules form functions available.
module_load_include('inc', 'rules', 'ui/ui.forms');

/**
 * Adds extra Rules controller callback.
 */
function _views_rules_rules_ui_menu_alter(&$items, $base_path, $base_count) {
  // Use existing "add" menu item as extension base.
  $add_item = $items[$base_path . '/manage/%rules_config/add/%rules_element'];

  // Extend "add".
  $items[$base_path . '/manage/%rules_config/add-view-loop/%rules_element'] = array(
    'title' => 'Add a new !plugin',
    'title callback' => 't',
    'title arguments' => array('!plugin' => t('view loop')),
    'page arguments' => array_filter(array('views_rules_add_view_loop')) + $add_item['page arguments'],
    'file' => 'rules/views_rules.rules_admin.inc',
    'file path' => drupal_get_path('module', 'views_rules'),
  ) + $add_item;
  $items[$base_path . '/manage/%rules_config/add-view-loop/%rules_element']['page arguments'][2] = 'view loop';
}

/**
 * Provides a variant of rules_ui_add_element() to present a view display
 * selector for creating a view loop.
 */
function views_rules_add_view_loop($form, &$form_state, $rules_config, $plugin_name, RulesContainerPlugin $parent, $base_path) {
  $cache = rules_get_cache();
  if (!isset($cache['plugin_info'][$plugin_name]['class'])) {
    drupal_not_found();
    exit;
  }

  // Build condition form.
  RulesPluginUI::$basePath = $base_path;
  if (!isset($form_state['rules_element'])) {
    RulesPluginUI::formDefaults($form, $form_state);
    $form_state += array('parent_element' => $parent, 'plugin' => $plugin_name);

    // Add view selector.
    $form['views_rules_display'] = array(
      '#type' => 'select',
      '#title' => t('Views iterator display'),
      '#options' => views_rules_list_iterators(),
      '#description' => t('Select the view display to use for looping. To add a display to the list, add a "Rules" display to a view.'),
      '#required' => TRUE,
      '#ajax' => rules_ui_form_default_ajax() + array(
        'trigger_as' => array('name' => 'continue'),
      ),
    );
    $form['continue'] = array(
      '#type' => 'submit',
      '#name' => 'continue',
      '#value' => t('Continue'),
      '#ajax' => rules_ui_form_default_ajax(),
    );
  }

  // Build element form.
  if (isset($form_state['rules_element'])) {
    $form_state['rules_element']->form($form, $form_state, array('button' => TRUE, 'init' => TRUE));
    $form['#validate'][] = 'rules_ui_edit_element_validate';
    $form['#submit'][] = 'rules_ui_edit_element_submit';
  }
  else {
    $form['#submit'][] = 'views_rules_add_view_loop_submit';
  }
  return $form;
}

/**
 * Provides a variant of rules_ui_add_element_submit() to create a view loop.
 */
function views_rules_add_view_loop_submit($form, &$form_state) {
  if (!isset($form_state['rules_element'])) {
    $display = $form_state['values']['views_rules_display'];
    list($viewName, $displayName) = explode(':', $display, 2);
    $element = views_rules_loop($viewName, $displayName);

    // Always add the new element at the bottom, thus set an appropriate weight.
    $iterator = $form_state['parent_element']->getIterator();
    if ($sibling = end($iterator)) {
      $element->weight = $sibling->weight + 1;
    }
    // Clear the element settings so they won't be processed on serialization as
    // there is nothing to be processed yet.
    $element->settings = array();
    $element->setParent($form_state['parent_element']);

    $form_state['rules_element'] = $element;
  }
  $form_state['rebuild'] = TRUE;
}
