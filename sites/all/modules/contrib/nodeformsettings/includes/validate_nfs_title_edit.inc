<?php

/**
 * Hide the Revision log field
 */
function _validate_nfs_title_edit($form, &$form_state) {
	if($form_state['values']['nfs_title_edit'] == '') {
		form_set_error('nfs_title_edit', t("<em>Page title when editing a node</em> cannot be empty"));
	}
}



