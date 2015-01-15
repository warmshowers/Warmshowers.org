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
function uc_stock_settings_form() {
  $form['uc_stock_threshold_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send email notification when stock level reaches its threshold value'),
    '#default_value' => variable_get('uc_stock_threshold_notification', FALSE),
  );

  $form['uc_stock_threshold_notification_recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Notification recipients'),
    '#default_value' => variable_get('uc_stock_threshold_notification_recipients', uc_store_email()),
    '#description' => t('The list of comma separated email addresses that will receive the notification.'),
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
    '#description' => t('The message the user receives when the stock level reaches its threshold value (uses <a href="!token-help-page">global, order, product and stock tokens</a>).', array('!token-help-page' => url('admin/store/help/tokens'))),
    '#rows' => 10,
  );

  return system_settings_form($form);
}

/**
 * Displays a stock report for products with stock tracking enabled.
 */
function uc_stock_report() {
  drupal_add_css(drupal_get_path('module', 'uc_stock') .'/uc_stock.css');

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

  $sql = "SELECT s.nid, sku, title, stock, threshold FROM {uc_product_stock} as s LEFT JOIN {node} as n ON s.nid = n.nid WHERE active = 1 AND title <> ''";
  if (arg(4) == 'threshold') {
    $sql .= ' AND threshold >= stock';
  }

  $result = pager_query($sql . tablesort_sql($header), $page_size, 0, NULL);
  while ($stock = db_fetch_object($result)) {
    $op = array();
    if (user_access('administer product stock')) {
      $op[] = l(t('edit'), 'node/'. $stock->nid .'/edit/stock', $options = array('query' => 'destination=admin/store/reports/stock'));
    }

    // Add the data to a table row for display.
    $rows[] = array(
      'data' => array(
        array('data' => $stock->sku),
        array('data' => l($stock->title, 'node/'. $stock->nid)),
        array('data' => $stock->stock),
        array('data' => $stock->threshold),
        array('data' => implode(' ', $op)),
      ),
      'class' => ($stock->threshold >= $stock->stock) ? 'uc-stock-below-threshold' : 'uc-stock-above-threshold',
    );

    // Add the data to the CSV contents for export.
    $csv_rows[] = array($stock->sku, $stock->title, $stock->stock, $stock->threshold);
  }

  $csv_data = uc_reports_store_csv('uc_stock', $csv_rows);

  $output = drupal_get_form('uc_stock_report_form')
          . theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-stock-table'))
          . theme('pager', NULL, $page_size);
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'&nbsp;&nbsp;&nbsp;'. ((isset($_GET['nopage'])) ? l(t('Show paged records'), 'admin/store/reports/stock') : l(t('Show all records'), 'admin/store/reports/stock', $options = array('query' => 'nopage=1'))) .'</div>';
  return $output;
}

/**
 * Form builder for stock report threshold filter.
 *
 * @see uc_stock_report_form_submit()
 * @ingroup forms
 */
function uc_stock_report_form() {
  $form['threshold'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only show SKUs that are below their threshold.'),
    '#default_value' => arg(4) == 'threshold' ? TRUE : FALSE,
    '#attributes' => array('onchange' => 'this.form.submit();'),
  );

  $form['submit'] = array(
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
 * @see theme_uc_stock_edit_form()
 * @see uc_stock_edit_form_submit()
 * @ingroup forms
 */
function uc_stock_edit_form($form_state, $node) {
  drupal_set_title(check_plain($node->title));

  $form = array();
  $form['stock'] = array('#tree' => TRUE);

  $skus = uc_product_get_models($node->nid);

  // Remove 'Any'.
  unset($skus[NULL]);

  if (!$skus) {
    drupal_set_message(t('No SKU found.'), 'error');
  }
  else {
    foreach (array_values($skus) as $id => $sku) {
      $stock = db_fetch_array(db_query("SELECT * FROM {uc_product_stock} WHERE sku = '%s'", $sku));

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
        '#value' => check_plain($sku),
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

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

/**
 * Returns HTML for uc_stock_edit_form().
 *
 * @see uc_stock_edit_form()
 * @see uc_stock_edit_form_submit()
 * @ingroup themeable
 */
function theme_uc_stock_edit_form($form) {
  $header = array(
    array('data' => '&nbsp;&nbsp;'. t('Active')) + theme('table_select_header_cell'),
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

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Form submission handler for uc_stock_edit_form().
 *
 * @see uc_stock_edit_form()
 * @see theme_uc_stock_edit_form()
 */
function uc_stock_edit_form_submit($form, &$form_state) {
  foreach (element_children($form_state['values']['stock']) as $id) {
    db_query("UPDATE {uc_product_stock} SET active = %d, stock = %d, threshold = %d WHERE sku = '%s'",
      $form_state['values']['stock'][$id]['active'] ? 1 : 0, intval($form_state['values']['stock'][$id]['stock']),
      intval($form_state['values']['stock'][$id]['threshold']), $form_state['values']['stock'][$id]['sku']);

    if (!db_affected_rows()) {
      @db_query("INSERT INTO {uc_product_stock} (sku, nid, active, stock, threshold) VALUES ('%s', %d, %d, %d, %d)",
        $form_state['values']['stock'][$id]['sku'], $form_state['values']['nid'], $form_state['values']['stock'][$id]['active'] ? 1 : 0,
        intval($form_state['values']['stock'][$id]['stock']), intval($form_state['values']['stock'][$id]['threshold']));
    }
  }

  drupal_set_message(t('Stock settings saved.'));
}
