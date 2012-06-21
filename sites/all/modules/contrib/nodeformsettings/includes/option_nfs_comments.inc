<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_comments(&$form, &$form_state, $settings, $node) {
	if($settings['nfs_comments'] == 1) { 
		$form['comment_settings']['#access'] = FALSE;
	}
	
	return $form;
}

