<?php

/**
 * @file
 * Stock administration menu items.
 */

/**
 * Form builder for stock settings form.
 *
 * @ingroup forms
 */
function uc_stock_settings_form($form, &$form_state) {
  $form['uc_stock_threshold_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email notification when stock level reaches its threshold value'),
    '#default_value' => variable_get('uc_stock_threshold_notification', FALSE),
  );

  $form['uc_stock_threshold_notification_recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification recipients'),
    '#default_value' => variable_get('uc_stock_threshold_notification_recipients', uc_store_email()),
    '#description' => t('The list of comma-separated email addresses that will receive the notification.'),
  );

  $form['uc_stock_threshold_notification_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject'),
    '#default_value' => variable_get('uc_stock_threshold_notification_subject', uc_get_message('uc_stock_threshold_notification_subject')),
  );

  $form['uc_stock_threshold_notification_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message text'),
    '#default_value' => variable_get('uc_stock_threshold_notification_message', uc_get_message('uc_stock_threshold_notification_message')),
    '#description' => t('The message the user receives when the stock level reaches its threshold value.'),
    '#rows' => 10,
  );

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#markup' => theme('token_tree', array('token_types' => array('uc_order', 'uc_stock', 'node', 'site', 'store'))),
    );
  }

  return system_settings_form($form);
}

/**
 * Displays a stock report for products with stock tracking enabled.
 */
function uc_stock_report() {
  $page_size = (isset($_GET['nopage'])) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $csv_rows = array();
  $rows = array();

  $header = array(
    array('data' => t('SKU'), 'field' => 'sku', 'sort' => 'asc'),
    array('data' => t('Product'), 'field' => 'title'),
    array('data' => t('Stock'), 'field' => 'stock'),
    array('data' => t('Threshold'), 'field' => 'threshold'),
    array('data' => t('Operations')),
  );

  $csv_rows[] = array(t('SKU'), t('Product'), t('Stock'), t('Threshold'));

  $query = db_select('uc_product_stock', 's')->extend('PagerDefault')->extend('TableSort')
    ->orderByHeader($header)
    ->limit($page_size)
    ->fields('s', array(
      'nid',
      'sku',
      'stock',
      'threshold',
    ));

  $query->leftJoin('node', 'n', 's.nid = n.nid');
  $query->addField('n', 'title');
  $query->condition('active', 1)
    ->condition('title', '', '<>');


  if (arg(4) == 'threshold') {
    $query->where('threshold >= stock');
  }

  $result = $query->execute();
  foreach ($result as $stock) {
    $op = array();
    if (user_access('administer product stock')) {
      $op[] = l(t('edit'), 'node/' . $stock->nid . '/edit/stock', $options = array('query' => array('destination' => 'admin/store/reports/stock')));
    }

    // Add the data to a table row for display.
    $rows[] = array(
      'data' => array(
        array('data' => $stock->sku),
        array('data' => l($stock->title, 'node/' . $stock->nid)),
        array('data' => $stock->stock),
        array('data' => $stock->threshold),
        array('data' => implode(' ', $op)),
      ),
      'class' => array(($stock->threshold >= $stock->stock) ? 'uc-stock-below-threshold' : 'uc-stock-above-threshold'),
    );

    // Add the data to the CSV contents for export.
    $csv_rows[] = array($stock->sku, $stock->title, $stock->stock, $stock->threshold);
  }

  module_load_include('inc', 'uc_reports', 'uc_reports.admin');
  $csv_data = uc_reports_store_csv('uc_stock', $csv_rows);

  $build['form'] = drupal_get_form('uc_stock_report_form');
  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-stock-table')),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );

  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
    '#suffix' => '&nbsp;&nbsp;&nbsp;',
  );

  if (isset($_GET['nopage'])) {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show paged records'), 'admin/store/reports/stock'),
    );
  }
  else {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show all records'), 'admin/store/reports/stock', array('query' => array('nopage' => '1'))),
    );
  }

  return $build;
}

/**
 * Form builder for stock report threshold filter.
 *
 * @see uc_stock_report_form_submit()
 * @ingroup forms
 */
