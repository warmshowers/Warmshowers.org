<?php

/**
 * @file
 * Store administration menu items.
 */

/**
 * Displays dashboard to other admin pages for the store.
 */
function uc_store_admin() {
  $tree = menu_navigation_links('navigation', 2);

  if (($type = variable_get('uc_store_admin_page_display', 1)) == 4) {
    $main_menu = menu_get_item('admin/store');
    $content = system_admin_menu_block((array)$main_menu);

    $output = theme('admin_block_content', $content);
  }
  else {
    $output = theme('uc_admin_dashboard', $type, $tree);
  }

  $results = module_invoke_all('store_status');

  foreach ($results as $message) {
    switch ($message['status']) {
      case 'warning':
        $icon = 'alert.gif';
        break;
      case 'error':
        $icon = 'error.gif';
        break;
      case 'ok':
      default:
        $icon = 'info.gif';
    }
    $icon = theme('image', drupal_get_path('module', 'uc_store') . '/images/' . $icon);

    $rows[] = array(
      array('data' => $icon, 'class' => 'icon'),
      array('data' => $message['title'], 'class' => 'title'),
      array('data' => $message['desc'], 'class' => 'message'),
    );
  }

  $output .= '<div style="clear: both;">';
  $output .= theme('table', NULL, $rows, array('class' => 'uc-store-status'), '<h2>' . t('Store status') . '</h2>');
  $output .= '</div>';

  return $output;
}

/**
 * Displays customer administration page.
 */
function uc_store_customers($message = NULL, $query = NULL, $count_query = NULL, $page_length = 25) {
  if (!module_exists('uc_order')) {
    return t('You must enable the order module to track customers.');
  }

  if (is_null($query)) {
    $query = "SELECT DISTINCT o.uid, u.mail, o.billing_first_name, "
            ."o.billing_last_name, o.billing_city, o.billing_zone, "
            ."o.billing_country FROM {uc_orders} AS o LEFT JOIN "
            ."{users} AS u ON o.uid = u.uid WHERE o.uid > 0 AND "
            ."o.order_status IN ". uc_order_status_list('general', TRUE);

    $count_query = "";
    switch ($GLOBALS['db_type']) {
      case 'mysql':
      case 'mysqli':
        $count_query = "SELECT COUNT(DISTINCT o.uid, o.billing_first_name, "
                      ."o.billing_last_name, u.mail) FROM {uc_orders} AS o "
                      ."LEFT JOIN {users} AS u ON o.uid = u.uid WHERE o.uid > 0 "
                      ."AND o.order_status IN ". uc_order_status_list('general', TRUE);
        break;
      case 'pgsql':
        $count_query = "SELECT DISTINCT o.uid, o.billing_last_name, o.billing_first_name, "
                      ."COUNT(*) "
                      ."FROM {uc_orders} AS o "
                      ."LEFT JOIN {users} AS u ON o.uid = u.uid WHERE o.uid > 0 "
                      ."AND o.order_status IN ". uc_order_status_list('general', TRUE)
                      ." GROUP BY o.uid, o.billing_last_name, o.billing_first_name ";
        break;
    }
  }

  $header = array(
    t('View'),
    array('data' => t('Name'), 'field' => 'o.billing_last_name', 'sort' => 'asc'),
    array('data' => t('E-mail'), 'field' => 'u.mail'),
    array('data' => t('City'), 'field' => 'o.billing_city'),
    array('data' => t('ID'), 'field' => 'o.uid'),
  );

  $query .= tablesort_sql($header);
  $count_query .= tablesort_sql($header);

  $address = variable_get('uc_customer_list_address', 'billing');
  if ($address == 'shipping') {
    $query = str_replace('billing', 'delivery', $query);
    $count_query = str_replace('billing', 'delivery', $count_query);
  }
  else {
    $address = 'billing';
  }

  $result = pager_query($query, $page_length, 0, $count_query);

  while ($customer = db_fetch_object($result)) {
    $icons = l(uc_store_get_icon('admin/store/customers', TRUE),
               'user/'. $customer->uid,
               array('html' => TRUE, 'attributes' => array('title' => t('View user details.')))
             )
           . l(uc_store_get_icon('admin/store/orders', TRUE),
               'admin/store/customers/orders/'. $customer->uid,
               array('html' => TRUE, 'attributes' => array('title' => t("View customer's order.")))
             );
    if ($address == 'shipping') {
      $name = ucfirst($customer->delivery_last_name) .', '
            . ucfirst($customer->delivery_first_name);
      $city = ucfirst($customer->delivery_city) .', '
            . uc_get_zone_code($customer->delivery_zone);
    }
    else {
      $name = ucfirst($customer->billing_last_name) .', '
            . ucfirst($customer->billing_first_name);
      $city = ucfirst($customer->billing_city) .', '
            . uc_get_zone_code($customer->billing_zone);
    }
    if ($name == ', ') {
      $name = db_result(db_query("SELECT name FROM {users} WHERE uid = %d", $customer->uid));
      $name = t('User: !name', array('!name' => $name));
    }
    $rows[] = array(
      'data' => array(
        array('data' => $icons),
        array('data' => check_plain($name)),
        array('data' => check_plain($customer->mail)),
        array('data' => check_plain($city)),
        array('data' => $customer->uid)),
      'id' => 'customer-'. $customer->uid,
    );
  }

  drupal_add_js(drupal_get_path('module', 'uc_store') .'/uc_store.js');

  return theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-customer-table'))
       . theme('pager');
}

/**
 * Displays the customer search page.
 */
