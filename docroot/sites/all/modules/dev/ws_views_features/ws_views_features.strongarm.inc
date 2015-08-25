<?php
/**
 * @file
 * ws_views_features.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function ws_views_features_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_block_hashes';
  $strongarm->value = array(
    'a0e2a8ab6066d8f20b8c3c1b25cf9735' => 'recent_forum_topics-topics_block',
  );
  $export['views_block_hashes'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_defaults';
  $strongarm->value = array(
    'taxonomy_term' => 'disabled',
    'comments_recent' => TRUE,
    'frontpage' => TRUE,
    'tracker' => TRUE,
    'admin_content' => TRUE,
    'admin_user_review' => TRUE,
    'admin_users' => TRUE,
    'feedback_counts' => TRUE,
    'flagged_unresponsive' => TRUE,
    'flagged_users' => TRUE,
    'login_status' => TRUE,
    'negative_feedbacks_for_user' => TRUE,
    'node_administration' => TRUE,
    'nodes_review' => TRUE,
    'referrals' => TRUE,
    'tips' => TRUE,
    'tutorial_videos' => TRUE,
    'user_flagging' => TRUE,
    'users_with_ordrs' => TRUE,
    'advanced_user_admin' => TRUE,
    'member_feedback' => FALSE,
    'user_referrals_by_referee' => TRUE,
    'user_referrals_by_referrer' => TRUE,
    'feedback_block' => TRUE,
    'photo_contest' => TRUE,
    'comments_review' => TRUE,
    'uc_orders_user' => TRUE,
  );
  $export['views_defaults'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_devel_output';
  $strongarm->value = 0;
  $export['views_devel_output'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_devel_region';
  $strongarm->value = 'footer';
  $export['views_devel_region'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_exposed_filter_any_label';
  $strongarm->value = 'old_any';
  $export['views_exposed_filter_any_label'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_no_javascript';
  $strongarm->value = 0;
  $export['views_no_javascript'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_show_additional_queries';
  $strongarm->value = 0;
  $export['views_show_additional_queries'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_skip_cache';
  $strongarm->value = 0;
  $export['views_skip_cache'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_sql_signature';
  $strongarm->value = 0;
  $export['views_sql_signature'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_ui_show_advanced_help_warning';
  $strongarm->value = FALSE;
  $export['views_ui_show_advanced_help_warning'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_ui_show_sql_query';
  $strongarm->value = TRUE;
  $export['views_ui_show_sql_query'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_ui_show_sql_query_where';
  $strongarm->value = 'below';
  $export['views_ui_show_sql_query_where'] = $strongarm;

  return $export;
}
