<?php

/**
 * @file
 * Reports administration menu items.
 */

/**
 * Form builder for the admin settings.
 *
 * @ingroup forms
 */
function uc_reports_settings_form() {
  $form['uc_reports_table_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Paged table size'),
    '#description' => t('The maximum number of rows displayed on one page for a report table.'),
    '#default_value' => variable_get('uc_reports_table_size', 30),
  );

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  $form['uc_reports_reported_statuses'] = array(
    '#type' => 'select',
    '#title' => t('Reported statuses'),
    '#description' => t('Only orders with selected statuses will be included in reports.'),
    '#options' => $options,
    '#default_value' => variable_get('uc_reports_reported_statuses', array('completed')),
    '#multiple' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Display the customer report
 */
function uc_reports_customers() {
  $address_preference = variable_get('uc_customer_list_address', 'billing');
  $first_name = ($address_preference == 'billing') ? 'billing_first_name' : 'delivery_first_name';
  $last_name = ($address_preference == 'billing') ? 'billing_last_name' : 'delivery_last_name';
  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $order_statuses = _uc_reports_order_statuses();
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

  $sql = '';
  $sql_count = '';

  switch ($GLOBALS['db_type']) {
    case 'mysqli':
    case 'mysql':
      $sql = "SELECT u.uid, u.name, ou.$first_name, ou.$last_name, (SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} as o WHERE o.uid = u.uid AND o.order_status IN $order_statuses) as orders, (SELECT SUM(qty) FROM {uc_order_products} as ps LEFT JOIN {uc_orders} as os ON ps.order_id = os.order_id WHERE os.order_status IN $order_statuses AND os.uid = u.uid) as products, (SELECT SUM(ot.order_total) FROM {uc_orders} as ot WHERE ot.uid = u.uid AND ot.order_status IN $order_statuses) as total, ROUND((SELECT SUM(ot.order_total) FROM {uc_orders} as ot WHERE ot.uid = u.uid AND ot.order_status IN $order_statuses)/(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} as o WHERE o.uid = u.uid AND o.order_status IN $order_statuses), 2) as average FROM {users} as u LEFT JOIN {uc_orders} as ou ON u.uid = ou.uid WHERE u.uid > 0 GROUP BY u.uid";
      $sql_count = "SELECT COUNT(DISTINCT(u.uid)) FROM {users} as u LEFT JOIN {uc_orders} as ou ON u.uid = ou.uid WHERE u.uid > 0";
      break;
    case 'pgsql':
      $sql = "SELECT u.uid, u.name, ou.$first_name, ou.$last_name, (SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} as o WHERE o.uid = u.uid AND o.order_status IN $order_statuses) as orders, (SELECT SUM(qty) FROM {uc_order_products} as ps LEFT JOIN {uc_orders} as os ON ps.order_id = os.order_id WHERE os.order_status IN $order_statuses AND os.uid = u.uid) as products, (SELECT SUM(ot.order_total) FROM {uc_orders} as ot WHERE ot.uid = u.uid AND ot.order_status IN $order_statuses) as total, ROUND((SELECT SUM(ot.order_total) FROM {uc_orders} as ot WHERE ot.uid = u.uid AND ot.order_status IN $order_statuses)/(SELECT COUNT(DISTINCT(order_id)) FROM {uc_orders} as o WHERE o.uid = u.uid AND o.order_status IN $order_statuses), 2) as average FROM {users} as u LEFT JOIN {uc_orders} as ou ON u.uid = ou.uid WHERE u.uid > 0 GROUP BY u.uid, u.name, ou.$first_name, ou.$last_name";
      $sql_count = "SELECT COUNT(DISTINCT(u.uid)) FROM {users} as u LEFT JOIN {uc_orders} as ou ON u.uid = ou.uid WHERE u.uid > 0";
      break;
  }

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );

  $customers = pager_query($sql . tablesort_sql($header), $page_size, 0, $sql_count);

  while ($customer = db_fetch_array($customers)) {
    $name = (!empty($customer[$last_name]) || !empty($customer[$first_name])) ? l($customer[$last_name] .', '. $customer[$first_name], 'admin/store/customers/orders/'. $customer['uid']) : l($customer['name'], 'admin/store/customers/orders/'. $customer['uid']);
    $customer_number = ($page * variable_get('uc_reports_table_size', 30)) + (count($rows) + 1);
    $customer_order_name = (!empty($customer[$last_name]) || !empty($customer[$first_name])) ? $customer[$last_name] .', '. $customer[$first_name] : $customer['name'];
    $customer_name = $customer['name'];
    $orders = !empty($customer['orders']) ? $customer['orders'] : 0;
    $products = !empty($customer['products']) ? $customer['products'] : 0;
    $total_revenue = uc_price($customer['total'], $context);
    $average_revenue = uc_price($customer['average'], $context);
    $rows[] = array(
      array('data' => $customer_number),
      array('data' => $name),
      array('data' => l($customer_name, 'user/'. $customer['uid'])),
      array('data' => $orders),
      array('data' => $products),
      array('data' => $total_revenue),
      array('data' => $average_revenue),
    );
    $csv_rows[] = array($customer_number, $customer_order_name, $customer_name, $orders, $products, $customer['total'], $customer['average']);
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No customers found'), 'colspan' => count($header)));
  }
  $csv_data = uc_reports_store_csv('uc_customers', $csv_rows);

  $output = theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  $output .= theme('pager', NULL, $page_size);
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'&nbsp;&nbsp;&nbsp;'. (isset($_GET['nopage']) ? l(t('Show paged records'), 'admin/store/reports/customers') : l(t('Show all records'), 'admin/store/reports/customers', array('query' => 'nopage=1'))) .'</div>';

  return $output;
}

