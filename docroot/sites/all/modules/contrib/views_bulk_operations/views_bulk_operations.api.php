<?php

/**
 * @file
 * Documentation of hooks.
 */

/**
 * Hook used by VBO to be able to handle different objects as does Views 2+ and the Drupal core action system.
 *
 * The array returned for each object type contains:
 *  'type' (required) => the object type name, should be the same as 'type' field in hook_action_info().
 *  'context' (optional) => the context name that should receive the object, defaults to the value of 'type' above.
 *  'base_table' (required) => the Views 2 table name corresponding to that object type, should be the same as the $view->base_table attribute.
 *  'oid' (currently unused) => an attribute on the object that returns the unique object identifier (should be the same as $view->base_field).
 *  'load' (required) => a function($oid) that returns the corresponding object.
 *  'title' (required) => an attribute on the object that returns a human-friendly identifier of the object.
 *  'access' (optional) => a function($op, $node, $account = NULL) that behaves like node_access().
 *
 * The following attributes allow VBO to show actions on view types different than the action's type:
 *  'hook' (optional) => the name of the hook supported by this object type, as defined in the 'hooks' attribute of hook_action_info().
 *  'normalize' (optional) => a function($type, $object) that takes an object type and the object instance, returning additional context information for cross-type
 *
 *  e.g., an action declaring hook => array('user') while of type 'system' will be shown on user views, and VBO will call the user's 'normalize' function to
 *        prepare the action to fit the user context.
 */
function hook_views_bulk_operations_object_info() {
  $object_info = array(
    'node' => array(
      'type' => 'node',
      'base_table' => 'node',
      'load' => '_views_bulk_operations_node_load',
      'oid' => 'nid',
      'title' => 'title',
      'access' => 'node_access',
      'hook' => 'nodeapi',
      'normalize' => '_views_bulk_operations_normalize_node_context',
    ),
  );
  return $object_info;
}

/**
 * Hook used by VBO to allow altering the object_info structure returned by other modules.
 */
function hook_views_bulk_operations_object_info_alter(&$object_info) {
  $object_info['node']['load'] = '_my_special_node_load_callback';
}

/**
 * Hook used by VBO to alter the way views results are indexed. 
 *
 * Indexing is essential to remember the selected objects between the server and the browser.
 * This hook is useful for situations where the view query can return multiple rows with the same
 * object primary id, as in the case of multiple-valued node reference fields returned separately.
 */
function hook_views_bulk_operations_object_hash_alter(&$hash, $object, $view) {
  if ($view->name == 'my_view_name') {
    $hash = md5($object->nid . $object->field_node_reference_nid);
  }
}

