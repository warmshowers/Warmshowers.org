<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function wsuser_flag_hosted_flag_default_flags() {
  $flags = array();
  // Exported flag: "Flag I hosted this member".
  $flags['i_hosted_this_member'] = array(
    'content_type' => 'user',
    'title' => 'Flag I hosted this member',
    'global' => '0',
    'types' => array(),
    'flag_short' => 'I hosted this member',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'I didn\'t host this member actually',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => 'You have hosted this member',
    'link_type' => 'normal',
    'roles' => array(
      'flag' => array(
        '0' => 2,
      ),
      'unflag' => array(),
    ),
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'api_version' => 2,
    'module' => 'wsuser_flag_hosted',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}

/**
* Implementation of hook_preprocess()
* provide a flag to be displayed in wsuser_member_actions_block.tpl.php
*/
function wsuser_flag_hosted_preprocess(&$variables, $key){
  if($key=='wsuser_member_actions'){
    $variables['wsuser_member_actions_block_links'][]=array(
      'type'=>'flag_checkbox',
      'data'=>array('flag_name'=>'i_hosted_this_member', 'user_id'=>arg(1)));
  }
}
