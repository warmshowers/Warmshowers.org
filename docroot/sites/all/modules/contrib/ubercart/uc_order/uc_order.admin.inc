<?php

/**
 * @file
 * Order administration menu items.
 */

/**
 * Displays an overview of the order settings.
 */
function uc_order_settings_overview() {
  $summaries = array();

  // don't load admin/store/settings/orders/edit/workflow/create
  $summaries = array_merge($summaries, summarize_child_form_pages('admin/store/settings/orders/edit/basic', FALSE, TRUE));
  $summaries = array_merge($summaries, summarize_child_form_pages('admin/store/settings/orders/edit/workflow', FALSE, TRUE));
  $summaries = array_merge($summaries, summarize_child_form_pages('admin/store/settings/orders/edit/panes', FALSE, TRUE));

  // Theme it all up in a summaries overview.
  return theme('summary_overview', $summaries);
}

/**
 * Generates the settings form for orders.
 *
 * @ingroup forms
 */
function uc_order_settings_form() {

  $form['admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Admin settings'),
    '#summary callback' => 'summarize_form',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['admin']['uc_order_number_displayed'] = array(
    '#type' => 'select',
    '#title' => t('Number of orders on overview screen'),
    '#options' => drupal_map_assoc(range(10, 100, 10)),
    '#summary' => t('Displaying @orders orders at a time on the admin overview', array('@orders' => variable_get('uc_order_number_displayed', 30))),
    '#default_value' => variable_get('uc_order_number_displayed', 30),
  );
  $form['admin']['uc_order_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable order logging'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Order logging is enabled.'),
      t('Order logging is disabled.'),
    ),
    '#default_value' => variable_get('uc_order_logging', TRUE),
  );
  $form['admin']['uc_order_capitalize_addresses'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capitalize address on order screens'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Addresses on order view pages are capitalized.'),
      t('Addresses on order view pages are not capitalized.'),
    ),
    '#default_value' => variable_get('uc_order_capitalize_addresses', TRUE),
  );

  $form['customer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customer settings'),
    '#summary callback' => 'summarize_form',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['customer']['uc_cust_view_order_invoices'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow customers to view order invoices from their order history.'),
    '#description' => t('Enabling this feature allows pop-up invoices to be opened when a particular order is being viewed.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Customers are allowed to view order invoices from their accounts.'),
      t('Customers are not allowed to view order invoices from their accounts.'),
    ),
    '#default_value' => variable_get('uc_cust_view_order_invoices', TRUE),
  );
  $form['customer']['uc_cust_order_invoice_template'] = array(
    '#type' => 'select',
    '#title' => t('On-site invoice template'),
    '#description' => t('Select the invoice template to use when invoices are viewed on the site.<br />This is separate from the template used to e-mail invoices to customers which is configured through <a href="!url">Conditional actions</a>.', array('!url' => url(CA_UI_PATH))),
    '#options' => uc_order_template_options(),
    '#summary' => t('You are using the %template order invoice template.', array('%template' => variable_get('uc_cust_order_invoice_template', 'customer'))),
    '#default_value' => variable_get('uc_cust_order_invoice_template', 'customer'),
  );

  return system_settings_form($form);
}

/**
 * Summarizes the order status settings.
 *
 * This function summarizes the order statuses that have been defined. It is
 * organized under a parent node specifying that the following settings are
 * order statuses.
 *
 * @param $form
 *   The form passed from the summarizer.
 * @param $states
 *   An array of order statuses.
 *
 * @return
 *   An array of summary information.
 */
function _uc_order_states_summarize($form, $states) {
  $statuses = $items = array();

  foreach ($states as $state) {
    $items[] = t('@state', array('@state' => $state['title']));
  }

  $statuses[] = array(
    'data' => t('The following order statuses have been defined:'),
    'children' => $items,
  );

  return $statuses;
}

/**
 * Displays the order workflow form for order state and status customization.
 *
 * @see uc_order_workflow_form_submit()
 * @see theme_uc_order_state_table()
 * @see theme_uc_order_status_table()
 * @ingroup forms
 */
function uc_order_workflow_form() {
  $states = uc_order_state_list();
  $statuses = uc_order_status_list();

  $form['order_states'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order states'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#summary callback' => 'summarize_null',
    '#theme' => 'uc_order_state_table',
    '#tree' => TRUE,
  );

  foreach ($states as $state) {

    $form['order_states'][$state['id']]['#summary callback'] = 'summarize_form';
    $form['order_states'][$state['id']]['title'] = array(
      '#value' => $state['title'],
    );

    // Create the select box for specifying a default status per order state.
    $options = array();
    foreach ($statuses as $status) {
      if ($status['state'] == $state['id']) {
        $options[$status['id']] = $status['title'];
      }
    }
    if (empty($options)) {
      $form['order_states'][$state['id']]['default'] = array(
        '#value' => t('- N/A -'),
      );
    }
    else {
      $form['order_states'][$state['id']]['default'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => uc_order_state_default($state['id']),
      );
    }
  }

  $form['order_statuses'] = array(
    '#type' => 'fieldset',
    '#title' => t('Order statuses'),
    '#collapsible' => FALSE,
    '#summary callback' => '_uc_order_states_summarize',
    '#summary arguments' => array($states),
    '#theme' => 'uc_order_status_table',
    '#tree' => TRUE,
  );

  // Build the state option array for the order status table.
  $options = array();
  foreach ($states as $state) {
    $options[$state['id']] = $state['title'];
  }

  foreach ($statuses as $status) {
    $form['order_statuses'][$status['id']]['id'] = array(
      '#value' => $status['id'],
    );
    $form['order_statuses'][$status['id']]['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $status['title'],
      '#size' => 32,
      '#required' => TRUE,
    );
    $form['order_statuses'][$status['id']]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 20,
      '#default_value' => $status['weight'],
    );
    if ($status['locked']) {
      $form['order_statuses'][$status['id']]['state'] = array(
        '#value' => uc_order_state_data($status['state'], 'title'),
      );
      $form['order_statuses'][$status['id']]['locked'] = array(
        '#type' => 'hidden',
        '#value' => TRUE,
      );
    }
    else {
      $form['order_statuses'][$status['id']]['state'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $status['state'],
      );
      $form['order_statuses'][$status['id']]['remove'] = array(
        '#type' => 'checkbox',
      );
    }
  }

  $form['order_statuses']['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create new status'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
  );

  return $form;
}

/**
 * Form submission handler for uc_order_workflow_form().
 *
 * @see uc_order_workflow_form()
 */
function uc_order_workflow_form_submit($form, &$form_state) {
  foreach ($form_state['values']['order_states'] as $key => $value) {
    variable_set('uc_state_'. $key .'_default', $value['default']);
  }

  foreach ($form_state['values']['order_statuses'] as $key => $value) {
    if ($value['locked'] != TRUE && $value['remove'] == TRUE) {
      db_query("DELETE FROM {uc_order_statuses} WHERE order_status_id = '%s'", $key);
      drupal_set_message(t('Order status %status removed.', array('%status' => $key)));
    }
    else {
      if ($value['locked'] == TRUE) {
        db_query("UPDATE {uc_order_statuses} SET title = '%s', weight = %d "
                ."WHERE order_status_id = '%s'", $value['title'],
                 $value['weight'], $key);
      }
      else {
        db_query("UPDATE {uc_order_statuses} SET title = '%s', state = '%s', "
                ."weight = %d WHERE order_status_id = '%s'", $value['title'],
                 $value['state'], $value['weight'], $key);
      }
    }
  }

  switch ($form_state['values']['op']) {
    case t('Submit changes'):
      drupal_set_message(t('Order workflow information saved.'));
      break;
    case t('Create new status'):
      $form_state['redirect'] = 'admin/store/settings/orders/edit/workflow/create';
  }
}

