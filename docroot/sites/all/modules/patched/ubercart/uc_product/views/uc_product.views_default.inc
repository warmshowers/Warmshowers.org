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
  $view->description = '';
  $view->tag = 'Ubercart';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Products';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer products';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uc_product_image' => 'uc_product_image',
    'title' => 'title',
    'display_price' => 'display_price',
    'changed' => 'changed',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'uc_product_image' => array(
      'align' => 'views-align-center',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'display_price' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  $handler->display->display_options['style_options']['order'] = 'desc';
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['uc_product_image']['id'] = 'uc_product_image';
  $handler->display->display_options['fields']['uc_product_image']['table'] = 'field_data_uc_product_image';
  $handler->display->display_options['fields']['uc_product_image']['field'] = 'uc_product_image';
  $handler->display->display_options['fields']['uc_product_image']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uc_product_image']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uc_product_image']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['uc_product_image']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uc_product_image']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['uc_product_image']['settings'] = array(
    'image_style' => 'uc_product_list',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['uc_product_image']['group_rows'] = 1;
  $handler->display->display_options['fields']['uc_product_image']['delta_limit'] = '1';
  $handler->display->display_options['fields']['uc_product_image']['delta_offset'] = '0';
  $handler->display->display_options['fields']['uc_product_image']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['uc_product_image']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Product: Display price */
  $handler->display->display_options['fields']['display_price']['id'] = 'display_price';
  $handler->display->display_options['fields']['display_price']['table'] = 'uc_products';
  $handler->display->display_options['fields']['display_price']['field'] = 'display_price';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['changed']['date_format'] = 'uc_store';
  $handler->display->display_options['path'] = 'products';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Node: Is a product */
  $handler->display->display_options['filters']['is_product']['id'] = 'is_product';
  $handler->display->display_options['filters']['is_product']['table'] = 'uc_products';
  $handler->display->display_options['filters']['is_product']['field'] = 'is_product';
  $handler->display->display_options['filters']['is_product']['value'] = 1;
  $handler->display->display_options['filters']['is_product']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Admin page', 'admin_page');
  $handler->display->display_options['path'] = 'admin/store/products/view';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'View products';
  $handler->display->display_options['menu']['description'] = 'View and search products available through your website.';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['menu']['name'] = 'management';

  $views[$view->name] = $view;

  return $views;
}
