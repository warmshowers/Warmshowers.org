<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_title(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_title'] == 1) {
		unset($form['comment_filter']['comment']['#title']);
	}
	
	return $form;
}

