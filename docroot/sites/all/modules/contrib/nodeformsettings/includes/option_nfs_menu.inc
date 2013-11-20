<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_menu(&$form, &$form_state, $settings, $node) {	
	if($settings['nfs_menu'] == 1) { 
		$form['menu']['#access'] = FALSE;
	}
	
	return $form;
}