/**
 * Settings for the order panes.
 *
 * @ingroup forms
 */
function uc_order_panes_form() {
  $panes = _order_pane_list();

  foreach ($panes as $pane) {
    foreach ($pane['show'] as $view) {
      $form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_show_'. $view] = array(
        '#type' => 'checkbox',
        '#title' => $pane['title'],
        '#default_value' => variable_get('uc_order_pane_'. $pane['id'] .'_show_'. $view, $pane['enabled']),
      );
      $form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_enabled'] = &$form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_show_'. $view];
      $form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_weight_'. $view] = array(
        '#type' => 'weight',
        '#default_value' => variable_get('uc_order_pane_'. $pane['id'] .'_weight_'. $view, $pane['weight']),
        '#attributes' => array('class' => 'uc-order-pane-' . $view . '-weight'),
      );
      $form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_weight'] = &$form['panes'][$view][$pane['id']]['uc_order_pane_'. $pane['id'] .'_weight_'. $view];
      $form['panes'][$view][$pane['id']]['weight'] = variable_get('uc_order_pane_'. $pane['id'] .'_weight_'. $view, $pane['weight']);
    }
  }

  $titles = _get_order_screen_titles();
  foreach ($form['panes'] as $view => $data) {

    if (!is_array($form['panes'][$view])) {
      continue;
    }

    uasort($form['panes'][$view], 'uc_weight_sort');
    foreach ($data as $pane => $info) {
      unset($form['panes'][$view][$pane]['weight']);
    }
    $fieldset = array(
      '#theme' => 'uc_pane_sort_table',
      '#pane_prefix' => 'uc_order_pane',
      '#draggable' => 'uc-order-pane-' . $view . '-weight',
      '#type' => 'fieldset',
      '#title' => t('Order panes on !screen screen', array('!screen' => $titles[$view])),
      '#summary callback' => '_uc_order_panes_summarize',
      '#summary arguments' => array($data, $titles[$view], $view),
    );
    $form['panes'][$view] = array_merge($fieldset, $form['panes'][$view]);
    $form['panes']['#summary callback'] = 'summarize_form';
  }

  return system_settings_form($form);
}

/**
 * Presents the form to create a custom order status.
 *
 * @see uc_order_status_create_form_validate()
 * @see uc_order_status_create_form_submit()
 * @ingroup forms
 */
function uc_order_status_create_form() {
  $form['status_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order status ID'),
    '#description' => t('Must be a unique ID with no spaces.'),
    '#summary callback' => 'summarize_null',
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['status_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The order status title displayed to users.'),
    '#summary callback' => 'summarize_null',
    '#size' => 32,
    '#maxlength' => 48,
    '#required' => TRUE,
  );

  // Build the state option array for the order status table.
  $options = array();
  foreach (uc_order_state_list() as $state) {
    $options[$state['id']] = $state['title'];
  }
  $form['status_state'] = array(
    '#type' => 'select',
    '#title' => t('Order state'),
    '#description' => t('Set which order state this status is for.'),
    '#summary callback' => 'summarize_null',
    '#options' => $options,
    '#default_value' => 'post_checkout',
  );

  $form['status_weight'] = array(
    '#type' => 'weight',
    '#title' => t('List position'),
    '#delta' => 20,
    '#default_value' => 0,
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  $form['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/store/settings/orders/edit/workflow'),
  );

  return $form;
}

/**
 * Ensures the new status id is unique and has no spaces.
 *
 * @see uc_order_status_create_form()
 * @see uc_order_status_create_form_submit()
 */
function uc_order_status_create_form_validate($form, &$form_state) {
  $new_status = strtolower(trim($form_state['values']['status_id']));
  if (strpos($new_status, ' ') !== FALSE || $new_status == 'all') {
    form_set_error('status_id', t('You have entered an invalid status ID.'));
  }

  $statuses = uc_order_status_list();
  foreach ($statuses as $status) {
    if ($new_status == $status['id']) {
      form_set_error('status_id', t('This ID is already in use.  Please specify a unique ID.'));
    }
  }
}

/**
 * Form submission handler for uc_order_status_create_form_submit().
 *
 * @see uc_order_status_create_form()
 * @see uc_order_status_create_form_validate()
 */
function uc_order_status_create_form_submit($form, &$form_state) {
  db_query("INSERT INTO {uc_order_statuses} (order_status_id, title, state, "
          ."weight, locked) VALUES ('%s', '%s', '%s', %d, 0)",
           $form_state['values']['status_id'], $form_state['values']['status_title'],
           $form_state['values']['status_state'], $form_state['values']['status_weight']);

  drupal_set_message(t('Custom order status created.'));

  $form_state['redirect'] = 'admin/store/settings/orders/edit/workflow';
}

/**
 * Displays the main order admin screen, an overview of all received orders.
 */
