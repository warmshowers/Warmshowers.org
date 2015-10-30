<?php

/**
 * @file
 * Shipping quotes administration menu items.
 */

/**
 * Default shipping settings.
 *
 * Sets the default shipping location of the store. Allows the user to
 * determine which quotin methods are enabled and which take precedence over
 * the others. Also sets the default quote and shipping types of all products
 * in the store. Individual products may be configured differently.
 *
 * @see uc_quote_admin_settings_submit()
 * @ingroup forms
 */
function uc_quote_admin_settings($form, &$form_state) {
  $address = variable_get('uc_quote_store_default_address', new UcAddress());

  $form['uc_quote_log_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log errors during checkout to watchdog'),
    '#default_value' => variable_get('uc_quote_log_errors', FALSE),
  );
  $form['uc_quote_display_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display debug information to administrators.'),
    '#default_value' => variable_get('uc_quote_display_debug', FALSE),
  );
  $form['uc_quote_require_quote'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent the customer from completing an order if a shipping quote is not selected.'),
    '#default_value' => variable_get('uc_quote_require_quote', TRUE),
  );

  $form['uc_quote_pane_description'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping quote pane description'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['uc_quote_pane_description']['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Message text'),
    '#default_value' => variable_get('uc_quote_pane_description', t('Shipping quotes are generated automatically when you enter your address and may be updated manually with the button below.')),
  );

  $form['uc_quote_err_msg'] = array(
    '#type' => 'fieldset',
    '#title' => t('Shipping quote error message'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['uc_quote_err_msg']['text'] = array(
    '#type' => 'textarea',
    '#title' => t('Message text'),
    '#default_value' => variable_get('uc_quote_err_msg', t("There were problems getting a shipping quote. Please verify the delivery and product information and try again.\nIf this does not resolve the issue, please call in to complete your order.")),
  );

  $form['default_address'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default pickup address'),
    '#description' => t("When delivering products to customers, the original location of the product must be known in order to accurately quote the shipping cost and set up a delivery. This form provides the default location for all products in the store. If a product's individual pickup address is blank, Ubercart uses the store's default pickup address specified here."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['default_address']['address'] = array(
    '#type' => 'uc_address',
    '#default_value' => isset($form_state['values']) ? $form_state['values'] : $address,
    '#required' => FALSE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
  return $form;
}

/**
 * Form submission handler for uc_quote_admin_settings().
 *
 * @see uc_quote_admin_settings()
 */
function uc_quote_admin_settings_submit($form, &$form_state) {
  $address = new UcAddress();
  $address->first_name = $form_state['values']['first_name'];
  $address->last_name = $form_state['values']['last_name'];
  $address->company = $form_state['values']['company'];
  $address->phone = $form_state['values']['phone'];
  $address->street1 = $form_state['values']['street1'];
  $address->street2 = $form_state['values']['street2'];
  $address->city = $form_state['values']['city'];
  $address->zone = $form_state['values']['zone'];
  $address->postal_code = $form_state['values']['postal_code'];
  $address->country = $form_state['values']['country'];

  variable_set('uc_quote_store_default_address', $address);
  variable_set('uc_quote_log_errors', $form_state['values']['uc_quote_log_errors']);
  variable_set('uc_quote_display_debug', $form_state['values']['uc_quote_display_debug']);
  variable_set('uc_quote_require_quote', $form_state['values']['uc_quote_require_quote']);
  variable_set('uc_quote_pane_description', $form_state['values']['uc_quote_pane_description']['text']);
  variable_set('uc_quote_err_msg', $form_state['values']['uc_quote_err_msg']['text']);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Settings for the shipping quote methods.
 *
 * Enables and reorders shipping quote methods. Sets the default shipping type.
 *
 * @see uc_quote_method_settings_validate()
 * @see uc_quote_method_settings_submit()
 * @see theme_uc_quote_method_settings()
 * @ingroup forms
 */
function uc_quote_method_settings($form, &$form_state) {
  $form['methods'] = array(
    '#tree' => TRUE,
  );

  foreach (uc_quote_methods(TRUE) as $method) {
    if (isset($method['quote'])) {
      $id = $method['id'];

      // Build a list of operations links.
      $operations = isset($method['operations']) ? $method['operations'] : array();
      $operations += array('conditions' => array(
        'title' => t('conditions'),
        'href' => 'admin/store/settings/quotes/manage/get_quote_from_' . $id,
        'weight' => 5,
      ));

      // Ensure "delete" comes towards the end of the list.
      if (isset($operations['delete'])) {
        $operations['delete']['weight'] = 10;
      }
      uasort($operations, 'drupal_sort_weight');

      $form['methods'][$id]['uc_quote_enabled'] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($method['title']),
        '#default_value' => $method['enabled'],
      );
      $form['methods'][$id]['description'] = array(
        '#markup' => isset($method['description']) ? $method['description'] : '',
      );
      $form['methods'][$id]['uc_quote_method_weight'] = array(
        '#type' => 'weight',
        '#default_value' => $method['weight'],
        '#attributes' => array('class' => array('uc-quote-method-weight')),
      );
      $form['methods'][$id]['operations'] = array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      );
    }
  }

  $shipping_types = uc_quote_shipping_type_options();
  if (is_array($shipping_types)) {
    $form['uc_quote_type_weight'] = array(
      '#type' => 'fieldset',
      '#title' => t('List position'),
      '#description' => t('Determines which shipping methods are quoted at checkout when products of different shipping types are ordered. Larger values take precedence.'),
      '#collapsible' => TRUE,
      '#tree' => TRUE,
    );
    $weight = variable_get('uc_quote_type_weight', array());
    $shipping_methods = module_invoke_all('uc_shipping_method');
    $method_types = array();
    foreach ($shipping_methods as $method) {
      // Get shipping method types from shipping methods that provide quotes
      if (isset($method['quote'])) {
        $method_types[$method['quote']['type']][] = $method['title'];
      }
    }
    if (isset($method_types['order']) && is_array($method_types['order'])) {
      $count = count($method_types['order']);
      $form['uc_quote_type_weight']['#description'] .= format_plural($count, '<br />The %list method is compatible with any shipping type.', '<br />The %list methods are compatible with any shipping type.', array('%list' => implode(', ', $method_types['order'])));
    }
    foreach ($shipping_types as $id => $title) {
      $form['uc_quote_type_weight'][$id] = array(
        '#type' => 'weight',
        '#title' => $title . (isset($method_types[$id]) && is_array($method_types[$id]) ? ' (' . implode(', ', $method_types[$id]) . ')' : ''),
        '#delta' => 5,
        '#default_value' => isset($weight[$id]) ? $weight[$id] : 0,
      );
    }
  }
  $form['uc_store_shipping_type'] = array(
    '#type' => 'select',
    '#title' => t('Default order fulfillment type for products'),
    '#options' => $shipping_types,
    '#default_value' => variable_get('uc_store_shipping_type', 'small_package'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );

  return $form;
}

/**
 * Displays a formatted list of shipping quote methods and form elements.
 *
 * @see uc_quote_method_settings()
 * @ingroup themeable
 */
function theme_uc_quote_method_settings($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('uc-quote-methods', 'order', 'sibling', 'uc-quote-method-weight');

  $header = array(t('Shipping method'), t('Details'),  array('data' => t('List position'), 'sort' => 'asc'), t('Operations'));
  $rows = array();
  foreach (element_children($form['methods']) as $method) {
    $row = array(
      drupal_render($form['methods'][$method]['uc_quote_enabled']),
      drupal_render($form['methods'][$method]['description']),
      drupal_render($form['methods'][$method]['uc_quote_method_weight']),
      drupal_render($form['methods'][$method]['operations']),
    );

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'uc-quote-methods'),
    'empty' => t('No shipping quotes have been configured yet.'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form validation for uc_quote_method_settings().
 *
 * Requires at least one enabled shipping method.
 *
 * @see uc_quote_method_settings()
 * @see uc_quote_method_settings_submit()
 */
function uc_quote_method_settings_validate($form, &$form_state) {
  $none_enabled = TRUE;
  if (is_array($form_state['values']['methods'])) {
    foreach ($form_state['values']['methods'] as $method) {
      if ($method['uc_quote_enabled']) {
        $none_enabled = FALSE;
      }
    }
  }
  if ($none_enabled) {
    form_set_error('uc_quote_enabled', t('At least one shipping quote method must be enabled.'));
  }
}

/**
 * Form submission handler for uc_quote_method_settings().
 *
 * @see uc_quote_method_settings()
 * @see uc_quote_method_settings_validate()
 */
function uc_quote_method_settings_submit($form, &$form_state) {
  $enabled = array();
  $method_weight = array();
  foreach ($form_state['values']['methods'] as $id => $method) {
    $enabled[$id] = $method['uc_quote_enabled'];
    $method_weight[$id] = $method['uc_quote_method_weight'];
  }

  variable_set('uc_quote_enabled', $enabled);
  variable_set('uc_quote_method_weight', $method_weight);
  variable_set('uc_quote_type_weight', $form_state['values']['uc_quote_type_weight']);
  variable_set('uc_store_shipping_type', $form_state['values']['uc_store_shipping_type']);
  drupal_set_message(t('The configuration options have been saved.'));
}
