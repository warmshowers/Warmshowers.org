<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_author_information(&$form, &$form_state, $settings, $node) {
	if($settings['nfs_author_information'] == 1) { 
		$form['author']['#access'] = FALSE;
	}
	
	return $form;
}

