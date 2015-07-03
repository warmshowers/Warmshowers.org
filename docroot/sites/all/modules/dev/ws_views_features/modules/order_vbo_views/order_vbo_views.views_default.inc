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
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 50;
  /* Relationship: Order: Customer */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'uc_orders';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Order user';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Global: View result counter */
  $handler->display->display_options['fields']['counter']['id'] = 'counter';
  $handler->display->display_options['fields']['counter']['table'] = 'views';
  $handler->display->display_options['fields']['counter']['field'] = 'counter';
  /* Field: Order: Creation date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  /* Field: Order: Email address */
  $handler->display->display_options['fields']['primary_email']['id'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['primary_email']['field'] = 'primary_email';
  $handler->display->display_options['fields']['primary_email']['link_to_user'] = 'user';
  /* Field: Order: Last modified */
  $handler->display->display_options['fields']['modified']['id'] = 'modified';
  $handler->display->display_options['fields']['modified']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['modified']['field'] = 'modified';
  $handler->display->display_options['fields']['modified']['label'] = 'Modification date';
  /* Field: Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 1;
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
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'Uid';
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'users';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['created_1']['label'] = 'User creation date';
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['relationship'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Field: Warmshowers: City */
  $handler->display->display_options['fields']['city']['id'] = 'city';
  $handler->display->display_options['fields']['city']['table'] = 'user_location';
  $handler->display->display_options['fields']['city']['field'] = 'city';
  $handler->display->display_options['fields']['city']['relationship'] = 'uid';
  /* Field: Warmshowers: State or Province */
  $handler->display->display_options['fields']['province']['id'] = 'province';
  $handler->display->display_options['fields']['province']['table'] = 'user_location';
  $handler->display->display_options['fields']['province']['field'] = 'province';
  $handler->display->display_options['fields']['province']['relationship'] = 'uid';
  $handler->display->display_options['fields']['province']['label'] = 'State';
  /* Field: Warmshowers: Country */
  $handler->display->display_options['fields']['country']['id'] = 'country';
  $handler->display->display_options['fields']['country']['table'] = 'user_location';
  $handler->display->display_options['fields']['country']['field'] = 'country';
  $handler->display->display_options['fields']['country']['relationship'] = 'uid';
  /* Field: UC Recurring Fees: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'uc_recurring_users';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['relationship'] = 'uid';
  $handler->display->display_options['fields']['status']['label'] = 'Recurring Status';
  /* Field: UC Recurring Fees: Next Charge Date */
  $handler->display->display_options['fields']['next_charge']['id'] = 'next_charge';
  $handler->display->display_options['fields']['next_charge']['table'] = 'uc_recurring_users';
  $handler->display->display_options['fields']['next_charge']['field'] = 'next_charge';
  $handler->display->display_options['fields']['next_charge']['relationship'] = 'uid';
  $handler->display->display_options['fields']['next_charge']['label'] = 'Next Charge';
  /* Field: UC Recurring Fees: Product Fee ID */
  $handler->display->display_options['fields']['pfid']['id'] = 'pfid';
  $handler->display->display_options['fields']['pfid']['table'] = 'uc_recurring_users';
  $handler->display->display_options['fields']['pfid']['field'] = 'pfid';
  $handler->display->display_options['fields']['pfid']['relationship'] = 'uid';
  /* Field: UC Recurring Fees: Regular Interval */
  $handler->display->display_options['fields']['regular_interval']['id'] = 'regular_interval';
  $handler->display->display_options['fields']['regular_interval']['table'] = 'uc_recurring_users';
  $handler->display->display_options['fields']['regular_interval']['field'] = 'regular_interval';
  $handler->display->display_options['fields']['regular_interval']['relationship'] = 'uid';
  $handler->display->display_options['fields']['regular_interval']['label'] = 'Recurring Interval';
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
  /* Filter criterion: UC Recurring Fees: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'uc_recurring_users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Recurring';
  $handler->display->display_options['filters']['status']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  /* Filter criterion: User: Created date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'users';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['relationship'] = 'uid';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['value']['value'] = '2015-01-01';
  $handler->display->display_options['filters']['created']['group'] = '0';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'User Created date';
  $handler->display->display_options['filters']['created']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
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

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'orders_vbo');
  $handler->display->display_options['path'] = 'admin/store/orders/orders_vbo';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Orders VBO';
  $handler->display->display_options['menu']['weight'] = '-20';

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'orders_feed');
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['path'] = 'admin/store/orders_vbo/feed';
  $handler->display->display_options['displays'] = array(
    'orders_vbo' => 'orders_vbo',
    'default' => 0,
  );

  /* Display: Order Comments */
  $handler = $view->new_display('page', 'Order Comments', 'donation_comments');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Donation Comments';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    27 => 27,
    26 => 26,
  );
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'uc_orders';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'Uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
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
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['fullname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fullname']['alter']['path'] = 'user/[uid]';
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
  /* Field: Warmshowers: City */
  $handler->display->display_options['fields']['city']['id'] = 'city';
  $handler->display->display_options['fields']['city']['table'] = 'user_location';
  $handler->display->display_options['fields']['city']['field'] = 'city';
  $handler->display->display_options['fields']['city']['relationship'] = 'uid';
  /* Field: Warmshowers: State or Province */
  $handler->display->display_options['fields']['province']['id'] = 'province';
  $handler->display->display_options['fields']['province']['table'] = 'user_location';
  $handler->display->display_options['fields']['province']['field'] = 'province';
  $handler->display->display_options['fields']['province']['relationship'] = 'uid';
  $handler->display->display_options['fields']['province']['label'] = 'State';
  /* Field: Warmshowers: Country */
  $handler->display->display_options['fields']['country']['id'] = 'country';
  $handler->display->display_options['fields']['country']['table'] = 'user_location';
  $handler->display->display_options['fields']['country']['field'] = 'country';
  $handler->display->display_options['fields']['country']['relationship'] = 'uid';
  /* Field: Comments: Comment */
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
    t('Order user'),
    t('View result counter'),
    t('Creation date'),
    t('Email address'),
    t('Modification date'),
    t('Order ID'),
    t('Status'),
    t('Total'),
    t('.'),
    t(','),
    t('Uid'),
    t('User creation date'),
    t('Full Name'),
    t('Name'),
    t('City'),
    t('State'),
    t('Country'),
    t('Recurring Status'),
    t('Next Charge'),
    t('Product Fee ID'),
    t('Recurring Interval'),
    t('Order Status'),
    t('Order Total'),
    t('Recurring'),
    t('User Created date'),
    t('Order date'),
    t('Payment method'),
    t('Page'),
    t('Feed'),
    t('Order Comments'),
    t('Donation Comments'),
    t('Donation date'),
    t('Donation'),
    t('Donation ID'),
    t('Member join date'),
    t('Comment'),
  );
  $export['order_vbo'] = $view;

  return $export;
}
