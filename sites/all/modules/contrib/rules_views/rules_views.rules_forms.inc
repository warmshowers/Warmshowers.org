<?php


/**
 * @file
 * Rules configuration forms for rules_views module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Condition: Views results comparision configuration form
 */
function rules_views_condition_number_results_compare_form($settings = array(), &$form) {
  $form['settings']['description'] = array(
    '#type' => 'markup',
    '#weight' => -10,
    '#value' => t('Select the view, it will be executed and its results counted and compared with the given number.'),
  );
  _rules_views_rules_view_form($settings, $form, array());
  $form['settings']['number']['#weight'] = 2;

  $form['settings']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#options' => array('greater' => t('Greater than'), 'equal' => t('Equal to'), 'less' => t('Less than')),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : 'equal',
  );
}

/**
 * A form to configure the views_rules_actions_render_view.
 *
 * We add elements to pick the view and choose the arguments to pass to the
 * view.
 */
function rules_views_rules_action_render_view_form($settings, &$form, $form_state) {
  _rules_views_rules_view_form($settings, $form, $form_state);
}

/**
 * Central place to provide a form for specifying a view and arguments.
 */
function _rules_views_rules_view_form($settings, &$form, $form_state) {

  $views = array();
  $views_objs = views_get_all_views();
  foreach ($views_objs as $view_ob) {
    // Add the view to the $views array:
    foreach (array_keys($view_ob->display) as $display_id) {
      $views[$view_ob->name][$view_ob->name . ':' . $display_id] = $view_ob->display[$display_id]->display_title;
    }
  }

  // compute the default value for the view selector:
  $default_view = '';
  if (isset($settings['view_name']) && isset($settings['display_id'])) {
    $default_view = $settings['view_name'] . ':' . $settings['display_id'];
  }

  $form['settings']['view'] = array(
    '#type' => 'select',
    '#title' => t('View to render'),
    '#description' => t("Select the display of the view that you would like to use."),
    '#options' => $views,
    '#default_value' => $default_view,
    '#weight' => -10,
  );

  $form['settings']['view_arguments'] = array(
    '#type' => 'textfield',
    '#title' => t('View arguments'),
    '#description' => t('Separate arguments with a / as though they were a URL path.'),
    '#default_value' => isset($settings['view_arguments']) ? $settings['view_arguments'] : '',
  );

}

/**
 * Submit callback for rules_views_rules_action_render_view_form().
 *
 * Splits the selected view and display into the constituent parts and saves
 * those.
 */
function rules_views_rules_action_render_view_submit(&$settings, $form, $form_state) {
  // Explode the selected view into its view_name and display_id:
  list($settings['view_name'], $settings['display_id']) = explode(':', $settings['view'], 2);
  // Don't save the combined selection:
  unset($settings['view']);
}

/**
 * Submit callback for rules_views_condition_number_results_compare_form().
 *
 * Splits the selected view and display into the constituent parts and saves
 * those.
 */
function rules_views_condition_number_results_compare_submit(&$settings, $form, $form_state) {
  // Explode the selected view into its view_name and display_id:
  list($settings['view_name'], $settings['display_id']) = explode(':', $settings['view'], 2);
  // Don't save the combined selection:
  unset($settings['view']);
}

/**
 * @}
 */
