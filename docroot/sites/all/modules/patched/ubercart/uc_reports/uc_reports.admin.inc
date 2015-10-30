<?php

/**
 * @file
 * Reports administration menu items.
 */

/**
 * Displays the customer report.
 */
function uc_reports_customers() {
  $address_preference = variable_get('uc_customer_list_address', 'billing');
  $first_name = ($address_preference == 'billing') ? 'billing_first_name' : 'delivery_first_name';
  $last_name = ($address_preference == 'billing') ? 'billing_last_name' : 'delivery_last_name';
  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $order_statuses = uc_reports_order_statuses();
  $rows = array();
  $csv_rows = array();

  $header = array(
    array('data' => t('#')),
    array('data' => t('Customer'), 'field' => "ou.$last_name"),
    array('data' => t('Username'), 'field' => "u.name"),
    array('data' => t('Orders'), 'field' => 'orders'),
    array('data' => t('Products'), 'field' => 'products'),
    array('data' => t('Total'), 'field' => 'total', 'sort' => 'desc'),
    array('data' => t('Average'), 'field' => 'average'),
  );
  $csv_rows[] = array(t('#'), t('Customer'), t('Username'), t('Orders'), t('Products'), t('Total'), t('Average'));

  $query = db_select('users', 'u', array('fetch' => PDO::FETCH_ASSOC))->extend('PagerDefault')->extend('TableSort');
  $query->leftJoin('uc_orders', 'ou', 'u.uid = ou.uid');
  $query->fields('u', array(
      'uid',
      'name',
    ))
    ->fields('ou', array(
      $first_name,
      $last_name,
    ))
    ->condition('u.uid', 0, '>')
    ->groupBy('u.uid');
  $query->addExpression("(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status IN (:statuses))", 'orders', array(':statuses' => $order_statuses));
  $query->addExpression("(SELECT SUM(qty) FROM {uc_order_products} ps LEFT JOIN {uc_orders} os ON ps.order_id = os.order_id WHERE os.order_status IN (:statuses2) AND os.uid = u.uid)", 'products', array(':statuses2' => $order_statuses));
  $query->addExpression("(SELECT SUM(ot.order_total) FROM {uc_orders} ot WHERE ot.uid = u.uid AND ot.order_status IN (:statuses3))", 'total', array(':statuses3' => $order_statuses));
  $query->addExpression("ROUND((SELECT SUM(ot.order_total) FROM {uc_orders} ot WHERE ot.uid = u.uid AND ot.order_status IN (:sum_statuses))/(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} o WHERE o.uid = u.uid AND o.order_status IN (:count_statuses)), 2)", 'average', array(':sum_statuses' => $order_statuses, ':count_statuses' => $order_statuses));

  $count_query = db_select('users', 'u');
  $count_query->leftJoin('uc_orders', 'ou', 'u.uid = ou.uid');
  $count_query->addExpression('COUNT(DISTINCT u.uid)');
  $count_query->condition('u.uid', 0, '>');

  $query->setCountQuery($count_query);
  $query->groupBy('u.uid')
    ->groupBy('u.name')
    ->groupBy("ou.$first_name")
    ->groupBy("ou.$last_name")
    ->orderByHeader($header)
    ->limit($page_size);

  $customers = $query->execute();

  foreach ($customers as $customer) {
    $name = (!empty($customer[$last_name]) || !empty($customer[$first_name])) ? l($customer[$last_name] . ', ' . $customer[$first_name], 'admin/store/customers/orders/' . $customer['uid']) : l($customer['name'], 'admin/store/customers/orders/' . $customer['uid']);
    $customer_number = ($page * variable_get('uc_reports_table_size', 30)) + (count($rows) + 1);
    $customer_order_name = (!empty($customer[$last_name]) || !empty($customer[$first_name])) ? $customer[$last_name] . ', ' . $customer[$first_name] : $customer['name'];
    $customer_name = $customer['name'];
    $orders = !empty($customer['orders']) ? $customer['orders'] : 0;
    $products = !empty($customer['products']) ? $customer['products'] : 0;
    $total_revenue = uc_currency_format($customer['total']);
    $average_revenue = uc_currency_format($customer['average']);
    $rows[] = array(
      array('data' => $customer_number),
      array('data' => $name),
      array('data' => l($customer_name, 'user/' . $customer['uid'])),
      array('data' => $orders),
      array('data' => $products),
      array('data' => $total_revenue),
      array('data' => $average_revenue),
    );
    $csv_rows[] = array($customer_number, $customer_order_name, $customer_name, $orders, $products, $customer['total'], $customer['average']);
  }
  $csv_data = uc_reports_store_csv('uc_customers', $csv_rows);

  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
    '#empty' => t('No customers found'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );
  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file.'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
    '#suffix' => '&nbsp;&nbsp;&nbsp;',
  );
  if (isset($_GET['nopage'])) {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show paged records'), 'admin/store/reports/customers'),
    );
  }
  else {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show all records'), 'admin/store/reports/customers', array('query' => array('nopage' => '1'))),
    );
  }

  return $build;
}

/**
 * Displays the product reports.
 */
