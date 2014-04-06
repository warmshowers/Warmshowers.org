<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function mark_as_favourite_flag_default_flags() {
  $flags = array();
  // Exported flag: "Mark user as favourite".
  $flags['mark_as_favourite'] = array(
    'content_type' => 'user',
    'title' => 'Mark user as favourite',
    'global' => '0',
    'types' => array(),
    'flag_short' => 'Mark <em>[fullname]</em> as favourite',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unmark <em>[fullname]</em> as favourite',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        '0' => 2,
      ),
      'unflag' => array(
        '0' => 2,
      ),
    ),
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'mark_as_favourite',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_views_api().
 */
function mark_as_favourite_views_api() {
  return array(
    'api' => '2',
  );
}
