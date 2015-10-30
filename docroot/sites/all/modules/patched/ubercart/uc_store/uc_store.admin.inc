<?php

/**
 * @file
 * Store administration menu items.
 */

/**
 * Menu callback which provides the store administration overview page.
 */
function uc_store_admin() {
  module_load_include('inc', 'system', 'system.admin');
  // Check for status report errors.
  if (system_status(TRUE) && user_access('administer site configuration')) {
    drupal_set_message(t('One or more problems were detected with your Drupal installation. Check the <a href="@status">status report</a> for more information.', array('@status' => url('admin/reports/status'))), 'error');
  }
  $blocks = array();
  if ($admin = db_query("SELECT menu_name, mlid FROM {menu_links} WHERE link_path = 'admin/store' AND module = 'system'")->fetchAssoc()) {
    $result = db_query("
      SELECT m.*, ml.*
      FROM {menu_links} ml
      INNER JOIN {menu_router} m ON ml.router_path = m.path
      WHERE ml.link_path != 'admin/help' AND menu_name = :menu_name AND ml.plid = :mlid AND hidden = 0", $admin, array('fetch' => PDO::FETCH_ASSOC));
    foreach ($result as $item) {
      _menu_link_translate($item);
      if (!$item['access']) {
        continue;
      }
      // The link 'description' either derived from the hook_menu 'description'
      // or entered by the user via menu module is saved as the title attribute.
      if (!empty($item['localized_options']['attributes']['title'])) {
        $item['description'] = $item['localized_options']['attributes']['title'];
      }
      $block = $item;
      $block['content'] = theme('admin_block_content', array('content' => system_admin_menu_block($item)));
      if (!empty($block['content'])) {
        $block['show'] = TRUE;
      }

      // Prepare for sorting as in function _menu_tree_check_access().
      // The weight is offset so it is always positive, with a uniform 5-digits.
      $blocks[(50000 + $item['weight']) . ' ' . $item['title'] . ' ' . $item['mlid']] = $block;
    }
  }

  ksort($blocks);
  $build['blocks'] = array(
    '#theme' => 'admin_page',
    '#blocks' => $blocks,
  );

  if ($results = module_invoke_all('uc_store_status')) {
    foreach ($results as $message) {
      switch ($message['status']) {
        case 'warning': $icon = 'alert.gif'; break;
        case 'error':   $icon = 'error.gif'; break;
        default:        $icon = 'info.gif';
      }
      $icon = theme('image', array('path' => drupal_get_path('module', 'uc_store') . '/images/' . $icon));

      $rows[] = array(
        array('data' => $icon, 'class' => array('icon')),
        array('data' => $message['title'], 'class' => array('title')),
        array('data' => $message['desc'], 'class' => array('message')),
      );
    }

    $build['status'] = array(
      '#theme' => 'table',
      '#caption' => '<h2>' . t('Store status') . '</h2>',
      '#rows' => $rows,
      '#attributes' => array('class' => array('uc-store-status')),
    );
  }

  return $build;
}

/**
 * Displays main reports page.
 */
function uc_store_reports() {
  $menu = menu_get_item('admin/store/reports');
  $content = system_admin_menu_block($menu);

  $build['menu'] = array(
    '#theme' => 'admin_block_content',
    '#content' => $content,
    '#weight' => 5,
  );

  return $build;
}

/**
 * Displays store configuration page.
 */
function uc_store_configuration_page() {
  $menu = menu_get_item('admin/store/settings');
  $content = system_admin_menu_block($menu);

  $build['menu'] = array(
    '#theme' => 'admin_block_content',
    '#content' => $content,
  );

  return $build;
}

/**
 * Form to configure address fields.
 *
 * @see uc_store_address_fields_form_submit()
 * @see theme_uc_store_address_fields_form()
 * @ingroup forms
 */
function uc_store_address_fields_form($form, &$form_state) {
  $form['uc_address_fields']['#tree'] = TRUE;
  $form['uc_address_fields_required']['#tree'] = TRUE;
  $form['uc_address_fields_weight']['#tree'] = TRUE;

  $fields = array(
    'first_name' => t('First name'),
    'last_name' => t('Last name'),
    'company' => t('Company'),
    'street1' => t('Street address 1'),
    'street2' => t('Street address 2'),
    'city' => t('City'),
    'zone' => t('State/Province'),
    'country' => t('Country'),
    'postal_code' => t('Postal code'),
    'phone' => t('Phone number'),
  );
  $current = variable_get('uc_address_fields', drupal_map_assoc(array('first_name', 'last_name', 'phone', 'company', 'street1', 'street2', 'city', 'zone', 'postal_code', 'country')));
  $required = variable_get('uc_address_fields_required', drupal_map_assoc(array('first_name', 'last_name', 'street1', 'city', 'zone', 'postal_code', 'country')));
  $weight = uc_store_address_field_weights();
  foreach ($fields as $field => $label) {
    $form['uc_address_fields'][$field] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($current[$field]) ? TRUE : FALSE,
    );
    $form['uc_address_fields_required'][$field] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($required[$field]) ? TRUE : FALSE,
    );
    $form['uc_address_fields_weight'][$field] = array(
      '#type' => 'weight',
      '#default_value' => (isset($weight[$field])) ? $weight[$field] : 0,
      '#attributes' => array('class' => array('uc-store-address-fields-weight')),
    );
    $form['fields'][$field]['default'] = array(
      '#markup' => $label,
    );
    $form['fields'][$field]['uc_field_' . $field] = array(
      '#type' => 'textfield',
      '#default_value' => uc_get_field_name($field),
      '#size' => 32,
    );
    $form['fields'][$field]['#weight'] = (isset($weight[$field])) ? $weight[$field] : 99;
  }

  $form['#submit'][] = 'uc_store_address_fields_form_submit';
  if (function_exists('i18n_variable_form_alter_settings')) {
    if ($i18n_variables = i18n_variable_form_alter_settings($form, i18n_variable_list())) {
      $form['#submit'][] = 'i18n_variable_form_submit';
      $form['#i18n_variables'] = $i18n_variables;
      $form += i18n_variable_form_selector();
    }
  }
  $form = system_settings_form($form);
  unset($form['#theme']);

  return $form;
}

