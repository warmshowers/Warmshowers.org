<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function redesign_theme_and_block_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function redesign_theme_and_block_flag_default_flags() {
  $flags = array();
  // Exported flag: "Has this member responded to your contacts?".
  $flags['responsive_member'] = array(
    'content_type' => 'user',
    'title' => 'Has this member responded to your contacts?',
    'global' => '0',
    'types' => array(),
    'flag_short' => 'Mark Responsive',
    'flag_long' => 'Clicking here will show that you have had good luck contacting this member',
    'flag_message' => 'Marked member as responsive',
    'unflag_short' => 'Unmark Responsive',
    'unflag_long' => 'You have previously marked this member as responsive. Click to un-mark this member as responsive. ',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        '0' => '2',
      ),
      'unflag' => array(
        '0' => '2',
      ),
    ),
    'show_on_profile' => 1,
    'access_uid' => '',
    'flag_confirmation' => 'Marked member as responsive',
    'unflag_confirmation' => '',
    'module' => 'redesign_theme_and_block',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Has this member been unresponsive to your contacts?".
  $flags['unresponsive_member'] = array(
    'content_type' => 'user',
    'title' => 'Has this member been unresponsive to your contacts?',
    'global' => '0',
    'types' => array(),
    'flag_short' => 'Mark Unresponsive',
    'flag_long' => 'If you have tried and have not gotten a response from this user, mark them as unresponsive',
    'flag_message' => 'Member marked as unresponsive',
    'unflag_short' => 'Unmark Unresponsive',
    'unflag_long' => 'Click here to unmark as unresponsive (if you got a response from them)',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        '0' => '2',
      ),
      'unflag' => array(
        '0' => '2',
      ),
    ),
    'show_on_profile' => 1,
    'access_uid' => '',
    'flag_confirmation' => 'Member marked unresponsive',
    'unflag_confirmation' => '',
    'module' => 'redesign_theme_and_block',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function redesign_theme_and_block_imagecache_default_presets() {
  $items = array(
    '400w' => array(
      'presetname' => '400w',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '400',
            'height' => '',
            'upscale' => 1,
          ),
        ),
      ),
    ),
    '800w' => array(
      'presetname' => '800w',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '800',
            'height' => '',
          ),
        ),
      ),
    ),
    'Center_Content' => array(
      'presetname' => 'Center_Content',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '440',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'rightcolumn' => array(
      'presetname' => 'rightcolumn',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '200',
            'height' => '',
            'upscale' => 1,
          ),
        ),
      ),
    ),
    'thumbnail' => array(
      'presetname' => 'thumbnail',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '125',
            'height' => '125',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function redesign_theme_and_block_views_api() {
  return array(
    'api' => '2',
  );
}
