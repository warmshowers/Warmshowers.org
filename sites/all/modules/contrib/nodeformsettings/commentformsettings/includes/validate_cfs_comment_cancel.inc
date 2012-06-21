<?php

/**
 * Validate if the Cancel link is not enabled when the comment form is below the post or comments
 */
function _validate_cfs_comment_cancel($form, &$form_state) {
	if($form_state['values']['cfs_comment_cancel'] == 0 && $form_state['values']['comment_form_location']) {
		form_set_error('cfs_comment_cancel', t("You can't Enable the Comment Cancel Button while you display the comment submission form below post or comments. Please change one or the other."));
	}
}