/**
 * Returns HTML for uc_store_address_fields_form().
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see uc_store_address_fields_form()
 * @ingroup themeable
 */
function theme_uc_store_address_fields_form($variables) {
  $form = $variables['form'];

  $header = array(t('Field'), t('Title'), t('Enabled'), t('Required'), t('List position'));

  // Sort fields by weight
  uasort($form['fields'], 'element_sort');

  foreach (element_children($form['fields']) as $field) {
    $row = array(
      drupal_render($form['fields'][$field]['default']),
      drupal_render($form['fields'][$field]['uc_field_' . $field]),
      drupal_render($form['uc_address_fields'][$field]),
      drupal_render($form['uc_address_fields_required'][$field]),
      drupal_render($form['uc_address_fields_weight'][$field]),
    );

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  drupal_add_tabledrag('uc-store-address-fields-weight-table', 'order', 'sibling', 'uc-store-address-fields-weight');

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'uc-store-address-fields-weight-table'),
  ));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Saves the address fields settings.
 *
 * @see uc_store_address_fields_form()
 */
function uc_store_address_fields_form_submit($form, &$form_state) {
  $form_state['values']['uc_address_fields'] = array_filter($form_state['values']['uc_address_fields']);
  $form_state['values']['uc_address_fields_required'] = array_filter($form_state['values']['uc_address_fields_required']);
}

/**
 * Form builder for store settings.
 *
 * @see uc_store_settings_form_validate()
 * @ingroup forms
 */
