<?php


/**
 * @file
 * Rules actions for views
 *
 * Provides a rules action which renders a view into a variable
 *
 * @addtogroup rules
 * @{
 */


/**
 * Implementation of hook_rules_action_info().
 *
 * Provides a rules action which renders a view into a variable.
 *
 */
function rules_views_rules_action_info() {

  return array(
    'rules_views_rules_action_render_view' => array(
      'label' => t('Render a View to a variable'),
      'new variables' => array(
        'rendered_view' => array('type' => 'string', 'label' => t('Rendered view')),
      ),
      'eval input' => array('view_arguments'),
      'module' => 'Views',
    ),
  );
}

/**
 * Implementation of hook_rules_condition_info().
 *
 * Provides a condition for the number of results in a view.
 */
function rules_views_rules_condition_info() {
  return array(
    'rules_views_condition_number_results_compare' => array(
      'label' => t('Number of results from a view'),
      'arguments' => array(
        'number' => array('label' => t('Number'), 'type' => 'number'),
      ),
      'eval input' => array('view_arguments'),
      'help' => t('Select greater than, less than or equal to.'),
      'module' => 'Views',
    ),
  );
}

/**
 * Action Implementation: Render a View
 *
 * @param $view_name
 *   A string containing the name of the view to be rendered
 * @param $display_id
 *  A string containing the name of the view's display_id to be rendered (eg. "default" or "page" etc).
 * @param $view_arguments
 *  Arguments for the view
 * @return
 *  A string containing the rendered view. Depending on the view this might be HTML or text or anything templated
 */
function rules_views_rules_action_render_view($settings) {
  $view_name = $settings['view_name'];
  $display_id = $settings['display_id'];
  $view_arguments = explode('/', $settings['view_arguments']);

  // We need to call views_embed_view with an array of arguments, so add on the
  // $view_name and the $display_id to the beginning:
  array_unshift($view_arguments, $view_name, $display_id);

  if ($rendered_view = call_user_func_array('views_embed_view', $view_arguments)) {

    // Log successful view rendering only if debug is on
    if (variable_get('rules_debug', TRUE)) {
      watchdog('rules', "Rendered view ". $view_name .":". $display_id);
    }
    return array('rendered_view' => $rendered_view);
  }
  else {
    // Always log failed rendering
    watchdog('rules', "View Rendering failed for view ". $view_name .":". $display_id);
  }
}

/**
 * Condition implementation: Number of results from a view.
 */
function rules_views_condition_number_results_compare($number2, $settings) {

  // Get the number of results of the view:
  $view_name = $settings['view_name'];
  $display_id = $settings['display_id'];
  $view_arguments = explode('/', $settings['view_arguments']);


  // Load and execute the view:
  $view = views_get_view($view_name);
  $view->set_display($display_id);
  $view->preview = TRUE;
  $view->pre_execute($view_arguments);
  // Force the count query to run:
  $view->set_use_pager(TRUE);
  $view->set_items_per_page(1);

  $view->execute();
  $view->post_execute();

  // $view->total_rows has the total rows in this view:
  $number1 = $view->total_rows;

  // Now return the result proper:
  switch ($settings['operation']) {
    case 'greater':
      return $number1 > $number2;

    case 'equal':
      return $number1 == $number2;

    case 'less':
      return $number1 < $number2;
  }
}

/**
 * @}
 */
