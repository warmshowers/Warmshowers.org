<?php
/**
 * @file
 * member_feedback_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function member_feedback_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'member_feedback';
  $view->description = 'All views for user feedback throughout the site.';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Feedback';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '3600';
  $handler->display->display_options['cache']['output_lifespan'] = '3600';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'fullname_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'To leave feedback for a member, visit their profile page and click the "Provide Feedback" button in the "Actions" block on their profile page.';
  $handler->display->display_options['header']['text']['format'] = '1';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['content'] = 'No members have given feedback for this member yet.';
  $handler->display->display_options['empty']['text']['format'] = '1';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Member (field_member_i_trust) */
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['label'] = 'member';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['delta'] = '-1';
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['link_to_node'] = TRUE;
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['relationship'] = 'uid';
  $handler->display->display_options['fields']['picture']['label'] = '';
  $handler->display->display_options['fields']['picture']['element_label_colon'] = FALSE;
  /* Field: Content: Overall Experience */
  $handler->display->display_options['fields']['field_rating']['id'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['table'] = 'field_data_field_rating';
  $handler->display->display_options['fields']['field_rating']['field'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['label'] = '';
  $handler->display->display_options['fields']['field_rating']['element_label_colon'] = FALSE;
  /* Field: Content: Date We Met */
  $handler->display->display_options['fields']['field_hosting_date']['id'] = 'field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['table'] = 'field_data_field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['field'] = 'field_hosting_date';
  $handler->display->display_options['fields']['field_hosting_date']['label'] = '';
  $handler->display->display_options['fields']['field_hosting_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_hosting_date']['settings'] = array(
    'format_type' => 'field_hosting_date_default',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['fullname']['label'] = '';
  $handler->display->display_options['fields']['fullname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fullname']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Date We Met (field_hosting_date) */
  $handler->display->display_options['sorts']['field_hosting_date_value']['id'] = 'field_hosting_date_value';
  $handler->display->display_options['sorts']['field_hosting_date_value']['table'] = 'field_data_field_hosting_date';
  $handler->display->display_options['sorts']['field_hosting_date_value']['field'] = 'field_hosting_date_value';
  $handler->display->display_options['sorts']['field_hosting_date_value']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Member (field_member_i_trust) */
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'trust_referral' => 'trust_referral',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Feedback received */
  $handler = $view->new_display('page', 'Feedback received', 'feedback_received');
  $handler->display->display_options['display_description'] = 'Feedback received by user.';
  $handler->display->display_options['defaults']['cache'] = FALSE;
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'To leave feedback for a member, visit their profile page and click the "Provide Feedback" button in the "Actions" block on their profile page.
';
  $handler->display->display_options['header']['text']['format'] = '1';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Member (field_member_i_trust) */
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['label'] = 'member';
  $handler->display->display_options['relationships']['field_member_i_trust_uid']['delta'] = '-1';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['link_to_node'] = TRUE;
  /* Field: Content: Overall Experience */
  $handler->display->display_options['fields']['field_rating']['id'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['table'] = 'field_data_field_rating';
  $handler->display->display_options['fields']['field_rating']['field'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['label'] = '';
  $handler->display->display_options['fields']['field_rating']['element_label_colon'] = FALSE;
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['relationship'] = 'uid';
  $handler->display->display_options['fields']['fullname']['label'] = '';
  $handler->display->display_options['fields']['fullname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fullname']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Content: Contest Photo */
  $handler->display->display_options['fields']['field_contest_photo']['id'] = 'field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['table'] = 'field_data_field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['field'] = 'field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['label'] = '';
  $handler->display->display_options['fields']['field_contest_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contest_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_contest_photo']['type'] = 'colorbox';
  $handler->display->display_options['fields']['field_contest_photo']['settings'] = array(
    'colorbox_node_style' => 'medium',
    'colorbox_node_style_first' => '',
    'colorbox_image_style' => '1200w',
    'colorbox_gallery' => 'post',
    'colorbox_gallery_custom' => '',
    'colorbox_caption' => 'none',
    'colorbox_caption_custom' => '',
  );
  $handler->display->display_options['fields']['field_contest_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_contest_photo']['delta_offset'] = '0';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Member (field_member_i_trust) */
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['id'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['table'] = 'field_data_field_member_i_trust';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['field'] = 'field_member_i_trust_uid';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_member_i_trust_uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'user/%/feedback';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Feedback';
  $handler->display->display_options['menu']['weight'] = '9';

  /* Display: Feedback given */
  $handler = $view->new_display('attachment', 'Feedback given', 'feedback_given');
  $handler->display->display_options['display_description'] = 'Feedback given by user.';
  $handler->display->display_options['defaults']['cache'] = FALSE;
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = TRUE;
  $handler->display->display_options['empty']['text']['content'] = 'This member has not yet given any feedback.';
  $handler->display->display_options['empty']['text']['format'] = '1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'field_member_i_trust_uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['link_to_node'] = TRUE;
  /* Field: Content: Overall Experience */
  $handler->display->display_options['fields']['field_rating']['id'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['table'] = 'field_data_field_rating';
  $handler->display->display_options['fields']['field_rating']['field'] = 'field_rating';
  $handler->display->display_options['fields']['field_rating']['label'] = '';
  $handler->display->display_options['fields']['field_rating']['element_label_colon'] = FALSE;
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['relationship'] = 'field_member_i_trust_uid';
  $handler->display->display_options['fields']['fullname']['label'] = '';
  $handler->display->display_options['fields']['fullname']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['fullname']['alter']['path'] = 'user/[uid]';
  $handler->display->display_options['fields']['fullname']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content: Contest Photo */
  $handler->display->display_options['fields']['field_contest_photo']['id'] = 'field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['table'] = 'field_data_field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['field'] = 'field_contest_photo';
  $handler->display->display_options['fields']['field_contest_photo']['label'] = '';
  $handler->display->display_options['fields']['field_contest_photo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_contest_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_contest_photo']['type'] = 'colorbox';
  $handler->display->display_options['fields']['field_contest_photo']['settings'] = array(
    'colorbox_node_style' => 'medium',
    'colorbox_node_style_first' => '',
    'colorbox_image_style' => '1200w',
    'colorbox_gallery' => 'post',
    'colorbox_gallery_custom' => '',
    'colorbox_caption' => 'auto',
    'colorbox_caption_custom' => '',
  );
  $handler->display->display_options['fields']['field_contest_photo']['delta_limit'] = '1';
  $handler->display->display_options['fields']['field_contest_photo']['delta_offset'] = '0';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['label'] = '';
  $handler->display->display_options['fields']['delete_node']['element_label_colon'] = FALSE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['displays'] = array(
    'feedback_received' => 'feedback_received',
    'default' => 0,
    'block_1' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';

  /* Display: User feedback */
  $handler = $view->new_display('entity_view', 'User feedback', 'user_feedback');
  $handler->display->display_options['display_description'] = 'Most recent feedback for a user.';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'text';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = TRUE;
  $handler->display->display_options['header']['text']['content'] = 'To leave feedback for a member..........';
  $handler->display->display_options['header']['text']['format'] = '1';
  $handler->display->display_options['entity_type'] = 'user';
  $handler->display->display_options['bundles'] = array(
    0 => 'user',
  );

  /* Display: Negative Feedback */
  $handler = $view->new_display('views_rules', 'Negative Feedback', 'negative_feedback');
  $handler->display->display_options['display_description'] = 'A rules display to list negative feedback.';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'trust_referral' => 'trust_referral',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = '0';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Overall Experience (field_rating) */
  $handler->display->display_options['filters']['field_rating_value']['id'] = 'field_rating_value';
  $handler->display->display_options['filters']['field_rating_value']['table'] = 'field_data_field_rating';
  $handler->display->display_options['filters']['field_rating_value']['field'] = 'field_rating_value';
  $handler->display->display_options['filters']['field_rating_value']['value'] = array(
    'Negative' => 'Negative',
  );
  $handler->display->display_options['rules_parameter'] = array(
    'field_member_i_trust_uid' => array(
      'enabled' => 1,
      'type' => 'user',
      'label' => 'Content: Member',
      'name' => 'field_member_i_trust_uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'node',
      'label' => 'Content: Nid',
      'name' => 'nid',
    ),
  );
  $translatables['member_feedback'] = array(
    t('Defaults'),
    t('Feedback'),
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
    t('To leave feedback for a member, visit their profile page and click the "Provide Feedback" button in the "Actions" block on their profile page.'),
    t('No members have given feedback for this member yet.'),
    t('author'),
    t('member'),
    t('All'),
    t('Feedback received'),
    t('Feedback received by user.'),
    t('To leave feedback for a member, visit their profile page and click the "Provide Feedback" button in the "Actions" block on their profile page.
'),
    t('Feedback given'),
    t('Feedback given by user.'),
    t('This member has not yet given any feedback.'),
    t('User feedback'),
    t('Most recent feedback for a user.'),
    t('To leave feedback for a member..........'),
    t('Negative Feedback'),
    t('A rules display to list negative feedback.'),
  );
  $export['member_feedback'] = $view;

  return $export;
}
