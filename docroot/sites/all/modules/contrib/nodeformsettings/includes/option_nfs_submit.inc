<?php

/**
 * Change the value for the submit button
 *
 * There's no if here
 */
function _option_nfs_submit(&$form, &$form_state, $settings, $node) {
  $form['actions']['submit']['#value'] = t($settings['nfs_submit']);

  return $form;
}
