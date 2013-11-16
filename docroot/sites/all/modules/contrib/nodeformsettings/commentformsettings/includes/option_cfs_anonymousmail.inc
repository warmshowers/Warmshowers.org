<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_anonymousmail(&$form, &$form_state, $settings, $node) {		
	if($settings['cfs_anonymousmail'] == 1) {
		unset($form['mail']);
	}
	
	return $form;
}

