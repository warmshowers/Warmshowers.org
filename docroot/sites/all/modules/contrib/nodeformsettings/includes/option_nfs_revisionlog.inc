<?php

/**
 * Hide the Revision log field
 */
function _option_nfs_revisionlog(&$form, &$form_state, $settings, $node) {		
	if($settings['nfs_revisionlog'] == 1) {
		$form['revision_information']['#access'] = user_access('view revisions');
	}
	elseif($settings['nfs_revisionlog'] == 2) {
		$form['revision_information']['#access'] = FALSE;
	}
	
	return $form;
}

