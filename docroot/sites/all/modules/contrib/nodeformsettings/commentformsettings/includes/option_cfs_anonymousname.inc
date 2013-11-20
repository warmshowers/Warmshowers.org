<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_anonymousname(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_anonymousname'] == 1) {
		unset($form['name']);
	}
	
	return $form;
}

