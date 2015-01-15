<?php

/**
 * @file
 * Payment administration menu items.
 */


/**
 * Displays an overview of the payment settings.
 */
function uc_payment_settings_overview() {
  // Theme all the pages beneath this path into summary overviews.
  return theme('summary_overview', summarize_child_form_pages('admin/store/settings/payment/edit'));
}

/**
 * Payment settings form.
 */
function uc_payment_settings_form() {
  $form['uc_payment_tracking'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable payment tracking.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Payment tracking is enabled.'),
      t('Payment tracking is disabled.'),
    ),
    '#default_value' => variable_get('uc_payment_tracking', TRUE),
  );
  $form['uc_payment_deleting'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow payments to be deleted by users with permission.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Payments can be deleted by users with permission.'),
      t('Payments cannot be deleted, even if the user has permission.'),
    ),
    '#default_value' => variable_get('uc_payment_deleting', TRUE),
  );
  $form['uc_payment_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log payments entered and deleted to order log.'),
    '#summary callback' => 'summarize_checkbox',
    '#summary arguments' => array(
      t('Log payments are being entered to and deleted from the order log.'),
      t('Log payments are not being entered to and deleted from the order log.'),
    ),
    '#default_value' => variable_get('uc_payment_logging', TRUE),
  );

  $form['uc_default_payment_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('Default payment details message'),
    '#description' => t('Message displayed when a payment method does not display any further details.'),
    '#default_value' => variable_get('uc_default_payment_msg', t('Continue with checkout to complete payment.')),
    '#summary' => t('Default payment details message is: <br /> %message', array('%message' => variable_get('uc_default_payment_msg', t('Continue with checkout to complete payment.')))),
  );

  return system_settings_form($form);
}

/**
 * Displays an overview of the available payment methods.
 *
 * @see theme_uc_payment_methods_table()
 */
function uc_payment_methods_form() {
  $methods = _payment_method_list();

  $form['pmtable'] = array(
    '#theme' => 'uc_payment_method_table',
    '#summary callback' => 'summarize_form',
  );

  if (is_array($methods) && count($methods) > 0) {
    foreach ($methods as $method) {

      $form['pmtable'][$method['id']]['#summary callback'] = 'summarize_form';

      $form['pmtable'][$method['id']]['uc_payment_method_'. $method['id'] .'_checkout'] = array(
        '#type' => 'checkbox',
        '#title' => $method['name'],
        '#summary callback' => 'summarize_checkbox',
        '#summary arguments' => array(
          t('@payment is enabled for checkout.', array('@payment' => $method['name'])),
          t('@payment is disabled for checkout.', array('@payment' => $method['name'])),
        ),
        '#default_value' => variable_get('uc_payment_method_'. $method['id'] .'_checkout', $method['checkout']),
      );
      $form['pmtable'][$method['id']]['uc_payment_method_'. $method['id'] .'_weight'] = array(
        '#type' => 'weight',
        '#default_value' => variable_get('uc_payment_method_'. $method['id'] .'_weight', $method['weight']),
        '#attributes' => array('class' => 'uc-payment-method-weight'),
      );

      if (isset($method['no_gateway']) && $method['no_gateway'] === TRUE) {
        $form['pmtable'][$method['id']]['uc_payment_'. $method['id'] .'_gateway'] = array(
          '#value' => '-',
        );
      }
      else {
        $gateways = _payment_gateway_list($method['id'], TRUE);
        $options = array();
        $default = FALSE;
        if (is_array($gateways)) {
          foreach ($gateways as $gateway) {
            if (!$default) {
              $default = $gateway['id'];
            }
            $options[$gateway['id']] = $gateway['title'];
          }
        }
        if (!$default) {
          $options = array('none' => t('None available.'));
        }
        $form['pmtable'][$method['id']]['uc_payment_'. $method['id'] .'_gateway'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#summary callback' => 'summarize_null',
          '#default_value' => variable_get('uc_payment_'. $method['id'] .'_gateway', 'none'),
        );
      }

      $null = NULL;
      if (function_exists($method['callback'])) {
        $method_settings = $method['callback']('settings', $null);
        if (is_array($method_settings)) {
          $form['method_'. $method['id']] = array(
            '#type' => 'fieldset',
            '#summary callback' => 'summarize_null',
            '#title' => t('!method settings', array('!method' => $method['name'], '!weight' => $method['weight'])),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
          );
          $form['method_'. $method['id']] = array_merge($form['method_'. $method['id']], $method_settings);
        }
      }
    }
  }
  else {
    $form['pmtable'] = array(
      '#value' => '<div>' . t('No payment methods are available. Modules providing payment methods must first be enabled on the !modules administration page under the "Ubercart - payment" fieldset.', array('!modules' => l(t('Modules'), 'admin/build/modules'))) . '</div>',
    );
  }

  return system_settings_form($form);
}

