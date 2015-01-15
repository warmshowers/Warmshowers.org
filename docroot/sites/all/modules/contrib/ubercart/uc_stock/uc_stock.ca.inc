<?php

/**
 * @file
 * This file contains all the Workflow-NG hooks that are necessary for Workflow
 * integeration with the uc_stock module
 */

/******************************************************************************
 * Conditional Action Hooks                                                   *
 ******************************************************************************/

/**
 * Implements hook_ca_predicate().
 */
function uc_stock_ca_predicate() {
  $predicates['uc_stock_decrement_on_order'] = array(
    '#title' => t('Decrement stock upon order submission'),
    '#trigger' => 'uc_checkout_complete',
    '#class' => 'uc_stock',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_stock_action_decrement_stock',
        '#title' => t('Decrement stock of products in order'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  $predicates['uc_stock_increment_on_cancel'] = array(
    '#title' => t('Increment stock on cancelling order'),
    '#trigger' => 'uc_order_status_update',
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status is Cancelled.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'order_status' => 'canceled',
          ),
        ),
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status wasn\'t Canceled.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'order_status' => 'canceled',
            'negate' => TRUE,
          ),
        ),
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status wasn\'t In Checkout.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'order_status' => 'in_checkout',
            'negate' => TRUE,
          ),
        ),
      ),
    ),
    '#class' => 'uc_stock',
    '#status' => 0,
    '#actions' => array(
      array(
        '#name' => 'uc_stock_action_increment_stock',
        '#title' => t('Increment stock of products in order'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  $predicates['uc_stock_increment_on_delete'] = array(
    '#title' => t('Increment stock on deleting an order'),
    '#trigger' => 'uc_order_delete',
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status wasn\'t Canceled.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'order_status' => 'canceled',
            'negate' => TRUE,
          ),
        ),
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status wasn\'t In Checkout.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'order_status' => 'in_checkout',
            'negate' => TRUE,
          ),
        ),
      ),
    ),
    '#class' => 'uc_stock',
    '#status' => 0,
    '#actions' => array(
      array(
        '#name' => 'uc_stock_action_increment_stock',
        '#title' => t('Increment stock of products in order'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  $predicates['uc_stock_decrement_on_uncancel'] = array(
    '#title' => t('Decrement stock when order cancellation is being undone'),
    '#trigger' => 'uc_order_status_update',
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status was Canceled.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'order_status' => 'canceled',
          ),
        ),
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status isn\'t Canceled.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'order_status' => 'canceled',
            'negate' => TRUE,
          ),
        ),
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status isn\'t In Checkout.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'order_status' => 'in_checkout',
            'negate' => TRUE,
          ),
        ),
      ),
    ),
    '#class' => 'uc_stock',
    '#status' => 0,
    '#actions' => array(
      array(
        '#name' => 'uc_stock_action_decrement_stock',
        '#title' => t('Decrement stock of products in order'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implements hook_action().
 */
function uc_stock_ca_action() {
  $actions['uc_stock_action_decrement_stock'] = array(
    '#title' => t('Decrement stock of products on the order with tracking activated.'),
    '#callback' => 'uc_stock_action_decrement_stock',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
    '#category' => t('Stock'),
  );
  $actions['uc_stock_action_increment_stock'] = array(
    '#title' => t('Increment stock of products on the order with tracking activated.'),
    '#callback' => 'uc_stock_action_increment_stock',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
    '#category' => t('Stock'),
  );

  return $actions;
}

/******************************************************************************
 * Conditional Action Callbacks and Forms                                     *
 ******************************************************************************/

/**
 * Decreases the stock of ordered products.
 */
function uc_stock_action_decrement_stock($order, $settings) {
  if (is_array($order->products)) {
    array_walk($order->products, 'uc_stock_adjust_product_stock', $order);
  }
}

/**
 * Increase the stock of ordered products.
 */
function uc_stock_action_increment_stock($order, $settings) {
  if (is_array($order->products)) {
    array_walk($order->products, 'uc_stock_increment_product_stock', $order);
  }
}