function uc_reports_products() {
  $views_column = module_exists('statistics') && variable_get('statistics_count_content_views', FALSE);

  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $order_statuses = uc_reports_order_statuses();
  $row_cell = $page * variable_get('uc_reports_table_size', 30) + 1;
  $rows = array();
  $csv_rows = array();

  // Hard code the ignore of the product kit for this report.
  $ignored_types = array('product_kit');

  // Build an array of valid product types to include on the report.
  $product_types = array();
  foreach (uc_product_types() as $type) {
    // Pass over any ignored types.
    if (!in_array($type, $ignored_types)) {
      $product_types[] = $type;
    }
  }

  $query = db_select('node', 'n', array('fetch' => PDO::FETCH_ASSOC))
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->limit($page_size);

  $query->addField('n', 'nid');
  $query->addField('n', 'title');
  $query->addExpression("(SELECT SUM(uop.qty) FROM {uc_order_products} uop LEFT JOIN {uc_orders} uo ON uop.order_id = uo.order_id WHERE uo.order_status IN (:statuses) AND uop.nid = n.nid)", 'sold', array(':statuses' => $order_statuses));
  $query->addExpression("(SELECT (SUM(uop.price * uop.qty) - SUM(uop.cost * uop.qty)) FROM {uc_order_products} uop LEFT JOIN {uc_orders} uo ON uop.order_id = uo.order_id WHERE uo.order_status IN (:statuses2) AND uop.nid = n.nid)", 'gross', array(':statuses2' => $order_statuses));
  $query->addExpression("(SELECT (SUM(uop.price * uop.qty)) FROM {uc_order_products} uop LEFT JOIN {uc_orders} uo ON uop.order_id = uo.order_id WHERE uo.order_status IN (:statuses3) AND uop.nid = n.nid)", 'revenue', array(':statuses3' => $order_statuses));

  $header = array(
    array('data' => t('#')),
    array('data' => t('Product'), 'field' => 'n.title'),
    array('data' => t('Sold'), 'field' => 'sold'),
    array('data' => t('Revenue'), 'field' => 'revenue', 'sort' => 'desc'),
    array('data' => t('Gross'), 'field' => 'gross'),
  );
  $csv_rows[] = array(t('#'), t('Product'), t('Sold'), t('Revenue'), t('Gross'));

  if ($views_column) {
    $header[] = array('data' => t('Views'), 'field' => 'nc.totalcount');
    $csv_rows[0][] = t('Views');
  }

  $query->orderByHeader($header);

  if ($views_column) {
    $query->leftJoin('node_counter', 'nc', 'n.nid = nc.nid');
    $query->addField('nc', 'totalcount');
  }

  $query->condition('n.type', $product_types, 'IN')
    ->groupBy('n.nid')
    ->groupBy('n.title');

  $products = $query->execute();
  foreach ($products as $product) {
    $product_cell = l($product['title'], 'node/' . $product['nid']);
    $product_csv = $product['title'];
    $sold_cell = empty($product['sold']) ? 0 : $product['sold'];
    $sold_csv = $sold_cell;
    $revenue_csv = empty($product['revenue']) ? 0 : $product['revenue'];
    $revenue_cell = uc_currency_format($revenue_csv);
    $gross_csv = empty($product['gross']) ? 0 : $product['gross'];
    $gross_cell = uc_currency_format($gross_csv);

    $row = array(
      'data' => array(
        $row_cell,
        $product_cell,
        "<strong>$sold_cell</strong>",
        "<strong>$revenue_cell</strong>",
        "<strong>$gross_cell</strong>",
      ),
      'primary' => TRUE,
    );
    $csv_row = array($row_cell, $product_csv, $sold_csv, $revenue_csv, $gross_csv);

    if ($views_column) {
      $views = isset($product['totalcount']) ? $product['totalcount'] : 0;
      $row['data'][] = $views;
      $csv_row[] = $views;
    }

    $rows[] = $row;
    $csv_rows[] = $csv_row;

    if (module_exists('uc_attribute')) {
      // Get the SKUs from this product.
      $models = uc_reports_product_get_skus($product['nid']);
      // Add the product breakdown rows
      foreach ($models as $model) {
        $sold = db_query("SELECT SUM(qty) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid", array(':statuses' => $order_statuses, ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $revenue = db_query("SELECT SUM(p.price * p.qty) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid", array(':statuses' => $order_statuses, ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $gross = db_query("SELECT (SUM(p.price * p.qty) - SUM(p.cost * p.qty)) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid", array(':statuses' => $order_statuses, ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $breakdown_product = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$model";
        $product_csv = "     $model";

        $sold_csv = !empty($sold) ? $sold : 0;
        $breakdown_sold = $sold_csv;

        $revenue_csv = !empty($revenue) ? $revenue : 0;
        $breakdown_revenue = theme('uc_price', array('price' => $revenue_csv));

        $gross_csv = !empty($gross) ? $gross : 0;
        $breakdown_gross = theme('uc_price', array('price' => $gross_csv));

        $row = array(
          'data' => array(
            '',
            $breakdown_product,
            $breakdown_sold,
            $breakdown_revenue,
            $breakdown_gross,
          ),
        );
        $csv_row = array('', $product_csv, $sold_csv, $revenue_csv, $gross_csv);

        if ($views_column) {
          $row['data'][] = '';
          $csv_row[] = '';
        }

        $rows[] = $row;
        $csv_rows[] = $csv_row;

      }
    }
    $row_cell++;
  }
  $csv_data = uc_reports_store_csv('uc_products', $csv_rows);

  $build['report'] = array(
    '#theme' => 'uc_reports_product_table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
    '#empty' => t('No products found'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );

  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file.'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
    '#suffix' => '&nbsp;&nbsp;&nbsp;',
  );
  if (isset($_GET['nopage'])) {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show paged records'), 'admin/store/reports/products'),
    );
  }
  else {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show all records'), 'admin/store/reports/products', array('query' => array('nopage' => '1'))),
    );
  }
  $build['instructions'] = array('#markup' => '<small>*' . t('Make sure %setting_name is set to %state in the <a href="!url">access log settings page</a> to enable views column.', array('%setting_name' => 'count content views', '%state' => 'enabled', '!url' => url('admin/config/system/statistics', array('query' => array('destination' => 'admin/store/reports/products'))))) . '</small>');

  return $build;
}

/**
 * Gets the SKUs on a product, including adjustments and past orders.
 *
 * @param $nid
 *   The product's node ID.
 *
 * @return
 *   A unique sorted array of all skus.
 */
function uc_reports_product_get_skus($nid) {
  // Product SKU.
  $models = array(db_query("SELECT model FROM {uc_products} WHERE nid = :nid", array(':nid' => $nid))->fetchField());
  // Adjustment SKUs.
  $models = array_merge($models, db_query("SELECT model FROM {uc_product_adjustments} WHERE nid = :nid", array(':nid' => $nid))->fetchCol());

  // SKUs from orders.
  $models = array_merge($models, db_query("SELECT DISTINCT model FROM {uc_order_products} WHERE nid = :nid", array(':nid' => $nid))->fetchCol());

  // Unique, sorted.
  $models = array_unique($models);
  asort($models);

  return $models;
}

/**
 * Displays the custom product report.
 */
function uc_reports_products_custom() {
  $views_column = module_exists('statistics') && variable_get('statistics_count_content_views', FALSE);

  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $rows = array();
  $csv_rows = array();

  // Hard code the ignore of the product kit for this report.
  $ignored_types = array('product_kit');

  // Build an array of valid product types to include on the report.
  $product_types = array();
  foreach (uc_product_types() as $type) {
    // Pass over any ignored types.
    if (!in_array($type, $ignored_types)) {
      $product_types[] = $type;
    }
  }

  // Use default report parameters if we don't detect values in the URL.
  if (arg(5) == '') {
    $args = array(
      'start_date' => mktime(0, 0, 0, date('n'), 1, date('Y') - 1),
      'end_date' => REQUEST_TIME,
      'status' => uc_reports_order_statuses(),
    );
  }
  else {
    $args = array(
      'start_date' => arg(5),
      'end_date' => arg(6),
      'status' => explode(',', arg(7)),
    );
  }

  $query = db_select('node', 'n', array('fetch' => PDO::FETCH_ASSOC))
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->limit($page_size)
    ->fields('n', array(
      'nid',
      'title',
    ))
    ->condition('type', $product_types, 'IN')
    ->groupBy('n.nid');

  $query->addExpression("(SELECT SUM(qty) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.nid = n.nid AND o.created >= :start AND o.created <= :end)", 'sold', array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']));
  $query->addExpression("(SELECT (SUM(p2.price * p2.qty)) FROM {uc_order_products} p2 LEFT JOIN {uc_orders} o ON p2.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p2.nid = n.nid AND o.created >= :start AND o.created <= :end)", 'revenue', array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']));
  $query->addExpression("(SELECT (SUM(p3.price * p3.qty) - SUM(p3.cost * p3.qty)) FROM {uc_order_products} p3 LEFT JOIN {uc_orders} o ON p3.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p3.nid = n.nid AND o.created >= :start AND o.created <= :end)", 'gross', array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']));

  $header = array(
    array('data' => t('#')),
    array('data' => t('Product'), 'field' => 'n.title'),
    array('data' => t('Sold'), 'field' => 'sold'),
    array('data' => t('Revenue'), 'field' => 'revenue', 'sort' => 'desc'),
    array('data' => t('Gross'), 'field' => 'gross'),
  );
  $csv_rows[] = array(t('#'), t('Product'), t('Sold'), t('Revenue'), t('Gross'));

  if ($views_column) {
    $header[] = array('data' => t('Views'), 'field' => 'nc.totalcount');
    $csv_rows[0][] = t('Views');
  }

  $query->orderByHeader($header);

  if ($views_column) {
    $query->leftJoin('node_counter', 'c', 'n.nid = c.nid');
    $query->addField('c', 'totalcount');
  }

  $products = $query->execute();
  foreach ($products as $product) {
    $row_cell = ($page * variable_get('uc_reports_table_size', 30)) + count($rows) + 1;
    $product_cell = l($product['title'], 'node/' . $product['nid']);
    $product_csv = $product['title'];
    $sold_cell = empty($product['sold']) ? 0 : $product['sold'];
    $sold_csv = $sold_cell;
    $revenue_csv = empty($product['revenue']) ? 0 : $product['revenue'];
    $revenue_cell = uc_currency_format($revenue_csv);
    $gross_csv = empty($product['gross']) ? 0 : $product['gross'];
    $gross_cell = uc_currency_format($gross_csv);

    if (module_exists('uc_attribute')) {
      $breakdown_product = $breakdown_sold = $breakdown_revenue = $breakdown_gross = '';

      // Get the SKUs from this product.
      $models = uc_reports_product_get_skus($product['nid']);
      // Add the product breakdown rows
      foreach ($models as $model) {
        $sold = db_query("SELECT SUM(qty) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid AND o.created >= :start AND o.created <= :end", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date'], ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $sold = empty($sold) ? 0 : $sold;
        $revenue = db_query("SELECT SUM(p.price * p.qty) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid AND o.created >= :start AND o.created <= :end", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date'], ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $revenue = empty($revenue) ? 0 : $revenue;
        $gross = db_query("SELECT (SUM(p.price * p.qty) - SUM(p.cost * p.qty)) FROM {uc_order_products} p LEFT JOIN {uc_orders} o ON p.order_id = o.order_id WHERE o.order_status IN (:statuses) AND p.model = :model AND p.nid = :nid AND o.created >= :start AND o.created <= :end", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date'], ':model' => $model, ':nid' => $product['nid']))->fetchField();
        $gross = empty($gross) ? 0 : $gross;

        $breakdown_product .= "<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$model";
        $product_csv .= "\n     $model";

        $breakdown_sold .= "<br />" . $sold;
        $sold_csv .= "\n     " . $sold;

        $breakdown_revenue .= "<br />" . uc_currency_format($revenue);
        $revenue_csv .= "\n     " . $revenue;

        $breakdown_gross .= "<br />" . uc_currency_format($gross);
        $gross_csv .= "\n     " . $gross;
      }
      $product_cell = $product_cell . $breakdown_product;
      $sold_cell = '<strong>' . $sold_cell . '</strong>' . $breakdown_sold;
      $revenue_cell = '<strong>' . $revenue_cell . '</strong>' . $breakdown_revenue;
      $gross_cell = '<strong>' . $gross_cell . '</strong>' . $breakdown_gross;
    }
    if ($views_column) {
      $views = empty($product['totalcount']) ? 0 : $product['totalcount'];
      $rows[] = array(
        array('data' => $row_cell),
        array('data' => $product_cell),
        array('data' => $sold_cell),
        array('data' => $revenue_cell),
        array('data' => $gross_cell),
        array('data' => $views),
      );
      $csv_rows[] = array($row_cell, $product_csv, $sold_csv, $revenue_csv, $gross_csv, $views);
    }
    else {
      $rows[] = array(
        array('data' => $row_cell),
        array('data' => $product_cell),
        array('data' => $sold_cell),
        array('data' => $revenue_cell),
        array('data' => $gross_cell),
      );
      $csv_rows[] = array($row_cell, $product_csv, $sold_csv, $revenue_csv, $gross_csv);
    }
  }
  $csv_data = uc_reports_store_csv('uc_products', $csv_rows);

  // Build the page output holding the form, table, and CSV export link.
  $build['form'] = drupal_get_form('uc_reports_products_custom_form', $args);
  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
    '#empty' => t('No products found'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
  );
  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file.'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
    '#suffix' => '&nbsp;&nbsp;&nbsp;',
  );
  if (isset($_GET['nopage'])) {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show paged records'), 'admin/store/reports/products/custom'),
    );
  }
  else {
    $build['links']['toggle_pager'] = array(
      '#markup' => l(t('Show all records'), 'admin/store/reports/products/custom', array('query' => array('nopage' => '1'))),
    );
  }

  $build['instructions'] = array('#markup' => '<small>*' . t('Make sure %setting_name is set to %state in the <a href="!url">access log settings page</a> to enable views column.', array('%setting_name' => 'count content views', '%state' => 'enabled', '!url' => url('admin/config/system/statistics', array('query' => array('destination' => 'admin/store/reports/products/custom'))))) . '</small>');

  return $build;
}

/**
 * Form builder for the custom product report.
 *
 * @see uc_reports_products_custom_form_validate()
 * @see uc_reports_products_custom_form_submit()
 * @ingroup forms
 */
function uc_reports_products_custom_form($form, &$form_state, $values) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize product report parameters'),
    '#description' => t('Adjust these values and update the report to build your custom product report. Once submitted, the report may be bookmarked for easy reference in the future.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['search']['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array(
      'month' => format_date($values['start_date'], 'custom', 'n'),
      'day' => format_date($values['start_date'], 'custom', 'j'),
      'year' => format_date($values['start_date'], 'custom', 'Y'),
    ),
  );
  $form['search']['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array(
      'month' => format_date($values['end_date'], 'custom', 'n'),
      'day' => format_date($values['end_date'], 'custom', 'j'),
      'year' => format_date($values['end_date'], 'custom', 'Y'),
    ),
  );

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  $form['search']['status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.'),
    '#options' => $options,
    '#default_value' => $values['status'],
  );

  $form['search']['actions'] = array('#type' => 'actions');
  $form['search']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );

  return $form;
}

/**
 * Validation handler for the custom product report.
 *
 * @see uc_reports_products_custom_form()
 * @see uc_reports_products_custom_form_submit()
 */
function uc_reports_products_custom_form_validate($form, &$form_state) {
  if (empty($form_state['values']['status'])) {
    form_set_error('status', t('You must select at least one order status.'));
  }
}

/**
 * Submission handler for the custom product report.
 *
 * @see uc_reports_products_custom_form()
 * @see uc_reports_products_custom_form_submit()
 */
function uc_reports_products_custom_form_submit($form, &$form_state) {
  $start_date = mktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']);
  $end_date = mktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']);

  $args = array(
    $start_date,
    $end_date,
    implode(',', array_keys(array_filter($form_state['values']['status']))),
  );

  $form_state['redirect'] = array('admin/store/reports/products/custom/' . implode('/', $args));
}

/**
 * Return a themed table for product reports.
 *
 * Straight duplication of theme_table, but our row handling is different.
 *
 * @see theme_table()
 * @ingroup themeable
 */
function theme_uc_reports_product_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }

  $output = '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array('data' => $empty, 'colspan' => $header_count, 'class' => array('empty', 'message')));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it follwed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    foreach ($rows as $number => $row) {
      $attributes = array();

      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        foreach ($row as $key => $value) {
          if ($key == 'data') {
            $cells = $value;
          }
          // The following elseif clause is where we differ from theme_table()
          elseif ($key == 'primary') {
            $class = $flip[$class];
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cells = $row;
      }
      if (count($cells)) {
        // Add odd/even class
        // We don't flip here like theme_table(), because we did that above.
        $attributes['class'][] = $class;

        // Build row
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  return $output;
}

/**
 * Displays the sales summary report.
 */
function uc_reports_sales_summary() {
  $order_statuses = uc_reports_order_statuses();

  $date_day_of_month = date('j');
  $date_month = date('n');
  $month_start = mktime(0, 0, 0, $date_month, 1);
  $month_end = mktime(0, 0, 0, $date_month + 1, 1) - 1;
  $today_start = mktime(0, 0, 0);
  $today_end = mktime(23, 59, 59);

  // Build the report table header.
  $header = array(t('Sales data'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Calculate and add today's sales summary to the report table.
  $today = uc_reports_get_sales($today_start);

  $rows[] = array(
    l(t('Today, !date', array('!date' => format_date($today_start, 'uc_store'))), 'admin/store/orders/search/results/0/0/0/0/0/0/' . $today_start . '/' . $today_end),
    $today['total'],
    array('data' => array('#theme' => 'uc_price', '#price' => $today['income'])),
    array('data' => array('#theme' => 'uc_price', '#price' => $today['average'])),
  );

  // Calculate and add yesterday's sales summary to the report table.
  $yesterday = uc_reports_get_sales($today_start - 86400);

  $rows[] = array(
    l(t('Yesterday, !date', array('!date' => format_date($today_start - 86400, 'uc_store'))), 'admin/store/orders/search/results/0/0/0/0/0/0/' . ($today_start - 86400) . '/' . ($today_end - 86400)),
    $yesterday['total'],
    array('data' => array('#theme' => 'uc_price', '#price' => $yesterday['income'])),
    array('data' => array('#theme' => 'uc_price', '#price' => $yesterday['average'])),
  );

  // Get the sales report for the month.
  $month = uc_reports_get_sales($month_start, 'month');
  $month_title = format_date($month_start, 'custom', 'M Y');

  // Add the month-to-date details to the report table.
  $rows[] = array(
    l(t('Month-to-date, @month', array('@month' => $month_title)), 'admin/store/orders/search/results/0/0/0/0/0/0/' . $month_start . '/' . $month_end),
    $month['total'],
    array('data' => array('#theme' => 'uc_price', '#price' => $month['income'])),
    array('data' => array('#theme' => 'uc_price', '#price' => $month['average'])),
  );

  // Calculate the daily averages for the month.
  $daily_orders = round($month['total'] / $date_day_of_month, 2);
  $daily_revenue = round($month['income'] / $date_day_of_month, 2);

  if ($daily_orders > 0) {
    $daily_average = round($daily_revenue / $daily_orders, 2);
  }
  else {
    $daily_average = 0;
  }

  // Add the daily averages for the month to the report table.
  $rows[] = array(
    t('Daily average for @month', array('@month' => $month_title)),
    $daily_orders,
    array('data' => array('#theme' => 'uc_price', '#price' => $daily_revenue)),
    '',
  );

  // Store the number of days remaining in the month.
  $remaining_days = date('t') - $date_day_of_month;

  // Add the projected totals for the month to the report table.
  $rows[] = array(
    t('Projected totals for @date', array('@date' => $month_title)),
    round($month['total'] + ($daily_orders * $remaining_days), 2),
    array('data' => array('#theme' => 'uc_price', '#price' => round($month['income'] + ($daily_revenue * $remaining_days), 2))),
    '',
  );

  // Add the sales data report table to the output.
  $build['sales'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-sales-table')),
  );

  // Build the header statistics table header.
  $header = array(array('data' => t('Statistics'), 'width' => '50%'), '');

  $rows = array(
    array(array('data' => t('Grand total sales')), array('data' => array('#theme' => 'uc_price', '#price' => db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE order_status IN (:statuses)", array(':statuses' => $order_statuses))->fetchField()))),
    array(array('data' => t('Customers total')), array('data' => db_query("SELECT COUNT(DISTINCT uid) FROM {uc_orders} WHERE order_status IN (:statuses)", array(':statuses' => $order_statuses))->fetchField())),
    array(array('data' => t('New customers today')), array('data' => db_query("SELECT COUNT(DISTINCT uid) FROM {uc_orders} WHERE order_status IN (:statuses) AND :start <= created AND created <= :end", array(':statuses' => $order_statuses, ':start' => $today_start, ':end' => $today_end))->fetchField())),
    array(array('data' => t('Online customers')), array('data' => db_query("SELECT COUNT(DISTINCT s.uid) FROM {sessions} s LEFT JOIN {uc_orders} o ON s.uid = o.uid WHERE s.uid > 0 AND o.order_status IN (:statuses)", array(':statuses' => $order_statuses))->fetchField())),
  );

  // Add the statistics table to the output.
  $build['statistics'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
  );

  // Build the total orders by status table header.
  $header = array(array('data' => t('Total orders by status'), 'width' => '50%'), '');
  $rows = array();
  $unknown = 0;

  // Loop through the order statuses with their total number of orders.
  $result = db_query("SELECT s.order_status_id, order_status, s.title, s.weight, COUNT(o.order_status) as order_count FROM {uc_orders} o LEFT JOIN {uc_order_statuses} s ON s.order_status_id = o.order_status GROUP BY s.order_status_id, order_status, s.title, s.weight ORDER BY s.weight DESC");
  while ($status = $result->fetchAssoc()) {
    if (!empty($status['title'])) {
      // Add the total number of orders with this status to the table.
      $rows[] = array(
        l($status['title'], 'admin/store/orders/view', array('query' => array('order_status' => $status['order_status_id']))),
        $status['order_count'],
      );
    }
    else {
      // Keep track of the count of orders with an unknown status.
      $unknown += $status['order_count'];
    }
  }

  // Add the unknown status count to the table.
  if ($unknown > 0) {
    $rows[] = array(
      t('Unknown status'),
      $unknown,
    );
  }

  // Add the total orders by status table to the output.
  $build['orders'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('uc-sales-table')),
  );

  return $build;
}

/**
 * Displays the yearly sales report form and table.
 */
function uc_reports_sales_year() {
  $order_statuses = uc_reports_order_statuses();

  // Get the year for the report from the URL.
  if (intval(arg(5)) == 0) {
    $year = date('Y');
  }
  else {
    $year = arg(5);
  }

  // Build the header for the report table.
  $header = array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Build the header to the CSV export.
  $csv_rows = array(array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order')));

  // For each month of the year...
  for ($i = 1; $i <= 12; $i++) {
    // Calculate the start and end timestamps for the month in local time.
    $month_start = mktime(0, 0, 0, $i, 1, $year);
    $month_end = mktime(23, 59, 59, $i + 1, 0, $year);

    // Get the sales report for the month.
    $month_sales = uc_reports_get_sales($month_start, 'month');

    // Calculate the average order total for the month.
    if ($month_sales['total'] != 0) {
      $month_average = round($month_sales['income'] / $month_sales['total'], 2);
    }
    else {
      $month_average = 0;
    }

    // Add the month's row to the report table.
    $rows[] = array(
      l(date('M Y', $month_start), 'admin/store/orders/search/results/0/0/0/0/0/0/' . $month_start . '/' . $month_end),
      $month_sales['total'],
      uc_currency_format($month_sales['income']),
      uc_currency_format($month_average),
    );

    // Add the data to the CSV export.
    $csv_rows[] = array(
      date('M Y', $month_start),
      $month_sales['total'],
      $month_sales['income'],
      $month_average,
    );
  }

  // Calculate the start and end timestamps for the year in local time.
  $year_start = mktime(0, 0, 0, 1, 1, $year);
  $year_end = mktime(23, 59, 59, 1, 0, $year + 1);

  // Get the sales report for the year.
  $year_sales = uc_reports_get_sales($year_start, 'year');

  // Calculate the average order total for the year.
  if ($year_sales['total'] != 0) {
    $year_average = round($year_sales['income'] / $year_sales['total'], 2);
  }
  else {
    $year_average = 0;
  }

  // Add the total row to the report table.
  $rows[] = array(
    l(t('Total @year', array('@year' => $year)), 'admin/store/orders/search/results/0/0/0/0/0/0/' . $year_start . '/' . $year_end),
    $year_sales['total'],
    uc_currency_format($year_sales['income']),
    uc_currency_format($year_average),
  );

  // Add the total data to the CSV export.
  $csv_rows[] = array(
    t('Total @year', array('@year' => $year)),
    $year_sales['total'],
    $year_sales['income'],
    $year_average,
  );

  // Cache the CSV export.
  $csv_data = uc_reports_store_csv('uc_sales_yearly', $csv_rows);

  // Build the page output holding the form, table, and CSV export link.
  $build['form'] = drupal_get_form('uc_reports_sales_year_form', $year);
  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
  );

  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file.'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
  );

  return $build;
}

/**
 * Form to specify a year for the yearly sales report.
 *
 * @see uc_reports_sales_year_form_submit()
 * @ingroup forms
 */
function uc_reports_sales_year_form($form, &$form_state, $year) {
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Sales year'),
    '#default_value' => $year,
    '#maxlength' => 4,
    '#size' => 4,
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit handler for uc_reports_sales_year_form().
 *
 * @see uc_reports_sales_year_form()
 */
function uc_reports_sales_year_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/store/reports/sales/year/' . $form_state['values']['year'];
}

/**
 * Displays the custom sales report form and table.
 */
function uc_reports_sales_custom() {
  // Use default report parameters if we don't detect values in the URL.
  if (arg(5) == '') {
    $args = array(
      'start_date' => mktime(0, 0, 0, date('n'), 1, date('Y') - 1),
      'end_date' => REQUEST_TIME,
      'length' => 'month',
      'status' => uc_reports_order_statuses(),
      'detail' => FALSE,
    );
  }
  else {
    $args = array(
      'start_date' => arg(5),
      'end_date' => arg(6),
      'length' => arg(7),
      'status' => explode(',', arg(8)),
      'detail' => arg(9),
    );
  }

  // Build the header for the report table.
  $header = array(t('Date'), t('Number of orders'), t('Products sold'), t('Total revenue'));

  // Build the header to the CSV export.
  $csv_rows = array(array(t('Date'), t('Number of orders'), t('Products sold'), t('Total revenue')));

  // Grab the subreports based on the date range and the report breakdown.
  $subreports = uc_reports_subreport_intervals($args['start_date'], $args['end_date'], $args['length']);

  // Loop through the subreports and build the report table.
  foreach ($subreports as $subreport) {
    $product_data = '';
    $product_csv = '';
    $order_data = '';
    $order_csv = '';

    // Create the date title for the subreport.
    if ($args['length'] == 'day') {
      $date = format_date($subreport['start'], 'custom', variable_get('date_format_uc_store', 'm/d/Y') . ' - D');
    }
    else {
      $date = format_date($subreport['start'], 'uc_store') . ' - ' . format_date($subreport['end'], 'uc_store');
    }

    // Build the order data for the subreport.
    $result = db_query("SELECT COUNT(*) as count, title FROM {uc_orders} LEFT JOIN {uc_order_statuses} ON order_status_id = order_status WHERE :start <= created AND created <= :end AND order_status IN (:statuses) GROUP BY order_status, {uc_order_statuses}.title, {uc_order_statuses}.weight ORDER BY weight ASC", array(':statuses' => $args['status'], ':start' => $subreport['start'], ':end' => $subreport['end']));
    $statuses = array();

    // Put the order counts into an array by status.
    foreach ($result as $status) {
      $statuses[] = t('!count - @title', array('!count' => $status->count, '@title' => $status->title));
    }

    $order_data = implode('<br />', $statuses);
    $order_csv = implode("\n", $statuses);

    // Build the product data for the subreport.
    if ($args['detail']) {
      // Grab the detailed product breakdown if selected.
      $result = db_query("SELECT SUM(op.qty) as count, n.title, n.nid FROM {uc_order_products} op LEFT JOIN {uc_orders} o ON o.order_id = op.order_id LEFT JOIN {node} n ON n.nid = op.nid WHERE :start <= o.created AND o.created <= :end AND o.order_status IN (:statuses) GROUP BY n.nid ORDER BY count DESC, n.title ASC", array(':statuses' => $args['status'], ':start' => $subreport['start'], ':end' => $subreport['end']));
      foreach ($result as $product_breakdown) {
        $product_data .= $product_breakdown->count . ' x ' . l($product_breakdown->title, 'node/' . $product_breakdown->nid) . "<br />\n";
        $product_csv .= $product_breakdown->count . ' x ' . $product_breakdown->title . "\n";
      }
    }
    else {
      // Otherwise just display the total number of products sold.
      $product_data = db_query("SELECT SUM(qty) FROM {uc_orders} o LEFT JOIN {uc_order_products} op ON o.order_id = op.order_id WHERE :start <= created AND created <= :end AND order_status IN (:statuses)", array(':statuses' => $args['status'], ':start' => $subreport['start'], ':end' => $subreport['end']))->fetchField();
      $product_csv = $product_data;
    }

    // Tally up the revenue from the orders.
    $revenue_count = db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE :start <= created AND created <= :end AND order_status IN (:statuses)", array(':statuses' => $args['status'], ':start' => $subreport['start'], ':end' => $subreport['end']))->fetchField();

    // Add the subreport's row to the report table.
    $rows[] = array(
      $date,
      empty($order_data) ? '0' : $order_data,
      empty($product_data) ? '0' : $product_data,
      uc_currency_format($revenue_count),
    );

    // Add the data to the CSV export.
    $csv_rows[] = array(
      $date,
      empty($order_csv) ? '0' : $order_csv,
      empty($product_csv) ? '0' : $product_csv,
      $revenue_count,
    );
  }

  // Calculate the totals for the report.
  $order_total = db_query("SELECT COUNT(*) FROM {uc_orders} WHERE :start <= created AND created <= :end AND order_status IN (:statuses)", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']))->fetchField();
  $product_total = db_query("SELECT SUM(qty) FROM {uc_orders} o LEFT JOIN {uc_order_products} op ON o.order_id = op.order_id WHERE :start <= created AND created <= :end AND order_status IN (:statuses)", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']))->fetchField();
  $revenue_total = db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE :start <= created AND created <= :end AND order_status IN (:statuses)", array(':statuses' => $args['status'], ':start' => $args['start_date'], ':end' => $args['end_date']))->fetchField();

  // Add the total row to the report table.
  $rows[] = array(
    t('Total'),
    $order_total,
    $product_total,
    uc_currency_format($revenue_total),
  );

  // Add the total data to the CSV export.
  $csv_rows[] = array(
    t('Total'),
    $order_total,
    $product_total,
    $revenue_total,
  );

  // Cache the CSV export.
  $csv_data = uc_reports_store_csv('uc_sales_custom', $csv_rows);

  // Build the page output holding the form, table, and CSV export link.
  $build['form'] = drupal_get_form('uc_reports_sales_custom_form', $args, $args['status']);
  $build['report'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('width' => '100%', 'class' => array('uc-sales-table')),
  );
  $build['links'] = array(
    '#prefix' => '<div class="uc-reports-links">',
    '#suffix' => '</div>',
  );
  $build['links']['export_csv'] = array(
    '#markup' => l(t('Export to CSV file.'), 'admin/store/reports/getcsv/' . $csv_data['report'] . '/' . $csv_data['user']),
  );

  return $build;
}

/**
 * Form builder for the custom sales report.
 *
 * @see uc_reports_sales_custom_form_validate()
 * @see uc_reports_sales_custom_form_submit()
 * @ingroup forms
 */
function uc_reports_sales_custom_form($form, &$form_state, $values, $statuses) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize sales report parameters'),
    '#description' => t('Adjust these values and update the report to build your custom sales summary. Once submitted, the report may be bookmarked for easy reference in the future.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['search']['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array(
      'month' => format_date($values['start_date'], 'custom', 'n'),
      'day' => format_date($values['start_date'], 'custom', 'j'),
      'year' => format_date($values['start_date'], 'custom', 'Y'),
    ),
  );
  $form['search']['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array(
      'month' => format_date($values['end_date'], 'custom', 'n'),
      'day' => format_date($values['end_date'], 'custom', 'j'),
      'year' => format_date($values['end_date'], 'custom', 'Y'),
    ),
  );

  $form['search']['length'] = array(
    '#type' => 'select',
    '#title' => t('Results breakdown'),
    '#description' => t('Large daily reports may take a long time to display.'),
    '#options' => array(
      'day' => t('daily'),
      'week' => t('weekly'),
      'month' => t('monthly'),
      'year' => t('yearly'),
    ),
    '#default_value' => $values['length'],
  );

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  if ($statuses === FALSE) {
    $statuses = uc_reports_order_statuses();
  }

  $form['search']['status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.'),
    '#options' => $options,
    '#default_value' => $statuses,
  );

  $form['search']['detail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a detailed list of products ordered.'),
    '#default_value' => $values['detail'],
  );

  $form['search']['actions'] = array('#type' => 'actions');
  $form['search']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );

  return $form;
}

/**
 * Ensure an order status was selected.
 *
 * @see uc_reports_sales_custom_form()
 * @see uc_reports_sales_custom_form_submit()
 */
function uc_reports_sales_custom_form_validate($form, &$form_state) {
  if (empty($form_state['values']['status'])) {
    form_set_error('status', t('You must select at least one order status.'));
  }
}

/**
 * Submission handler for uc_reports_sales_custom_form().
 *
 * @see uc_reports_sales_custom_form()
 * @see uc_reports_sales_custom_form_validate()
 */
function uc_reports_sales_custom_form_submit($form, &$form_state) {
  // Build the start and end dates from the form.
  $start_date = mktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']);
  $end_date = mktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']);

  $args = array(
    $start_date,
    $end_date,
    $form_state['values']['length'],
    implode(',', array_keys(array_filter($form_state['values']['status']))),
    $form_state['values']['detail'],
  );

  $form_state['redirect'] = 'admin/store/reports/sales/custom/' . implode('/', $args);
}

/**
 * Stores a CSV file for a report in Drupal's cache to be retrieved later.
 *
 * @param $report_id
 *   A unique string that identifies the report of the CSV file.
 * @param $rows
 *   The rows (table header included) that make CSV file.
 *
 * @return
 *   An array containing the values need to build URL that return the CSV file
 *   of the report and the CSV data itself.
 */
function uc_reports_store_csv($report_id, $rows) {
  global $user;
  $csv_output = '';
  $user_id = empty($user->uid) ? session_id() : $user->uid;
  foreach ($rows as $row) {
    foreach ($row as $index => $column) {
      $row[$index] = '"' . str_replace('"', '""', $column) . '"';
    }
    $csv_output .= implode(',', $row) . "\n";
  }
  cache_set('uc_reports_' . $report_id . '_' . $user_id, $csv_output, 'cache', REQUEST_TIME + 86400);
  return array('user' => $user_id, 'report' => $report_id, 'csv' => $csv_output);
}

/**
 * Retrieves a cached CSV report & send its data.
 *
 * @param $report_id
 *   A unique string that identifies the specific report CSV to retrieve.
 * @param $user_id
 *   The user id to who's retrieving the report:
 *   - uid: Equals uid for authenticated users.
 *   - sid: Equals session_id for anonymous users.
 */
function uc_reports_get_csv($report_id, $user_id) {
  global $user;
  $user_check = empty($user->uid) ? session_id() : $user->uid;
  $csv_data = cache_get('uc_reports_' . $report_id . '_' . $user_id, 'cache');

  if (!$csv_data || $user_id != $user_check) {
    drupal_set_message(t("The CSV data could not be retrieved. It's possible the data might have expired. Refresh the report page and try to retrieve the CSV file again."), 'error');
    drupal_not_found();
    exit();
  }
  else {
    ob_end_clean();
    $http_headers = array(
      'Pragma' => 'private',
      'Expires' => '0',
      'Cache-Control' => 'private, must-revalidate',
      'Content-Transfer-Encoding' => 'binary',
      'Content-Length' => strlen($csv_data->data),
      'Content-Disposition' => 'attachment; filename="' . $report_id . '.csv"',
      'Content-Type' => 'text/csv'
    );
    foreach ($http_headers as $header => $value) {
      $value = preg_replace('/\r?\n(?!\t| )/', '', $value);
      drupal_add_http_header($header, $value);
    }

    print $csv_data->data;
    exit();
  }
}

/**
 * Returns sales that occurred in a given time period.
 *
 * @param $time
 *   A UNIX timestamp representing the time in which to get sales data.
 * @param $interval
 *   The amount of time over which to count sales (e.g. [1] day, month, year).
 *
 * @return
 *   An associative array containing information about sales:
 *   - date: A string representing the day counting was started.
 *   - income: The total revenue that occurred during the time period.
 *   - total: The total number of orders completed during the time period.
 *   - average: The average revenue produced for each order.
 */
function uc_reports_get_sales($start, $interval = 'day') {
  // Add one to the granularity chosen, and use it to calc the new time.
  $end = strtotime('+1 ' . $interval, $start) - 1;

  // Set up the default SQL for getting orders with the proper status
  // within this period.
  $order_statuses = uc_reports_order_statuses();

  // Get the total value of the orders.
  $output = array('income' => 0);
  $orders = db_query("SELECT o.order_total FROM {uc_orders} o WHERE o.order_status IN (:statuses) AND :start <= created AND created <= :end", array(':statuses' => $order_statuses, ':start' => $start, ':end' => $end));
  while ($order = $orders->fetchObject()) {
    $output['income'] += $order->order_total;
  }

  // Get the total amount of orders.
  $count = db_query("SELECT COUNT(o.order_total) FROM {uc_orders} o WHERE o.order_status IN (:statuses) AND :start <= created AND created <= :end", array(':statuses' => $order_statuses, ':start' => $start, ':end' => $end))->fetchField();
  $output['total'] = $count;

  // Average for this period.
  $output['average'] = ($count != 0) ? round($output['income'] / $count, 2) : 0;

  return $output;
}

/**
 * Returns a list of timespans for subreports over that report's time span.
 *
 * To be used with a given time span for a report and specified interval for
 * subreports.
 *
 * @param $start
 *   A UNIX timestamp representing the time to start the report.
 * @param $end
 *   A UNIX timestamp representing the time to end the report.
 * @param $interval
 *   Text representing the time span of the subreport (e.g. 'day', 'week').
 *
 * @return
 *   An array of keyed arrays with the following values:
 *   - start: The starting point of the sub report.
 *   - end: The ending point of the sub report.
 */
function uc_reports_subreport_intervals($start, $report_end, $interval) {
  $subreports = array();

  while ($start < $report_end) {
    $end = strtotime('+1 ' . $interval, $start) - 1;
    $subreports[] = array(
      'start' => $start,
      'end' => min($end, $report_end),
    );

    $start = $end + 1;
  }

  return $subreports;
}