function uc_store_customer_search() {
  $output = drupal_get_form('uc_store_customer_search_form');

  if (arg(4) == 'results') {
    $first_name = strtolower(str_replace('*', '%%', check_plain(arg(5))));
    $last_name = strtolower(str_replace('*', '%%', check_plain(arg(6))));
    $email = strtolower(str_replace('*', '%%', check_plain(arg(7))));

    if ($first_name !== '' && $first_name !== '0' && $first_name !== '%%') {
      $where .= " AND LOWER(o.billing_first_name) LIKE '". $first_name ."'";
    }
    if ($last_name !== '' && $last_name !== '0' && $last_name !== '%%') {
      $where .= " AND LOWER(o.billing_last_name) LIKE '". $last_name ."'";
    }
    if ($email !== '' && $email !== '0' && $email !== '%%') {
      $where .= " AND LOWER(o.primary_email) LIKE '". $email ."'";
    }

    $query = "SELECT DISTINCT o.uid, u.mail, o.billing_first_name,"
            ."o.billing_last_name, o.billing_city, o.billing_zone, "
            ."o.billing_country FROM {uc_orders} AS o LEFT JOIN "
            ."{users} AS u ON o.uid = u.uid WHERE o.uid > 0 AND "
            ."o.order_status IN ". uc_order_status_list('general', TRUE)
           . $where;// ." ORDER BY o.billing_last_name ASC";


    $count_query = '';
    switch ($GLOBALS['db_type']) {
      case 'mysql':
      case 'mysqli':
        $count_query = "SELECT COUNT(DISTINCT o.uid, o.billing_first_name, "
                       ."o.billing_last_name, u.mail) FROM {uc_orders} AS o "
                       ."LEFT JOIN {users} AS u ON o.uid = u.uid WHERE o.uid > 0 AND "
                       ."o.order_status IN ". uc_order_status_list('general', TRUE)
                       . $where;// ." ORDER BY o.billing_last_name ASC";
        break;
      case 'pgsql':
        $count_query = "SELECT DISTINCT o.uid, o.billing_first_name, "
                      ."o.billing_last_name, u.mail, COUNT(*) "
                      ."FROM {uc_orders} AS o "
                      ."LEFT JOIN {users} AS u ON o.uid = u.uid WHERE o.uid > 0 AND "
                      ."o.order_status IN ". uc_order_status_list('general', TRUE)
                      . $where
                      ."GROUP BY o.uid, o.billing_first_name, o.billing_last_name, u.mail ";
                      //."ORDER BY o.billing_last_name ASC";
        break;
    }

    $message = t('Search returned the following results:');

    $output .= uc_store_customers($message, $query, $count_query, 100);
  }

  return $output;
}

/**
 * The customer search form.
 *
 * @see uc_store_customer_search_form_submit()
 * @ingroup forms
 */
function uc_store_customer_search_form() {
  $form = array();

  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search options'),
    '#description' => t('Search for customers based on any of the following fields. Use * as a wildcard to match any character.') .'<br />'. t("For example, searching by last name for 's*' will return all customers whose last name starts with an s.") .'<br />'. t('(<em>Leave a field empty to ignore it in the search.</em>)'),
    '#collapsible' => TRUE,
    '#collapsed' => arg(4) == 'results' ? TRUE : FALSE,
  );

  $form['search']['table1'] = array('#value' => '<table><tbody style="border: 0px;"><tr><td>');

  $form['search']['first_name'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#default_value' => arg(5) != '0' ? arg(5) : '',
    '#size' => 24,
    '#maxlength' => 32,
  );

  $form['search']['table2'] = array('#value' => '</td><td>');

  $form['search']['last_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#default_value' => arg(6) != '0' ? arg(6) : '',
    '#size' => 24,
    '#maxlength' => 32,
  );

  $form['search']['table3'] = array('#value' => '</td><td>');

  $form['search']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail'),
    '#default_value' => arg(7) != '0' ? arg(7) : '',
    '#size' => 24,
    '#maxlength' => 96,
  );

  $form['search']['table4'] = array('#value' => '</td><td>');

  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  $form['search']['table5'] = array('#value' => '</td></tr></tbody></table>');

  return $form;
}

/**
 * Submit handler for uc_store_customer_search_form().
 *
 * @see uc_store_customer_search_form()
 */
function uc_store_customer_search_form_submit($form, &$form_state) {
  if (strlen(trim($form_state['values']['first_name'])) == 0) {
    $first_name = '0';
  }
  else {
    $first_name = strtolower(trim($form_state['values']['first_name']));
  }

  if (strlen(trim($form_state['values']['last_name'])) == 0) {
    $last_name = '0';
  }
  else {
    $last_name = strtolower(trim($form_state['values']['last_name']));
  }

  if (strlen(trim($form_state['values']['email'])) == 0) {
    $email = '0';
  }
  else {
    $email = strtolower(trim($form_state['values']['email']));
  }

  drupal_goto('admin/store/customers/search/results/'. $first_name .'/'
            . $last_name .'/'. $email);
}

/**
 * Displays main reports page.
 */
function uc_store_reports() {
  $menu = menu_get_item('admin/store/reports');
  $content = system_admin_menu_block($menu);

  $message = (module_exists('uc_reports')) ? t('Various reports generated by Ubercart modules can be found here. Click the links below to view the reports.') : t('Various reports generated by Ubercart modules can be found here. Click the links below to view the reports. To view core Ubercart statistics enable the <strong>Reports</strong> module on the <a href="!url">module administration page</a>', array('!url' => url('admin/build/modules')));
  $output = $message . theme('admin_block_content', $content);

  return $output;
}

/**
 * Displays store configuration page.
 */
function uc_store_configuration_page() {
  $menu = menu_get_item('admin/store/settings');
  $content = system_admin_menu_block($menu);

  $output = theme('admin_block_content', $content);

  return $output;
}

