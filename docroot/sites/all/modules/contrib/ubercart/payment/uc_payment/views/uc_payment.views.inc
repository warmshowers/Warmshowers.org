<?php

/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_data().
 */
function uc_payment_views_data() {
  $data['uc_orders']['payments'] = array(
    'relationship' => array(
      'title' => t('Payments'),
      'help' => t('Relate payments to an order. This relationship will create one record for each payment received.'),
      'handler' => 'views_handler_relationship',
      'base' => 'uc_payment_receipts',
      'base field' => 'order_id',
      'relationship field' => 'order_id',
      'label' => t('payments'),
    ),
  );

  $data['uc_payment_receipts']['table']['group'] = t('Payment');

  $data['uc_payment_receipts']['method'] = array(
    'title' => t('Payment method'),
    'help' => t('The method of payment.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['uc_payment_receipts']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The amount paid.'),
    'field' => array(
      'handler' => 'uc_order_handler_field_money_amount',
      'click sortable' => TRUE,
      'float' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_payment_receipts']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a payment to the user who made it.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('user'),
    ),
  );

  $data['uc_payment_receipts']['comment'] = array(
    'title' => t('Comment'),
    'help' => t('Any remarks that were included with the payment.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['uc_payment_receipts']['received'] = array(
    'title' => t('Receipt date'),
    'help' => t('The date and time the payment was received.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
