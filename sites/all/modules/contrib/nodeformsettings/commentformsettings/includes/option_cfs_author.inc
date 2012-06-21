<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_author(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_author'] == 1) {
		unset($form['_author']);	
	}
	
	return $form;
}

