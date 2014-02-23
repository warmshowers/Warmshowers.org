<?php

/**
 * Implementation of hook_flag_default_flags().
 */
function test_flag_default_flags() {
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
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 2,
      ),
      'unflag' => array(),
    ),
    'show_on_profile' => 0,
    'access_uid' => 'others',
    'module' => 'test',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

function test_theme(){
	$template_path = drupal_get_path('module', 'wsuser') . '/templates';
	return array(
	'test_flags'=>array(
		'path'=>$template_path,
		'template'=>'wsuser_member_actions_block'),
	'flag_checkbox' => array(
      'arguments' => array('link' => NULL),
    ),
	);
	}
	
function theme_flag_checkbox($link){
	print flag_create_link($link[0],$link[1]);
	}

	
function test_preprocess(&$variables, $key){
	if($key=='wsuser_member_actions'){
		$variables['wsuser_member_actions_block_links'][]=array(
		'flag_checkbox',
		array('i_hosted_this_member', arg(1)));
		}
	}

