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
    'unflag_denied_text' => 'I hosted this member',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 2,
      ),
      'unflag' => array(),
    ),
    'show_on_profile' => 1,
    'access_uid' => 'others',
    'api_version' => 2,
    'module' => 'test',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}

function test_block($op = 'list', $delta = 0, $edit = array()){
	if ($op == 'list') {
		$blocks['test_flags'] = array(
		  'info' => t('[test] Flags block'),
		  'cache' => BLOCK_CACHE_GLOBAL,
		);
		return $blocks;
		}
	else if($op=='view'){
		switch($delta){
			case 'test_flags':
				$block=array(
				'subject'=>t('Flags'),
				'content'=>theme('test_flags'),
				);
				return $block;
			}
		}
	}
	
function test_theme(){
	$template_path = drupal_get_path('module', 'test') . '/templates';
	return array(
	'test_flags'=>array(
		'path'=>$template_path,
		'template'=>'test-flags-block'),
	);
	}


