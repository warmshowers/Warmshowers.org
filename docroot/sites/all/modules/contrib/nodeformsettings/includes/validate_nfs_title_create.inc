<?php

/**
 * Hide the Revision log field
 */
function _validate_nfs_title_create($form, &$form_state) {
	if($form_state['values']['nfs_title_create'] == '') {
		form_set_error('nfs_title_create', t("<em>Page title when creating a node</em> cannot be empty"));
	}
}

