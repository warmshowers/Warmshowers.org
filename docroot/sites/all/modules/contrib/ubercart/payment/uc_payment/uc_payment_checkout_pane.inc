<?php

/**
 * @file
 * Checkout pane functions for uc_payment.module.
 *
 * The checkout pane holds form to select the payment method. It also shows a
 * preview of the line items and order total.
 */

function uc_checkout_pane_payment($op, &$order, $form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'view':
      $contents['#attached']['css'][] = drupal_get_path('module', 'uc_payment') . '/uc_payment.css';

      if (variable_get('uc_payment_show_order_total_preview', TRUE)) {
        $contents['line_items'] = array(
          '#theme' => 'uc_payment_totals',
          '#order' => $order,
          '#prefix' => '<div id="line-items-div">',
          '#suffix' => '</div>',
          '#weight' => -20,
        );
      }

      // Ensure that the form builder uses #default_value to determine which
      // button should be selected after an ajax submission. This is
      // necessary because the previously selected value may have become
      // unavailable, which would result in an invalid selection.
      unset($form_state['input']['panes']['payment']['payment_method']);

      $options = array();
      foreach (_uc_payment_method_list() as $id => $method) {
        $set = rules_config_load('uc_payment_method_' . $method['id']);
        if ($set && !$set->execute($order)) {
          continue;
        }

        if ($method['checkout'] && !isset($method['express'])) {
          $options[$id] = $method['title'];
        }
      }

      drupal_alter('uc_payment_method_checkout', $options, $order);

      $description = '';
      if (!$options) {
        $description = t('Checkout cannot be completed without any payment methods enabled. Please contact an administrator to resolve the issue.');
        $options[''] = t('No payment methods available');
      }
      elseif (count($options) > 1) {
        $description = t('Select a payment method from the following options.');
      }

      if (!isset($options[$order->payment_method])) {
        $order->payment_method = key($options);
      }

      $contents['payment_method'] = array(
        '#type' => 'radios',
        '#title' => t('Payment method'),
        '#title_display' => 'invisible',
        '#options' => $options,
        '#default_value' => $order->payment_method,
        '#disabled' => count($options) == 1,
        '#required' => TRUE,
        '#ajax' => array(
          'callback' => 'uc_payment_checkout_payment_details',
          'wrapper' => 'payment-details',
          'progress' => array(
            'type' => 'throbber',
          ),
        ),
      );

      $contents['details'] = array(
        '#prefix' => '<div id="payment-details" class="clearfix payment-details-' . $order->payment_method . '">',
        '#markup' => t('Continue with checkout to complete payment.'),
        '#suffix' => '</div>',
      );

      $func = _uc_payment_method_data($order->payment_method, 'callback');
      if (function_exists($func) && $details = $func('cart-details', $order, $form, $form_state)) {
        unset($contents['details']['#markup']);
        $contents['details'] += $details;
      }

      return array('description' => $description, 'contents' => $contents);

    case 'process':
      if (empty($form_state['values']['panes']['payment']['payment_method'])) {
        form_set_error('panes][payment][payment_method', t('You cannot check out without selecting a payment method.'));
        return FALSE;
      }
      $order->payment_method = $form_state['values']['panes']['payment']['payment_method'];
      $func = _uc_payment_method_data($order->payment_method, 'callback');
      if (function_exists($func)) {
        $result = $func('cart-process', $order, $form, $form_state);
        if ($result === FALSE) {
          return FALSE;
        }
      }
      return TRUE;

    case 'review':
      $line_items = uc_order_load_line_items_display($order);
      foreach ($line_items as $line_item) {
        $review[] = array('title' => $line_item['title'], 'data' => theme('uc_price', array('price' => $line_item['amount'])));
      }
      $review_data = _uc_payment_method_data($order->payment_method, 'review');
      if (empty($review_data)) {
        $review_data = _uc_payment_method_data($order->payment_method, 'name');
      }
      $review[] = array('border' => 'top', 'title' => t('Paying by'), 'data' => $review_data);
      $func = _uc_payment_method_data($order->payment_method, 'callback');
      if (function_exists($func)) {
        $result = $func('cart-review', $order);
        if (is_array($result)) {
          $review = array_merge($review, $result);
        }
      }
      return $review;

    case 'settings':
      $form['uc_payment_show_order_total_preview'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show the order total preview on the payment pane.'),
        '#default_value' => variable_get('uc_payment_show_order_total_preview', TRUE),
      );
      return $form;
  }
}

/**
 * AJAX callback for payment method details on the checkout form.
 */
function uc_payment_checkout_payment_details($form, $form_state) {
  return $form['panes']['payment']['details'];
}