function uc_order_admin($sql = NULL, $args = NULL, $search = FALSE) {
  $header = array(
    array('data' => t('Actions')),
    array('data' => t('Order ID'), 'field' => 'o.order_id', 'sort' => 'desc'),
    array('data' => t('Customer')),
    array('data' => t('Total'), 'align' => 'center', 'field' => 'o.order_total'),
    array('data' => t('Purchase date'), 'align' => 'center', 'field' => 'o.created'),
    array('data' => t('Status'), 'field' => 'os.title'),
  );

  if (is_null($sql)) {
    $args = array();
    $show_status = 1;
    $sql = 'SELECT o.order_id, o.uid, o.billing_first_name, o.billing_last_name, o.order_total, o.order_status, o.created, os.title '
          .'FROM {uc_orders} o LEFT JOIN {uc_order_statuses} os ON o.order_status = os.order_status_id ';


    if (arg(3) == 'sort' && !is_null(arg(4))) {
      $_SESSION['sort_status'] = arg(4);
      $args[] = arg(4);
    }
    $where = '';
    if (!isset($_SESSION['sort_status']) || $_SESSION['sort_status'] != 'all') {
      if (!empty($_SESSION['sort_status']) && is_string($_SESSION['sort_status'])) {
        $where = "WHERE o.order_status = '%s'";
        $args[] = $_SESSION['sort_status'];
      }
      else {
        $where = 'WHERE o.order_status IN '. uc_order_status_list('general', TRUE);
      }
    }
    $sql .= $where;
  }

  // Need to include tablesort_sql() when the SQL command is passed as an
  // argument AND when we build the SQL command here in this function.
  $sql .= tablesort_sql($header);

  $address = variable_get('uc_customer_list_address', 'billing');
  if ($address == 'shipping') {
      $sql = str_replace('billing', 'delivery', $sql);
  }
  else {
    $address = 'billing';
  }

  $context = array(
    'revision' => 'themed-original',
    'type' => 'amount',
  );

  $rows = array();

  $result = pager_query($sql, variable_get('uc_order_number_displayed', 30), 0, NULL, $args);
  while ($order = db_fetch_object($result)) {
    if ($address == 'shipping') {
      $order_name = $order->delivery_first_name .' '. $order->delivery_last_name;
    }
    else {
      $order_name = $order->billing_first_name .' '. $order->billing_last_name;
    }
    if (trim($order_name) == '') {
      if ($order->uid !== 0) {
        $account = db_result(db_query("SELECT name FROM {users} WHERE uid = %d", $order->uid));
      }
      if (empty($account)) {
        $order_name = t('User: none');
      }
      else {
        $order_name = t('User: !name', array('!name' => $account));
      }
    }

    $rows[] = array(
      'data' => array(
        array('data' => uc_order_actions($order, TRUE)),
        array('data' => $order->order_id),
        array('data' => check_plain($order_name)),
        array('data' => uc_price($order->order_total, $context), 'align' => 'right'),
        array('data' => format_date($order->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y')), 'align' => 'center'),
        array('data' => $order->title),
      ),
      'id' => 'order-'. $order->order_id,
    );
  }

  drupal_add_js(array(
    'ucURL' => array(
      'adminOrders' => url('admin/store/orders/'),
    ),
  ), 'setting');
  drupal_add_js(drupal_get_path('module', 'uc_order') .'/uc_order.js');

  $output = '';
  if ($search === FALSE) {
    $output = '<div class="order-overview-form">'. drupal_get_form('uc_order_select_form') .'</div>'
             .'<div class="order-overview-form">'. drupal_get_form('uc_order_admin_sort_form') .'</div>';
  }

  $output .= theme('table', $header, $rows, array('class' => 'uc-orders-table'));
  $output .= theme('pager', NULL, variable_get('uc_order_number_displayed', 30), 0);
  return $output;
}

/**
 * Creates the order status select box on the order overview screen.
 *
 * @see uc_order_admin_sort_form_submit()
 * @ingroup forms
 */
function uc_order_admin_sort_form() {
  $options = array('-1' => t('Active orders'));

  foreach (uc_order_status_list() as $status) {
    $options[$status['id']] = $status['title'];
  }

  $options['all'] = t('All orders');

  if (!isset($_SESSION['sort_status']) || is_null($_SESSION['sort_status'])) {
    $default_status = -1;
  }
  else {
    $default_status = $_SESSION['sort_status'];
  }

  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('View by status'),
    '#options' => $options,
    '#default_value' => $default_status,
    '#attributes' => array('onchange' => 'this.form.submit();')
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'View',
    '#attributes' => array('style' => 'display: none;')
  );

  return $form;
}

/**
 * Form submission handler for uc_order_admin_sort_form().
 *
 * @see uc_order_admin_sort_form()
 */
function uc_order_admin_sort_form_submit($form, &$form_state) {
  if ($form_state['values']['status'] == '-1') {
    unset($_SESSION['sort_status']);
    drupal_goto('admin/store/orders');
  }
  else {
    $_SESSION['sort_status'] = $form_state['values']['status'];
    drupal_goto('admin/store/orders/sort/'. $form_state['values']['status']);
  }
}

/**
 * Creates a textfield box to select an order by ID on the order
 * overview screen.
 *
 * @see uc_order_select_form_submit()
 * @ingroup forms
 */
function uc_order_select_form() {
  $form['order_id'] = array(
    '#type' => 'textfield',
    '#title' => t('View order number'),
    '#size' => 10,
    '#maxlength' => 10
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('View'),
    '#attributes' => array('style' => 'display: none;')
  );

  return $form;
}

/**
 * Form submission handler for uc_order_select_form().
 *
 * @see uc_order_select_form()
 */
function uc_order_select_form_submit($form, &$form_state) {
  if (uc_order_exists($form_state['values']['order_id'])) {
    drupal_goto('admin/store/orders/'. $form_state['values']['order_id']);
  }
}

/**
 * Creates a new order and redirect to its edit screen.
 *
 * @see uc_order_create_form_create_validate()
 * @see uc_order_create_form_create_submit()
 * @ingroup forms
 */
function uc_order_create($account = NULL) {
  drupal_add_js(array('ucURL' => array('adminOrders' => url('admin/store/orders/'))), 'setting');
  drupal_add_js(drupal_get_path('module', 'uc_order') .'/uc_order.js');

  $output = '<div id="customer-search-link" style="position: relative; top: 2px; cursor: pointer;" '
           .'onclick="load_customer_search();">'
          . uc_store_get_icon('file:order_view') .' '
          . t('Search for an existing customer.') .'</div>';
  $output .= '<div id="customer-create-link" style="position: relative; top: 2px; cursor: pointer;" '
            .'onclick="load_new_customer_form();">'
           . uc_store_get_icon('file:menu_customers_small') .' '
           . t('Create a new customer.') .'</div>';
  $output .= '<div id="customer-select"></div><br />'
           . drupal_get_form('uc_order_create_form', $account);

  return $output;
}

/**
 * Selects a customer to whom to assign a new order.
 *
 * @see uc_order_create_form_submit()
 * @ingroup forms
 */
function uc_order_create_form(&$form_state, $account) {
  $form['customer'] = array(
    '#type' => 'fieldset',
    '#title' => t('New order customer'),
    '#description' => t('Use the buttons above to have these fields filled in or just submit the form with the fields blank to create a blank order.'),
    '#collapsible' => FALSE,
  );
  $form['customer']['uid'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($account) ? $account->uid : 0,
  );
  $form['customer']['text']['uid_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer number'),
    '#default_value' => isset($account) ? $account->uid : 0,
    '#maxlength' => 10,
    '#size' => 10,
    '#disabled' => TRUE,
  );
  $form['customer']['primary_email'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($account) ? $account->mail : '',
  );
  $form['customer']['text']['primary_email_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Primary e-mail'),
    '#default_value' => isset($account) ? $account->mail : '',
    '#maxlength' => 64,
    '#size' => 32,
    '#disabled' => TRUE,
  );

  $form['create'] = array(
    '#type' => 'submit',
    '#value' => t('Create order'),
  );

  return $form;
}

/**
 * Form submission handler for uc_order_create_form().
 *
 * @see uc_order_create_form()
 */
function uc_order_create_form_submit($form, &$form_state) {
  global $user;

  $order = uc_order_new($form_state['values']['uid'], 'post_checkout');
  uc_order_comment_save($order->order_id, $user->uid, t('Order created by the administration.'), 'admin');

  $form_state['redirect'] = 'admin/store/orders/'. $order->order_id .'/edit';
}

/**
 * Displays a search form to browse all received orders.
 */
