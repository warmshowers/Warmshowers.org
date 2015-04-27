<?php

/**
 * Change the page title when creating a node
 */
function _option_nfs_title_create($form, &$form_state, $settings, $node) {
  if (empty($node->nid)) {
    $replace_pairs = array(
      '!node_type' => node_type_get_name($node),
    );
    $title = strtr($settings['nfs_title_create'], $replace_pairs);
    drupal_set_title(t($title));
  }

  return $form;
}
