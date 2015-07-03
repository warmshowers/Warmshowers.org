<?php
/**
 * @file
 * mobile_app_content_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mobile_app_content_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'mobile_app_content';
  $view->description = '';
  $view->tag = 'json';
  $view->base_table = 'node';
  $view->human_name = 'App JSON Feedback';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['root_object'] = 'recommendations';
  $handler->display->display_options['style_options']['top_child_object'] = 'recommendation';
  $handler->display->display_options['style_options']['field_output'] = 'raw';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['remove_newlines'] = 0;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['style_options']['object_arrays'] = 0;
  $handler->display->display_options['style_options']['numeric_strings'] = 0;
  $handler->display->display_options['style_options']['bigint_string'] = 0;
  $handler->display->display_options['style_options']['pretty_print'] = 0;
  $handler->display->display_options['style_options']['unescaped_slashes'] = 0;
  $handler->display->display_options['style_options']['unescaped_unicode'] = 0;
  $handler->display->display_options['style_options']['char_encoding'] = array();
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Content: Member (field_member_i_trust) */
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['delta'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
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
  /* Field: Content: Author uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'node';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  /* Field: Content: Date We Met */
  $handler->display->display_options['fields']['field_hosting_date']['id'] = 'field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['table'] = 'field_data_field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['field'] = 'field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['settings'] = array(
    'format_type' => 'timestamp',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Content: Feedback is for */
  $handler->display->display_options['fields']['field_guest_or_host']['id'] = 'field_guest_or_host';
  $handler->display->display_options['fields']['field_guest_or_host']['table'] = 'field_data_field_guest_or_host';
  $handler->display->display_options['fields']['field_guest_or_host']['field'] = 'field_guest_or_host';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'field_member_i_trust_uid';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['fields']['uid_1']['table'] = 'users';
  $handler->display->display_options['fields']['uid_1']['field'] = 'uid';
  $handler->display->display_options['fields']['uid_1']['relationship'] = 'field_member_i_trust_uid';
  /* Field: Content: Overall Experience */
  $handler->display->display_options['fields']['field_rating']['id'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['table'] = 'field_data_field_rating';
  $handler->display->display_options['fields']['field_rating']['field'] = 'field_rating';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Member (field_member_i_trust) */
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'trust_referral' => 'trust_referral',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/json_recommendations';
  $translatables['mobile_app_content'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('author'),
    t('field_member_i_trust'),
    t('Nid'),
    t('Full Name'),
    t('Name'),
    t('Author uid'),
    t('Body'),
    t('Date We Met'),
    t('Feedback is for'),
    t('Uid'),
    t('Overall Experience'),
    t('All'),
    t('Page'),
  );
  $export['mobile_app_content'] = $view;

  return $export;
}