function uc_order_usearch() {
  $output = drupal_get_form('uc_order_search_form');

  if (arg(4) == 'results') {
    $output .= '<p>'. t('Search returned the following results:') .'</p>';

    $billing_first_name = strtolower(str_replace('*', '%', check_plain(arg(5))));
    $billing_last_name = strtolower(str_replace('*', '%', check_plain(arg(6))));
    $billing_company = strtolower(str_replace('*', '%', check_plain(arg(7))));
    $shipping_first_name = strtolower(str_replace('*', '%', check_plain(arg(8))));
    $shipping_last_name = strtolower(str_replace('*', '%', check_plain(arg(9))));
    $shipping_company = strtolower(str_replace('*', '%', check_plain(arg(10))));
    $start_date = check_plain(arg(11));
    $end_date = check_plain(arg(12));

    $where = '';
    $args = array();
    if ($billing_first_name !== '0' && $billing_first_name !== '%') {
      $where .= " AND LOWER(o.billing_first_name) LIKE '%s'";
      $args[] = $billing_first_name;
    }
    if ($billing_last_name !== '0' && $billing_last_name !== '%') {
      $where .= " AND LOWER(o.billing_last_name) LIKE '%s'";
      $args[] = $billing_last_name;
    }
    if ($billing_company !== '0' && $billing_company !== '%') {
      $where .= " AND LOWER(o.billing_company) LIKE '%s'";
      $args[] = $billing_company;
    }
    if ($shipping_first_name !== '0' && $shipping_first_name !== '%') {
      $where .= " AND LOWER(o.delivery_first_name) LIKE '%s'";
      $args[] = $shipping_first_name;
    }
    if ($shipping_last_name !== '0' && $shipping_last_name !== '%') {
      $where .= " AND LOWER(o.delivery_last_name) LIKE '%s'";
      $args[] = $shipping_last_name;
    }
    if ($shipping_company !== '0' && $shipping_company !== '%') {
      $where .= " AND LOWER(o.delivery_company) LIKE '%s'";
      $args[] = $shipping_company;
    }

    if ($start_date !== '0') {
      $where .= " AND o.created >= %d";
      $args[] = $start_date;
    }
    if ($end_date !== '0') {
      $where .= " AND o.created <= %d";
      $args[] = $end_date;
    }

    $sql = 'SELECT o.order_id, o.uid, o.billing_first_name, o.billing_last_name, o.order_total, '
          .'o.order_status, o.created, os.title FROM {uc_orders} o LEFT JOIN {uc_order_statuses} os '
          .'ON o.order_status = os.order_status_id WHERE o.order_status NOT IN '. uc_order_status_list('specific', TRUE)
         . $where;

    $output .= uc_order_admin($sql, $args, TRUE);
  }

  return $output;
}

/**
 * Display a form to select a previously entered address.
 *
 * @see uc_order_address_book_form()
 */
function uc_order_address_book() {
  $uid = intval($_POST['uid']);
  $type = $_POST['type'];
  $func = $_POST['func'];

  print drupal_get_form('uc_order_address_book_form', $uid, $type, $func);
  exit();
}

/**
 * Presents previously entered addresses as selectable options.
 *
 * @see uc_order_address_book()
 * @ingroup forms
 */
function uc_order_address_book_form($form_state, $uid = 0, $type = 'billing', $func = '') {
  $select = uc_select_address($uid, $type, $func);

  if ($uid == 0) {
    $form['desc'] = array('#value' => '<br />'. t('You must select a customer before address<br />information is available.<br />') .'<br />');
  }
  elseif (is_null($select)) {
    $form['desc'] = array('#value' => '<br />'. t('No addresses found for customer.') .'<br />');
  }
  else {
    $form['addresses'] = uc_select_address($uid, $type, $func, t('Select an address'));
    $form['addresses']['#prefix'] = '<div style="float: left; margin-right: 1em;">';
    $form['addresses']['#suffix'] = '</div>';
  }

  $form['close'] = array(
    '#type' => 'button',
    '#value' => t('Close'),
    '#attributes' => array('onclick' => "return close_address_select('#". $type ."_address_select');"),
  );

  return $form;
}

/**
 * Presents the customer search results and let one of them be chosen.
 *
 * @see uc_order_select_customer_form()
 */
function uc_order_select_customer($email = NULL) {
  $options = NULL;
  $where = '';
  $output = '';

  // Return the search results and let them pick one!
  if (arg(4) == 'search') {
    $first_name = strtolower(str_replace('*', '%', $_POST['first_name']));
    $last_name = strtolower(str_replace('*', '%', $_POST['last_name']));
    $email = strtolower(str_replace('*', '%', $_POST['email']));
    $username = strtolower(str_replace('*', '%', $_POST['username']));

    if ($first_name && $first_name !== '%') {
      $where .= " AND LOWER(o.billing_first_name) LIKE '". db_escape_string($first_name) ."'";
    }
    if ($last_name && $last_name !== '%') {
      $where .= " AND LOWER(o.billing_last_name) LIKE '". db_escape_string($last_name) ."'";
    }
    if ($email && $email !== '%') {
      $where .= " AND (LOWER(o.primary_email) LIKE '". db_escape_string($email) ."' OR LOWER(u.mail) LIKE '" . db_escape_string($email) ."')";
    }
    if ($username && $username !== '%') {
      $where .= " AND LOWER(u.name) LIKE '". db_escape_string($username) ."'";
    }

    $query = "SELECT DISTINCT u.uid, u.name, u.mail, o.billing_first_name, "
            ."o.billing_last_name FROM {users} AS u LEFT JOIN {uc_orders} "
            ."AS o ON u.uid = o.uid WHERE u.uid > 0 AND (o.order_status "
            ."IS NULL OR o.order_status IN ". uc_order_status_list('general', TRUE) .")". $where
            ." ORDER BY o.billing_last_name ASC";
    $result = db_query($query);

    $options = array();
    while ($user = db_fetch_object($result)) {
      $name = '';
      if (!empty($user->billing_first_name) && !empty($user->billing_last_name)) {
        $name = $user->billing_last_name .', '. $user->billing_first_name .' ';
      }
      // Options formated as "First Last <email@example.com> (username)".
      $options[$user->uid . ':' . $user->mail] = $name . '<' . $user->mail . '>' . ' (' . $user->name . ')';
    }

    if (count($options) == 0) {
      // No search results found.
      $output .= '<p>'. t('Search returned no results.') .'</p>';
      $options = NULL;
    }
    else {
      // Display search results.
      $output .= '<p>'. t('Search returned the following:') .'</p>';
    }
  }

  // Check to see if the e-mail address for a new user is unique.
  if (arg(5) == 'check') {
    $email = $_POST['email'];
    if (!valid_email_address($email)) {
      $output .= t('Invalid e-mail address.') .'<br />';
    }
    $result = db_query("SELECT uid, mail FROM {users} WHERE LOWER(mail) = LOWER('%s')", $email);
    if ($user = db_fetch_object($result)) {
      $output .= t('An account already exists for that e-mail.') .'<br /><br />';
      $output .= '<b>'. t('Use this account now?') .'</b><br />'
               . t('User @uid - @mail', array('@uid' => $user->uid, '@mail' => $user->mail)) .' <input type="button" '
                .'onclick="select_existing_customer('. $user->uid .', \''
               . $user->mail .'\');" value="'. t('Apply') .'" /><br /><br /><hr /><br />';
    }
    else {
      $name = uc_store_email_to_username($email);

      $fields = array(
        'name' => $name,
        'mail' => $email,
        'pass' => user_password(6),
        'status' => variable_get('uc_new_customer_status_active', TRUE) ? 1 : 0,
      );

      $account = user_save('', $fields);

      if ($_POST['sendmail'] == 'true') {
        // Manually set the password so it appears in the e-mail.
        $account->password = $fields['pass'];

        // Send the e-mail through the user module.
        drupal_mail('user', 'register_admin_created', $email, NULL, array('account' => $account), uc_store_email_from());

        $output .= t('Account details sent to e-mail provided.<br /><br /><strong>Username:</strong> @username<br /><strong>Password:</strong> @password', array('@username' => $fields['name'], '@password' => $fields['pass'])) .'<br /><br />';
      }

      $output .= '<strong>'. t('Use this account now?') .'</strong><br />'
               . t('User @uid - @mail', array('@uid' => $account->uid, '@mail' => $account->mail)) .' <input type="button" '
                .'onclick="select_existing_customer('. $account->uid .', \''
               . $account->mail .'\');" value="'. t('Apply') .'" /><br /><br /><hr /><br />';
    }
  }

  $output .= drupal_get_form('uc_order_select_customer_form', $options);

  print $output;
  exit();
}

