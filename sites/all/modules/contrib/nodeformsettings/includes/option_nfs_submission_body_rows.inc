<?php

/**
 * Change the default height of the body field
 */
function _option_nfs_submission_body_rows(&$form, &$form_state, $settings, $node) {		
	
  if($settings['nfs_submission_body_rows'] == 1 && $form['body_field']['body']) {
    $form['body_field']['body']['#type'] = 'textfield';
  }
  elseif(isset($settings['nfs_submission_body_rows']) && $form['body_field']['body']) {
    $form['body_field']['body']['#rows'] = $settings['nfs_submission_body_rows'];
  }

	return $form;
}