/**
 * Displays store help page.
 */
function uc_store_ubercart_help() {
  $output = '<p>'. t('Use the following links to find documentation and support:') .'</p>';

  $items[] = l(t("Ubercart User's Guide"), 'http://www.ubercart.org/docs/user');
  $items[] = l(t('Support Forums'), 'http://www.ubercart.org/forum');
  $items[] = l(t('Drupal Handbook'), 'http://drupal.org/node/258');
  $output .= theme('item_list', $items);

  return $output;
}

/**
 * Displays the tokens help page.
 */
function uc_store_ubercart_help_tokens() {
  $output = '<p>'. t('Tokens are bracketed phrases you can use in certain text fields and boxes as placeholders for some other text. Tokens represent things like store variables, links to certain pages, order information, etc.  Tokens are used by including the token listed below in a text field that uses them.  The description for the textfields will alert you to which groups of tokens listed below may be used.') .'</p>';

  $output .= theme('token_help', array('ubercart', 'node'));

  return $output;
}

/**
 * Displays an overview of the country settings.
 */
function uc_country_settings_overview() {
  // Theme all the pages beneath this path into summary overviews.
  return theme('summary_overview', summarize_child_form_pages('admin/store/settings/countries/edit'));

}

/**
 * Import settings from a country file.
 *
 * @ingroup forms
 * @see uc_country_import_form_submit()
 */
function uc_country_import_form() {
  $result = db_query("SELECT * FROM {uc_countries}");
  while ($country = db_fetch_object($result)) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');
  $files = _country_import_list();

  $header = array(t('Country'), t('Code'), t('Version'), t('Operations'));
  $rows = array();
  if (is_array($countries)) {
    $link_ops = array('query' => array('token' => drupal_get_token('uc_country_op_link')));
    foreach ($countries as $country) {
      $row = array(
        t($country->country_name),
        $country->country_iso_code_3,
        array('data' => abs($country->version), 'align' => 'center')
      );

      $ops = array();
      if ($country->version < 0) {
        $ops[] = l(t('enable'), 'admin/store/settings/countries/'. $country->country_id .'/enable', $link_ops);
      }
      else {
        $ops[] = l(t('disable'), 'admin/store/settings/countries/'. $country->country_id .'/disable', $link_ops);
      }
      if ($country->version < $files[$country->country_id]['version'] && $country->version > 0) {
        $ops[] = l(t('update'), 'admin/store/settings/countries/'. $country->country_id .'/update/'. $files[$country->country_id]['version'], $link_ops);
      }
      $ops[] = l(t('remove'), 'admin/store/settings/countries/'. $country->country_id .'/remove');
      $row[] = implode(' ', $ops);

      $rows[] = $row;
      unset($files[$country->country_id]);
    }
  }

  $import_list = array();
  foreach ($files as $file) {
    $import_list[$file['file']] = $file['file'];
  }
  if (is_array($import_list)) {
    ksort($import_list);
  }

  if (is_array($import_list)) {
    $options = $import_list;
  }
  else {
    $options = array(t('-None available-'));
  }

  $form['country_import'] = array(
    '#title' => t('Country importer'),
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#summary callback' => '_uc_country_summarize',
  );

  $form['country_import']['text'] = array(
    '#value' => '<p>'. t('To import new country data, select it in the list and click the import button. If you are using a custom or contributed import file, it must be placed in the Ubercart folder uc_store/countries.') .'</p>',
  );
  $form['country_import']['import_file'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#options' => $options,
    '#disabled' => !is_array($import_list),
    '#multiple' => is_array($import_list),
    '#size' => min(10, count($options)),
  );
  $form['country_import']['import_button'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#disabled' => !is_array($import_list),
  );
  $form['country_table'] = array(
    '#value' => theme('table', $header, $rows),
  );

  return $form;
}

/**
 * Summary callback for country settings.
 *
 * @see uc_country_settings_overview()
 */
function _uc_country_summarize() {
  $items = array();

  $result = db_query("SELECT * FROM {uc_countries} ORDER BY country_name ASC");
  while ($country = db_fetch_object($result)) {

    $items[] = $country->version > 0
    ? t(
      '!country version !version is enabled.',
      array('!country' => t($country->country_name), '!version' => abs($country->version))
    )
    : t(
      '!country version !version is disabled.',
      array('!country' => t($country->country_name), '!version' => abs($country->version))
    );
  }

  return $items;
}

/**
 * @see uc_country_import_form()
 */
function uc_country_import_form_submit($form, &$form_state) {
  $files = $form_state['values']['import_file'];

  foreach ($files as $file) {
    if (uc_country_import($file)) {
      drupal_set_message(t('Country file @file imported.', array('@file' => $file)));
    }
    else {
      drupal_set_message(t('Country file @file could not import or had no install function.', array('@file' => $file)), 'error');
    }
  }
}

/**
 * Form to set country address formats.
 *
 * @ingroup forms
 * @see uc_country_formats_form_submit()
 */
