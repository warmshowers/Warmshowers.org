<?php

/**
 * @file
 * Checkout pane functions for uc_payment.module.
 *
 * The checkout pane holds form to select the payment method. It also shows a
 * preview of the line items and order total.
 */

function uc_checkout_pane_payment($op, &$order, $arg2) {
  switch ($op) {
    case 'view':
      // Add the default payment message as a JS variable.
      // Add URL paths as JS variables (be nice to i18n, allow URL rewrite).
      drupal_add_js(array(
        'defPaymentMsg' => addslashes(variable_get('uc_default_payment_msg', t('Continue with checkout to complete payment.'))),
        'ucURL' => array(
          'adminOrders' => url('admin/store/orders/'),
          'checkoutPaymentDetails' => url('cart/checkout/payment_details/'),
          'checkoutLineItems' => url('cart/checkout/line_items'),
          'creditCardCVVInfo' => url('cart/checkout/credit/cvv_info'),
        ),
      ), 'setting');

      if (variable_get('uc_payment_show_order_total_preview', TRUE)) {
        drupal_add_js('misc/progress.js');

        $contents['current_total'] = array(
          '#type' => 'hidden',
          '#value' => $order->order_total > 0 ? $order->order_total : NULL,
        );
        $contents['shown_total'] = array(
          '#value' => '<div style="padding: .5em 1em; margin-bottom: 1em; border: dashed 1px #bbb;" id="line-items-div"><em>'. t('Javascript must be enabled to view the order total preview.') .'</em></div>',
          '#weight' => -20,
        );

        // let the script know we need to show a progressbar
        drupal_add_js(array('ucShowProgressBar' => TRUE), 'setting');
      }

      $methods = _payment_method_list();
      $options = array();
      $default = NULL;

      foreach ($methods as $method) {
        if ($method['checkout']) {
          $options[$method['id']] = $method['title'];
        }
      }

      if (count($options)) {
        if (isset($_POST['panes']) &&
            isset($_POST['panes']['payment']['payment_method']) &&
            in_array($_POST['panes']['payment']['payment_method'], array_keys($options))) {
          $default = $_POST['panes']['payment']['payment_method'];
        }
        else {
          $default = (count($options) == 1 || empty($order->payment_method)) ? key($options) : $order->payment_method;
        }
      }

      if (count($options) > 1) {
        $description = t('Select a payment method from the following options.');
      }
      else {
        $description = '';
      }

      $contents['payment_method'] = array(
        '#type' => 'radios',
        '#title' => t('Payment method'),
        '#options' => $options,
        '#default_value' => $default,
        '#disabled' => count($options) == 1 ? TRUE : FALSE,
        '#required' => TRUE,
        '#attributes' => array('onclick' => "get_payment_details(Drupal.settings.ucURL.checkoutPaymentDetails + this.value);"),
        '#theme' => 'uc_payment_method_select',
      );
      $contents['details'] = array(
        '#value' => '<div id="payment_details" class="solid-border display-none"></div>',
      );

      return array('description' => $description, 'contents' => $contents);

    case 'process':
      $order->payment_method = $arg2['payment_method'];
      $func = _payment_method_data($order->payment_method, 'callback');
      if (function_exists($func)) {
        $result = $func('cart-process', $order);
        if ($result === FALSE) {
          return FALSE;
        }
      }
      return TRUE;

    case 'review':
      $line_items = $order->line_items;
      $items = _line_item_list();
      foreach ($items as $item) {
        if (isset($item['display_only']) && $item['display_only'] == TRUE) {
          $result = $item['callback']('display', $order);
          if (is_array($result)) {
            foreach ($result as $line) {
              $line_items[] = array(
                'title' => $line['title'],
                'amount' => $line['amount'],
                'weight' => $item['weight']
              );
            }
          }
        }
      }
      usort($line_items, 'uc_weight_sort');

      $context = array(
        'revision' => 'themed',
        'type' => 'line_item',
        'subject' => array(
          'order' => $order,
        ),
      );
      foreach ($line_items as $line_item) {
        $context['subject']['line_item'] = $line_item;
        $review[] = array('title' => $line_item['title'], 'data' => uc_price($line_item['amount'], $context));
      }
      $review_data = _payment_method_data($order->payment_method, 'review');
      if (empty($review_data)) {
        $review_data = _payment_method_data($order->payment_method, 'name');
      }
      $review[] = array('border' => 'top', 'title' => t('Paying by'), 'data' => $review_data);
      $func = _payment_method_data($order->payment_method, 'callback');
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
 * We need a theme function for the radios element in case another module alters
 * the default or available payment methods... we need the JS to grab the right
 * default payment details.
 */
function theme_uc_payment_method_select($form) {
  if (empty($form['#options'])) {
    drupal_set_message(t('Checkout cannot be completed without any payment methods enabled. Please contact an administrator to resolve the issue.'), 'error');
    return;
  }

  // Perhaps instead this should be a normal JS function and we just print the
  // default payment method to a JS variable or use a selector.  -RS
  drupal_add_js(array('ucDefaultPayment' => $form['#default_value']), 'setting');

  return drupal_render($form);
}
