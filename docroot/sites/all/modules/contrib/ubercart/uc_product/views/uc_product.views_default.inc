<?php

/**
 * @file
 * Default views for uc_product.module.
 */

/**
 * Implements hook_views_default_views().
 */
function uc_product_views_default_views() {
  $view = new view;
  $view->name = 'uc_products';
  $view->description = 'List of products.';
  $view->tag = 'Ubercart';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to TRUE to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');

  $fields = array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'sell_price' => array(
      'label' => 'Sell price',
      'exclude' => 0,
      'id' => 'sell_price',
      'table' => 'uc_products',
      'field' => 'sell_price',
      'relationship' => 'none',
    ),
    'buyitnowbutton' => array(
      'label' => 'Buy it now button',
      'exclude' => 0,
      'id' => 'buyitnowbutton',
      'table' => 'uc_products',
      'field' => 'buyitnowbutton',
      'relationship' => 'none',
    ),
  );

  if ($field = variable_get('uc_image_product', '')) {
    $fields[$field .'_fid'] = array(
      'label' => '',
      'link_to_node' => 1,
      'label_type' => 'widget',
      'format' => 'product_list_linked',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '1',
        'multiple_from' => '0',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => $field .'_fid',
      'table' => 'node_data_'. $field,
      'field' => $field .'_fid',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    );
  }
  $handler->override_option('fields', $fields);
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
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
    'is_product' => array(
      'operator' => '=',
      'value' => 1,
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
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('title', 'Products');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'sell_price' => 'sell_price',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'sell_price' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'title',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'products');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));

  $views[$view->name] = $view;

  return $views;
}