/**
 * Themes the table that displays available payment methods.
 *
 * @see uc_payment_methods_form()
 * @ingroup themeable
 */
function theme_uc_payment_method_table($form) {
  drupal_add_tabledrag('uc-payment-methods', 'order', 'sibling', 'uc-payment-method-weight');

  $header = array(t('Payment method'), t('List position'), t('Default gateway'));

  $rows = array();
  foreach (element_children($form) as $method) {
    $row = array(
      drupal_render($form[$method]['uc_payment_method_'. $method .'_checkout']),
      drupal_render($form[$method]['uc_payment_method_'. $method .'_weight']),
      drupal_render($form[$method]['uc_payment_'. $method .'_gateway']),
    );

    $rows[] = array(
      'data' => $row,
      'class' => 'draggable',
    );
  }

  return theme('table', $header, $rows, array('id' => 'uc-payment-methods'));
}

/**
 * Displays an overview of the available payment gateways.
 */
function uc_payment_gateways_form() {
  $gateways = _payment_gateway_list();
  $methods = _payment_method_list();
  $form = array();

  $form['gateways'] = array(
    '#summary callback' => '_uc_payment_gateways_summarize',
    '#summary arguments' => array($gateways),
  );

  if (is_array($gateways) && count($gateways) > 0) {
    $form['gateways_info'] = array(
      '#value' => '<div>' . t('Payment gateways are web services that allow you to process various types of payments remotely. The settings forms below are for the payment gateways you have installed. Click a name to expand its options and adjust the settings accordingly.') .'</div>',
      '#weight' => -10,
    );

    foreach ($gateways as $gateway) {
      $form['gateways'][$gateway['id']] = array(
        '#type' => 'fieldset',
        '#title' => t('@gateway_name settings', array('@gateway_name' => $gateway['title'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $supported_methods = array();
      foreach ($methods as $method) {
        if (isset($gateway[$method['id']]) && function_exists($gateway[$method['id']])) {
          $supported_methods[] = $method['name'];
        }
      }

      $form['gateways'][$gateway['id']]['supported_methods'] = array(
        '#value' => '<div>'. t('This gateway supports the following payment methods:')
                   .'<br />'. implode(',', $supported_methods) .'</div>',
        '#weight' => -10,
      );
      $form['gateways'][$gateway['id']]['uc_pg_'. $gateway['id'] .'_enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable this payment gateway for use.'),
        '#default_value' => variable_get('uc_pg_'. $gateway['id'] .'_enabled', TRUE),
      );

      // Find any additional settings defined in the payment gateway callback.
      if (isset($gateway['settings']) && function_exists($gateway['settings'])) {
        $gateway_settings = $gateway['settings']();
        if (is_array($gateway_settings)) {
          $form['gateways'][$gateway['id']] = array_merge($form['gateways'][$gateway['id']], $gateway_settings);
        }
      }
    }
  }
  else {
    $form['gateways_info'] = array(
      '#value' => '<div>' . t('Payment gateways are web services that allow you to process various types of payments remotely. No payment gateways are currently enabled. Modules providing payment gateways must first be enabled on the !modules administration page under the "Ubercart - payment" fieldset.', array('!modules' => l(t('Modules'), 'admin/build/modules'))) . '</div>',
      '#weight' => -10,
    );
  }

  return system_settings_form($form);
}

/**
 * Returns an array of enabled payment gateways for the form summary.
 */
function _uc_payment_gateways_summarize($form, $gateways) {
  $items = array();

  foreach ($gateways as $gateway) {
    $items[] = t('@title is @enabled.', array('@title' => $gateway['title'], '@enabled' => $gateway['enabled'] ? t('enabled') : t('disabled')));
  }

  if (empty($items)) {
    $items[] = t('No payment gateway modules are installed. Modules providing payment gateways must first be enabled on the !modules administration page under the "Ubercart - payment" fieldset.', array('!modules' => l(t('Modules'), 'admin/modules')));
  }

  return $items;
}

/**
 * Selects a specific gateway when multiple gateways are available.
 */
function uc_payment_gateway_select() {

  // Find the available gateways.
  $gateways = _payment_gateway_list($_SESSION['uc_payment_method'], TRUE);
  foreach ($gateways as $gateway) {
    $options[$gateway['id']] = $gateway['title'];
  }

  // Construct form for selection of a gateway.
  $output = t('Please choose a payment gateway to use for that payment.');

  $output .= drupal_get_form(
    'uc_payment_gateway_select_form',
    $options,
    $_SESSION['uc_payment_method'],
    $_SESSION['uc_payment_order_id'],
    $_SESSION['uc_payment_amount'],
    $_SESSION['uc_payment_data']
  );

  return $output;
}

/**
 * Form builder: Selects a specific payment gateway.
 *
 * @see uc_payment_gateway_select_form_submit()
 * @ingroup forms
 */
function uc_payment_gateway_select_form($form_state, $options, $method, $order_id, $amount, $data) {
  $form['method'] = array(
    '#type' => 'hidden',
    '#value' => $method,
  );
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order_id,
  );
  $form['amount'] = array(
    '#type' => 'hidden',
    '#value' => $amount,
  );
  $form['p_data'] = array(
    '#type' => 'hidden',
    '#value' => $data,
  );
  $form['p_selected'] = array(
    '#type' => 'select',
    '#title' => t('Use gateway'),
    '#options' => $options,
    '#default_value' => variable_get('uc_payment_'. $method .'_gateway', ''),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process'),
  );

  return $form;
}

/**
 * Submit handler for uc_payment_gateway_select_form().
 *
 * @see uc_payment_gateway_select_form()
 */
function uc_payment_gateway_select_form_submit($form, &$form_state) {
  unset($_SESSION['uc_payment_method']);
  unset($_SESSION['uc_payment_order_id']);
  unset($_SESSION['uc_payment_amount']);
  unset($_SESSION['uc_payment_data']);

  uc_payment_process($form_state['values']['method'], $form_state['values']['order_id'],
    $form_state['values']['amount'], unserialize($form_state['values']['p_data']), FALSE,
    $form_state['values']['p_selected']);

  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'];
}

/**
 * Displays a list of payments attached to an order.
 *
 * @see uc_payment_by_order_form_validate()
 * @see uc_payment_by_order_form_submit()
 * @see theme_uc_payment_by_order_form()
 * @ingroup forms
 */
function uc_payment_by_order_form($form_state, $order) {
  $total = $order->order_total;
  $payments = uc_payment_load_payments($order->order_id);

  $context = array(
    'revision' => 'themed-original',
    'type' => 'order_total',
    'subject' => array(
      'order' => $order,
    ),
  );
  $form['order_total'] = array('#value' => uc_price($total, $context));
  $form['payments'] = tapir_get_table('uc_payments_table');

  if ($payments !== FALSE) {
    foreach ($payments as $payment) {
      $context['type'] = 'payment';
      $context['subject']['payment'] = $payment;
      $form['payments'][$payment->receipt_id]['#attributes'] = array('valign' => 'top');
      $form['payments'][$payment->receipt_id]['received'] = array(
        '#value' => format_date($payment->received, 'custom', variable_get('uc_date_format_default', 'm/d/Y') .'<b\r>H:i:s'),
      );
      $form['payments'][$payment->receipt_id]['user'] = array(
        '#value' => uc_get_initials($payment->uid),
      );
      $form['payments'][$payment->receipt_id]['method'] = array(
        '#value' => ($payment->method == '') ? t('Unknown') : $payment->method,
      );
      $form['payments'][$payment->receipt_id]['amount'] = array(
        '#value' => uc_price($payment->amount, $context),
      );
      $total -= $payment->amount;
      unset($context['subject']['payment']);
      $context['type'] = 'amount';
      $context['subject']['total'] = $total;
      $form['payments'][$payment->receipt_id]['balance'] = array(
        '#value' => uc_price($total, $context, array('cache' => FALSE)),
      );
      $form['payments'][$payment->receipt_id]['comment'] = array(
        '#value' => ($payment->comment == '') ? '-' : filter_xss_admin($payment->comment),
      );
      if (variable_get('uc_payment_deleting', TRUE) && user_access('delete payments')) {
        $action_value = l(t('Delete'), 'admin/store/orders/'. $order->order_id .'/payments/'
                          . $payment->receipt_id .'/delete');
      }
      else {
        $action_value = '-';
      }
      $form['payments'][$payment->receipt_id]['action'] = array(
        '#value' => $action_value,
      );
    }
  }

  $form['balance'] = array('#value' => uc_price($total, $context));
  $form['order_id'] = array(
    '#type' => 'hidden',
    '#value' => $order->order_id,
  );

  $now = time();
  if (user_access('manual payments')) {
    $form['payments']['new']['#attributes'] = array('valign' => 'top');
    $form['payments']['new']['received'] = array(
      '#type' => 'date',
      '#default_value' => array(
        'month' => format_date($now, 'custom', 'n'),
        'day' => format_date($now, 'custom', 'j'),
        'year' => format_date($now, 'custom', 'Y'),
      ),
    );
    $form['payments']['new']['user'] = array(
      '#value' => '-',
    );
    $methods = _payment_method_list();
    foreach ($methods as $method) {
      $options[$method['id']] = $method['name'];
    }
    $form['payments']['new']['method'] = array(
      '#type' => 'select',
      '#options' => $options,
    );
    $form['payments']['new']['amount'] = array(
      '#type' => 'textfield',
      '#size' => 6,
    );
    $form['payments']['new']['balance'] = array(
      '#value' => '-',
    );
    $form['payments']['new']['comment'] = array(
      '#type' => 'textfield',
      '#size' => 32,
      '#maxlength' => 256,
    );
    $form['payments']['new']['action'] = array(
      '#type' => 'submit',
      '#value' => t('Enter'),
    );
  }

  return $form;
}

/**
 * Themes uc_payment_by_order_form().
 *
 * @see uc_payment_by_order_form()
 * @ingroup themeable
 */
function theme_uc_payment_by_order_form($form) {
  $output = '<p><strong>'. t('Order total:') .'</strong> '. drupal_render($form['order_total'])
           .'<br /><strong>'. t('Current balance:') .'</strong> '
          . drupal_render($form['balance']) .'</p>';
  $output .= '<p>'. drupal_render($form['payments']) .'</p>'
           .'<p>'. drupal_render($form['form_id'])
           . drupal_render($form['form_token']) .'</p>';
  return $output;
}

/**
 * Form validation for uc_payment_by_order_form().
 *
 * @see uc_payment_by_order_form()
 * @see uc_payment_by_order_form_submit()
 */
function uc_payment_by_order_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['payments']['new']['amount'])) {
    form_set_error('payments][new][amount', t('You must enter a number for the amount.'));
  }
  return TRUE;
}