function uc_country_formats_form() {
  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address variables instructions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#summary' => t('Tweak the address formatting for a specific country.'),
    '#summary arguments' => array(FALSE),
  );
  $header = array(t('Variable'), t('Description'));
  $rows = array(
    array('!first_name', t("Customer's first name")),
    array('!last_name', t("Customer's last name")),
    array('!company', t('Company name')),
    array('!street1', t('First street address field')),
    array('!street2', t('Second street address field')),
    array('!city', t('City name')),
    array('!zone_name', t('Full name of the zone')),
    array('!zone_code', t('Abbreviation of the zone')),
    array('!postal_code', t('Postal code')),
    array('!country_name', t('Name of the country')),
    array('!country_code2', t('2 digit country abbreviation')),
    array('!country_code3', t('3 digit country abbreviation')),
  );
  $form['instructions']['text'] = array(
    '#value' => '<div>'. t('The following variables should be used in configuring addresses for the countries you ship to:') .'<p>'
              . theme('table', $header, $rows) .'</p><p>'. t('*Adding _if to any country variable will make it display only for addresses<br />whose country is different than the default store country.') .'</div>',
  );

  $result = db_query("SELECT * FROM {uc_countries}");
  while ($country = db_fetch_object($result)) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');

  if (is_array($countries)) {
    $form['countries'] = array(
      '#tree' => TRUE,
    );
    foreach ($countries as $country) {
      $form['countries'][$country->country_id] = array(
        '#type' => 'fieldset',
        '#title' => t($country->country_name),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#summary callback' => 'summarize_null',
      );
      $form['countries'][$country->country_id]['address_format'] = array(
        '#type' => 'textarea',
        '#title' => t('Address format'),
        '#default_value' => variable_get('uc_address_format_'. $country->country_id, ''),
        '#description' => t('Uses the variables mentioned in the instructions to format an address for this country.'),
        '#rows' => 6,
        '#wysiwyg' => FALSE,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
  );

  return $form;
}

/**
 * Submit handler for uc_country_formats_form().
 *
 * @see uc_country_formats_form()
 */
function uc_country_formats_form_submit($form, &$form_state) {
  foreach ($form_state['values']['countries'] as $country_id => $data) {
    variable_set('uc_address_format_'. $country_id, $data['address_format']);
  }
  drupal_set_message(t('Country settings saved.'));
}

/**
 * Displays an overview of the store settings.
 */
function uc_store_store_settings_overview() {
  // Theme all the pages beneath this path into summary overviews.
  return theme('summary_overview', summarize_child_form_pages('admin/store/settings/store/edit'));
}

/**
 * Summarizes the store's address settings.
 *
 * @param $form
 *   The form passed from the summarizer.
 *
 * @return
 *   An array of summary information.
 */
function _uc_store_address_summarize($form) {
  $address = trim(uc_store_address());

  return array(
    'data' => t('Store address:<br />!address', array('!address' => empty($address) ? t('Not set yet.') : $address)),
  );
}

/**
 * Displays the form for store settings.
 *
 * @ingroup forms
 */
function uc_store_store_settings_form() {
  $form['uc_store_name'] = uc_textfield(t('Store name'), variable_get('uc_store_name', NULL), FALSE, NULL, 64);
  $form['uc_store_owner'] = uc_textfield(t('Store owner'), variable_get('uc_store_owner', NULL), FALSE, NULL, 64);

  $form['uc_store_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#description' => NULL,
    '#size' => 32,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => uc_store_email(),
    '#summary' => uc_store_email() ? t('Store e-mail address is %email.', array('%email' => uc_store_email())) : t('Store e-mail address is not set.'),
  );

  $form['uc_store_email_include_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the store name in the "From" line of store e-mails.'),
    '#description' => t('May not be available on all server configurations. Turn off if this causes problems.'),
    '#default_value' => variable_get('uc_store_email_include_name', TRUE),
    '#summary callback' => uc_store_email() ? NULL : 'summarize_null',
  );

  $form['uc_store_phone'] = uc_textfield(t('Phone number'), variable_get('uc_store_phone', NULL), FALSE);
  $form['uc_store_fax'] = uc_textfield(t('Fax number'), variable_get('uc_store_fax', NULL), FALSE);

  $form['store_address'] = array('#summary callback' => '_uc_store_address_summarize');
  $form['store_address']['uc_store_street1'] = uc_textfield(uc_get_field_name('street1'), variable_get('uc_store_street1', NULL), FALSE, NULL, 128);
  $form['store_address']['uc_store_street2'] = uc_textfield(uc_get_field_name('street2'), variable_get('uc_store_street2', NULL), FALSE, NULL, 128);
  $form['store_address']['uc_store_city'] = uc_textfield(uc_get_field_name('city'), variable_get('uc_store_city', NULL), FALSE);
  $form['store_address']['uc_store_country'] = uc_country_select(uc_get_field_name('country'), uc_store_default_country());

  if (isset($_POST['uc_store_country'])) {
    $country_id = intval($_POST['uc_store_country']);
  }
  else {
    $country_id = uc_store_default_country();
  }
  $form['store_address']['uc_store_zone'] = uc_zone_select(uc_get_field_name('zone'), variable_get('uc_store_zone', NULL), NULL, $country_id);
  $form['store_address']['uc_store_postal_code'] = uc_textfield(uc_get_field_name('postal_code'), variable_get('uc_store_postal_code', NULL), FALSE, NULL, 10);

  $form['uc_store_help_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Store help page'),
    '#description' => t('The Drupal page for the store help link.'),
    '#default_value' => variable_get('uc_store_help_page', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    '#summary' => variable_get('uc_store_help_page', '') ? t('Help page is <a href="!url">!url</a>.', array('!url' => url(variable_get('uc_store_help_page', ''), array('absolute' => TRUE)))) : t('Help page is not set.'),
  );

  // Register additional validation handler
  $form['#validate'][] = 'uc_store_store_settings_form_validate';

  return system_settings_form($form);
}

/**
 * Validation handler for uc_store_store_settings_form().
 *
 * Validates store e-mail address
 */
function uc_store_store_settings_form_validate($form, &$form_state) {
  $mail = trim($form_state['values']['uc_store_email']);
  if (!valid_email_address($mail)) {
    form_set_error('uc_store_email', t('The e-mail address %mail is not valid.', array('%mail' => $mail)));
  }
}

/**
 * Settings for how miscellaneous store components are displayed.
 *
 * @ingroup forms
 */
