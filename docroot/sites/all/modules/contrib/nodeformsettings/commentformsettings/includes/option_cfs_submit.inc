<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_submit(&$form, &$form_state, $settings, $node) {	
	if(!empty($settings['cfs_submit'])) {
		$form['submit']['#value'] = $settings['cfs_submit'];
	}
	else {
		$form['submit']['#value'] = t("Submit");
	}
	
	return $form;
}

