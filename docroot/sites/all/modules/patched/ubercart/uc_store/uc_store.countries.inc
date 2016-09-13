<?php

/**
 * @file
 * Store administration forms for country and address handling.
 */

/**
 * Imports settings from a country file.
 *
 * @see uc_country_import_form_submit()
 * @ingroup forms
 */
function uc_country_import_form($form, &$form_state) {
  $countries = array();
  $result = db_query("SELECT * FROM {uc_countries}");
  foreach ($result as $country) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');
  $files = _uc_country_import_list();

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
        $ops[] = l(t('enable'), 'admin/store/settings/countries/' . $country->country_id . '/enable', $link_ops);
      }
      else {
        $ops[] = l(t('disable'), 'admin/store/settings/countries/' . $country->country_id . '/disable', $link_ops);
      }
      if ($country->version < $files[$country->country_id]['version'] && $country->version > 0) {
        $ops[] = l(t('update'), 'admin/store/settings/countries/' . $country->country_id . '/update/' . $files[$country->country_id]['version'], $link_ops);
      }
      $ops[] = l(t('remove'), 'admin/store/settings/countries/' . $country->country_id . '/remove');
      $row[] = implode(' ', $ops);

      $rows[] = $row;
      unset($files[$country->country_id]);
    }
  }

  $import_list = array();
  foreach ($files as $file) {
    $import_list[$file['file']] = $file['file'];
  }

  if (!empty($import_list)) {
    ksort($import_list);

    $form['country_import'] = array(
      '#title' => t('Import countries'),
      '#type' => 'fieldset',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $form['country_import']['text'] = array(
      '#markup' => '<p>' . t('To import new country data, select it in the list and click the import button. If you are using a custom or contributed import file, it must be placed in the Ubercart folder uc_store/countries.') . '</p>',
    );
    $form['country_import']['import_file'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#options' => $import_list,
      '#multiple' => TRUE,
      '#size' => min(10, count($import_list)),
    );
    $form['country_import']['actions'] = array('#type' => 'actions');
    $form['country_import']['actions']['import_button'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
    );
  }

  $form['country_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $form;
}

/**
 * Form submission handler for uc_country_import_form().
 *
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
 * Form builder to set country address formats.
 *
 * @see uc_country_formats_form_submit()
 * @ingroup forms
 */
function uc_country_formats_form($form, &$form_state) {
  $form['instructions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Address format variables'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
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
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#prefix' => '<div><p>' . t('The following variables should be used in configuring addresses for the countries you ship to:') . '</p>',
    '#suffix' => '<p>' . t('Adding _if to any country variable will make it display only for addresses whose country is different than the default store country.') . '</p></div>',
  );

  $countries = array();
  $result = db_query("SELECT * FROM {uc_countries}");
  foreach ($result as $country) {
    $countries[t($country->country_name)] = $country;
  }
  uksort($countries, 'strnatcasecmp');

  if (is_array($countries)) {
    $form['countries'] = array(
      '#type' => 'vertical_tabs',
      '#tree' => TRUE,
    );
    foreach ($countries as $country) {
      $form['countries'][$country->country_id] = array(
        '#type' => 'fieldset',
        '#title' => check_plain(t($country->country_name)),
        '#group' => 'country',
      );
      $form['countries'][$country->country_id]['address_format'] = array(
        '#type' => 'textarea',
        '#title' => t('@country address format', array('@country' => t($country->country_name))),
        '#default_value' => variable_get('uc_address_format_' . $country->country_id, ''),
        '#description' => t('Use the variables mentioned in the instructions to format an address for this country.'),
        '#rows' => 7,
      );
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes'),
  );

  return $form;
}

/**
 * Form submission handler for uc_country_formats_form().
 *
 * @see uc_country_formats_form()
 */
function uc_country_formats_form_submit($form, &$form_state) {
  foreach ($form_state['values']['countries'] as $country_id => $data) {
    variable_set('uc_address_format_' . $country_id, $data['address_format']);
  }
  drupal_set_message(t('Country settings saved.'));
}

/**
 * Disables a country so it remains installed but is no longer selectable.
 *
 * @param $country_id
 *   The ISO 3166-1 numeric country code.
 */
function uc_country_disable($country_id) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = :id", array(':id' => $country_id));
  if ($country = $result->fetchObject()) {
    if ($country->version > 0) {
      db_update('uc_countries')
        ->fields(array(
          'version' => -$country->version,
        ))
        ->condition('country_id', $country_id)
        ->execute();
      drupal_set_message(t('@country disabled.', array('@country' => t($country->country_name))));
    }
    else {
      drupal_set_message(t('@country is already disabled.', array('@country' => t($country->country_name))), 'error');
    }
  }
  else {
    drupal_set_message(t('Attempted to disable an invalid country.'), 'error');
  }
  drupal_goto('admin/store/settings/countries');
}

/**
 * Enables a disabled country.
 *
 * @param $country_id
 *   The ISO 3166-1 numeric country code.
 */
function uc_country_enable($country_id) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = :id", array(':id' => $country_id));
  if ($country = $result->fetchObject()) {
    if ($country->version < 0) {
      db_update('uc_countries')
        ->fields(array(
          'version' => abs($country->version),
        ))
        ->condition('country_id', $country_id)
        ->execute();
      drupal_set_message(t('@country enabled.', array('@country' => t($country->country_name))));
    }
    else {
      drupal_set_message(t('@country is already enabled.', array('@country' => t($country->country_name))), 'error');
    }
  }
  else {
    drupal_set_message(t('Attempted to enable an invalid country.'), 'error');
  }
  drupal_goto('admin/store/settings/countries');
}

