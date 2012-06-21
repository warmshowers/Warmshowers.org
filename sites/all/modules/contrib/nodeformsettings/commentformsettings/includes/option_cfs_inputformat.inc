<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_inputformat(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_inputformat'] == 1) {
		unset($form['comment_filter']['format']);
	}
	
	return $form;
}

