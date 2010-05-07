<?php
// $Id: views_service.inc,v 1.1.2.12.2.4 2010/02/20 05:13:29 heyrocker Exp $
/**
 * @file
 *  Link general views functionalities to services module.
 */

/**
 * Get a view from the database.
 *
 * @param $view_name
 *   String. The views name.
 * @param $display_id
 *   String (optional). The display name.
 * @param $offset
 *   Integer (optional). An offset integer for paging.
 * @param $limit
 *   Integer (optional). A limit integer for paging.
 * @param $args
 *   Array (optional). A list of arguments.
 * @param format_output
 *   Boolean (optional). TRUE if view should be formatted using the defined
 *   style plugin.
 * @return
 *   Array. The views return.
 */
function views_service_get($view_name, $display_id = 'default', $args = array(), $offset = 0, $limit = 0, $format_output = FALSE) {
  $result = array();
  $view = views_get_view($view_name);

  // Put all arguments and then execute.
  $view->set_arguments($args, FALSE);
  $view->set_offset($offset);
  // If offset is set we can't have a user pager.
  if (empty($offset)) {
    $view->set_use_pager(TRUE);
    $view->set_items_per_page($limit);
  }
  else {
    // Disable the user pager.
    $view->set_use_pager(FALSE);
  }
  if (!$format_output) {
    $view->set_display($display_id);
    $view->execute();
    $result = $view->result;

    $row_plugin = $view->display_handler->get_option('row_plugin');
    // If row plugin is node, then we should load each node
    if ($row_plugin == 'node') {
      $nodes = array();
      foreach ($view->result as $row) {
        $nodes[] = services_node_load(node_load($row->nid));
      }
      $result = $nodes;
    }
  }
  else {
    // We want to keep the result an array.
    $result[] = $view->preview($display_id);
  }
  return $result;
}

/**
 * Check the access permission to a given views.
 *
 * @param view_name
 *   String. The views name.
 * @return
 *   Boolean. TRUE if the user is allowed to load the given view.
 */
function views_service_get_access($view_name) {
  $view = views_get_view($view_name);
  if (empty($view)) {
    return FALSE;
  }
  return $view->access('default');
}

/**
 * Export a view.
 *
 * @param view_name
 *   String. The views name.
 * @return
 *   Array. The view object.
 */
function views_service_export($view_name) {
  $view = views_get_view($view_name);
  if (empty($view)) {
    return services_error('View does not exist.', 404);
  }

  return $view->export();
}

/**
 * Import a view.
 *
 * @param $view_import
 *   Array. The view object.
 * @param $view_name
 *   String (optional). The view name.
 * @return
 *   Number. The new view ID
 */
function views_service_import($view_import, $view_name = '') {
  // Include the necessary files
  require_once drupal_get_path('module', 'views') .'/includes/admin.inc';
  // If this view exists, delete it so it can be re-imported.
  $existing_view = views_get_view($view_name);
  if ($existing_view) {
    $existing_view->delete();

    // This forces the static cache in views_get_view() to be reset.
    $existing_view = views_get_view($view_name, TRUE);
  }
  // Import the the views using the same form as in-site import
  $form_state['values']['name'] = $view_name;
  $form_state['values']['view'] = $view_import;
  $form_state['values']['op'] = t('Import');
  drupal_execute('views_ui_import_page', $form_state);

  // Check if there is a any error
  if ($errors = form_set_error()) {
    return services_error($errors, 406);
  }

  // At this point, the new view was only cached and now its time
  // to save it and return the new View ID
  $view = $form_state['view'];
  $view->save();
  return $view->vid;
}
