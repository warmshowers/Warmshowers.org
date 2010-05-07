<?php
// $Id: services_keyauth.admin.inc,v 1.1.2.2.2.7 2010/02/26 17:32:33 heyrocker Exp $

/**
 * @file
 *  Generate security keys.
 */

/**
 * Display all the available API keys.
 */
function services_keyauth_admin_keys_list() {
  $keys = services_keyauth_get_keys();
  $header = array(t('Key'), t('Title'), t('Domain'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();

  foreach ($keys as $kid => $key) {
    $row = array();

    $row[] = $kid;
    $row[] = $key->title;
    $row[] = $key->domain;

    // Populate the operations field.
    $operations = array();

    // Set the edit column.
    $operations[] = array('data' => l(t('edit'), 'admin/build/services/keys/'. $kid));

    // Set the delete column.
    $operations[] = array('data' => l(t('delete'), 'admin/build/services/keys/'. $kid .'/delete'));

    foreach ($operations as $operation) {
      $row[] = $operation;
    }
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No API keys created.'), 'colspan' => '5', 'class' => 'message'));
  }

  return theme('table', $header, $rows);
}

/**
 * Display the form to create or edit API keys.
 */
function services_keyauth_admin_keys_form() {
  $kid = arg(4);

  $key = db_fetch_object(db_query("SELECT * FROM {services_keys} WHERE kid = '%s'", $kid));

  $key_kid = isset($key->kid) ? $key->kid : '';
  $key_title = isset($key->title) ? $key->title : '';
  $form['kid'] = array(
    '#type'           => 'hidden',
    '#default_value'  => $key_kid,
  );

  $accessible_methods = array();
  if ($key_kid != '') {
    $form['key'] = array(
      '#type'           => 'markup',
      '#title'          => t('Key'),
      '#value'          => '<strong>'. t('API Key') .':</strong> '. $key_kid,
    );
    $result = db_query("SELECT method FROM {services_key_permissions} WHERE kid = '%s'", $key->kid);
    while ($kid = db_fetch_object($result)) {
      $accessible_methods[] = $kid->method;
    }
  }

  $form['title'] = array(
    '#title'          => t('Application title'),
    '#type'           => 'textfield',
    '#default_value'  => $key_title,
    '#description'    => t('The title of the application or website using the service.'),
  );
  $form['domain'] = array(
    '#title'          => t('Allowed domain'),
    '#type'           => 'textfield',
    '#default_value'  => isset($key->domain) ? $key->domain : '',
    '#description'    => t('External domain allowed to use this key.'),
    '#required'       => TRUE,
  );

  $methods = services_get_all();
  foreach ($methods as $method) {
    $form_methods[$method['method']] = $method['method'];
  }

  $form['method_access'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Method access'),
    '#options' => $form_methods,
    '#default_value' => $accessible_methods,
    '#description' => t('Define which methods are accessible.'),
  );

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => $key_title != '' ? t('Save key') : t('Create key'),
  );

  return $form;
}

/**
 * Submit callback for services_keyauth_admin_keys_form().
 */
function services_keyauth_admin_keys_form_submit($form, &$form_state) {
  services_keyauth_admin_keys_save($form_state['values']);

  $form_state['redirect']  =  'admin/build/services/keys';
}

/**
 * Save an API key.
 *
 * @param $key
 *   Associative array containing API key information.
 *
 * @return
 *   A constant, either SAVED_NEW or SAVED_UDPATED.
 */
function services_keyauth_admin_keys_save(&$key) {
  $is_existing = FALSE;
  $key['kid'] = !empty($key['kid']) ? $key['kid'] : md5(uniqid(mt_rand(), TRUE));
  $is_existing =  db_result(db_query("SELECT count(*) FROM {services_keys}
    WHERE kid = '%s'", $key['kid']));

  if ($is_existing) {
    db_query("UPDATE {services_keys} SET title = '%s', domain = '%s'
      WHERE kid = '%s'", $key['title'], $key['domain'], $key['kid']);
    db_query("DELETE FROM {services_key_permissions} WHERE kid = '%s'", $key['kid']);
    $return = SAVED_UPDATED;
  }
  else {
    db_query("INSERT INTO {services_keys} (kid, title, domain)
      VALUES ('%s', '%s', '%s')", $key['kid'], $key['title'], $key['domain']);
    $return = SAVED_NEW;
  }
  foreach ($key['method_access'] as $method => $value) {
    if ($value) {
      db_query("INSERT INTO {services_key_permissions} (kid, method) VALUES ('%s', '%s')", $key['kid'], $value);
    }
  }
  return $return;
}

/**
 * Delete an API key.
 *
 * @param $kid
 *   Unique identifier from the services_keys table.
 */
function services_keyauth_admin_keys_delete($kid) {
  db_query("DELETE FROM {services_keys} WHERE kid = '%s'", $kid);
}

/**
 * Show a form to confirm whether an API key should be deleted.
 */
function services_keyauth_admin_keys_delete_confirm(&$form_state, $kid = 0) {
  $key = db_fetch_object(db_query("SELECT * FROM {services_keys} WHERE kid = '%s'", $kid));

  $form['kid'] = array('#type' => 'value', '#value' => $key->kid);

  $message = t('Are you sure you want to delete the key %key?', array('%key' => $key->kid));
  $caption = '';

  $caption .= '<p>'. t('This action cannot be undone.') .'</p>';

  return confirm_form($form, $message, 'admin/build/services/keys', $caption, t('Delete'));
}

/**
 * Submit form callback for services_keyauth_admin_keys_delete_confirm().
 */
function services_keyauth_admin_keys_delete_confirm_submit($form, &$form_state) {
  services_keyauth_admin_keys_delete($form_state['values']['kid']);

  $t_args = array('%key' => $form_state['kid']);
  drupal_set_message(t('The key %key has been deleted.', $t_args));
  watchdog('menu', 'Deleted key %key.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/build/services/keys';
}