/**
 * Form to completely remove a country.
 *
 * @param $country_id
 *   The ISO 3166-1 numeric country code.
 *
 * @see uc_country_remove_form_submit()
 * @ingroup forms
 */
function uc_country_remove_form($form, &$form_state, $country_id) {
  // Fetch the country name from the database.
  $country = t(db_query("SELECT country_name FROM {uc_countries} WHERE country_id = :id", array(':id' => $country_id))->fetchField());

  // If orders exist for this country, show a warning message prior to removal.
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#value'] != t('Remove') && module_exists('uc_order')) {
    $count = db_query("SELECT COUNT(order_id) FROM {uc_orders} WHERE delivery_country = :delivery_country OR billing_country = :billing_country", array(':delivery_country' => $country_id, ':billing_country' => $country_id))->fetchField();
    if ($count > 0) {
      drupal_set_message(t('Warning: @count orders were found with addresses in this country. Removing this country now will cause errors to show on those order pages. You might consider simply disabling this country instead.', array('@count' => $count)), 'error');
    }
  }

  // Store the country ID in the form array for processing.
  $form['country_id'] = array(
    '#type' => 'value',
    '#value' => $country_id,
  );

  return confirm_form($form, t('Are you sure you want to remove @country from the system?', array('@country' => $country)), 'admin/store/settings/countries', NULL, t('Remove'));
}

/**
 * Form submission handler for uc_country_remove_form().
 *
 * @see uc_country_remove_form()
 */
function uc_country_remove_form_submit($form, &$form_state) {
  $country_id = $form_state['values']['country_id'];

  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = :id", array(':id' => $country_id));
  if (!($country = $result->fetchObject())) {
    drupal_set_message(t('Attempted to remove an invalid country.'), 'error');
    drupal_goto('admin/store/settings/countries');
  }

  db_delete('uc_countries')
    ->condition('country_id', $country_id)
    ->execute();
  db_delete('uc_zones')
    ->condition('zone_country_id', $country_id)
    ->execute();
  variable_del('uc_address_format_' . $country_id);

  $func_base = _uc_country_import_include($country_id, $country->version);
  if ($func_base !== FALSE) {
    $func = $func_base . '_uninstall';
    if (function_exists($func)) {
      $func();
    }
  }

  drupal_set_message(t('@country removed.', array('@country' => t($country->country_name))));
  drupal_goto('admin/store/settings/countries');
}

/**
 * Updates a country definition to a specific CIF file version.
 *
 * @param $country_id
 *   The ISO 3166-1 numeric country code.
 * @param $version
 *   Version number of CIF file.
 */
function uc_country_update($country_id, $version) {
  $result = db_query("SELECT * FROM {uc_countries} WHERE country_id = :id", array(':id' => $country_id));
  if (!($country = $result->fetchObject())) {
    drupal_set_message(t('Attempted to update an invalid country.'));
    drupal_goto('admin/store/settings/countries');
  }

  if ($version < $country->version) {
    drupal_set_message(t('You cannot update to a previous version.'));
    drupal_goto('admin/store/settings/countries');
  }

  $func_base = _uc_country_import_include($country_id, $version);
  if ($func_base !== FALSE) {
    $func = $func_base . '_update';
    if (function_exists($func)) {
      for ($i = $country->version; $i <= $version; $i++) {
        $func($i);
      }
    }

    db_update('uc_countries')
      ->fields(array(
        'version' => $version,
      ))
      ->condition('country_id', $country_id)
      ->execute();
    drupal_set_message(t('Country update complete.'));
  }
  else {
    drupal_set_message(t('Attempted to update an invalid country.'));
  }

  drupal_goto('admin/store/settings/countries');
}

/**
 * Imports an Ubercart country file by filename.
 *
 * @param $file
 *   The filename of the country to import.
 *
 * @return
 *   TRUE or FALSE indicating whether or not the country was imported.
 */
function uc_country_import($file) {
  require_once(drupal_get_path('module', 'uc_store') . '/countries/' . $file);

  $pieces = explode('_', substr($file, 0, strlen($file) - 4));

  $country_id = $pieces[count($pieces) - 2];
  $version = $pieces[count($pieces) - 1];
  $country = substr($file, 0, strlen($file) - strlen($country_id) - strlen($version) - 6);

  $func = $country . '_install';

  if (function_exists($func)) {
    $func();
    return TRUE;
  }

  return FALSE;
}

/**
 * Includes the appropriate country file and return the base for hooks.
 *
 * @param $country_id
 *   ISO 3166-1 numeric country code for the CIF file to import.
 * @param $version
 *   Version number of the CIF to import.
 *
 * @return
 *   A string containing the portion of the filename holding the country name.
 */
function _uc_country_import_include($country_id, $version) {
  $dir = drupal_get_path('module', 'uc_store') . '/countries/';
  $match = '_' . $country_id . '_' . $version . '.cif';
  $matchlen = strlen($match);

  $countries = array();
  if (is_dir($dir)) {
    if ($dh = opendir($dir)) {
      while (($file = readdir($dh)) !== FALSE) {
        switch (filetype($dir . $file)) {
          case 'file':
            if (substr($file, -$matchlen) == $match) {
              require_once($dir . $file);
              return substr($file, 0, strlen($file) - $matchlen);
            }
            break;
        }
      }
      closedir($dh);
    }
  }

  return FALSE;
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
