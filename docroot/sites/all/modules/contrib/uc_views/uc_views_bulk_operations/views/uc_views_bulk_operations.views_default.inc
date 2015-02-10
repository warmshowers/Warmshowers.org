<?php

/**
 * @file
 * Default views for uc_views_bulk_operations.module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function uc_views_bulk_operations_views_default_views() {
  $view = new view;
  $view->name = 'order_management';
  $view->description = 'Order lmanagement';
  $view->tag = 'order management';
  $view->view_php = '';
  $view->base_table = 'uc_orders';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to false to make a default view enabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'order_id' => array(
      'label' => 'Order id',
      'link_to_order' => 1,
      'exclude' => 0,
      'id' => 'order_id',
      'table' => 'uc_orders',
      'field' => 'order_id',
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => 'Name',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'primary_email' => array(
      'label' => 'Email',
      'exclude' => 0,
      'id' => 'primary_email',
      'table' => 'uc_orders',
      'field' => 'primary_email',
      'relationship' => 'none',
    ),
    'country_name' => array(
      'label' => 'Delivery country',
      'exclude' => 0,
      'id' => 'country_name',
      'table' => 'delivery_countries',
      'field' => 'country_name',
      'relationship' => 'none',
    ),
    'created' => array(
      'label' => 'Date',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'uc_orders',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'order_total' => array(
      'label' => 'Total',
      'exclude' => 0,
      'id' => 'order_total',
      'table' => 'uc_orders',
      'field' => 'order_total',
      'relationship' => 'none',
    ),
    'order_status' => array(
      'label' => 'Status',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'order_status',
      'table' => 'uc_orders',
      'field' => 'order_status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'order_id' => array(
      'operator' => '=',
      'value' => array(
        'value' => '',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'order_id_op',
        'identifier' => 'order_id',
        'label' => 'Ubercart order id',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'order_id',
      'table' => 'uc_orders',
      'field' => 'order_id',
      'relationship' => 'none',
    ),
    'order_status' => array(
      'operator' => 'in',
      'value' => array(
        'canceled' => 'canceled',
        'in_checkout' => 'in_checkout',
        'pending' => 'pending',
        'processing' => 'processing',
        'payment_received' => 'payment_received',
        'completed' => 'completed',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'order_status_op',
        'identifier' => 'order_status',
        'label' => 'Order Status',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 1,
      ),
      'id' => 'order_status',
      'table' => 'uc_orders',
      'field' => 'order_status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view all orders',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Order management');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'bulk');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'order_id' => 'order_id',
      'name' => 'name',
      'primary_email' => 'primary_email',
      'country_name' => 'country_name',
      'created' => 'created',
      'order_total' => 'order_total',
      'order_status' => 'order_status',
    ),
    'info' => array(
      'order_id' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'primary_email' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'country_name' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'order_total' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'order_status' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
    'execution_type' => '1',
    'display_type' => '1',
    'skip_confirmation' => 0,
    'display_result' => 1,
    'merge_single_action' => 1,
    'selected_operations' => array(
      '312fec933a99b4250aa7fd2b514c94c8' => '312fec933a99b4250aa7fd2b514c94c8',
      'b8d136bb8135688dace81dc0fbc9ac1a' => 'b8d136bb8135688dace81dc0fbc9ac1a',
      '132fddcb3f367a243bee632db31985c1' => 0,
      'feb13e750bd2575b1f36109233087905' => 0,
      '334d20af1ae7ac4b770b7ec3210b2638' => 0,
      '246fdc2a4672eb371d05b48b2a7cb51e' => 0,
      '52aec7fee2070ce530da1304653ae1ec' => 0,
      'daa75f478e3093ab107e657da6620a91' => 0,
      '2178a36c0b51f3a7ea1d854780e33cc5' => 0,
    ),
  ));
  $handler = $view->new_display('page', 'Page', 'page_2');
  $handler->override_option('path', 'admin/store/order_management');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Order management',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  return $views;
}