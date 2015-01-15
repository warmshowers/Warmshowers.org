<?php

/**
 * @file
 * Displays sales tax report.
 */

/**
 * Displays the sales tax report form and table.
 */
function uc_tax_report_report_page($start_date = NULL, $end_date = NULL, $status = NULL) {
  // "Now" timestamp
  $time = time();

  // Site time minus GMT time, in seconds
  $timezone_offset = _uc_reports_timezone_offset();

  $format = variable_get('uc_date_format_default', 'm/d/Y');

  // Use default report parameters if we don't detect values in the URL.
  if ($start_date == '') {
    $args = array(
      'start_date' => gmmktime(0, 0, 0, format_date($time, 'custom', 'n', $timezone_offset), 1, format_date($time, 'custom', 'Y', $timezone_offset) - 1) - $timezone_offset,
      'end_date' => time(),
      'status' => FALSE,
    );
  }
  else {
    $args = array(
      'start_date' => $start_date,
      'end_date' => $end_date,
      'status' => explode(',', urldecode($status)),
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
  $header = array(t('Tax Name'), t('Jurisdiction'), t('Tax rate'), t('Total taxable amount'), t('Total tax collected'));
  $rows = array();
  $csv_rows = array();
  $csv_rows[] = $header;

  // Query to get the tax line items in this date range

  $result = db_query("SELECT ucoli.amount, ucoli.title, ucoli.data FROM {uc_orders} ucord LEFT JOIN {uc_order_statuses} ON order_status_id = order_status LEFT JOIN {uc_order_line_items} ucoli ON ucord.order_id = ucoli.order_id WHERE %d <= created AND created <= %d AND order_status IN $order_statuses AND ucoli.type = 'tax'", $args['start_date'], $args['end_date']);

  // add up the amounts by jurisdiction
  $totals = array();
  $no_meta_totals = array();

  while ($item = db_fetch_object($result)) {
    $name = trim($item->title);
    $amount = floatval($item->amount);

    // get the meta-data out of the serialized array
    $data = unserialize($item->data);
    $jurisdiction = trim($data['tax_jurisdiction']);
    $taxable_amount = floatval($data['taxable_amount']);
    $rate = floatval($data['tax_rate']);

    // make a line item in the report for each name/jurisdiction/rate
    $key = strtolower($name) . strtolower($jurisdiction) . number_format($rate, 5);

    if (!empty($jurisdiction) && $amount && $taxable_amount) {
      // we have meta-data
      if (empty($totals[$key])) {
        $totals[$key] = array(
          'name' => $name,
          'jurisdiction' => $jurisdiction,
          'rate' => $rate,
          'taxable_amount' => $taxable_amount,
          'amount' => $amount,
        );
      }
      else {
        $totals[$key]['taxable_amount'] += $taxable_amount;
        $totals[$key]['amount'] += $amount;
      }
    }
    elseif ($amount) {
      // Old data: no meta-data was stored. Just report the amount collected.
      if (empty($no_meta_totals[$key])) {
        $no_meta_totals[$key] = array(
          'name' => $name,
          'amount' => $amount,
        );
      }
      else {
        $no_meta_totals[$key]['amount'] += $amount;
      }
    }
  }

  // sort and make this into a report

  ksort($totals);
  ksort($no_meta_totals);

  $taxable_amount = 0;
  $amount = 0;
  $star_legend = '';

  $context = array(
    'revision' => 'themed-original',
    'type' => 'amount',
  );
  foreach ($totals as $line) {
    $row = array(
      $line['name'],
      $line['jurisdiction'],
      number_format($line['rate'] * 100, 3) . '%',
      uc_price($line['taxable_amount'], $context),
      uc_price($line['amount'], $context),
    );
    $rows[] = $row;
    $csv_rows[] = array(
      $line['name'],
      $line['jurisdiction'],
      number_format($line['rate'] * 100, 3) . '%',
      $line['taxable_amount'],
      $line['amount'],
    );
    $taxable_amount += $line['taxable_amount'];
    $amount += $line['amount'];
  }

  foreach ($no_meta_totals as $line) {
    $row = array(
      $line['name'],
      '*',
      '*',
      '*',
      uc_price($line['amount'], $context),
    );
    $rows[] = $row;
    $csv_rows[] = $row = array(
      $line['name'],
      '*',
      '*',
      '*',
      $line['amount'],
    );
    $amount += $line['amount'];
    // We have at least one no-meta-data line. Explain why.
    $star_legend = t('* No information on jurisdiction, tax rate, or taxable amount is available for this line.');
  }

  // Add a totals line.
  $row = array(
    t('Total'),
    '',
    '',
    uc_price($taxable_amount, $context),
    uc_price($amount, $context),
  );
  $rows[] = $row;
  $csv_rows[] = array(
    t('Total'),
    '',
    '',
    $taxable_amount,
    $amount,
  );

  // Cache the CSV export.
  $csv_data = uc_reports_store_csv('uc_tax_report', $csv_rows);

  // Build the page output holding the form, table, and CSV export link.
  $output = drupal_get_form('uc_tax_report_params_form', $args, $args['status']);
  $output .= theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-sales-table'));
  if ($star_legend) {
    $output .= '<div class="uc-reports-note"><p>' . $star_legend . '</p></div>';
  }

  $output .= '<div class="uc-reports-links">'. l(t('Export to CSV file.'), 'admin/store/reports/getcsv/'. $csv_data['report'] .'/'. $csv_data['user']) .'</div>';

  return $output;
}

/**
 * Form for parameters on the tax report.
 *
 * @see uc_tax_report_params_form_validate()
 * @see uc_tax_report_params_form_submit()
 * @ingroup forms
 */
function uc_tax_report_params_form($form_state, $values) {
  $form = array();

  // Site time minus GMT time, in seconds
  $timezone_offset = _uc_reports_timezone_offset();

  $form['params'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customize tax report parameters'),
    '#description' => t('Adjust these values and update the report to build your sales tax report. Once submitted, the report may be bookmarked for easy reference in the future.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['params']['start_date'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array(
      'month' => format_date($values['start_date'], 'custom', 'n', $timezone_offset),
      'day' => format_date($values['start_date'], 'custom', 'j', $timezone_offset),
      'year' => format_date($values['start_date'], 'custom', 'Y', $timezone_offset),
    ),
  );
  $form['params']['end_date'] = array(
    '#type' => 'date',
    '#title' => t('End date'),
    '#default_value' => array(
      'month' => format_date($values['end_date'], 'custom', 'n', $timezone_offset),
      'day' => format_date($values['end_date'], 'custom', 'j', $timezone_offset),
      'year' => format_date($values['end_date'], 'custom', 'Y', $timezone_offset),
    ),
  );

  $options = array();
  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  $stat = $values['status'];
  if ($stat === FALSE) {
    $stat = variable_get('uc_reports_reported_statuses', array('completed'));
  }

  $form['params']['status'] = array(
    '#type' => 'select',
    '#title' => t('Order statuses'),
    '#description' => t('Only orders with selected statuses will be included in the report.') .'<br />'. t('Hold Ctrl + click to select multiple statuses.'),
    '#options' => $options,
    '#default_value' => $stat,
    '#multiple' => TRUE,
    '#size' => 5,
  );

  $form['params']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update report'),
  );

  return $form;
}

/**
 * Ensures an order status is selected.
 *
 * @see uc_tax_report_params_form()
 */
function uc_tax_report_params_form_validate($form, &$form_state) {
  if (empty($form_state['values']['status'])) {
    form_set_error('status', t('You must select at least one order status.'));
  }
}

/**
 * Form submission handler for uc_tax_report_params_form().
 *
 * @see uc_tax_report_params_form()
 * @see uc_tax_report_params_form_validate()
 */
function uc_tax_report_params_form_submit($form, &$form_state) {
  $timezone_offset = _uc_reports_timezone_offset();

  // Build the start and end dates from the form.
  $start_date = gmmktime(0, 0, 0, $form_state['values']['start_date']['month'], $form_state['values']['start_date']['day'], $form_state['values']['start_date']['year']) - $timezone_offset;
  $end_date = gmmktime(23, 59, 59, $form_state['values']['end_date']['month'], $form_state['values']['end_date']['day'], $form_state['values']['end_date']['year']) - $timezone_offset;

  $args = array(
    $start_date,
    $end_date,
    drupal_urlencode(implode(',', array_keys($form_state['values']['status']))),
  );

  $form_state['redirect'] = 'admin/store/reports/tax/'. implode('/', $args);
}
