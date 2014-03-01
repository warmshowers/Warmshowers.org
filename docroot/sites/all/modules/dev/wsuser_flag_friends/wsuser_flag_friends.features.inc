<?php

/**
 * Implementation of hook_flag_default_flags().
 */

function wsuser_flag_friends_flag_default_flags() {
  $flags = array();
  // Exported flag: "Add user as a friend".
  $flags['friends'] = array(
    'content_type' => 'user',
    'title' => 'Add user as a friend',
    'global' => '0',
    'types' => array(),
    'flag_short' => 'Add <em>[fullname]</em> in your friends list',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Remove <em>[fullname]</em> from your friends list',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
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
    'api_version' => 2,
    'module' => 'wsuser_flag_friends',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}

/**
 * Implementation of hook_init().
 */
function wsuser_flag_friends_init() {
  drupal_add_css(drupal_get_path('module','wsuser_flag_friends') . '/css/style_friends.css');
}

/**
 * Implementation of hook_menu().
 */
function wsuser_flag_friends_menu() {
  $items['user/%user/friends'] = array(
    'title' => t('Friends'),
    'type' => MENU_LOCAL_TASK,
    'page callback' => 'wsuser_flag_friends_view_friends',
    'page arguments'   => array(1),
    'access callback' => 'wsuser_flag_friends_check_perm',
    );
  return $items;
}

function wsuser_flag_friends_check_perm() {
  global $user;
  if (in_array('authenticated user' , array_values($user->roles))) {
    if ($user->uid==arg(1) || user_access('view member friends')) return True;
  }
  return False;
}

/**
 * Implementation of hook_perm().
 */
function wsuser_flag_friends_perm() {
  return array( 'view member friends' );
}

/**
 * Implementation of hook_theme().
 */
function wsuser_flag_friends_theme() {
  $path_friends_tpl = drupal_get_path('module', 'wsuser_flag_friends') . '/templates';
  return array(
    'view_friends' => array(
      'path' => $path_friends_tpl,
      'template' => 'view_friends'
      )
    );
}

/**
 * Implementation of hook_preprocess().
 * Declare a new link to display in wsuser_member_actions_block.tpl.php
 * Declare a list of friends of the user being viewed to be displayed in view_friends.tpl.php
 */
function wsuser_flag_friends_preprocess(&$variables , $key) {
  global $user;
  if ($key == 'wsuser_member_actions') {
    $variables['wsuser_member_actions_block_links'][] = array(
      'type' => 'flag_checkbox',
      'data' => array('flag_name'=>'friends',
      'user_id' => arg(1)
      ));
  }
  if ($key == 'view_friends') {
    $user2 = user_load(arg(1));
    $variables['username'] = $user2->name;
    $variables['is_self'] = ($user->uid == arg(1) ? True : False);
    $variables['friend_list'] = wsuser_flag_friends_get_friends(arg(1));
  }
}

/**
* Load a user friend list from database
* return a array of users.
*/
function wsuser_flag_friends_get_friends($uid) {
  $flag = flag_get_flag('friends');
  $result = db_query("select content_id,timestamp from flag_content where fid=$flag->fid and uid=$uid");
  $users = array();
  while ($user = db_fetch_object($result)){
    $u = user_load(array('uid' => $user->content_id));
    $u->added_as_friend = $user->timestamp;
    $users[] = $u;
  }
  return $users;
}


function wsuser_flag_friends_view_friends($uid) {
  return theme('view_friends');
}
