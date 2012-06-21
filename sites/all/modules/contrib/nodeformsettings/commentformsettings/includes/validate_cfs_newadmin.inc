<?php

/**
 * Validate if the Cancel link is not enabled when the comment form is below the post or comments
 */
function _validate_cfs_newadmin($form, &$form_state) {
	if($form_state['values']['cfs_newadmin'] == 0 && $form_state['values']['comment'] != 2) {
		form_set_error('cfs_newadmin', t("If you want to enable the functionality to override the author of a comment you need to enable comments for this content type. Please fix one or the other."));
	}
}