/**
 * Form to choose a customer from a list.
 *
 * @see uc_order_select_customer()
 * @ingroup forms
 */
function uc_order_select_customer_form($form_state, $options = NULL) {
  if (is_null(arg(4))) {
    $form['desc'] = array(
      '#value' => '<div>'. t('Search for a customer based on these fields.')
                 .'<br />'. t('Use * as a wildcard to match any character.') .'<br />'
                 .'(<em>'. t('Leave a field empty to ignore it in the search.')
                 .'</em>)</div>',
    );

    // Customer first name.
    $form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#size' => 24,
      '#maxlength' => 32,
    );

    // Customer last name.
    $form['last_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#size' => 24,
      '#maxlength' => 32,
    );

    // Customer e-mail address.
    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 24,
      '#maxlength' => 96,
    );

    // Customer username.
    $form['customer']['username'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#size' => 24,
      '#maxlength' => 96,
    );
  }
  elseif (arg(4) == 'search' && !is_null($options)) {
    // Display search results.
    $form['cust_select'] = array(
      '#type' => 'select',
      '#title' => t('Select a customer'),
      '#size' => 7,
      '#options' => $options,
      '#default_value' => key($options),
      '#attributes' => array('ondblclick' => 'return select_customer_search();'),
    );
  }
  elseif (arg(4) == 'new') {
    $form['desc'] = array(
      '#value' => '<div>'. t('Enter an e-mail address for the new customer.') .'</div>',
    );

    // Customer e-mail address.
    $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Customer e-mail address'),
      '#size' => 24,
      '#maxlength' => 96,
    );
  }

  if (is_null(arg(4))) {
    $form['search'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#attributes' => array('onclick' => 'return load_customer_search_results();'),
    );
  }
  elseif (arg(4) == 'search') {
    if (!is_null($options)) {
      $form['select'] = array(
        '#type' => 'submit',
        '#value' => t('Select'),
        '#attributes' => array('onclick' => 'return select_customer_search();'),
      );
    }
    $form['back'] = array(
      '#type' => 'submit',
      '#value' => t('Back'),
      '#attributes' => array('onclick' => 'return load_customer_search();'),
    );
  }
  elseif (arg(4) == 'new') {
    // Option to notify customer.
    $form['sendmail'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail account details to customer.'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#attributes' => array('onclick' => 'return check_new_customer_address();'),
    );
  }

  $form['close'] = array(
    '#type' => 'submit',
    '#value' => t('Close'),
    '#attributes' => array('onclick' => 'return close_customer_select();'),
  );

  return $form;
}

/**
 * Returns the sortable table listing of a customer's orders.
 *
 * @param $uid
 *   The user ID whose orders you wish to list.
 */
