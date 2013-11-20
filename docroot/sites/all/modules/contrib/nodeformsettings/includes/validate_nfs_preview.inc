<?php

/**
 * Hide the Revision log field
 */
function _validate_nfs_preview($form, &$form_state) {
	if(variable_get('node_preview', 1)) {
		if($form_state['values']['nfs_preview'] == 1 || $form_state['values']['nfs_preview'] == 2) {
			form_set_error('nfs_preview',
				t("<em>Preview is set to <em>Required</em>, if you're trying to disable it or force it, change the <em>Preview post</em> at !url to <em>Optional</em>",
					array(
						'!url' => l(t("Post settings"), 'admin/content/node-settings')
					)
				)
			);
		}
	}
}



