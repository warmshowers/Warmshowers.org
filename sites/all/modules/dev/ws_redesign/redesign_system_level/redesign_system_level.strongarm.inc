<?php

/**
 * Implementation of hook_strongarm().
 */
function redesign_system_level_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'advanced_forum_user_picture_preset';
  $strongarm->value = 'thumbnail';
  $export['advanced_forum_user_picture_preset'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'advanced_forum_use_topic_navigation';
  $strongarm->value = 0;
  $export['advanced_forum_use_topic_navigation'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_notify_default_mailtext';
  $strongarm->value = 'Hi !name,

!commname has commented on: "!node_title":

!commtext

You can view and reply to the comment by <a href="!comment_url">clicking here</a>. (Please don\'t respond via email as the email just goes to the site managers.)

You can stop receiving emails when someone replies to this post by <a href="!link1">clicking here</a>.

- Warmshowers.org team';
  $export['comment_notify_default_mailtext'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'languageicons_placement';
  $strongarm->value = 'replace';
  $export['languageicons_placement'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeblock_block';
  $strongarm->value = '1';
  $export['nodeblock_block'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeformsettings_trust_referral';
  $strongarm->value = array(
    'nfs_submission_body_rows' => '20',
    'nfs_splitsummary' => '1',
    'nfs_inputformat' => '0',
    'nfs_revisionlog' => '0',
    'nfs_author_information' => '0',
    'nfs_path' => '0',
    'nfs_menu' => '0',
    'nfs_publishingoptions' => '0',
    'nfs_comments' => '0',
    'nfs_taxonomy' => '0',
    'nfs_preview' => '0',
    'nfs_cancel' => array(
      'nfs_cancel_status' => '1',
      'nfs_cancel_behaviour' => '0',
    ),
    'nfs_submit' => 'Submit',
    'nfs_hide_node_title' => '1',
    'nfs_title_create' => 'Create !node_type',
    'nfs_title_edit' => '!node_title',
  );
  $export['nodeformsettings_trust_referral'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_notify_default_mailtext';
  $strongarm->value = 'Hi !name,

You have received a comment on: "!node_title":

!commtext

You can view the comment by <a href="!comment_url">clicking here</a>.  (Please don\'t respond via email as the email just goes to the site managers.)

You will receive emails like this for all replies to your posts.

- Warmshowers team';
  $export['node_notify_default_mailtext'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_name';
  $strongarm->value = 'Warm Showers';
  $export['site_name'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'translatableregions_translate_selectors';
  $strongarm->value = '#profile-container .content, #content .forum-post';
  $export['translatableregions_translate_selectors'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'translatableregions_translation_api_key';
  $strongarm->value = 'AIzaSyBwT3hoRXWU1UkYIBK5Mm_tdpdRb30YC0Y';
  $export['translatableregions_translation_api_key'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_location_google_key';
  $strongarm->value = 'AIzaSyBwT3hoRXWU1UkYIBK5Mm_tdpdRb30YC0Y';
  $export['user_location_google_key'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_default';
  $strongarm->value = 'files/default_picture.jpg';
  $export['user_picture_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_dimensions';
  $strongarm->value = '';
  $export['user_picture_dimensions'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_file_size';
  $strongarm->value = '10000';
  $export['user_picture_file_size'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_comments';
  $strongarm->value = 'thumbnail';
  $export['user_picture_imagecache_comments'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles';
  $strongarm->value = 'profile_picture';
  $export['user_picture_imagecache_profiles'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_default';
  $strongarm->value = 'thumbnail';
  $export['user_picture_imagecache_profiles_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_min_height';
  $strongarm->value = '';
  $export['user_picture_imagecache_profiles_min_height'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_min_width';
  $strongarm->value = '';
  $export['user_picture_imagecache_profiles_min_width'] = $strongarm;

  return $export;
}
