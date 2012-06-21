<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_taxonomy(&$form, &$form_state, $settings, $node) {
	if($settings['nfs_taxonomy'] == 1) {
		$form['taxonomy']['#type'] = 'markup';
	}
	
	return $form;
}

