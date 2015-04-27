<?php

/**
 * Hide the Revision log field
 */
function _option_cfs_inputformat(&$form, &$form_state, $settings, $node) {
  if ($settings['cfs_inputformat'] == 1) {
    $format = filter_default_format();
    $form['comment_body'][LANGUAGE_NONE][0]['#format'] = $format;
    $form['comment_body'][LANGUAGE_NONE][0]['format'] = array('access' => FALSE);
  }

  return $form;
}