function uc_store_display_settings_form() {
  $display_options = array(
    1 => t('Dashboard with collapsed submenu links'),
    2 => t('Dashboard with expanded submenu links'),
    3 => t('Dashboard with no submenu links'),
    4 => t('Normal Drupal submenu listing'),
  );

  $form['uc_store_admin_page_display'] = array(
    '#type' => 'radios',
    '#title' => t('Display type for the main store administration page'),
    '#description' => t('Some options are better suited for different themes, so feel free to try them all out!'),
    '#options' => $display_options,
    '#summary' => t('Store admin page is displaying: <br />@display', array('@display' => $display_options[variable_get('uc_store_admin_page_display', 1)])),
    '#default_value' => variable_get('uc_store_admin_page_display', 1),
  );

  $address_options = array(
    'billing' => t('Billing address'),
    'shipping' => t('Shipping address'),
  );

  $desc = t('Select the address to be used on customer lists and summaries.');
  $form['uc_customer_list_address'] = array(
    '#type' => 'radios',
    '#title' => t('Primary customer address'),
    '#description' => $desc,
    '#options' => $address_options,
    '#summary' => t("Customer's %billing is being used in lists.", array('%billing' => $address_options[variable_get('uc_customer_list_address', 'billing')])),
    '#default_value' => variable_get('uc_customer_list_address', 'billing'),
  );

  $options = array_merge(array(t('Randomly select a message from the list below.')), _store_footer_options());

  // format the message nicely for the user
  switch (variable_get('uc_footer_message', 0)) {
    case 'none':
      $user_footer = 'no message.';
    break;

    case 0:
      $user_footer = 'a random message.';
    break;

    default:
      $user_footer = '"'. $options[variable_get('uc_footer_message', 0)] .'"';
    break;
  }

  $form['uc_footer_message'] = array(
    '#type' => 'radios',
    '#title' => t('Footer message for store pages'),
    '#options' => $options,
    '#summary' => t('Footer using !footer', array('!footer' => $user_footer)),
    '#default_value' => variable_get('uc_footer_message', 0),
  );

  return system_settings_form($form);
}

/**
 * Displays the form for format settings.
 *
 * @ingroup forms
 */
function uc_store_format_settings_form() {
  $form['currency'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currency format'),
    '#summary callback' => 'summarize_form',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['currency']['uc_currency_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Default currency'),
    '#description' => t('While not used directly in formatting, the currency code is used by other modules as the primary currency for your site.  Enter here your three character <a href="!url">ISO 4217</a> currency code.', array('!url' => 'http://en.wikipedia.org/wiki/ISO_4217#Active_codes')),
    '#default_value' => variable_get('uc_currency_code', 'USD'),
    '#maxlength' => 3,
    '#size' => 5,
  );

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );
  $form['currency']['example'] = array(
    '#type' => 'textfield',
    '#title' => t('Current format'),
    '#value' => uc_price(1000.1234, $context),
    '#summary' => t('Currency format: @format', array('@format' => uc_price(1000.1234, $context))),
    '#disabled' => TRUE,
    '#size' => 10,
  );
  $form['currency']['uc_currency_sign'] = uc_textfield(t('Currency Sign'), variable_get('uc_currency_sign', '$'), FALSE, NULL, 10, 10);
  $form['currency']['uc_currency_sign']['#summary callback'] = 'summarize_null';

  $form['currency']['uc_sign_after_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display currency sign after amount.'),
    '#summary callback' => 'summarize_null',
    '#default_value' => variable_get('uc_sign_after_amount', FALSE),
  );

  $form['currency']['uc_currency_thou'] = uc_textfield(t('Thousands Marker'), variable_get('uc_currency_thou', ','), FALSE, NULL, 10, 10);
  $form['currency']['uc_currency_thou']['#summary callback'] = 'summarize_null';

  $form['currency']['uc_currency_dec'] = uc_textfield(t('Decimal Marker'), variable_get('uc_currency_dec', '.'), FALSE, NULL, 10, 10);
  $form['currency']['uc_currency_dec']['#summary callback'] = 'summarize_null';

  $form['currency']['uc_currency_prec'] = array(
    '#type' => 'select',
    '#title' => t('Number of decimal places'),
    '#options' => drupal_map_assoc(array(0, 1, 2)),
    '#summary callback' => 'summarize_null',
    '#default_value' => variable_get('uc_currency_prec', 2),
  );

  $form['weight'] = array(
    '#type' => 'fieldset',
    '#title' => t('Weight format'),
    '#summary callback' => 'summarize_form',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['weight']['instructions'] = array(
    '#value' => '<div>'. t('Supply a format string for each unit. !value represents the weight value.') .'</div>',
    '#summary callback' => 'summarize_null',
  );
  $units = array(
    'lb' => t('Pounds'),
    'oz' => t('Ounces'),
    'kg' => t('Kilograms'),
    'g' => t('Grams'),
  );
  $form['weight']['uc_weight_unit'] = array(
    '#type' => 'select',
    '#title' => t('Default unit of measurement'),
    '#summary' => t('Weight format: @weight', array('@weight' => uc_weight_format(36))),
    '#default_value' => variable_get('uc_weight_unit', 'lb'),

    '#options' => $units,
  );
  foreach ($units as $unit => $name) {
    $form['weight']['uc_weight_format_'. $unit] = array(
      '#type' => 'textfield',
      '#title' => t('@unit format string', array('@unit' => $name)),
      '#summary callback' => 'summarize_null',
      '#default_value' => variable_get('uc_weight_format_'. $unit, '!value '. $unit),
    );
  }

  $form['length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Length format'),
    '#summary callback' => 'summarize_null',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['length']['instructions'] = array(
    '#value' => '<div>'. t('Supply a format string for each unit. !value represents the length value.') .'</div>',
  );
  $units = array(
    'in' => t('Inches'),
    'ft' => t('Feet'),
    'cm' => t('Centimeters'),
    'mm' => t('Millimeters'),
  );
  $form['length']['uc_length_unit'] = array(
    '#type' => 'select',
    '#title' => t('Default unit of measurement'),
    '#default_value' => variable_get('uc_length_unit', 'in'),
    '#options' => $units,
  );
  foreach ($units as $unit => $name) {
    $form['length']['uc_length_format_'. $unit] = array(
      '#type' => 'textfield',
      '#title' => t('@unit format string', array('@unit' => $name)),
      '#default_value' => variable_get('uc_length_format_'. $unit, '!value '. $unit),
    );
  }

  $form['date'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date format'),
    '#summary callback' => 'summarize_form',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['date']['instructions'] = array(
    '#value' => '<div>'. t('Supply a format string using !link syntax.', array('!link' => l(t('PHP date'), 'http://www.php.net/date'))) .'</div>',
    '#summary callback' => 'summarize_form',
  );
  $form['date']['uc_date_format_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default format string'),
    '#default_value' => variable_get('uc_date_format_default', 'm/d/Y'),
    '#summary' => t('Date format: @date', array('@date' => uc_date_format(8, 18, 2007))),
  );

  return system_settings_form($form);
}