function uc_order_history($user) {
  drupal_set_title(t('My order history'));

  $header = array(
    array('data' => t('Date'), 'field' => 'o.created', 'sort' => 'desc'),
    array('data' => t('Order #'), 'field' => 'o.order_id'),
    array('data' => t('Status'), 'field' => 'os.title'),
    array('data' => t('Products'), 'field' => 'products'),
    array('data' => t('Total'), 'field' => 'o.order_total')
  );

  $rows = array();

  $context = array(
    'revision' => 'themed-original',
    'type' => 'amount',
  );

  $result = pager_query("SELECT o.order_id, o.uid, o.created, os.title, SUM(op.qty) AS products, o.order_total AS total, o.order_status FROM {uc_orders} AS o LEFT JOIN {uc_order_statuses} AS os ON o.order_status = os.order_status_id LEFT JOIN {uc_order_products} AS op ON o.order_id = op.order_id WHERE o.uid = %d AND o.order_status IN ". uc_order_status_list('general', TRUE) ." GROUP BY o.order_id, o.uid, o.created, os.title, o.order_total, o.order_status". tablesort_sql($header), 20, 0, "SELECT COUNT(*) FROM {uc_orders} WHERE uid = %d AND order_status NOT IN ". uc_order_status_list('specific', TRUE), $user->uid);

  // Build a table based on the customer's orders.
  while ($order = db_fetch_object($result)) {
    $context['subject'] = array('order' => $order);

    $link = l($order->order_id, 'user/'. $user->uid .'/order/'. $order->order_id);

    if (user_access('view all orders')) {
      $link .= '<span class="order-admin-icons">'. uc_order_actions($order, TRUE) .'</span>';
    }

    $rows[] = array(
      array('data' => format_date($order->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y'))),
      array('data' => $link),
      array('data' => check_plain($order->title)),
      array('data' => (!is_null($order->products) ? $order->products : 0), 'align' => 'center'),
      array('data' => uc_price($order->total, $context), 'align' => 'right'),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No orders available.'), 'colspan' => 5));
  }

  return theme('table', $header, $rows, array('class' => 'uc-order-history'))
       . theme('pager', NULL, 20, 0);
}

/**
 * Displays the order view screen, constructed via hook_order_pane().
 */
function uc_order_view($order, $view = 'view') {
  if ($view == 'customer') {
    $breadcrumb = drupal_get_breadcrumb();
    $breadcrumb[2] = l(t('Order history'), 'user/'. arg(1) .'/orders');
    drupal_set_breadcrumb($breadcrumb);
  }

  if ($view == 'invoice') {
    $output = theme('uc_order', $order, 'print', variable_get('uc_cust_order_invoice_template', 'customer'));
    $output .= '<div align="right" style="margin-top: 1em; margin-right: 1em;"><input type="button" value="'. t('Print invoice') .'" onclick="window.print();" /> '
              .'<input type="button" value="'. t('Close window') .'" onclick="window.close();" /></div>';
    print $output;
    exit();
  }

  $output = '';
  $panes = _order_pane_list($view);
  foreach ($panes as $pane) {
    if (in_array($view, $pane['show']) &&
        variable_get('uc_order_pane_'. $pane['id'] .'_show_'. $view, $pane['enabled'])) {
      $func = $pane['callback'];
      if (function_exists($func) && ($contents = $func($view, $order)) != NULL) {
        $output .= '<div class="order-pane '. $pane['class'] .'" id="order-pane-' . $pane['id'] . '">';
        if ($func('show-title', NULL) !== FALSE) {
          $output .= '<div class="order-pane-title">'. $pane['title'] .': '
                   . $func('view-title', $order) .'</div>';
        }
        $output .= is_array($contents) ? drupal_render($contents) : $contents;
        $output .= '</div>';
      }
    }
  }

  if ($view == 'customer' && variable_get('uc_cust_view_order_invoices', TRUE)) {
    $url = url('user/' . arg(1) . '/order/' . arg(3) . '/invoice/print');
    drupal_add_js("function open_invoice() { window.open('$url', '". t('Invoice') ."', 'toolbar=0,scrollbars=1,location=0,statusbar=0,menubar=0,resizable=1,width=600,height=480,left=50,top=50'); }", 'inline');
    $contents = '<a href="#" onclick="open_invoice();">'
               . uc_store_get_icon('file:print') .' '
               . t('Click to open a window with a printable invoice.') .'</a>';
    $output .= '<div class="order-pane">'. $contents .'</div>';
  }

  return $output;
}

/**
 * Displays the order edit screen, constructed via hook_order_pane().
 *
 * @see uc_order_edit_form_validate()
 * @see uc_order_edit_form_submit()
 * @see theme_uc_order_edit_form()
 * @ingroup forms
 */
function uc_order_edit_form($form_state, $order) {
  $form['order_id'] = array('#type' => 'hidden', '#value' => $order->order_id);
  $form['order_uid'] = array('#type' => 'hidden', '#value' => $order->uid);

  $modified = isset($form_state['post']['order_modified']) ? $form_state['post']['order_modified'] : $order->modified;
  $form['order_modified'] = array('#type' => 'hidden', '#value' => $modified);

  $panes = _order_pane_list('edit');
  foreach ($panes as $pane) {
    if (in_array('edit', $pane['show']) &&
        variable_get('uc_order_pane_'. $pane['id'] .'_show_edit', $pane['enabled'])) {
      $func = $pane['callback'];
      if (function_exists($func) && ($contents = $func('edit-form', $order)) != NULL) {
        $form = array_merge($form, $contents);
      }
    }
  }

  $form['submit-changes'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
    '#attributes' => array('class' => 'save-button'),
  );

  if (uc_order_can_delete($order)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }

  return $form;
}

/**
 * Formats the uc_order_edit_form().
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_validate()
 * @see uc_order_edit_form_submit()
 * @ingroup themeable
 */
function theme_uc_order_edit_form($form) {
  $output = '';

  $panes = _order_pane_list();
  foreach ($panes as $pane) {
    if (in_array('edit', $pane['show']) &&
        variable_get('uc_order_pane_'. $pane['id'] .'_show_edit', TRUE)) {
      $func = $pane['callback'];
      if (function_exists($func) && ($contents = _call_order_pane_byref($func, 'edit-theme', $form)) != NULL) {
        if (isset($pane['theme_all']) && is_array($pane['theme_all']) && in_array('edit', $pane['theme_all'])) {
          $output .= $contents;
        }
        else {
          $output .= '<div class="order-pane '. $pane['class'] .'" id="order-pane-' . $pane['id'] . '">';
          if ($func('show-title', NULL) !== FALSE) {
            $output .= '<div class="order-pane-title">'. $pane['title'] .': '
                     . $func('edit-title', $form) .'</div>';
          }
          $output .= $contents .'</div>';
        }
      }
    }
  }

  $output .= '<div class="order-pane abs-left">'. drupal_render($form['order_id']) . drupal_render($form['order_modified'])
           . drupal_render($form['form_id']) . drupal_render($form['form_token'])
           . drupal_render($form['submit-changes']) . drupal_render($form['delete'])
            .'</div>';

  return $output;
}

/**
 * Prevents order edits from colliding.
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_submit()
 * @see theme_uc_order_edit_form()
 */
function uc_order_edit_form_validate($form, &$form_state) {
  $order = uc_order_load($form_state['values']['order_id']);
  if ($form_state['values']['order_modified'] != $order->modified) {
    form_set_error('order_modified', t('This order has been modified by another user, changes cannot be saved.'));
  }
}

/**
 * Form submission handler for uc_order_edit_form().
 *
 * @see uc_order_edit_form()
 * @see uc_order_edit_form_validate()
 * @see theme_uc_order_edit_form()
 */
function uc_order_edit_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Delete')) {
    drupal_goto('admin/store/orders/'. $form_state['values']['order_id'] .'/delete');
  }

  $order = uc_order_load($form_state['values']['order_id']);
  $log = array();

  uc_order_module_invoke('edit-products', $order, $_POST);

  $panes = _order_pane_list();
  foreach ($panes as $pane) {
    if (in_array('edit', $pane['show']) &&
        variable_get('uc_order_pane_'. $pane['id'] .'_show_edit', TRUE)) {
      $func = $pane['callback'];
      if (function_exists($func)) {
        if (($changes = $func('edit-process', $form_state['values'])) != NULL) {
          foreach ($changes as $key => $value) {
            if ($order->$key != $value) {
              if (!is_array($value)) {
                $log[$key] = array('old' => $order->$key, 'new' => $value);
              }
              $order->$key = $value;
            }
          }
        }
        if (($ops = $func('edit-ops', NULL)) != NULL) {
          $perform[$func] = $ops;
        }
      }
    }
  }

  unset($order->products);
  if (isset($_POST['products']) && is_array($_POST['products'])) {
    foreach ($_POST['products'] as $product) {
      if (!isset($product['remove']) && intval($product['qty']) > 0) {
        $product['data'] = unserialize($product['data']);
        $product = (object)$product;
        $order->products[] = $product;
      }
      else {
        $log['remove_'. $product['nid']] = $product['title'] .' removed from order.';
      }
    }
  }

  // Load line items again, since some may have been updated by the form.
  $order->line_items = uc_order_load_line_items($order, TRUE);

  // Merge it with the defaultish line items.
  $order->line_items = array_merge($order->line_items, uc_order_load_line_items($order, FALSE));
  usort($order->line_items, 'uc_weight_sort');

  if (variable_get('uc_order_logging', TRUE)) {
    uc_order_log_changes($order->order_id, $log);
  }

  uc_order_save($order);

  if (is_array($perform)) {
    foreach ($perform as $func => $ops) {
      if (in_array($form_state['values']['op'], $ops)) {
        $func($form_state['values']['op'], $form_state['values']);
      }
    }
  }

  drupal_set_message(t('Order changes saved.'));
}

/**
 * Populates the product add/edit div on the order edit screen.
 */
function uc_order_edit_products($order) {
  uc_order_module_invoke('edit-products', $order, $_POST);

  if (is_array($_POST['products'])) {
    foreach ($_POST['products'] as $key => $product) {
      $product['data'] = unserialize($product['data']);
      uc_order_product_save($order->order_id, (object) $product);
    }
  }

  switch ($_POST['action']) {
    case 'add_blank':
      db_query("INSERT INTO {uc_order_products} (order_id, qty) VALUES (%d, 1)", $order->order_id);
      if (variable_get('uc_order_logging', TRUE)) {
        uc_order_log_changes($order->order_id, array('add' => t('Added new product line to order.')));
      }
      break;
    case 'add':
      $form_state['values'] = $_POST;
      $product = node_load(intval($_POST['nid']));
      $product->qty = intval($_POST['qty']);
      $product->price = $product->sell_price;
      $product->data = module_invoke_all('add_to_cart_data', $form_state['values']);

      foreach (module_list() as $module) {
        $function = $module .'_cart_item';
        if (function_exists($function)) {
          // $product must be passed by reference.
          $function('load', $product);
        }
      }

      $price_info = array(
        'price' => $product->price,
        'qty' => 1,
      );
      $context = array(
        'revision' => 'original',
        'type' => 'order_product',
        'subject' => array(
          'order' => $order,
          'product' => $product,
          'node' => clone $product,
        ),
      );
      $product->price = uc_price($price_info, $context);

      drupal_alter('order_product', $product, $order);
      uc_order_product_save($order->order_id, $product);

      if (variable_get('uc_order_logging', TRUE)) {
        uc_order_log_changes($order->order_id, array('add' => t('Added (@qty) @product-title to order.', array('@qty' => $product->qty, '@product-title' => $product->title))));
      }
      break;
    case 'remove':
      $order_product_id = intval($_POST['opid']);
      db_query("DELETE FROM {uc_order_products} WHERE order_product_id = %d", $order_product_id);
      break;
  }

  $result = db_query("SELECT * FROM {uc_order_products} WHERE order_id = %d ORDER BY order_product_id", $order->order_id);
  while ($product = db_fetch_object($result)) {
    $products[] = $product;
  }

  print uc_strip_form(drupal_get_form('uc_order_edit_products_form', $products));
  exit();
}

/**
 * Form to allow ordered products' data to be changed.
 *
 * @see op_products_edit_table()
 * @see theme_uc_order_edit_products_form()
 * @see theme_uc_order_remove_product()
 */
function uc_order_edit_products_form($form_state, $products) {
  if (($product_count = count($products)) > 0) {
    $form['products'] = tapir_get_table('op_products_edit_table');
    for ($i = 0; $i < $product_count; $i++) {
      $form['products'][$i]['remove'] = array(
        '#type' => 'checkbox',
        '#name' => "products[$i][remove]",
        '#theme' => 'uc_order_remove_product',
        '#parents' => array(),
        '#img_id' => $products[$i]->order_product_id,
      );
      $form['products'][$i]['order_product_id'] = array(
        '#type' => 'hidden',
        '#value' => $products[$i]->order_product_id,
        '#name' => "products[$i][order_product_id]",
        '#parents' => array(),
      );
      $form['products'][$i]['nid'] = array(
        '#type' => 'hidden',
        '#value' => $products[$i]->nid,
        '#name' => "products[$i][nid]",
        '#parents' => array(),
      );
      $form['products'][$i]['qty'] = array(
        '#type' => 'textfield',
        '#value' => $products[$i]->qty,
        '#name' => "products[$i][qty]",
        '#parents' => array(),
        '#size' => 2,
        '#maxlength' => 6,
      );
      $form['products'][$i]['title'] = array(
        '#type' => 'textfield',
        '#value' => $products[$i]->title,
        '#name' => "products[$i][title]",
        '#parents' => array(),
        '#size' => 30,
      );
      $form['products'][$i]['model'] = array(
        '#type' => 'textfield',
        '#value' => $products[$i]->model,
        '#name' => "products[$i][model]",
        '#parents' => array(),
        '#size' => 6,
      );
      $form['products'][$i]['weight'] = array(
        '#type' => 'textfield',
        '#value' => $products[$i]->weight,
        '#name' => "products[$i][weight]",
        '#parents' => array(),
        '#size' => 3,
      );
      $form['products'][$i]['cost'] = array(
        '#type' => 'textfield',
        '#value' => uc_store_format_price_field_value($products[$i]->cost),
        '#name' => "products[$i][cost]",
        '#parents' => array(),
        '#size' => 5,
      );
      $form['products'][$i]['price'] = array(
        '#type' => 'textfield',
        '#value' => uc_store_format_price_field_value($products[$i]->price),
        '#name' => "products[$i][price]",
        '#parents' => array(),
        '#size' => 5,
      );
      $form['products'][$i]['data'] = array(
        '#type' => 'hidden',
        '#value' => $products[$i]->data,
        '#name' => "products[$i][data]",
        '#parents' => array(),
      );
    }
  }
  else {
    $form['products'] = array(
      '#value' => t('This order contains no products.'),
    );
  }
  return $form;
}

/**
 * @see uc_order_edit_products_form()
 * @ingroup themeable
 */
function theme_uc_order_edit_products_form($form) {
  return drupal_render($form);
}

/**
 * Formats the button to remove products from an order.
 *
 * @see op_products_edit_table()
 * @ingroup themeable
 */
function theme_uc_order_remove_product($form) {
  return '<img id="'. $form['#img_id'] .'" src="'
    .  base_path() . drupal_get_path('module', 'uc_store')
    .'/images/error.gif" style="padding-top: 1px; padding-left: .8em; '
    .'padding-right: 2px; float: left; cursor: pointer;" '
    .'alt="'. t('Remove this product.') .'" onclick="'
    .'remove_product_button(\''. addslashes(t('Remove product from order?')) .'\', this.id);" />'
    . drupal_render($form);
}

/**
 * Form to add a line item to an order.
 *
 * @see uc_order_add_line_item_form_validate()
 * @see uc_order_add_line_item_submit()
 */
function uc_order_add_line_item_form($form_state, $order, $line_item_id) {
  $func = _line_item_data($line_item_id, 'callback');
  if (!function_exists($func) || ($form = $func('form', $order->order_id)) == NULL) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Line Item Title'),
      '#description' => t('Display title of the line item.'),
      '#size' => 32,
      '#maxlength' => 128,
      '#default_value' => _line_item_data($line_item_id, 'title'),
    );
    $form['amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Line Item Amount'),
      '#description' => t('Amount of the line item without a currency sign.'),
      '#size' => 6,
      '#maxlength' => 13,
    );
  }

  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['line_item_id'] = array(
    '#type' => 'hidden',
    '#value' => $line_item_id,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add line item'),
    '#suffix' => l(t('Cancel'), 'admin/store/orders/'. $order->order_id .'/edit'),
  );

  return $form;
}

