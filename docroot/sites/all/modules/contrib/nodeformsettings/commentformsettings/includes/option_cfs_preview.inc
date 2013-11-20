<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_preview(&$form, &$form_state, $settings, $node) {
	if($settings['cfs_preview'] == 1) {
		unset($form['preview']);	
	}
	
	return $form;
}