/**
 * Display the product reports
 */
function uc_reports_products() {
  $views_column = module_exists('statistics') && variable_get('statistics_count_content_views', FALSE);

  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $order_statuses = _uc_reports_order_statuses();
  $row_cell = $page * variable_get('uc_reports_table_size', 30) + 1;
  $csv_rows = array();

  // Hard code the ignore of the product kit for this report.
  $ignored_types = array('product_kit');

  // Build an array of valid product types to include on the report.
  $product_types = array();
  foreach (uc_product_types() as $type) {
    // Pass over any ignored types.
    if (!in_array($type, $ignored_types)) {
      $product_types[] = "'". $type ."'";
    }
  }
  $product_types = '('. implode(', ', $product_types) .')';

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

  $var_tables = "{uc_order_products} AS uop LEFT JOIN {uc_orders} AS uo ON uop.order_id = uo.order_id WHERE uo.order_status IN $order_statuses AND uop.nid = n.nid";
  $sql_vars = array(
    "n.nid",
    "n.title",
    "(SELECT SUM(uop.qty) FROM $var_tables) AS sold",
    "(SELECT (SUM(uop.price * uop.qty) - SUM(uop.cost * uop.qty)) FROM $var_tables) AS gross",
    "(SELECT (SUM(uop.price * uop.qty)) FROM $var_tables) AS revenue",
  );
  if ($views_column) {
    $sql_vars[] = "nc.totalcount";
  }
  $sql_vars = implode(", ", $sql_vars);

  $sql_tables = "{node} as n";
  if ($views_column) {
    $sql_tables .= " LEFT JOIN {node_counter} AS nc ON n.nid = nc.nid";
  }

  $sql_conditions = "n.type IN $product_types";

  $sql_count = "SELECT COUNT(n.nid) FROM $sql_tables WHERE $sql_conditions";

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );

  $products = pager_query("SELECT $sql_vars FROM $sql_tables WHERE $sql_conditions GROUP BY n.nid, n.title" . tablesort_sql($header), $page_size, 0, $sql_count);
  while ($product = db_fetch_array($products)) {
    $product_cell = l($product['title'], 'node/'. $product['nid']);
    $product_csv = $product['title'];
    $sold_cell = empty($product['sold']) ? 0 : $product['sold'];
    $sold_csv = $sold_cell;
    $revenue_csv = empty($product['revenue']) ? 0 : $product['revenue'];
    $revenue_cell = uc_price($revenue_csv, $context);
    $gross_csv = empty($product['gross']) ? 0 : $product['gross'];
    $gross_cell = uc_price($gross_csv, $context);

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
      $models = _uc_reports_product_get_skus($product['nid']);
      // Add the product breakdown rows
      foreach ($models as $model) {
        $sold = db_result(db_query("SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d", $model, $product['nid']));
        $revenue = db_result(db_query("SELECT SUM(p.price * p.qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d", $model, $product['nid']));
        $gross = db_result(db_query("SELECT (SUM(p.price * p.qty) - SUM(p.cost * p.qty)) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d", $model, $product['nid']));
        $breakdown_product = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$model";
        $product_csv = "     $model";

        $sold_csv = !empty($sold) ? $sold : 0;
        $breakdown_sold = $sold_csv;

        $context['revision'] = 'themed';
        $revenue_csv = !empty($revenue) ? $revenue : 0;
        $breakdown_revenue = uc_price($revenue_csv, $context);

        $gross_csv = !empty($gross) ? $gross : 0;
        $breakdown_gross = uc_price($gross_csv, $context);
        $context['revision'] = 'formatted';

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
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No products found'), 'colspan' => count($header)));
  }
  $csv_data = uc_reports_store_csv('uc_products', $csv_rows);

  $output = theme('uc_reports_product_table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  $output .= theme('pager', NULL, $page_size);
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'&nbsp;&nbsp;&nbsp;'. (isset($_GET['nopage']) ? l(t('Show paged records'), 'admin/store/reports/products') : l(t('Show all records'), 'admin/store/reports/products', array('query' => 'nopage=1'))) .'</div>';
  $output .= '<small>*'. t('Make sure %setting_name is set to %state in the <a href="!url">access log settings page</a> to enable views column.', array('%setting_name' => 'count content views', '%state' => 'enabled', '!url' => url('admin/reports/settings', array('query' => 'destination=admin/store/reports/products')))) .'</small>';

  return $output;
}

/**
 * Get the SKUs on a product, first from the product itself, then from the
 * adjustments table, finally pulling any SKUs out of previous orders.
 *
 * @param $nid
 *   The product's node ID.
 * @return
 *   A unique sorted array of all skus.
 */
function _uc_reports_product_get_skus($nid) {
  // Product SKU.
  $models = array(db_result(db_query("SELECT model FROM {uc_products} WHERE nid = %d", $nid)));
  // Adjustment SKUs.
  $product_models = db_query("SELECT model FROM {uc_product_adjustments} WHERE nid = %d", $nid);
  while ($product_model = db_fetch_object($product_models)) {
    $models[] = $product_model->model;
  }
  // SKUs from orders.
  $order_product_models = db_query("SELECT DISTINCT model FROM {uc_order_products} WHERE nid = %d", $nid);
  while ($order_product_model = db_fetch_object($order_product_models)) {
    $models[] = $order_product_model->model;
  }
  // Unique, sorted.
  $models = array_unique($models);
  asort($models);
  return $models;
}

/**
 * Display the custom product report.
 */
function uc_reports_products_custom() {
  $timezone = _uc_reports_timezone_offset();
  $timezone_offset = time() + $timezone;
  $views_column = module_exists('statistics') && variable_get('statistics_count_content_views', FALSE);

  $page = isset($_GET['page']) ? intval($_GET['page']) : 0;
  $page_size = isset($_GET['nopage']) ? UC_REPORTS_MAX_RECORDS : variable_get('uc_reports_table_size', 30);
  $product_types = array("'product'");
  $csv_rows = array();

  $types = db_query("SELECT DISTINCT(pcid) FROM {uc_product_classes}");
  while ($type = db_fetch_object($types)) {
    $product_types[] = "'". $type->pcid ."'";
  }

  // Use default report parameters if we don't detect values in the URL.
  if (arg(5) == '') {
    $args = array(
      'start_date' => gmmktime(0, 0, 0, gmdate('n', $timezone_offset), 1, gmdate('Y', $timezone_offset) - 1),
      'end_date' => time(),
      'status' => FALSE,
    );
  }
  else {
    $args = array(
      'start_date' => arg(5),
      'end_date' => arg(6),
      'status' => explode(',', urldecode(arg(7))),
    );
  }

  // Pull the order statuses into a SQL friendly array.
  if ($args['status'] === FALSE) {
    $order_statuses = _uc_reports_order_statuses();
  }
  else {
    $order_statuses = "('". implode("', '", $args['status']) ."')";
  }
  $time_condition = "o.created >= ". $args['start_date'] ." AND o.created <= ". $args['end_date'];

  if ($views_column) {
    $header = array(
      array('data' => t('#')),
      array('data' => t('Product'), 'field' => 'n.title'),
      array('data' => t('Sold'), 'field' => 'sold'),
      array('data' => t('Revenue'), 'field' => 'revenue', 'sort' => 'desc'),
      array('data' => t('Gross'), 'field' => 'gross'),
      array('data' => t('Views'), 'field' => 'c.totalcount'),
    );
    $csv_rows[] = array(t('#'), t('Product'), t('Sold'), t('Revenue'), t('Gross'), t('Views'));

    $sql = '';
    switch ($GLOBALS['db_type']) {
      case 'mysqli':
      case 'mysql':
        $sql = "SELECT n.nid, n.title, c.totalcount, (SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.nid = n.nid AND $time_condition) AS sold, (SELECT (SUM(p2.price * p2.qty)) FROM {uc_order_products} AS p2 LEFT JOIN {uc_orders} AS o ON p2.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p2.nid = n.nid AND $time_condition) AS revenue, (SELECT (SUM(p3.price * p3.qty) - SUM(p3.cost * p3.qty)) FROM {uc_order_products} AS p3 LEFT JOIN {uc_orders} AS o ON p3.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p3.nid = n.nid AND $time_condition) AS gross FROM {node} AS n LEFT JOIN {node_counter} AS c ON n.nid = c.nid WHERE type IN (". implode(", ", $product_types) .") GROUP BY n.nid DESC";
        break;
      case 'pgsql':
        $sql = "SELECT n.nid, n.title, c.totalcount, (SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.nid = n.nid AND $time_condition) AS sold, (SELECT (SUM(p2.price * p2.qty)) FROM {uc_order_products} AS p2 LEFT JOIN {uc_orders} AS o ON p2.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p2.nid = n.nid AND $time_condition) AS revenue, (SELECT (SUM(p3.price * p3.qty) - SUM(p3.cost * p3.qty)) FROM {uc_order_products} AS p3 LEFT JOIN {uc_orders} AS o ON p3.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p3.nid = n.nid AND $time_condition) AS gross FROM {node} AS n LEFT JOIN {node_counter} AS c ON n.nid = c.nid WHERE type IN (". implode(", ", $product_types) .") GROUP BY n.nid";
        break;
    }
  }
  else {
    $header = array(
      array('data' => t('#')),
      array('data' => t('Product'), 'field' => 'n.title'),
      array('data' => t('Sold'), 'field' => 'sold'),
      array('data' => t('Revenue'), 'field' => 'revenue', 'sort' => 'desc'),
      array('data' => t('Gross'), 'field' => 'gross'),
    );
    $csv_rows[] = array(t('#'), t('Product'), t('Sold'), t('Revenue'), t('Gross'));


    switch ($GLOBALS['db_type']) {
      case 'mysqli':
      case 'mysql':
        $sql = "SELECT n.nid, n.title, (SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.nid = n.nid AND $time_condition) AS sold, (SELECT (SUM(p2.price * p2.qty)) FROM {uc_order_products} AS p2 LEFT JOIN {uc_orders} AS o ON p2.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p2.nid = n.nid AND $time_condition) AS revenue, (SELECT (SUM(p3.price * p3.qty) - SUM(p3.cost * p3.qty)) FROM {uc_order_products} AS p3 LEFT JOIN {uc_orders} AS o ON p3.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p3.nid = n.nid AND $time_condition) AS gross FROM {node} AS n WHERE type IN (". implode(', ', $product_types) .') GROUP BY n.nid DESC';
        break;
      case 'pgsql':
        $sql = "SELECT n.nid, n.title, (SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.nid = n.nid AND $time_condition) AS sold, (SELECT (SUM(p2.price * p2.qty)) FROM {uc_order_products} AS p2 LEFT JOIN {uc_orders} AS o ON p2.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p2.nid = n.nid AND $time_condition) AS revenue, (SELECT (SUM(p3.price * p3.qty) - SUM(p3.cost * p3.qty)) FROM {uc_order_products} AS p3 LEFT JOIN {uc_orders} AS o ON p3.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p3.nid = n.nid AND $time_condition) AS gross FROM {node} AS n WHERE type IN (". implode(', ', $product_types) .') GROUP BY n.nid, n.title';
        break;
    }
  }

  $sql_count = "SELECT COUNT(nid) FROM {node} WHERE type IN (". implode(", ", $product_types) .")";
  $products = pager_query($sql . tablesort_sql($header), $page_size, 0, $sql_count);

  $rows = array();
  while ($product = db_fetch_array($products)) {
    $row_cell = ($page * variable_get('uc_reports_table_size', 30)) + count($rows) + 1;
    $product_cell = l($product['title'], 'node/'. $product['nid']);
    $product_csv = $product['title'];
    $sold_cell = empty($product['sold']) ? 0 : $product['sold'];
    $sold_csv = $sold_cell;
    $revenue_cell = uc_currency_format(empty($product['revenue']) ? 0 : $product['revenue']);
    $revenue_csv = empty($product['revenue']) ? 0 : $product['revenue'];
    $gross_cell = uc_currency_format(empty($product['gross']) ? 0 : $product['gross']);
    $gross_csv = empty($product['gross']) ? 0 : $product['gross'];

    if (module_exists('uc_attribute')) {
      $breakdown_product = $breakdown_sold = $breakdown_revenue = $breakdown_gross = '';

      foreach (_uc_reports_product_get_skus($product['nid']) as $model) {  // was $models
        $sold = db_result(db_query("SELECT SUM(qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d AND $time_condition", $model, $product['nid']));
        $sold = empty($sold) ? 0 : $sold;
        $revenue = db_result(db_query("SELECT SUM(p.price * p.qty) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d AND $time_condition", $model, $product['nid']));
        $revenue = empty($revenue) ? 0 : $revenue;
        $gross = db_result(db_query("SELECT (SUM(p.price * p.qty) - SUM(p.cost * p.qty)) FROM {uc_order_products} AS p LEFT JOIN {uc_orders} AS o ON p.order_id = o.order_id WHERE o.order_status IN $order_statuses AND p.model = '%s' AND p.nid = %d AND $time_condition", $model, $product['nid']));
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
      $sold_cell = '<strong>'. $sold_cell .'</strong>'. $breakdown_sold;
      $revenue_cell = '<strong>'. $revenue_cell .'</strong>'. $breakdown_revenue;
      $gross_cell = '<strong>'. $gross_cell .'</strong>'. $breakdown_gross;
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
      $csv_rows[] = array($row_cell, $product_csv, $views, $sold_csv, $revenue_csv, $gross_csv);
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
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No products found'), 'colspan' => count($header)));
  }
  $csv_data = uc_reports_store_csv('uc_products', $csv_rows);

  // Build the page output holding the form, table, and CSV export link.
  $output = drupal_get_form('uc_reports_products_custom_form', $args, $args['status']);
  $output .= theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'&nbsp;&nbsp;&nbsp;'. (isset($_GET['nopage']) ? l(t('Show paged records'), 'admin/store/reports/products/custom') : l(t('Show all records'), 'admin/store/reports/products/custom', array('query' => 'nopage=1'))) .'</div>';
  $output .= theme('pager', NULL, $page_size);
  $output .= '<small>*'. t('Make sure %setting_name is set to %state in the <a href="!url">access log settings page</a> to enable views column.', array('%setting_name' => 'count content views', '%state' => 'enabled', '!url' => url('admin/reports/settings', array('query' => 'destination=admin/store/reports/products/custom')))) .'</small>';

  return $output;
}

// Form builder for the custom product report.
function uc_reports_products_custom_form(&$form_state, $values, $statuses) {
  $form = array();

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

  if ($statuses === FALSE) {
    $statuses = variable_get('uc_reports_reported_statuses', array('completed'));
  }

  $form['search']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.') .'<br />'. t('Hold Ctrl + click to select multiple statuses.'),
    '#options' => $options,
    '#default_value' => $statuses,
    '#multiple' => TRUE,
    '#size' => 5,
  );

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );

  return $form;
}

function uc_reports_products_custom_form_validate($form, &$form_state) {
  if (empty($form_state['values']['status'])) {
    form_set_error('status', t('You must select at least one order status.'));
  }
}

function uc_reports_products_custom_form_submit($form, &$form_state) {
  $timezone_offset = _uc_reports_timezone_offset();

  // Build the start and end dates from the form.
  $start_date = gmmktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']);
  $end_date = gmmktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']);

  $args = array(
    $start_date,
    $end_date,
    urlencode(implode(',', array_keys($form_state['values']['status']))),
  );

  $form_state['redirect'] = array('admin/store/reports/products/custom/'. implode('/', $args));
}

/**
 * Return a themed table for product reports.
 *
 * Straight duplication of theme_table, but our row handling is different.
 *
 * @see theme_table()
 * @ingroup themeable
 */
function theme_uc_reports_product_table($header, $rows, $attributes = array(), $caption = NULL) {

  // Add sticky headers, if applicable.
  if (count($header)) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'] = empty($attributes['class']) ? 'sticky-enabled' : ($attributes['class'] .' sticky-enabled');
  }

  $output = '<table'. drupal_attributes($attributes) .">\n";

  if (isset($caption)) {
    $output .= '<caption>'. $caption ."</caption>\n";
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
        if (isset($attributes['class'])) {
          $attributes['class'] .= ' '. $class;
        }
        else {
          $attributes['class'] = $class;
        }

        // Build row
        $output .= ' <tr'. drupal_attributes($attributes) .'>';
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
 * Display the sales summary report.
 */
function uc_reports_sales_summary() {
  // "Now" timestamp
  $time = time();

  // Site time minus GMT time, in seconds
  $timezone_offset = _uc_reports_timezone_offset();

  // Find day/month/year of "Now" in site timezone
  $date_month = format_date($time, 'custom', 'n', $timezone_offset);
  $date_year = format_date($time, 'custom', 'Y', $timezone_offset);
  $date_day_of_month = format_date($time, 'custom', 'j', $timezone_offset);
  $date_days_in_month = format_date($time, 'custom', 't', $timezone_offset);

  // Calculate Unix timecodes (defined to be in GMT time)
  // for beginning and ending of reporting periods
  // Use gm functions so PHP won't try to do any timezone conversions by itself
  $month_start = gmmktime(0, 0, 0, $date_month, 1, $date_year) - $timezone_offset;
  $month_end = gmmktime(23, 59, 59, $date_month, $date_days_in_month, $date_year) - $timezone_offset;
  $today_start = gmmktime(0, 0, 0, $date_month, $date_day_of_month, $date_year) - $timezone_offset;
  $today_end = gmmktime(23, 59, 59, $date_month, $date_day_of_month, $date_year) - $timezone_offset;

  // Initialize variables used later
  $order_statuses = _uc_reports_order_statuses();
  $format = variable_get('uc_date_format_default', 'm/d/Y');
  // Build the report table header.
  $header = array(t('Sales data'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Calculate and add today's sales summary to the report table.
  $today = _uc_reports_get_sales($today_start);

  $context = array(
    'revision' => 'themed-original',
    'type' => 'amount',
  );

  $rows[] = array(
    l(t('Today, !date', array('!date' => format_date($today_start, 'custom', $format, $timezone_offset))), 'admin/store/orders/search/results/0/0/0/0/0/0/'. $today_start .'/'. $today_end),
    $today['total'],
    uc_price($today['income'], $context),
    uc_price($today['average'], $context),
  );

  // Calculate and add yesterday's sales summary to the report table.
  $yesterday = _uc_reports_get_sales($today_start - 86400);

  $rows[] = array(
    l(t('Yesterday, !date', array('!date' => format_date($today_start - 86400, 'custom', $format, $timezone_offset))), 'admin/store/orders/search/results/0/0/0/0/0/0/'. ($today_start - 86400) .'/'. ($today_end - 86400)),
    $yesterday['total'],
    uc_price($yesterday['income'], $context),
    uc_price($yesterday['average'], $context),
  );

  // Get the sales report for the month.
  $month = _uc_reports_get_sales($month_start, 'month');
  $month_title = format_date($month_start, 'custom', 'M Y', $timezone_offset);

  // Add the month-to-date details to the report table.
  $rows[] = array(
    l(t('Month-to-date, @month', array('@month' => $month_title)), 'admin/store/orders/search/results/0/0/0/0/0/0/'. $month_start .'/'. $month_end),
    $month['total'],
    uc_price($month['income'], $context),
    uc_price($month['average'], $context),
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
    uc_price($daily_revenue, $context),
    '',
  );

  // Store the number of days remaining in the month.
  $remaining_days = $date_days_in_month - $date_day_of_month;

  // Add the projected totals for the month to the report table.
  $rows[] = array(
    t('Projected totals for @date', array('@date' => $month_title)),
    round($month['total'] + ($daily_orders * $remaining_days), 2),
    uc_price(round($month['income'] + ($daily_revenue * $remaining_days), 2), $context),
    '',
  );

  // Add the sales data report table to the output.
  $output = theme('table', $header, $rows, array('class' => 'uc-sales-table'));

  // Build the header statistics table header.
  $header = array(array('data' => t('Statistics'), 'width' => '50%'), '');

  $rows = array(
    array(array('data' => t('Grand total sales')), array('data' => uc_price(db_result(db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE order_status IN $order_statuses")), $context))),
    array(array('data' => t('Customers total')), array('data' => db_result(db_query("SELECT COUNT(DISTINCT uid) FROM {uc_orders} WHERE order_status IN $order_statuses")))),
    array(array('data' => t('New customers today')), array('data' => db_result(db_query("SELECT COUNT(DISTINCT uid) FROM {uc_orders} WHERE order_status IN $order_statuses AND %d >= created AND created >= %d", $today_end, $today_start)))),
    array(array('data' => t('Online customers')), array('data' => db_result(db_query("SELECT COUNT(DISTINCT s.uid) FROM {sessions} as s LEFT JOIN {uc_orders} as o ON s.uid = o.uid WHERE s.uid > 0 AND o.order_status IN $order_statuses")))),
  );

  // Add the statistics table to the output.
  $output .= theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));

  // Build the total orders by status table header.
  $header = array(array('data' => t('Total orders by status'), 'width' => '50%'), '');
  $rows = array();
  $unknown = 0;

  // Loop through the order statuses with their total number of orders.
  $result = db_query("SELECT s.order_status_id, order_status, s.title, s.weight, COUNT(o.order_status) as order_count FROM {uc_orders} as o LEFT JOIN {uc_order_statuses} as s ON s.order_status_id = o.order_status GROUP BY s.order_status_id, order_status, s.title, s.weight ORDER BY s.weight DESC");
  while ($status = db_fetch_array($result)) {
    if (!empty($status['title'])) {
      // Add the total number of orders with this status to the table.
      $rows[] = array(
        l($status['title'], 'admin/store/orders/sort/'. $status['order_status_id']),
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
  $output .= theme('table', $header, $rows, array('class' => 'uc-sales-table'));

  return $output;
}

/**
 * Display the yearly sales report form and table.
 */
function uc_reports_sales_year() {
  // "Now" timestamp
  $time = time();

  // Site time minus GMT time, in seconds
  $timezone_offset = _uc_reports_timezone_offset();

  $order_statuses = _uc_reports_order_statuses();

  // Get the year for the report from the URL.
  if (intval(arg(5)) == 0) {
    $year = format_date($time, 'custom', 'Y', $timezone_offset);
  }
  else {
    $year = arg(5);
  }

  // Build the header for the report table.
  $header = array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order'));

  // Build the header to the CSV export.
  $csv_rows = array(array(t('Month'), t('Number of orders'), t('Total revenue'), t('Average order')));

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );

  // For each month of the year...
  for ($i = 1; $i <= 12; $i++) {
    // Calculate the start and end timestamps for the month in local time.
    $month_start = gmmktime(0, 0, 0, $i, 1, $year) - $timezone_offset;
    $month_end = gmmktime(23, 59, 59, $i + 1, 0, $year) - $timezone_offset;

    // Get the sales report for the month.
    $month_sales = _uc_reports_get_sales($month_start, 'month');

    // Calculate the average order total for the month.
    if ($month_sales['total'] != 0) {
      $month_average = round($month_sales['income'] / $month_sales['total'], 2);
    }
    else {
      $month_average = 0;
    }

    // Add the month's row to the report table.
    $rows[] = array(
      l(format_date($month_start, 'custom', 'M Y', $timezone_offset), 'admin/store/orders/search/results/0/0/0/0/0/0/'. $month_start .'/'. $month_end),
      $month_sales['total'],
      uc_price($month_sales['income'], $context),
      uc_price($month_average, $context),
    );

    // Add the data to the CSV export.
    $csv_rows[] = array(
      format_date($month_start, 'custom', 'M Y', $timezone_offset),
      $month_sales['total'],
      $month_sales['income'],
      $month_average,
    );
  }

  // Calculate the start and end timestamps for the year in local time.
  $year_start = gmmktime(0, 0, 0, 1, 1, $year) - $timezone_offset;
  $year_end = gmmktime(23, 59, 59, 1, 0, $year + 1) - $timezone_offset;

  // Get the sales report for the year.
  $year_sales = _uc_reports_get_sales($year_start, 'year');

  // Calculate the average order total for the year.
  if ($year_sales['total'] != 0) {
    $year_average = round($year_sales['income'] / $year_sales['total'], 2);
  }
  else {
    $year_average = 0;
  }

  // Add the total row to the report table.
  $rows[] = array(
    l(t('Total @year', array('@year' => $year)), 'admin/store/orders/search/results/0/0/0/0/0/0/'. $year_start .'/'. $year_end),
    $year_sales['total'],
    uc_price($year_sales['income'], $context),
    uc_price($year_average, $context),
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
  $output = drupal_get_form('uc_reports_sales_year_form', $year);
  $output .= theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'</div>';

  return $output;
}

/**
 * Form to specify a year for the yearly sales report.
 *
 * @ingroup forms
 * @see uc_reports_sales_year_form_submit()
 */
function uc_reports_sales_year_form($form_state, $year) {
  $form['year'] = array(
    '#type' => 'textfield',
    '#title' => t('Sales year'),
    '#default_value' => $year,
    '#maxlength' => 4,
    '#size' => 4,
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    '#prefix' => '<div class="sales-year">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * @see uc_reports_sales_year_form()
 */
function uc_reports_sales_year_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/store/reports/sales/year/'. $form_state['values']['year'];
}

/**
 * Display the custom sales report form and table.
 */
function uc_reports_sales_custom() {
  $timezone = _uc_reports_timezone_offset();
  $timezone_offset = time() + $timezone;
  $format = variable_get('uc_date_format_default', 'm/d/Y');

  // Use default report parameters if we don't detect values in the URL.
  if (arg(5) == '') {
    $args = array(
      'start_date' => gmmktime(0, 0, 0, gmdate('n', $timezone_offset), 1, gmdate('Y', $timezone_offset) - 1),
      'end_date' => time(),
      'length' => 'month',
      'status' => FALSE,
      'detail' => FALSE,
    );
  }
  else {
    $args = array(
      'start_date' => arg(5),
      'end_date' => arg(6),
      'length' => arg(7),
      'status' => explode(',', urldecode(arg(8))),
      'detail' => arg(9),
    );
  }

  // Pull the order statuses into a SQL friendly array.
  if ($args['status'] === FALSE) {
    $order_statuses = _uc_reports_order_statuses();
  }
  else {
    $order_statuses = "('". implode("', '", $args['status']) ."')";
  }

  // Build the header for the report table.
  $header = array(t('Date'), t('Number of orders'), t('Products sold'), t('Total revenue'));

  // Build the header to the CSV export.
  $csv_rows = array(array(t('Date'), t('Number of orders'), t('Products sold'), t('Total revenue')));

  // Grab the subreports based on the date range and the report breakdown.
  $subreports = _uc_reports_subreport_intervals($args['start_date'], $args['end_date'], $args['length']);

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );

  // Loop through the subreports and build the report table.
  foreach ($subreports as $subreport) {
    $product_data = '';
    $product_csv = '';
    $order_data = '';
    $order_csv = '';

    // Create the date title for the subreport.
    if ($args['length'] == 'day') {
      $date = format_date($subreport['start'], 'custom', $format .' - D', $timezone);
    }
    else {
      $date = format_date($subreport['start'], 'custom', $format, $timezone) .' - '. format_date($subreport['end'], 'custom', $format, $timezone);
    }

    // Build the order data for the subreport.
    $result = db_query("SELECT COUNT(*) as count, title FROM {uc_orders} LEFT JOIN {uc_order_statuses} ON order_status_id = order_status WHERE %d <= created AND created <= %d AND order_status IN $order_statuses GROUP BY order_status, {uc_order_statuses}.title, {uc_order_statuses}.weight ORDER BY weight ASC", $subreport['start'], $subreport['end']);
    $statuses = array();

    // Put the order counts into an array by status.
    while ($status = db_fetch_object($result)) {
      $statuses[] = t('!count - @title', array('!count' => $status->count, '@title' => $status->title));
    }

    $order_data = implode('<br />', $statuses);
    $order_csv = implode("\n", $statuses);

    // Build the product data for the subreport.
    if ($args['detail']) {
      // Grab the detailed product breakdown if selected.
      $result = db_query("SELECT SUM(op.qty) as count, n.title, n.nid FROM {uc_order_products} as op LEFT JOIN {uc_orders} as o ON o.order_id = op.order_id LEFT JOIN {node} as n ON n.nid = op.nid WHERE %d <= o.created AND o.created <= %d AND o.order_status IN $order_statuses GROUP BY n.nid, n.title ORDER BY count DESC, n.title ASC", $subreport['start'], $subreport['end']);
      while ($product_breakdown = db_fetch_object($result)) {
        $product_data .= $product_breakdown->count .' x '. l($product_breakdown->title, 'node/'. $product_breakdown->nid) ."<br />\n";
        $product_csv .= $product_breakdown->count .' x '. $product_breakdown->title ."\n";
      }
    }
    else {
      // Otherwise just display the total number of products sold.
      $product_data = db_result(db_query("SELECT SUM(qty) FROM {uc_orders} as o LEFT JOIN {uc_order_products} as op ON o.order_id = op.order_id WHERE %d <= created AND created <= %d AND order_status IN $order_statuses", $subreport['start'], $subreport['end']));
      $product_csv = $product_data;
    }

    // Tally up the revenue from the orders.
    $revenue_count = db_result(db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE %d <= created AND created <= %d AND order_status IN $order_statuses", $subreport['start'], $subreport['end']));

    // Add the subreport's row to the report table.
    $rows[] = array(
      $date,
      empty($order_data) ? '0' : $order_data,
      empty($product_data) ? '0' : $product_data,
      uc_price($revenue_count, $context),
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
  $order_total = db_result(db_query("SELECT COUNT(*) FROM {uc_orders} WHERE %d <= created AND created <= %d AND order_status IN $order_statuses", $args['start_date'], $args['end_date']));
  $product_total = db_result(db_query("SELECT SUM(qty) FROM {uc_orders} AS o LEFT JOIN {uc_order_products} AS op ON o.order_id = op.order_id WHERE %d <= created AND created <= %d AND order_status IN $order_statuses", $args['start_date'], $args['end_date']));
  $revenue_total = db_result(db_query("SELECT SUM(order_total) FROM {uc_orders} WHERE %d <= created AND created <= %d AND order_status IN $order_statuses", $args['start_date'], $args['end_date']));

  // Add the total row to the report table.
  $rows[] = array(
    t('Total'),
    $order_total,
    $product_total,
    uc_price($revenue_total, $context),
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
  $output = drupal_get_form('uc_reports_sales_custom_form', $args, $args['status']);
  $output .= theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'</div>';

  return $output;
}

/**
 * Form builder for the custom sales report.
 *
 * @ingroup forms
 * @see
 *   uc_reports_sales_custom_form_validate()
 *   uc_reports_sales_custom_form_submit()
 */
function uc_reports_sales_custom_form($form_state, $values, $statuses) {
  $timezone_offset = _uc_reports_timezone_offset();
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
      'month' => format_date($values['start_date'], 'custom', 'n', $timezone_offset),
      'day' => format_date($values['start_date'], 'custom', 'j', $timezone_offset),
      'year' => format_date($values['start_date'], 'custom', 'Y', $timezone_offset),
    ),
  );
  $form['search']['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array(
      'month' => format_date($values['end_date'], 'custom', 'n', $timezone_offset),
      'day' => format_date($values['end_date'], 'custom', 'j', $timezone_offset),
      'year' => format_date($values['end_date'], 'custom', 'Y', $timezone_offset),
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
    $statuses = variable_get('uc_reports_reported_statuses', array('completed'));
  }

  $form['search']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.') .'<br />'. t('Hold Ctrl + click to select multiple statuses.'),
    '#options' => $options,
    '#default_value' => $statuses,
    '#multiple' => TRUE,
    '#size' => 5,
  );

  $form['search']['detail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a detailed list of products ordered.'),
    '#default_value' => $values['detail'],
  );

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );

  return $form;
}

/**
 * Ensure an order status was selected.
 *
 * @see uc_reports_sales_custom_form()
 */
function uc_reports_sales_custom_form_validate($form, &$form_state) {
  if (empty($form_state['values']['status'])) {
    form_set_error('status', t('You must select at least one order status.'));
  }
}

/**
 * @see uc_reports_sales_custom_form()
 */
function uc_reports_sales_custom_form_submit($form, &$form_state) {
  $timezone_offset = _uc_reports_timezone_offset();

  // Build the start and end dates from the form.
  $start_date = gmmktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']) - $timezone_offset;
  $end_date = gmmktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']) - $timezone_offset;
  $args = array(
    $start_date,
    $end_date,
    $form_state['values']['length'],
    implode(',', array_keys($form_state['values']['status'])),
    $form_state['values']['detail'],
  );

  $form_state['redirect'] = 'admin/store/reports/sales/custom/'. implode('/', $args);
}
