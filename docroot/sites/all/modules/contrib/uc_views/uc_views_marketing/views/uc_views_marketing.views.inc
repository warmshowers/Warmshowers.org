<?php
/**
 * @file
 * Views 2 hooks and callback registries.
 */

/**
 * Implementation of hook_views_data().
 */
function uc_views_marketing_views_data() {

  //Make the uc_order_products_qty_vw view visible for Views module
  $data['uc_order_products_qty_vw']['table']['group'] = t('Ubercart order product');
  $data['uc_order_products_qty_vw']['table']['join']['node'] = array(
      'left_field' => 'nid',
      'field' => 'nid',
  );
  $data['uc_order_products_qty_vw']['order_count'] = array(
    'title' => t('order_count'),
    'help' => t('The number of orderlines with the product.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_qty_vw']['avg_qty'] = array(
    'title' => t('avg_qty'),
    'help' => t('The average amount ordered.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['uc_order_products_qty_vw']['sum_qty'] = array(
    'title' => t('sum_qty'),
    'help' => t('The summed amount of all orders.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['uc_order_products_qty_vw']['max_qty'] = array(
    'title' => t('max_qty'),
    'help' => t('The maximum amount ordered.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_qty_vw']['min_qty'] = array(
    'title' => t('min_qty'),
    'help' => t('The minimum amount ordered.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_qty_vw']['ordering'] = array(
    'title' => t('List position'),
    'help' => t('The default sort criteria in the catalog.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //Make the uc_order_products_user_vw view visible for Views module
  $data['uc_order_products_user_vw']['table']['group'] = t('Ubercart order product');
  $data['uc_order_products_user_vw']['table']['join']['node'] = array(
      'left_field' => 'nid',
      'field' => 'nid',
  );
  $data['uc_order_products_user_vw']['table']['join']['users'] = array(
      'left_field' => 'uid',
      'field' => 'uid',
  );
  $data['uc_order_products_user_vw']['uid'] = array(
    'title' => t('user_id'),
    'help' => t('The id of the user.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
     'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'uid',
    ),
     'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_user_vw']['order_count'] = array(
    'title' => t('user_order_count'),
    'help' => t('The number of orderlines with the product for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_user_vw']['avg_qty'] = array(
    'title' => t('user_avg_qty'),
    'help' => t('The average amount ordered for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['uc_order_products_user_vw']['sum_qty'] = array(
    'title' => t('user_sum_qty'),
    'help' => t('The summed amount of all orders for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['uc_order_products_user_vw']['max_qty'] = array(
    'title' => t('user_max_qty'),
    'help' => t('The maximum amount ordered for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
  );
  $data['uc_order_products_user_vw']['min_qty'] = array(
    'title' => t('user_min_qty'),
    'help' => t('The minimum amount ordered for the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_user_vw']['ordering'] = array(
    'title' => t('List position'),
    'help' => t('The default sort criteria in the catalog.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  //Make the uc_order_products_pair_vw view visible for Views module
  $data['uc_order_products_pair_vw']['table']['group'] = t('Ubercart order product');
  $data['uc_order_products_pair_vw']['table']['join']['node'] = array(
      'left_field' => 'nid',
      'field' => 'pair_nid',
  );
  $data['uc_order_products_pair_vw']['nid'] = array(
    'title' => t('pairs_product_nid'),
    'help' => t('The nid of the product for which we are seeking pairs.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
     'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
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
  $data['uc_order_products_pair_vw']['pair_nid'] = array(
    'title' => t('pairs_product_pair_nid'),
    'help' => t('The nid of the pair product.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
     'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_pair_vw']['order_count'] = array(
    'title' => t('pairs_order_count'),
    'help' => t('The number of orders in which the product pair has occured along with the product with nid.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_pair_vw']['pair_sum_qty'] = array(
    'title' => t('pairs_sum_qty'),
    'help' => t('The summed qty purchased on orders which also include the product with nid.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['uc_order_products_pair_vw']['ordering'] = array(
    'title' => t('List position'),
    'help' => t('The default sort criteria in the catalog.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