function uc_store_settings_form($form, &$form_state) {
  $form['store'] = array('#type' => 'vertical_tabs');

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic information'),
    '#group' => 'store',
  );
  $form['basic']['uc_store_name'] = uc_textfield(t('Store name'), uc_store_name(), FALSE, NULL, 64);
  $form['basic']['uc_store_owner'] = uc_textfield(t('Store owner'), variable_get('uc_store_owner', NULL), FALSE, NULL, 64);
  $form['basic']['uc_store_email'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#description' => NULL,
    '#size' => 32,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => uc_store_email(),
  );
  $form['basic']['uc_store_email_include_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the store name in the "From" line of store e-mails.'),
    '#description' => t('May not be available on all server configurations. Turn off if this causes problems.'),
    '#default_value' => variable_get('uc_store_email_include_name', TRUE),
  );
  $form['basic']['uc_store_phone'] = uc_textfield(t('Phone number'), variable_get('uc_store_phone', NULL), FALSE);
  $form['basic']['uc_store_fax'] = uc_textfield(t('Fax number'), variable_get('uc_store_fax', NULL), FALSE);
  $form['basic']['uc_store_help_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Store help page'),
    '#description' => t('The Drupal page for the store help link.'),
    '#default_value' => variable_get('uc_store_help_page', ''),
    '#size' => 32,
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  $form['address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Store address'),
    '#group' => 'store',
  );
  $form['address']['address'] = array(
    '#type' => 'uc_address',
    '#default_value' => array(
      'uc_store_street1' => variable_get('uc_store_street1', ''),
      'uc_store_street2' => variable_get('uc_store_street2', ''),
      'uc_store_city' => variable_get('uc_store_city', ''),
      'uc_store_zone' => variable_get('uc_store_zone', 0),
      'uc_store_country' => isset($form_state['values']) ? $form_state['values']['uc_store_country'] : uc_store_default_country(),
      'uc_store_postal_code' => variable_get('uc_store_postal_code', ''),
    ),
    '#required' => FALSE,
    '#key_prefix' => 'uc_store',
  );

  $form['currency'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currency format'),
    '#group' => 'store',
  );
  $form['currency']['uc_currency_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Default currency'),
    '#description' => t('While not used directly in formatting, the currency code is used by other modules as the primary currency for your site.  Enter here your three character <a href="!url">ISO 4217</a> currency code.', array('!url' => 'http://en.wikipedia.org/wiki/ISO_4217#Active_codes')),
    '#default_value' => variable_get('uc_currency_code', 'USD'),
    '#maxlength' => 3,
    '#size' => 5,
  );
  $form['currency']['example'] = array(
    '#type' => 'textfield',
    '#title' => t('Current format'),
    '#value' => uc_currency_format(1000.1234),
    '#disabled' => TRUE,
    '#size' => 10,
  );
  $form['currency']['uc_currency_sign'] = uc_textfield(t('Currency Sign'), variable_get('uc_currency_sign', '$'), FALSE, NULL, 10, 10);
  $form['currency']['uc_sign_after_amount'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display currency sign after amount.'),
    '#default_value' => variable_get('uc_sign_after_amount', FALSE),
  );
  $form['currency']['uc_currency_thou'] = uc_textfield(t('Thousands Marker'), variable_get('uc_currency_thou', ','), FALSE, NULL, 10, 10);
  $form['currency']['uc_currency_dec'] = uc_textfield(t('Decimal Marker'), variable_get('uc_currency_dec', '.'), FALSE, NULL, 10, 10);
  $form['currency']['uc_currency_prec'] = array(
    '#type' => 'select',
    '#title' => t('Number of decimal places'),
    '#options' => drupal_map_assoc(array(0, 1, 2)),
    '#default_value' => variable_get('uc_currency_prec', 2),
  );

  $form['weight'] = array(
    '#type' => 'fieldset',
    '#title' => t('Weight format'),
    '#description' => t('Supply a format string for each unit. !value represents the weight value.'),
    '#group' => 'store',
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
    '#default_value' => variable_get('uc_weight_unit', 'lb'),
    '#options' => $units,
  );
  foreach ($units as $unit => $name) {
    $form['weight']['uc_weight_format_' . $unit] = array(
      '#type' => 'textfield',
      '#title' => t('@unit format string', array('@unit' => $name)),
      '#default_value' => variable_get('uc_weight_format_' . $unit, '!value ' . $unit),
    );
  }

  $form['length'] = array(
    '#type' => 'fieldset',
    '#title' => t('Length format'),
    '#description' => t('Supply a format string for each unit. !value represents the length value.'),
    '#group' => 'store',
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
    $form['length']['uc_length_format_' . $unit] = array(
      '#type' => 'textfield',
      '#title' => t('@unit format string', array('@unit' => $name)),
      '#default_value' => variable_get('uc_length_format_' . $unit, '!value ' . $unit),
    );
  }

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#group' => 'store',
  );
  $form['display']['uc_customer_list_address'] = array(
    '#type' => 'radios',
    '#title' => t('Primary customer address'),
    '#description' => t('Select the address to be used on customer lists and summaries.'),
    '#options' => array(
      'billing' => t('Billing address'),
      'shipping' => t('Shipping address'),
    ),
    '#default_value' => variable_get('uc_customer_list_address', 'billing'),
  );
  $form['display']['uc_footer_message'] = array(
    '#type' => 'radios',
    '#title' => t('Footer message for store pages'),
    '#options' => array_merge(
      array(0 => t('Randomly select a message from the list below.')),
      _uc_store_footer_options(),
      array('none' => t('(Do not display a message in the footer.)'))
    ),
    '#default_value' => variable_get('uc_footer_message', 0),
    '#weight' => 10,
  );

  return system_settings_form($form);
}

/**
 * Validates store e-mail address for uc_store_settings_form().
 *
 * @see uc_store_settings_form()
 */
function uc_store_settings_form_validate($form, &$form_state) {
  $mail = trim($form_state['values']['uc_store_email']);
  if (!valid_email_address($mail)) {
    form_set_error('uc_store_email', t('The e-mail address %mail is not valid.', array('%mail' => $mail)));
  }
}

/**
 * Implements hook_uc_store_status().
 */
function uc_store_uc_store_status() {
  $messages = array();

  // Check to see if there are any updated CIF files that need to be installed.
  $countries = array();
  $result = db_query("SELECT * FROM {uc_countries}");
  foreach ($result as $country) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');
  $files = _uc_country_import_list();

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
      'desc'   => t('Updates are available for the following installed countries: %countries. You may update these countries on the <a href="@url">country settings</a> page.', array('%countries' => implode(', ', $updates), '@url' => url('admin/store/settings/countries'))),
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
      'desc'   => t('Store e-mail address has not been set. Please enter it <a href="@url">here</a>.', array('@url' => url('admin/store/settings/store'))),
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