/**
 * Form to enter initials for an administrative user.
 *
 * @see uc_store_initials_submit()
 * @ingroup forms
 */
function uc_store_initials() {
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#description' => t('Enter the name of the user whose initials you want to adjust.'),
    '#required' => TRUE,
    '#size' => 32,
    '#autocomplete_path' => 'user/autocomplete',
  );
  $form['initials'] = array(
    '#type' => 'textfield',
    '#title' => t('Initials'),
    '#description' => t('Enter initials or leave blank to erase current initials.'),
    '#size' => 6,
    '#maxlength' => 32,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Submit handler for uc_store_initials() form.
 *
 * @see uc_store_initials()
 */
function uc_store_initials_submit($form, &$form_state) {
  $result = db_query("SELECT uid FROM {users} WHERE name = '%s'", $form_state['values']['username']);
  if ($user = db_fetch_object($result)) {
    if ($form_state['values']['initials'] == '') {
      variable_del('user_initials_'. $user->uid);
      drupal_set_message(t('Initials for !username deleted.', array('!username' => $form_state['values']['username'])));
    }
    else {
      variable_set('user_initials_'. $user->uid, $form_state['values']['initials']);
      drupal_set_message(t('Initials for !username set to !initials.',
                           array('!username' => $form_state['values']['username'],
                                 '!initials' => $form_state['values']['initials'])));
    }
  }
}

/**
 * Lists extensible TAPIr tables used by Ubercart.
 */
function uc_store_tables() {
  $output = '<p>'. t('The following tables are used to display various parts of your store to your administrators and customers. Click on a table id to configure the display of that table.');
  $output .= '<p>'. tapir_table_list('admin/store/settings/tables') .'</p>';

  return $output;
}

/**
 * Displays a list of orders made by a customer.
 */
function uc_store_customer_orders($uid) {
  $result = pager_query("SELECT * FROM {uc_orders} WHERE uid = %d AND "
                       ."order_status IN ". uc_order_status_list('general', TRUE)
                       ." ORDER BY created DESC", 50, 0,
                        NULL, $uid);

  $header = array(t('View'), t('Order ID'), t('Date'), t('Billing name'),
                  t('Shipping name'), t('Items'), t('Total'));

  $totals = array('orders' => 0, 'items' => 0, 'total' => 0);
  while ($order = db_fetch_object($result)) {
    $icons = l(uc_store_get_icon('file:order_view'),
               'admin/store/orders/'. $order->order_id,
               array('html' => TRUE, 'attributes' => array('title' => t("View order !order_id.", array('!order_id' => $order->order_id))))
             )
           . l(uc_store_get_icon('file:order_edit'),
               'admin/store/orders/'. $order->order_id .'/edit',
               array('html' => TRUE, 'attributes' =>
                 array(
                   'title' => t("Edit order !order_id.",
                     array('!order_id' => $order->order_id)
                   )
                 )
               )
             );
    $bname = ucfirst($order->billing_first_name) .' '
           . ucfirst($order->billing_last_name);
    $sname = ucfirst($order->delivery_first_name) .' '
           . ucfirst($order->delivery_last_name);
    $result2 = db_query("SELECT COUNT(*) FROM {uc_order_products} WHERE "
                       ."order_id = %d", $order->order_id);
    $item_count = db_fetch_array($result2);

    $totals['orders'] += 1;
    $totals['items'] += $item_count['COUNT(*)'];
    $totals['total'] += $order->order_total;

    $context = array(
      'revision' => 'themed-original',
      'type' => 'order_total',
      'subject' => array(
        'order' => $order,
      ),
    );

    $rows[] = array(
      'data' => array(
        array('data' => $icons),
        array('data' => $order->order_id),
        array('data' => format_date($order->created, 'custom', variable_get('uc_date_format_default', 'm/d/Y'))),
        array('data' => check_plain($bname)),
        array('data' => check_plain($sname)),
        array('data' => $item_count['COUNT(*)']),
        array('data' => uc_price($order->order_total, $context), 'nowrap' => 'nowrap')),
      'id' => 'order-'. $order->order_id,
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array('data' => t('No orders found.'), 'colspan' => 7)
    );
  }

  drupal_add_js(drupal_get_path('module', 'uc_store') .'/uc_store.js');

  $context = array(
    'revision' => 'formatted-original',
    'type' => 'amount',
  );
  if (user_access('create orders')) {
    $output = '<p>'. l(t('Create an order for this customer.'),
                       'admin/store/orders/create/'. $uid) .'</p>';
  }
  $output .= '<p>'. t('!totals_orders orders shown match that account with !totals_items items purchased and !totals_total spent:',
                     array('!totals_orders' => $totals['orders'],
                           '!totals_items' => $totals['items'],
                           '!totals_total' => uc_price($totals['total'], $context))) .'</p>'
           . theme('table', $header, $rows, array('width' => '100%', 'class' => 'uc-cust-orders-table'))
            .'<br />'. theme('pager', NULL, 50);

  return $output;
}

/**
 * Disables a country so it remains installed but is no longer selectable.
 */
function uc_country_disable($country_id) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = %d", $country_id);
  if (($country = db_fetch_object($result))) {
    // Version is positive if the country is enabled.
    // Version is negative if the country is disabled.
    if ($country->version > 0) {
      db_query("UPDATE {uc_countries} SET version = %d WHERE country_id = %d",
               0 - $country->version, $country_id);
      drupal_set_message(t('!country disabled.', array('!country' => t($country->country_name))));
    }
    else {
      drupal_set_message(t('!country is already disabled.', array('!country' => t($country->country_name))), 'error');
    }
  }
  else {
    drupal_set_message(t('Attempted to disable an invalid country.'), 'error');
  }
  drupal_goto('admin/store/settings/countries/edit');
}

