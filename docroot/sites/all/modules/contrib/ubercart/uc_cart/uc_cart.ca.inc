<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to
 * make the cart related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implements hook_ca_condition().
 */
function uc_cart_ca_condition() {
  $conditions['uc_cart_condition_product_class'] = array(
    '#title' => t('Order has a product of a particular class'),
    '#category' => t('Order: Product'),
    '#callback' => 'uc_cart_condition_product_class',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_ca_trigger().
 */
function uc_cart_ca_trigger() {
  $triggers['uc_checkout_complete'] = array(
    '#title' => t('Customer completes checkout'),
    '#category' => t('Cart'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'account' => array(
        '#entity' => 'user',
        '#title' => t('Customer user account'),
      )
    ),
  );

  return $triggers;
}

/**
 * Implements hook_ca_predicate().
 */
function uc_cart_ca_predicate() {
  $predicates = array();

  // Setup a default predicate for customer checkout notifications.
  $predicates['uc_checkout_customer_notification'] = array(
    '#title' => t('E-mail customer checkout notification'),
    '#description' => t('E-mail the customer an invoice from their recent order.'),
    '#class' => 'notification',
    '#status' => 1,
    '#trigger' => 'uc_checkout_complete',
    '#actions' => array(
      array(
        '#name' => 'uc_order_email_invoice',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email-raw]',
          'subject' => t('Your Order at [store-name]'),
          'template' => 'customer',
          'view' => 'checkout-mail',
        ),
      ),
    ),
  );

  // Setup a default predicate for admin checkout notifications.
  $predicates['uc_checkout_admin_notification'] = array(
    '#title' => t('E-mail admin checkout notification'),
    '#description' => t('E-mail a short order summary to an administrator when a customer checks out.'),
    '#class' => 'notification',
    '#status' => 1,
    '#trigger' => 'uc_checkout_complete',
    '#actions' => array(
      array(
        '#name' => 'uc_order_email_invoice',
        '#title' => t('Send an e-mail to the administrator(s)'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => uc_store_email(),
          'subject' => t('New Order at [store-name]'),
          'template' => 'admin',
          'view' => 'admin-mail',
        ),
      ),
    ),
  );

  return $predicates;
}

/******************************************************************************
 * Condition Callbacks and Forms                                              *
 ******************************************************************************/

/**
 * Check that an order has a product of the selected class.
 *
 * @see uc_cart_condition_product_class_form()
 */
function uc_cart_condition_product_class($order, $settings) {
  $result = FALSE;
  $types = array();
  foreach ($order->products as $product) {
    // Ignore "blank line" custom products.
    if ($product->nid) {
      // Cache product types to avoid extra queries.
      if (!isset($types[$product->nid])) {
        if (isset($product->type)) {
          $types[$product->nid] = $product->type;
        }
        else {
          $types[$product->nid] = db_result(db_query("SELECT type FROM {node} WHERE nid = %d", $product->nid));
        }
      }
      if ($types[$product->nid] == $settings['class']) {
        $result = TRUE;
        break;
      }
    }
  }

  return $result;
}

/**
 * @see uc_cart_condition_product_class()
 */
function uc_cart_condition_product_class_form($form_state, $settings = array()) {
  $form = array();

  $form['class'] = array(
    '#type' => 'select',
    '#title' => t('Product class'),
    '#options' => uc_product_type_names(),
    '#default_value' => $settings['class'],
  );

  return $form;
}
