<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_anonymoushomepage(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_anonymoushomepage'] == 1) {
		unset($form['homepage']);
	}
	
	return $form;
}

