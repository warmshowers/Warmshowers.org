<?php

/**
 * @file
 * Default views for uc_views.module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function uc_views_views_default_views() {
  $view = new view;
  $view->name = 'orders';
  $view->description = 'Order listing';
  $view->tag = '';
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
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => 'in',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
       'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'Username',
        'optional' => 1,
        'remember' => 1,
        'reduce' => 0,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
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
  $handler->override_option('title', 'Orders');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'order_id' => 'order_id',
      'name' => 'name',
      'primary_email' => 'name',
      'created' => 'created',
      'order_total' => 'order_total',
      'country_name' => 'name',
    ),
    'info' => array(
      'order_id' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 0,
        'separator' => '<br/>',
      ),
      'primary_email' => array(
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'order_total' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'country_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'order_id',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'orders');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  if (module_exists('draggableviews')) {
    $view = new view;
    $view->name = 'product_order';
    $view->description = '';
    $view->tag = 'Ubercart';
    $view->view_php = '';
    $view->base_table = 'node';
    $view->is_cacheable = FALSE;
    $view->api_version = 2;
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->override_option('fields', array(
      'title' => array(
        'label' => 'Title',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 1,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
      ),
      'sell_price' => array(
        'label' => 'Sell price',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'set_precision' => 0,
        'precision' => '0',
        'decimal' => '.',
        'separator' => ',',
        'prefix' => '',
        'suffix' => '',
        'format' => 'uc_price',
        'revision' => 'themed',
        'exclude' => 0,
        'id' => 'sell_price',
        'table' => 'uc_products',
        'field' => 'sell_price',
        'relationship' => 'none',
      ),
      'ordering' => array(
        'label' => 'List position',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'set_precision' => FALSE,
        'precision' => 0,
        'decimal' => '.',
        'separator' => ',',
        'prefix' => '',
        'suffix' => '',
        'exclude' => 0,
        'id' => 'ordering',
        'table' => 'uc_products',
        'field' => 'ordering',
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('sorts', array(
      'ordering' => array(
        'order' => 'ASC',
        'id' => 'ordering',
        'table' => 'uc_products',
        'field' => 'ordering',
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('filters', array(
      'is_product' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'is_product',
        'table' => 'uc_products',
        'field' => 'is_product',
        'relationship' => 'none',
      ),
      'status' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'status',
        'table' => 'node',
        'field' => 'status',
        'relationship' => 'none',
      ),
      'tid' => array(
        'operator' => 'or',
        'value' => array(),
        'group' => '0',
        'exposed' => TRUE,
        'expose' => array(
          'use_operator' => 0,
          'operator' => 'tid_op',
          'identifier' => 'tid',
          'label' => 'Taxonomy term',
          'optional' => 1,
          'single' => 1,
          'remember' => 0,
          'reduce' => 0,
        ),
        'type' => 'select',
        'limit' => TRUE,
        'vid' => variable_get('uc_catalog_vid', 0),
        'id' => 'tid',
        'table' => 'term_node',
        'field' => 'tid',
        'hierarchy' => 1,
        'relationship' => 'none',
        'reduce_duplicates' => 0,
      ),
    ));
    $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'administer products',
    ));
    $handler->override_option('cache', array(
      'type' => 'none',
    ));
    $handler->override_option('title', 'Product display order');
    $handler->override_option('items_per_page', 30);
    $handler->override_option('use_pager', '1');
    $handler->override_option('style_plugin', 'draggabletable');
    $handler->override_option('style_options', array(
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'sell_price' => 'sell_price',
        'ordering' => 'ordering',
      ),
      'info' => array(
        'title' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'sell_price' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'ordering' => array(
          'separator' => '',
        ),
      ),
      'default' => -1,
      'tabledrag_order' => array(
        'field' => 'ordering',
        'handler' => 'ubercart',
      ),
      'tabledrag_order_visible' => array(
        'visible' => 0,
      ),
      'tabledrag_hierarchy' => array(
        'field' => 'none',
        'handler' => 'native',
      ),
      'tabledrag_hierarchy_visible' => array(
        'visible' => 0,
      ),
      'draggableviews_depth_limit' => '0',
      'draggableviews_repair' => array(
        'repair' => 0,
      ),
      'tabledrag_types_add' => 'Add type',
      'tabledrag_expand' => array(
        'expand_links' => 0,
        'collapsed' => 0,
        'by_uid' => 0,
      ),
      'draggableviews_extensions' => array(
        'extension_top' => '3',
        'extension_bottom' => '3',
      ),
      'tabledrag_lock' => array(
        'lock' => 0,
      ),
      'draggableviews_default_on_top' => '1',
      'draggableviews_button_text' => 'Save order',
      'draggableviews_arguments' => array(
        'use_args' => 0,
      ),
    ));
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->override_option('path', 'admin/store/products/order');
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));

    $views[$view->name] = $view;
  }

  return $views;
}