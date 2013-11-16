<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_publishingoptions(&$form, &$form_state, $settings, $node) {	
	if($settings['nfs_publishingoptions'] == 1) { 
		$form['options']['#access'] = FALSE;
	}
	
	return $form;
}

