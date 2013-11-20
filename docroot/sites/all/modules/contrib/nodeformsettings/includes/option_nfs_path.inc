<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_path(&$form, &$form_state, $settings, $node) {	
	if($settings['nfs_path'] == 1) { 
		$form['path']['#access'] = FALSE;
	}
	
	return $form;
}

