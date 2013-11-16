<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_size(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_size'] == 1) {
		$form['cfs_filter']['comment']['#rows'] = $settings['cfs_size'];
	}
	
	return $form;
}

