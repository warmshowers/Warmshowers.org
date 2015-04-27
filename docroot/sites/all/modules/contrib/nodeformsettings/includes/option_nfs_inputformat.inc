<?php

/**
 * Hide the Input Format Fieldset
 */
function _option_nfs_inputformat(&$form, &$form_state, $settings, $node) {
  if (isset($form['body']) && $settings['nfs_inputformat'] == 1) {
    $format = filter_default_format();
    $form['body'][LANGUAGE_NONE][0]['#format'] = $format;
    $form['body'][LANGUAGE_NONE][0]['format'] = array('access' => FALSE);
  }

  return $form;
}