/**
 * Form submission handler for uc_payment_by_order_form().
 *
 * @see uc_payment_by_order_form()
 * @see uc_payment_by_order_form_validate()
 */
function uc_payment_by_order_form_submit($form, &$form_state) {
  global $user;

  $payment = $form_state['values']['payments']['new'];
  $received = strtotime($payment['received']['year'] . '-' . $payment['received']['month'] . '-' . $payment['received']['day'] . ' 00:00:00');

  // If the value entered is today, use the exact timestamp instead
  $startofday = mktime(0, 0, 0);

  if ($received == $startofday) {
    $received = mktime();
  }

  uc_payment_enter($form_state['values']['order_id'], $payment['method'], $payment['amount'], $user->uid, '', $payment['comment'], $received);

  drupal_set_message(t('Payment entered.'));
}

/**
 * Confirmation form to delete a payment from an order.
 *
 * @see uc_payment_delete_confirm_form_submit()
 * @ingroup forms
 */
function uc_payment_delete_confirm_form($form_state, $order, $payment) {
  // Make sure the payment is for the specified order.
  if ($payment->order_id != $order->order_id) {
    drupal_set_message(t('An error loading the payment information occurred.'));
    drupal_goto('admin/store/orders/'. $order->order_id .'/payments');
  }

  $context = array(
    'revision' => 'formatted',
    'type' => 'payment',
    'subject' => array(
      'payment' => $payment,
    ),
  );

  $desc = '<strong>'. t('Payment information:') .'</strong> '
        . t('@method payment of @amount received on @date.', array('@method' => $payment->method, '@amount' => uc_price($payment->amount, $context), '@date' => format_date($payment->received, 'short')));

  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => $order->order_id
  );
  $form['receipt_id'] = array(
    '#type' => 'value',
    '#value' => $payment->receipt_id,
  );

  return confirm_form($form, t('Are you sure you want to delete this payment?'), 'admin/store/orders/'. $order->order_id .'/payments', $desc, t('Delete'));
}

/**
 * Form submission handler for uc_payment_delete_confirm_form().
 *
 * @see uc_payment_delete_confirm_form()
 */
function uc_payment_delete_confirm_form_submit($form, &$form_state) {
  uc_payment_delete($form_state['values']['receipt_id']);

  drupal_set_message(t('Payment deleted.'));

  $form_state['redirect'] = 'admin/store/orders/'. $form_state['values']['order_id'] .'/payments';
}
