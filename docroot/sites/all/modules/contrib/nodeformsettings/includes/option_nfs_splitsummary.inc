<?php

/**
 * Hide split summary
 */
function _option_nfs_splitsummary(&$form, &$form_state, $settings, $node) {
	// Hide the 'Split Summary at Cursor' button
	if($settings['nfs_splitsummary'] == 1) {
		unset($form['body_field']['teaser_include']);
	}
	
	return $form;
}

