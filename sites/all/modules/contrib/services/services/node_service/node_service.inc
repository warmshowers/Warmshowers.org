<?php
// $Id: node_service.inc,v 1.1.2.11.2.10 2010/02/20 23:50:05 heyrocker Exp $

/**
 * @file
 *  Link general node functionalities to services module.
 */

/**
 * Returns a specified node.
 *
 * @param $nid
 *   Number. The node ID.
 * @param $fields
 *   Array (optinonal). The node fields needed. If its empty,
 *   all fields will be returned
 * @return
 *   Object. The node object with all wanted fields.
 */
function node_service_get($nid, $fields = array()) {
  global $user;
  $node = node_load(array('nid' => $nid));

  if ($node) {
    // Apply field level content permissions
    if (module_exists('content') && variable_get('services_use_content_permissions', FALSE)) {
      $content_fields = content_fields(NULL, $node->type);
      foreach ($content_fields as $field_name => $field_info) {
        if (isset($node->$field_name)) {
          $access = module_invoke_all('field_access', 'view', $field_info, $user, $node);
          if (in_array(FALSE, $access)) {
            unset($node->$field_name);
          }
        }
      }
    }
    $result = services_node_load($node, $fields);
  }
  else {
    $result = services_error(t('Could not find the node.'), 404);
  }

  return $result;
}

/**
 * Check if the user has the permission to get the
 * node's data thru services.
 *
 * @param $nid
 *   Number. The node ID.
 * @return
 *   Boolean. TRUE if the user has view access.
 */
function node_service_get_access($nid) {
  global $user;
  $node = node_load($nid);
  return (node_access('view', $node) && user_access('load node data'));
}

/**
 * Returns a specified node.
 *
 * @param $nid
 *   Number. The node ID.
 * @param $fields
 *   Array (optional). The node fields needed. If its empty,
 *   all fields will be returned
 * @param $teaser
 *   Boolean (optional). Format as teaser.
 * @param $page
  *   Boolean (optional). Format as page.
 * @return
 *   Object. The node object with all wanted fields.
 */
function node_service_view($nid, $fields = array(), $teaser = FALSE, $page = FALSE) {
  $node = services_node_load(node_build_content(node_load($nid), $teaser, $page), $fields);

  if (!$node) {
    return services_error(t("Could not find the node."), 404);
  }

  return $node;
}

/**
 * Check if the user has the permission to get the
 * node's formated data thru services.
 *
 * @param $nid
 *   Number. The node ID.
 * @return
 *   Boolean. TRUE if the user has the permission to get the
 *   node's data thru services.
 */
function node_service_view_access($nid) {
  global $user;
  $node = node_load($nid);
  return (node_access('view', $node) && user_access('load node data'));
}

/**
 * Save a node. It creates a new one, in case the 'nid' field
 * is missing.
 *
 * @param $edit
 *   Array. The node fields' values, just like created on
 *   node edit form.
 * @return
 *   Number. The node ID.
 */
function node_service_save($edit) {
  // Load the required includes for drupal_execute
  module_load_include('inc', 'node', 'node.pages');
  $nid = NULL;
  $edit = (array) $edit;
  if ($edit['nid']) {
    $node = node_load($edit['nid']);
    if ($node->nid) {
      // Setup form_state.
      $form_state = array();
      $form_state['values'] = (array) $edit;
      $form_state['values']['op'] = t('Save');

      // Later on in the chain node_form seems to expect a copy of
      // the old node object.
      $form_state['node'] = (array) $node;

      $ret = drupal_execute($node->type .'_node_form', $form_state, (object)$node);

      // If the node is immediately reloaded after update, it will
      // load the OLD cached version.
      node_load(0, NULL, TRUE);

      // Set $nid, so it can be returned
      $nid = $node->nid;
    }
    else {
      return services_error(t('Node not found'), 401);
    }
  }
  else {
    // If the submitted node has no author (for instance if it was
    // created programmatically by hand) then set the currently logged 
    // in user as the author. 
    if (!$edit['uid']) {
      global $user;
      $edit['uid'] = $user->uid;
    }

    // Setup form_state
    $form_state = array();
    $form_state['values'] = $edit;
    $form_state['values']['op'] = t('Save');

    $ret = drupal_execute($edit['type'] .'_node_form', $form_state, (object)$edit);

    // Fetch $nid out of $form_state
    $nid = $form_state['nid'];
  }
  if ($errors = form_get_errors()) {
    return services_error(implode("\n", $errors), 401);
  }
  return $nid;
}

/**
 * Check if the user has the permission to save a node.
 *
 * @param $node
 *   Object. The node object.
 * @return
 *   Boolean. TRUE if the user has the permission to save a node.
 */
function node_service_save_access($node) {
  $node = (array) $node;
  if (isset($node['nid'])) {
    return node_access('update', $node);
  }
  return node_access('create', $node['type']);
}

/**
 * Check if the user has the permission to delete a node.
 *
 * @param $nid
 *   Number. The node ID.
 * @return
 *   Boolean. TRUE if the user has the permission to delete a node.
 */
function node_service_delete_access($nid) {
  $node = node_load($nid);
  return node_access('delete', $node);
}
