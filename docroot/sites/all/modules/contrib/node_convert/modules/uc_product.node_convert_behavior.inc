<?php

/**
 * @file
 * Node convert ubercart.module include
 *
 * Performs necessary changes regarding ubercart type conversions.
 *
 * @TODO Fix for Drupal 7.
 */

/**
 * Implementation of node_convert_change().
 */
function uc_product_node_convert_change($data, $op) {
  if ($op == 'insert') {
    if ($data['dest_node_type'] == 'product' && module_exists('uc_product')) {
      $node = $data['node'];
      $hook_options = $data['hook_options'];
      $node = (object) array_merge((array) $node, $hook_options);
      uc_product_insert($node);
    }
  }
  elseif ($op == 'delete') {
    if ($data['node']->type == 'product' && module_exists('uc_product')) {
      $node = $data['node'];
      uc_product_delete($node);
    }
  }
  elseif ($op == 'options') {
    $form = array();
    if ($data['dest_node_type'] == 'product' && module_exists('uc_product')) {
      module_load_include('pages.inc', 'node');
      $node = new stdClass();
      $node->type = 'product';
      // We add the uc_product additional fields.
      $product_form = uc_product_form($node);
      $form = $product_form['base'];
      // We collate the values in hook_options so validation using uc_product_validate works.
      $parents = array('hook_options');
      $form['prices']['#parents'] = $parents;
      $form['weight']['#parents'] = $parents;
      $form['dimensions']['#parents'] = $parents;
      $form['#weight'] = 1;
    }
    return $form;
  }
  elseif ($op == 'options validate') {
    $form_state = $data['form_state'];
    if ($data['dest_node_type'] == 'product' && module_exists('uc_product')) {
      $node = $form_state['values']['node'];
      // Collate the hook_options values array with $node object, into a new $node object
      $node = (object) array_merge((array) $node, $form_state['values']['hook_options']);
      // Validate.
      uc_product_validate($node);
    }
  }
}
