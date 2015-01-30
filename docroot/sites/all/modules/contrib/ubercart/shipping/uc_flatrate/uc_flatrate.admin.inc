<?php

/**
 * @file
 * Flat rate shipping method administration menu items.
 */

/**
 * List and compare all flatrate shipping quote methods.
 */
function uc_flatrate_admin_methods() {
  $output = '';

  $context = array(
    'revision' => 'themed',
    'type' => 'amount',
  );

  $rows = array();
  $enabled = variable_get('uc_quote_enabled', array());
  $weight = variable_get('uc_quote_method_weight', array());
  $result = db_query("SELECT mid, title, label, base_rate, product_rate FROM {uc_flatrate_methods}");
  while ($method = db_fetch_object($result)) {
    $row = array();
    $row[] = check_plain($method->title);
    $row[] = check_plain($method->label);
    $row[] = uc_price($method->base_rate, $context);
    $row[] = uc_price($method->product_rate, $context);
    $row[] = l(t('edit'), 'admin/store/settings/quotes/methods/flatrate/'. $method->mid);
    $row[] = l(t('conditions'), CA_UI_PATH .'/uc_flatrate_get_quote_'. $method->mid .'/edit/conditions');
    $rows[] = $row;
  }
  if (count($rows)) {
    $header = array(t('Title'), t('Label'), t('Base rate'), t('Default product rate'), array('data' => t('Operations'), 'colspan' => 2));
    $output .= theme('table', $header, $rows);
  }
  $output .= l(t('Add a new flat rate shipping method.'), 'admin/store/settings/quotes/methods/flatrate/add');
  return $output;
}

/**
 * Configure the store default product shipping rates.
 *
 * @ingroup forms
 * @see uc_flatrate_admin_method_edit_form_validate()
 * @see uc_flatrate_admin_method_edit_form_delete()
 * @see uc_flatrate_admin_method_edit_form_submit()
 */
function uc_flatrate_admin_method_edit_form($form_state, $mid = 0) {
  $form = array();
  $sign_flag = variable_get('uc_sign_after_amount', FALSE);
  $currency_sign = variable_get('uc_currency_sign', '$');

  if (is_numeric($mid) && ($method = db_fetch_object(db_query("SELECT * FROM {uc_flatrate_methods} WHERE mid = %d", $mid)))) {
    $form['mid'] = array('#type' => 'value', '#value' => $mid);
  }
  else {
    $method = (object) array(
      'title' => '',
      'label' => '',
      'base_rate' => '',
      'product_rate' => '',
    );
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping method title'),
    '#description' => t('The name shown to distinguish it from other flatrate methods.'),
    '#default_value' => $method->title,
    '#required' => TRUE,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Line item label'),
    '#description' => t('The name shown to the customer when they choose a shipping method at checkout.'),
    '#default_value' => $method->label,
    '#required' => TRUE,
  );
  $form['base_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Base price'),
    '#description' => t('The starting price for shipping costs.'),
    '#default_value' => $method->base_rate,
    '#size' => 16,
    '#field_prefix' => $sign_flag ? '' : $currency_sign,
    '#field_suffix' => $sign_flag ? $currency_sign : '',
  );
  $form['product_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Default product shipping rate'),
    '#description' => t('Additional shipping cost per product in cart.'),
    '#default_value' => $method->product_rate,
    '#size' => 16,
    '#field_prefix' => $sign_flag ? '' : $currency_sign,
    '#field_suffix' => $sign_flag ? $currency_sign : '',
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('uc_flatrate_admin_method_edit_form_delete'),
  );

  return $form;
}

/**
 * @see uc_flatrate_admin_method_edit_form()
 */
function uc_flatrate_admin_method_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Submit')) {
    if (!empty($form_state['values']['base_rate']) && !is_numeric($form_state['values']['base_rate'])) {
      form_set_error('base_rate', t('The base rate must be a numeric amount.'));
    }
    if (!empty($form_state['values']['product_rate']) && !is_numeric($form_state['values']['product_rate'])) {
      form_set_error('product_rate', t('The product rate must be a numeric amount.'));
    }
  }
}

/**
 * @see uc_flatrate_admin_method_edit_form()
 */
function uc_flatrate_admin_method_edit_form_delete($form, &$form_state) {
  drupal_goto('admin/store/settings/quotes/flatrate/'. $form_state['values']['mid'] .'/delete');
}

/**
 * @see uc_flatrate_admin_method_edit_form()
 */
function uc_flatrate_admin_method_edit_form_submit($form, &$form_state) {
  if (isset($form_state['values']['mid'])) {
    db_query("UPDATE {uc_flatrate_methods} SET title = '%s', label = '%s', base_rate = %f, product_rate = %f WHERE mid = %d",
      $form_state['values']['title'], $form_state['values']['label'], $form_state['values']['base_rate'], $form_state['values']['product_rate'], $form_state['values']['mid']);
    drupal_set_message(t("Flat rate shipping method was updated."));
  }
  else {
    db_query("INSERT INTO {uc_flatrate_methods} (title, label, base_rate, product_rate) VALUES ('%s', '%s', %f, %f)",
      $form_state['values']['title'], $form_state['values']['label'], $form_state['values']['base_rate'], $form_state['values']['product_rate']);
    $mid = db_last_insert_id('uc_flatrate_methods', 'mid');
    $enabled = variable_get('uc_quote_enabled', array());
    $enabled['flatrate_'. $mid] = TRUE;
    variable_set('uc_quote_enabled', $enabled);
    $weight = variable_get('uc_quote_method_weight', array());
    $weight['flatrate_'. $mid] = 0;
    variable_set('uc_quote_method_weight', $weight);
    drupal_set_message(t("Created and enabled new flat rate shipping method."));
  }
  $form_state['redirect'] = 'admin/store/settings/quotes/methods/flatrate';
}

/******************************************************************************
 * Menu Callbacks                                                             *
 ******************************************************************************/

/**
 * Confirm deletion of a flat rate shipping method.
 *
 * @see uc_flatrate_admin_method_confirm_delete_submit()
 */
function uc_flatrate_admin_method_confirm_delete($form_state, $mid) {
  $form = array();
  $form['mid'] = array('#type' => 'value', '#value' => $mid);

  return confirm_form($form, t('Do you want to delete this shipping method?'),
    'admin/store/settings/quotes/methods/flatrate',
    t('This will remove the shipping method, Conditional Action predicate, and the
      product-specific overrides (if applicable). This action can not be undone.'),
    t('Delete'));
}

/**
 * @see uc_flatrate_admin_method_confirm_delete()
 */
function uc_flatrate_admin_method_confirm_delete_submit($form, &$form_state) {
  db_query("DELETE FROM {uc_flatrate_methods} WHERE mid = %d", $form_state['values']['mid']);
  db_query("DELETE FROM {uc_flatrate_products} WHERE mid = %d", $form_state['values']['mid']);
  ca_delete_predicate('uc_flatrate_get_quote_'. $form_state['values']['mid']);

  $enabled = variable_get('uc_quote_enabled', array());
  unset($enabled['flatrate_'. $form_state['values']['mid']]);
  variable_set('uc_quote_enabled', $enabled);

  $weight = variable_get('uc_quote_method_weight', array());
  unset($weight['flatrate_'. $form_state['values']['mid']]);
  variable_set('uc_quote_method_weight', $weight);

  drupal_set_message(t('Flat rate shipping method deleted.'));
  $form_state['redirect'] = 'admin/store/settings/quotes/methods/flatrate';
}