/**
 * Validates new line item data.
 *
 * @see uc_order_add_line_item_form()
 */
function uc_order_add_line_item_form_validate($form, &$form_state) {
  $func = _line_item_data($form_state['values']['line_item_id'], 'callback');
  if (function_exists($func) && ($form = $func('form', $form_state['values']['order_id'])) != NULL) {
    $func('validate', $form, $form_state);
  }
  else {
    if (!is_numeric($form_state['values']['amount'])) {
      form_set_error('amount', t('Amount must be numeric.'));
    }
  }
}

/**
 * Form submission handler for uc_order_add_line_item_form().
 *
 * @see uc_order_add_line_item_form()
 */
function uc_order_add_line_item_form_submit($form, &$form_state) {
  $func = _line_item_data($form_state['values']['line_item_id'], 'callback');
  if (function_exists($func) && ($form = $func('form', $form_state['values']['order_id'])) != NULL) {
    $func('submit', $form, $form_state);
  }
  else {
    uc_order_line_item_add($form_state['values']['order_id'], $form_state['values']['line_item_id'],
             $form_state['values']['title'], $form_state['values']['amount']);
    drupal_set_message(t('Line item added to order.'));
  }

  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'] .'/edit';
}

/**
 * Selects a product to add to the order.
 *
 * @see uc_order_load_product_select_form()
 */
