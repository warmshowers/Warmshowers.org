<?php


/**
 * Build and display a report of periodically executed views
 */
function views_periodic_execution_report() {
  // This is the base plugin class for our display:
  $display_type = 'views_periodic_execution_plugin_display';
  
  $rows = array();
  $header = array(
    t('View'),
    t('Display'),
    t('Execution interval'),
    t('Last execution'),
    t('View active/inactive'),
    t('Operations'),
  );

  $executor_views = array();
  $views = views_get_all_views();

  foreach ($views as $view) {
    $displays = array();
    foreach ($view->display as $id => $display) {
      $display_plugin = views_fetch_plugin_data('display', $display->display_plugin);
      if (is_subclass_of($display_plugin['handler'], $display_type)) {

        $key = empty($views[$view->name]->disabled) ? 'A' : 'D';
        $key .= ':' . $view->name;
        $key .= ':' . $id;

        $executor_views[$key] = $view;

      }
    }
  }
  asort($executor_views);

  foreach ($executor_views as $key => $view) {
    list($active, $view_name, $display_id) = explode(':', $key, 3);
    $row = array(
      $view_name,
    );

    $view->set_display($display_id);
    $row[] = t('@display_title (%display_id)', array(
      '@display_title' => $view->display_handler->display->display_title,
      '%display_id' => $display_id,
    ));

    // Add the execution interval:
    $interval = $view->display_handler->get_interval();
    if ($interval > 0) {
      $row[] = format_interval($interval);
    }
    elseif ($interval == 0) {
      $row[] = t('As often as possible');
    }
    else {
      $row[] = t('Never automatically');
    }

    $last_run = $view->display_handler->get_last_execution_time();
    $row[] = t('@time ago', array('@time' => format_interval(time() - $last_run)));

    if ($active == 'A') {
      $row[] = t('Active');
    }
    else {
      $row[] = t('Inactive');
    }

    // Add the operations:
    $operations = array();

    $operations[] = l(t('Edit'), 'admin/structure/views/view/' . $view_name . '/edit/' . $display_id, array('query' => array('destination' => $_GET['q'])));

    $operations[] = l(t('Reset execution time'), 'admin/reports/views_periodic_execution/reset_time/' . $view_name . '/' . $display_id , array('query' => array('token' => drupal_get_token('views_periodic_execution_reset'), 'destination' => $_GET['q'])));

    $operations[] = l(t('Execute now'), 'admin/reports/views_periodic_execution/execute/' . $view_name . '/' . $display_id , array('query' => array('token' => drupal_get_token('views_periodic_execution_execute'), 'destination' => $_GET['q'])));
    
    // Cheap and cheerful, but it works:
    $row[] = implode(' | ', $operations);

    $rows[] = $row;
  }


  return theme('table', array('header' => $header, 'rows' => $rows));
}

function views_periodic_execution_reset_time($view_name, $display_id) {
  // Check the token in the query string:
  if (!drupal_valid_token($_GET['token'], 'views_periodic_execution_reset')) {
    return drupal_access_denied();
  }

  views_periodic_execution_view_last_executed_time_set($view_name, $display_id, 0);
  drupal_set_message(t('Last execution time set to zero.'));
  drupal_goto('admin/reports/views_periodic_execution');
}

function views_periodic_execution_execute($view_name, $display_id) {
  // Check the token in the query string:
  if (!drupal_valid_token($_GET['token'], 'views_periodic_execution_execute')) {
    return drupal_access_denied();
  }
  if (($view = views_get_view($view_name)) && isset($view->display[$display_id])) {
    $view->execute_display($display_id);
    drupal_set_message(t('View executed.'));
  }
  else {
    return drupal_not_found();
  }


  drupal_goto('admin/reports/views_periodic_execution');
}