function uc_stock_report_form($form, &$form_state) {
  $form['threshold'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show SKUs that are below their threshold.'),
    '#default_value' => arg(4) == 'threshold' ? TRUE : FALSE,
    '#attributes' => array('onchange' => 'this.form.submit();'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#attributes' => array('style' => "display:none;"),
  );

  return $form;
}

/**
 * Form submission handler for uc_stock_report_form().
 *
 * @see uc_stock_report_form()
 */
function uc_stock_report_form_submit($form, &$form_state) {
  if ($form_state['values']['threshold']) {
    drupal_goto('admin/store/reports/stock/threshold');
  }
  else {
    drupal_goto('admin/store/reports/stock');
  }
}

/**
 * Form builder for product stock edit form.
 *
 * @see uc_stock_edit_form_submit()
 * @see theme_uc_stock_edit_form()
 * @ingroup forms
 */
function uc_stock_edit_form($form, &$form_state, $node) {
  drupal_set_title($node->title);

  $form['stock'] = array('#tree' => TRUE);

  $skus = uc_product_get_models($node->nid);

  // Remove 'Any'.
  unset($skus[NULL]);

  if (!$skus) {
    drupal_set_message(t('No SKU found.'), 'error');
  }
  else {
    foreach (array_values($skus) as $id => $sku) {
      $stock = db_query("SELECT * FROM {uc_product_stock} WHERE sku = :sku", array(':sku' => $sku))->fetchAssoc();

      $form['stock'][$id]['sku'] = array(
        '#type' => 'value',
        '#value' => $sku,
      );

      // Checkbox to mark this as active.
      $form['stock'][$id]['active'] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($stock['active']) ? $stock['active'] : 0,
      );

      // Sanitized version of the SKU for display.
      $form['stock'][$id]['display_sku'] = array(
        '#markup' => check_plain($sku),
      );

      // Textfield for entering the stock level.
      $form['stock'][$id]['stock'] = array(
        '#type' => 'textfield',
        '#default_value' => !empty($stock['stock']) ? $stock['stock'] : 0,
        '#maxlength' => 9,
        '#size' => 9,
      );

      // Textfield for entering the threshold level.
      $form['stock'][$id]['threshold'] = array(
        '#type' => 'textfield',
        '#default_value' => !empty($stock['threshold']) ? $stock['threshold'] : 0,
        '#maxlength' => 9,
        '#size' => 9,
      );
    }
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Form submission handler for uc_stock_edit_form().
 *
 * @see uc_stock_edit_form()
 * @see theme_uc_stock_edit_form()
 */
function uc_stock_edit_form_submit($form, &$form_state) {
  foreach (element_children($form_state['values']['stock']) as $id) {
    $stock = $form_state['values']['stock'][$id];

    db_merge('uc_product_stock')
      ->key(array('sku' => $stock['sku']))
      ->updateFields(array(
        'active' => $stock['active'],
        'stock' => $stock['stock'],
        'threshold' => $stock['threshold'],
      ))
      ->insertFields(array(
        'sku' => $stock['sku'],
        'active' => $stock['active'],
        'stock' => $stock['stock'],
        'threshold' => $stock['threshold'],
        'nid' => $form_state['values']['nid'],
      ))
      ->execute();
  }

  drupal_set_message(t('Stock settings saved.'));
}

/**
 * Returns HTML for uc_stock_edit_form().
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see uc_stock_edit_form()
 * @see uc_stock_edit_form_submit()
 * @ingroup themeable
 */
function theme_uc_stock_edit_form($variables) {
  $form = $variables['form'];
  drupal_add_js('misc/tableselect.js');

  $header = array(
    array('data' => '&nbsp;&nbsp;' . t('Active'), 'class' => array('select-all')),
    array('data' => t('SKU')),
    array('data' => t('Stock')),
    array('data' => t('Threshold')),
  );
  $rows = array();

  foreach (element_children($form['stock']) as $id) {
    $rows[] = array(
      array('data' => drupal_render($form['stock'][$id]['active'])),
      array('data' => drupal_render($form['stock'][$id]['display_sku'])),
      array('data' => drupal_render($form['stock'][$id]['stock'])),
      array('data' => drupal_render($form['stock'][$id]['threshold'])),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows)) . drupal_render_children($form);
}
