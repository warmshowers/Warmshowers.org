<?php

/**
 * Hide the Input Format Fieldset
 */
function _option_nfs_inputformat(&$form, &$form_state, $settings, $node) {
	if(isset($form['body_field']['body']) && $settings['nfs_inputformat'] == 1) {
		unset($form['body_field']['format']);
	}
	
	return $form;
}

