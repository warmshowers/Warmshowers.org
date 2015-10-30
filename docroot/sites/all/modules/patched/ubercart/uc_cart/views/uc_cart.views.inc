<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data().
 */
function uc_cart_views_data() {
  // Cart items table.
  $data['uc_cart_products']['table']['group'] = t('Cart item');
  $data['uc_cart_products']['table']['base'] = array(
    'field' => 'cart_item_id',
    'title' => t('Cart items'),
    'help' => t('Products in customer carts.'),
  );

  // Pull in node fields directly.
  $data['node']['table']['join']['uc_cart_products'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['uc_cart_products']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of a product in the cart.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_cart_products']['cart_id'] = array(
    'title' => t('Cart ID'),
    'help' => t('The ID of the cart (user ID for authenticated users, session ID for anonymous users).'),
    'field' => array(
      'handler' => 'uc_cart_handler_field_cart_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
      'numeric' => TRUE,
      'validate type' => 'cart_id',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_cart_products']['qty'] = array(
    'title' => t('Quantity'),
    'help' => t('The quantity to be ordered.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  $data['uc_cart_products']['changed'] = array(
    'title' => t('Last modified'),
    'help' => t('The time the cart item was last modified'),
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
