<?php

/**
 * Hide the Preview button
 */
function _option_nfs_preview(&$form, &$form_state, $settings, $node) {	
	// Hide the preview button
	if($settings['nfs_preview'] == 1) {
		unset($form['buttons']['preview']);
	}
	
	if($settings['nfs_preview'] == 2) {
		//drupal_set_message('<pre>' . htmlentities(print_r($form_state, 1)) . '</pre>');
		$form['buttons']['submit']['#access'] = FALSE;
	}
	
	if($settings['nfs_preview'] == 2 && $form_state['node_preview']) {
		$form['buttons']['submit']['#access'] = TRUE;
		$form['buttons']['submit']['#value'] = t($settings['nfs_submit']);
	}
	
	return $form;
}

