<?php

/**
 * Validate if the Cancel link is not enabled when the comment form is below the post or comments
 */
function _validate_cfs_preview($form, &$form_state) {
	if($form_state['values']['cfs_preview'] == 1 && $form_state['values']['comment_preview'] == 1) {
		form_set_error('cfs_preview', t("You're trying to hide the <em>preview button in comments</em> but the button is set to be <em>Required</em>, please change one or the other."));
	}
}