/**
 * Enables a disabled country.
 */
function uc_country_enable($country_id) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = %d", $country_id);
  if (($country = db_fetch_object($result))) {
    if ($country->version < 0) {
      db_query("UPDATE {uc_countries} SET version = %d WHERE country_id = %d", abs($country->version), $country_id);
      drupal_set_message(t('@country enabled.', array('@country' => t($country->country_name))));
    }
    else {
      drupal_set_message(t('@country is already enabled.', array('@country' => t($country->country_name))), 'error');
    }
  }
  else {
    drupal_set_message(t('Attempted to enable an invalid country.'), 'error');
  }
  drupal_goto('admin/store/settings/countries/edit');
}

/**
 * Form to completely remove a country.
 *
 * @see uc_country_remove_form_submit()
 * @ingroup forms
 */
function uc_country_remove_form($form_state, $country_id) {
  // Fetch the country name from the database.
  $country = t(db_result(db_query("SELECT country_name FROM {uc_countries} WHERE country_id = %d", $country_id)));

  // If orders exist for this country, show a warning message prior to removal.
  if (isset($_POST['op']) && ($_POST['op'] != t('Remove')) && module_exists('uc_order')) {
    $count = db_result(db_query("SELECT COUNT(order_id) FROM {uc_orders} WHERE delivery_country = %d OR billing_country = %d", $country_id, $country_id));
    if ($count > 0) {
      drupal_set_message(t('Warning: @count orders were found with addresses in this country. Removing this country now will cause errors to show on those order pages. You might consider simply disabling this country instead.', array('@count' => $count)), 'error');
    }
  }

  // Store the country ID in the form array for processing.
  $form['country_id'] = array(
    '#type' => 'value',
    '#value' => $country_id,
  );

  return confirm_form($form, t('Are you sure you want to remove @country from the system?', array('@country' => $country)), 'admin/store/settings/countries/edit', NULL, t('Remove'));
}

/**
 * Submit handler for uc_country_remove_form().
 *
 * @see uc_country_remove_form()
 */
function uc_country_remove_form_submit($form, &$form_state) {
  $country_id = $form_state['values']['country_id'];

  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = %d", $country_id);
  if (!($country = db_fetch_object($result))) {
    drupal_set_message(t('Attempted to remove an invalid country.'), 'error');
    drupal_goto('admin/store/settings/countries/edit');
  }

  db_query("DELETE FROM {uc_countries} WHERE country_id = %d", $country_id);
  db_query("DELETE FROM {uc_zones} WHERE zone_country_id = %d", $country_id);
  variable_del('uc_address_format_'. $country_id);

  $func_base = _country_import_include($country_id, $country->version);
  if ($func_base !== FALSE) {
    $func = $func_base .'_uninstall';
    if (function_exists($func)) {
      $func();
    }
  }

  drupal_set_message(t('!country removed.', array('!country' => t($country->country_name))));
  drupal_goto('admin/store/settings/countries/edit');
}

/**
 * Updates a country to its latest version.
 */
function uc_country_update($country_id, $version) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = %d", $country_id);
  if (!($country = db_fetch_object($result))) {
    drupal_set_message(t('Attempted to update an invalid country.'));
    drupal_goto('admin/store/settings/countries');
  }

  if ($version < $country->version) {
    drupal_set_message(t('You cannot update to a previous version.'));
    drupal_goto('admin/store/settings/countries');
  }

  $func_base = _country_import_include($country_id, $version);
  if ($func_base !== FALSE) {
    $func = $func_base .'_update';
    if (function_exists($func)) {
      for ($i = $country->version; $i <= $version; $i++) {
        $func($i);
      }
    }
    db_query("UPDATE {uc_countries} SET version = %d WHERE country_id = %d", $version, $country_id);
    drupal_set_message(t('Country update complete.'));
  }
  else {
    drupal_set_message(t('Attempted to update an invalid country.'));
  }

  drupal_goto('admin/store/settings/countries/edit/import');
}

/**
 * Form for enabling/weighting price alterers and selecting a price formatter.
 *
 * @see uc_price_settings_form_submit()
 * @see theme_uc_price_settings_form()
 * @ingroup forms
 */