function uc_order_load_product_select($order) {
  $types = uc_product_types();

  if (!empty($_POST['search'])) {
    $search = strtolower(str_replace('*', '%%', check_plain($_POST['search'])));
    $search_args = array_merge($types, array($search, $search));

    $result = db_query("SELECT n.nid, n.title FROM {node} AS n LEFT JOIN "
                      ."{uc_products} AS p ON n.nid = p.nid WHERE n.type IN "
                      ."(". db_placeholders($types, 'varchar') .") AND (LOWER(n.title) LIKE '%s' OR LOWER(p.model) LIKE '%s')"
                      ." ORDER BY n.title", $search_args);
  }
  else {
    $result = db_query("SELECT nid, title FROM {node} WHERE type IN (". db_placeholders($types, 'varchar') .") "
                      ."ORDER BY title", $types);
  }
  while ($row = db_fetch_object($result)) {
    $options[$row->nid] = $row->title;
  }

  $output = drupal_get_form('uc_order_product_select_form', $order->order_id, $options);

  print $output;
  exit();
}

/**
 * Form to choose a product to add to the order.
 */
function uc_order_product_select_form($form_state, $order_id, $options = array()) {
  if (count($options) == 0) {
    $options[0] = t('No products found.');
  }

  $form['unid'] = array(
    '#type' => 'select',
    '#title' => t('Select a product'),
    '#options' => $options,
    '#size' => 7,
    '#attributes' => array('ondblclick' => 'return select_product();'),
  );
  $form['product_search'] = array(
    '#type' => 'textfield',
    '#title' => t('Search by name or model/SKU (* is the wildcard)'),
  );

  $form['select'] = array(
    '#type' => 'button',
    '#value' => t('Select'),
    '#attributes' => array('onclick' => 'return select_product();'),
  );
  $form['search'] = array(
    '#type' => 'button',
    '#value' => t('Search'),
    '#attributes' => array('onclick' => 'return load_product_select('. $order_id .', true);'),
  );
  $form['close'] = array(
    '#type' => 'button',
    '#value' => t('Close'),
    '#attributes' => array('onclick' => 'return close_product_select();'),
  );

  return $form;
}

/**
 * Intermediate div that lets you set the qty and attributes for a product.
 *
 * @see uc_order_add_product_form()
 */
function uc_order_add_product($order, $product) {
  $output = '<div style="margin: 1em;"><strong>Add '. check_plain($product->title) .'</strong>'
          . drupal_get_form('uc_order_add_product_form', $order, $product) .'</div>';

  print $output;
  exit();
}

/**
 * Sets the quantity and attributes of a product added to the order.
 *
 * @see uc_order_add_product_form()
 */
function uc_order_add_product_form($form_state, $order, $node) {
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['add-qty'] = array(
    '#type' => 'textfield',
    '#title' => t('Qty'),
    '#default_value' => '1',
    '#size' => 2,
    '#maxlength' => 5,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add to order'),
    '#attributes' => array('onclick' => 'return add_product_to_order('. $order->order_id .', '. $node->nid .');')
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#attributes' => array('onclick' => "\$('#add-product-button').click(); return false;"),
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  uc_form_alter($form, $form_state, __FUNCTION__);

  return $form;
}

/**
 * Displays an invoice in the browser.
 */
function uc_order_invoice($order, $op = 'view') {
  $output = theme('uc_order', $order, $op, variable_get('uc_cust_order_invoice_template', 'customer'));

  if ($op == 'print') {
    print $output;
    exit();
  }

  return $output;
}

/**
 * Sets recipients of an invoice, and mail it.
 *
 * @see uc_order_mail_invoice_form_validate()
 * @see uc_order_mail_invoice_form_submit()
 * @ingroup forms
 */
function uc_order_mail_invoice_form($form_state, $order) {
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient e-mail address'),
    '#default_value' => $order->primary_email,
  );
  $form['submit' ] = array(
    '#type' => 'submit',
    '#value' => t('Mail invoice'),
  );

  return $form;
}

/**
 * Only mail invoices to valid email addresses.
 *
 * @see uc_order_mail_invoice_form()
 */
function uc_order_mail_invoice_form_validate($form, &$form_state) {
  $recipient = $form_state['values']['email'];
  if (empty($recipient) || !valid_email_address($recipient)) {
    form_set_error('email', t('Invalid e-mail address.'));
  }
}

/**
 * Form submission handler for uc_order_mail_invoice_form().
 *
 * @see uc_order_mail_invoice_form()
 */
function uc_order_mail_invoice_form_submit($form, &$form_state) {
  $order = uc_order_load($form_state['values']['order_id']);

  if ($order === FALSE) {
    drupal_set_message(t('Order @order_id does not exist.', array('@order_id' => $order_id)));
    drupal_goto('admin/store/orders');
  }

  $recipient = $form_state['values']['email'];

  $params = array('order' => $order);
  $sent = drupal_mail('uc_order', 'invoice', $recipient, uc_store_mail_recipient_language($recipient), $params, uc_store_email_from());

  if (!$sent) {
    drupal_set_message(t('E-mail failed.'));
  }
  else {
    $message = t('Invoice e-mailed to @email.', array('@email' => $recipient));
    drupal_set_message($message);
    uc_order_log_changes($order->order_id, array($message));
  }
}

/**
 * Displays a log of changes made to an order.
 */
function uc_order_log($order) {
  $result = db_query("SELECT * FROM {uc_order_log} WHERE order_id = %d ORDER BY created, order_log_id", $order->order_id);

  while ($change = db_fetch_object($result)) {
    $user = uc_get_initials($change->uid);
    $rows[] = array('data' => array(format_date($change->created, 'short'), $user == '-' ? $user : l($user, 'user/'. $change->uid), $change->changes), 'valign' => 'top');
  }

  if (count($rows)) {
    $header = array(t('Time'), t('User'), t('Changes'));
    $output = theme('table', $header, $rows);
  }
  else {
    $output = t('No changes have been logged for this order.');
  }

  return $output;
}

/**
 * Confirmation form to delete an order.
 *
 * @see uc_order_delete_confirm_form_submit()
 */
function uc_order_delete_confirm_form($form_state, $order) {
  if (!uc_order_can_delete($order)) {
    drupal_set_message(t('It is not possible to delete order @id.', array('@id' => $order->order_id)));
    drupal_goto('admin/store/orders');
  }

  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $order->order_id
  );

  return confirm_form($form, t('Are you sure you want to delete order @order_id?', array('@order_id' => $order->order_id)), 'admin/store/orders', NULL, t('Delete'));
}

/**
 * Form submission handler for uc_order_delete_confirm_form().
 *
 * @see uc_order_delete_confirm_form()
 */
function uc_order_delete_confirm_form_submit($form, &$form_state) {
  // Delete the specified order.
  uc_order_delete($form_state['values']['order_id']);

  // Display a message to the user and return to the order admin page.
  drupal_set_message(t('Order @order_id completely removed from the database.', array('@order_id' => $form_state['values']['order_id'])));

  $form_state['redirect'] = 'admin/store/orders';
}
