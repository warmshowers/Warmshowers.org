<?php
/**
 * @file
 * order_vbo_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function order_vbo_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'order_vbo';
  $view->description = 'A view containing all custom order and Ubercart VBO options.';
  $view->tag = '';
  $view->base_table = 'uc_orders';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Orders VBO';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 50;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'counter' => 'counter',
    'order_id' => 'order_id',
    'uid' => 'uid',
    'created' => 'created',
    'primary_email' => 'primary_email',
    'modified' => 'modified',
    'order_status' => 'order_status',
    'order_total' => 'order_total',
    'created_1' => 'created_1',
  );
  $handler->display->display_options['style_options']['default'] = 'modified';
  $handler->display->display_options['style_options']['info'] = array(
    'counter' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'primary_email' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'modified' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  /* Field: Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 1;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Order: Creation date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Order Creation';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Order: Email address */
  $handler->display->display_options['fields']['primary_email']['id'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['primary_email']['field'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['link_to_user'] = 'user';
  /* Field: Order: Last modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['label'] = 'Order Mod';
  $handler->display->display_options['fields']['modified']['date_format'] = 'short';
  $handler->display->display_options['fields']['modified']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Order: Order status */
  $handler->display->display_options['fields']['order_status']['id'] = 'order_status';
  $handler->display->display_options['fields']['order_status']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_status']['field'] = 'order_status';
  $handler->display->display_options['fields']['order_status']['label'] = 'Status';
  /* Field: Order: Order total */
  $handler->display->display_options['fields']['order_total']['id'] = 'order_total';
  $handler->display->display_options['fields']['order_total']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_total']['field'] = 'order_total';
  $handler->display->display_options['fields']['order_total']['label'] = 'Total';
  $handler->display->display_options['fields']['order_total']['precision'] = '0';
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'users';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['created_1']['label'] = 'User creation date';
  /* Filter criterion: Order: Order status */
  $handler->display->display_options['filters']['order_status']['id'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['order_status']['field'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['group'] = '0';
  $handler->display->display_options['filters']['order_status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_status']['expose']['operator_id'] = 'order_status_op';
  $handler->display->display_options['filters']['order_status']['expose']['label'] = 'Order Status';
  $handler->display->display_options['filters']['order_status']['expose']['operator'] = 'order_status_op';
  $handler->display->display_options['filters']['order_status']['expose']['identifier'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['expose']['remember'] = TRUE;
  /* Filter criterion: Order: Order total */
  $handler->display->display_options['filters']['order_total']['id'] = 'order_total';
  $handler->display->display_options['filters']['order_total']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['order_total']['field'] = 'order_total';
  $handler->display->display_options['filters']['order_total']['group'] = '0';
  $handler->display->display_options['filters']['order_total']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_total']['expose']['operator_id'] = 'order_total_op';
  $handler->display->display_options['filters']['order_total']['expose']['label'] = 'Order Total';
  $handler->display->display_options['filters']['order_total']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['order_total']['expose']['operator'] = 'order_total_op';
  $handler->display->display_options['filters']['order_total']['expose']['identifier'] = 'order_total';
  /* Filter criterion: Order: Last modified */
  $handler->display->display_options['filters']['modified']['id'] = 'modified';
  $handler->display->display_options['filters']['modified']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['modified']['field'] = 'modified';
  $handler->display->display_options['filters']['modified']['operator'] = 'between';
  $handler->display->display_options['filters']['modified']['value']['value'] = '2015-01-01';
  $handler->display->display_options['filters']['modified']['group'] = '0';
  $handler->display->display_options['filters']['modified']['exposed'] = TRUE;
  $handler->display->display_options['filters']['modified']['expose']['operator_id'] = 'modified_op';
  $handler->display->display_options['filters']['modified']['expose']['label'] = 'Order date';
  $handler->display->display_options['filters']['modified']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['modified']['expose']['operator'] = 'modified_op';
  $handler->display->display_options['filters']['modified']['expose']['identifier'] = 'modified';
  /* Filter criterion: Order: Order ID */
  $handler->display->display_options['filters']['order_id']['id'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['order_id']['field'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['operator'] = 'between';
  $handler->display->display_options['filters']['order_id']['group'] = '0';
  $handler->display->display_options['filters']['order_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_id']['expose']['operator_id'] = 'order_id_op';
  $handler->display->display_options['filters']['order_id']['expose']['label'] = 'Order ID';
  $handler->display->display_options['filters']['order_id']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['order_id']['expose']['operator'] = 'order_id_op';
  $handler->display->display_options['filters']['order_id']['expose']['identifier'] = 'order_id';
  /* Filter criterion: Order: Payment method */
  $handler->display->display_options['filters']['payment_method']['id'] = 'payment_method';
  $handler->display->display_options['filters']['payment_method']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['payment_method']['field'] = 'payment_method';
  $handler->display->display_options['filters']['payment_method']['group'] = '0';
  $handler->display->display_options['filters']['payment_method']['exposed'] = TRUE;
  $handler->display->display_options['filters']['payment_method']['expose']['operator_id'] = 'payment_method_op';
  $handler->display->display_options['filters']['payment_method']['expose']['label'] = 'Payment method';
  $handler->display->display_options['filters']['payment_method']['expose']['operator'] = 'payment_method_op';
  $handler->display->display_options['filters']['payment_method']['expose']['identifier'] = 'payment_method';

  /* Display: Order VBO Page */
  $handler = $view->new_display('page', 'Order VBO Page', 'orders_vbo');
  $handler->display->display_options['path'] = 'admin/store/orders/orders_vbo';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Orders VBO';
  $handler->display->display_options['menu']['weight'] = '-20';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Order Comments */
  $handler = $view->new_display('page', 'Order Comments', 'donation_comments');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Donation Comments';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    26 => '26',
  );
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'uid' => 'uid',
    'created' => 'created',
    'primary_email' => 'primary_email',
    'modified' => 'modified',
    'fullname' => 'fullname',
    'order_total' => 'order_total',
    'order_id' => 'order_id',
    'order_status' => 'order_status',
    'created_1' => 'created_1',
    'name' => 'name',
    'city' => 'city',
    'province' => 'province',
    'country' => 'country',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = 'modified';
  $handler->display->display_options['style_options']['info'] = array(
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'primary_email' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'modified' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'fullname' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'city' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'province' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'country' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  /* Field: Order: Creation date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Donation date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d';
  /* Field: Order: Email address */
  $handler->display->display_options['fields']['primary_email']['id'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['primary_email']['field'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['exclude'] = TRUE;
  $handler->display->display_options['fields']['primary_email']['link_to_user'] = 'user';
  /* Field: Order: Last modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['label'] = 'Modification date';
  $handler->display->display_options['fields']['modified']['exclude'] = TRUE;
  /* Field: Order: Order total */
  $handler->display->display_options['fields']['order_total']['id'] = 'order_total';
  $handler->display->display_options['fields']['order_total']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_total']['field'] = 'order_total';
  $handler->display->display_options['fields']['order_total']['label'] = 'Donation';
  $handler->display->display_options['fields']['order_total']['precision'] = '0';
  /* Field: Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'Donation ID';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 1;
  /* Field: Order: Order status */
  $handler->display->display_options['fields']['order_status']['id'] = 'order_status';
  $handler->display->display_options['fields']['order_status']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_status']['field'] = 'order_status';
  $handler->display->display_options['fields']['order_status']['label'] = 'Status';
  $handler->display->display_options['fields']['order_status']['exclude'] = TRUE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'users';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['created_1']['label'] = 'Member join date';
  $handler->display->display_options['fields']['created_1']['date_format'] = 'time ago';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  /* Field: Order comments: Comment */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'uc_order_comments';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['hide_empty'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Order: Order status */
  $handler->display->display_options['filters']['order_status']['id'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['order_status']['field'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['value'] = array(
    'payment_received' => 'payment_received',
    'completed' => 'completed',
  );
  $handler->display->display_options['filters']['order_status']['group'] = '0';
  $handler->display->display_options['filters']['order_status']['expose']['label'] = 'Order Status';
  $handler->display->display_options['filters']['order_status']['expose']['operator'] = 'order_status_op';
  $handler->display->display_options['filters']['order_status']['expose']['identifier'] = 'order_status';
  $handler->display->display_options['filters']['order_status']['expose']['remember'] = TRUE;
  /* Filter criterion: Order: Order total */
  $handler->display->display_options['filters']['order_total']['id'] = 'order_total';
  $handler->display->display_options['filters']['order_total']['table'] = 'uc_orders';
  $handler->display->display_options['filters']['order_total']['field'] = 'order_total';
  $handler->display->display_options['filters']['order_total']['group'] = '0';
  $handler->display->display_options['filters']['order_total']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_total']['expose']['operator_id'] = 'order_total_op';
  $handler->display->display_options['filters']['order_total']['expose']['label'] = 'Order Total';
  $handler->display->display_options['filters']['order_total']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['order_total']['expose']['operator'] = 'order_total_op';
  $handler->display->display_options['filters']['order_total']['expose']['identifier'] = 'order_total';
  $handler->display->display_options['path'] = 'donation_comments';
  $translatables['order_vbo'] = array(
    t('Defaults'),
    t('Orders VBO'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('View result counter'),
    t('Order ID'),
    t('Uid'),
    t('Order Creation'),
    t('Email address'),
    t('Order Mod'),
    t('Status'),
    t('Total'),
    t('.'),
    t(','),
    t('User creation date'),
    t('Order Status'),
    t('Order Total'),
    t('Order date'),
    t('Payment method'),
    t('Order VBO Page'),
    t('Order Comments'),
    t('Donation Comments'),
    t('Donation date'),
    t('Modification date'),
    t('Donation'),
    t('Donation ID'),
    t('Member join date'),
    t('Name'),
    t('Comment'),
  );
  $export['order_vbo'] = $view;

  return $export;
}