function uc_price_settings_form() {
  $options = array();
  $form = array();

  // Get all the handlers available.
  $handlers = _uc_price_get_handlers(array('rebuild_handlers' => TRUE, 'all_handlers' => TRUE));

  // Build a draggable price alterer table.
  $form['uc_price_alterers']['#tree'] = TRUE;

  // Loop through the handlers.
  foreach ($handlers['hook_data'] as $module => $data) {
    // If the handler defines an alterer, add it to the form.
    if (isset($data['alter'])) {
      $description = $data['alter']['title'];

      if (!empty($data['alter']['description'])) {
        $description .= '<div class="description">'. $data['alter']['description'] .'</div>';
      }

      $form['uc_price_alterers'][$module]['module'] = array(
        '#value' => $description,
      );
      $form['uc_price_alterers'][$module]['enabled'] = array(
        '#type' => 'checkbox',
        '#default_value' => $data['enabled'],
      );
      $form['uc_price_alterers'][$module]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $data['weight'],
        '#attributes' => array('class' => 'uc-price-handler-table-weight'),
      );
    }

    // If the handler defines a formatter, add it to the options array.
    if (isset($data['format'])) {
      $description = $data['format']['title'];

      if (!empty($data['format']['description'])) {
        $description .= '<div class="description">'. $data['format']['description'] .'</div>';
      }

      $options[$data['format']['callback']] = $description;
    }
  }

  $form['price_alterers_description'] = array(
    '#value' => '<div class="description">'. t('Prices will be altered by enabled price alterers in their order in the table above.') .'</div>',
  );

  $form['uc_price_format_callback'] = array(
    '#type' => 'radios',
    '#title' => t('Price formatter'),
    '#options' => $options,
    '#default_value' => variable_get('uc_price_format_callback', 'uc_store_price_handler_format'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#suffix' => l(t('Cancel changes'), 'admin/store/settings/price-handlers'),
  );

  // Rebuild the handler cache!
  _uc_price_get_handlers(array('rebuild_handlers' => TRUE));

  return $form;
}

/**
 * Renders the price handler form, adding tabledrag.
 *
 * @see uc_price_settings_form()
 * @ingroup themeable
 */
function theme_uc_price_settings_form($form) {
  // Add the tabledrag to the alterer table.
  drupal_add_tabledrag('uc-price-handler-table', 'order', 'sibling', 'uc-price-handler-table-weight');

  $header = array(t('Price alterers'), t('Enabled'), t('Weight'));

  if (count(element_children($form['uc_price_alterers'])) > 0) {
    foreach (element_children($form['uc_price_alterers']) as $module) {
      $row = array();

      foreach (element_children($form['uc_price_alterers'][$module]) as $field) {
        $row[] = drupal_render($form['uc_price_alterers'][$module][$field]);
      }

      $rows[] = array(
        'data' => $row,
        'class' => 'draggable',
      );
    }
  }

  $output = theme('table', $header, $rows, array('id' => 'uc-price-handler-table'))
          . drupal_render($form);

  return $output;
}

/**
 * Submit handler for uc_price_settings_form().
 *
 * @see uc_price_settings_form()
 */
function uc_price_settings_form_submit($form, &$form_state) {
  // Save the selected price formatter.
  variable_set('uc_price_format_callback', $form_state['values']['uc_price_format_callback']);

  // Save the alterer configuration based on the table form.
  $config = array();

  foreach ($form_state['values']['uc_price_alterers'] as $module => $alterer) {
    $config[$module] = $alterer;
  }

  variable_set('uc_price_handler_config', $config);

  drupal_set_message(t('Price handler configuration saved.'));
}



/**
 * Implements hook_store_status().
 */
function uc_store_store_status() {
  $messages = array();

  // Check to see if there are any updated CIF files that need to be installed.
  $countries = array();
  $result = db_query("SELECT * FROM {uc_countries}");
  while ($country = db_fetch_object($result)) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');
  $files = _country_import_list();

  $updates = array();
  if (is_array($countries)) {
    foreach ($countries as $country) {
      if ($country->version < $files[$country->country_id]['version'] && $country->version > 0) {
        $updates[] = $country->country_name;
      }
      unset($files[$country->country_id]);
    }
  }

  if (count($updates)) {
    $messages[] = array(
      'status' => 'warning',
      'title'  => t('Countries'),
      'desc'   => t('Updates are available for the following installed countries:  %countries. You may update these countries on the !link page.', array('%countries' => implode(', ', $updates), '!link' => l('country settings', 'admin/store/settings/countries/edit'))),
    );
  }
  else {
    $messages[] = array(
      'status' => 'ok',
      'title'  => t('Countries'),
      'desc'   => t('Country definitions are up-to-date.'),
    );
  }

  // Check to see if the store e-mail address has been set.
  if (variable_get('uc_store_email', '') == '') {
    $messages[] = array(
      'status' => 'error',
      'title'  => t('Store settings'),
      'desc'   => t('Store e-mail address has not been set. Please enter it !link.', array('!link' => l('here', 'admin/store/settings/store/edit'))),
    );
  }
  else {
    $messages[] = array(
      'status' => 'ok',
      'title'  => t('Store settings'),
      'desc'   => t('Store e-mail address is set.'),
    );
  }

  return $messages;
}

/**
 * Performs country actions after validating the CSRF-prevention token.
 *
 * @param $function
 *   The function name to run.
 * @param $param
 *   Parameter to pass to the function specified.
 * @param $param2
 *   (optional) Parameter to pass to the function specified.
 */
function _uc_country_perform_country_action($function, $param, $param2 = NULL) {
  if (isset($_GET['token']) && drupal_valid_token($_GET['token'], 'uc_country_op_link')) {
    call_user_func($function, $param, $param2);
  }
  else {
    return MENU_ACCESS_DENIED;
  }
}
