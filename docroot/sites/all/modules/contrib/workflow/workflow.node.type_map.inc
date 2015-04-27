<?php
/**
 * @file
 * Node specific functions, remnants of nodeapi.
 */

/**
 * Functions related to table workflow_type_map.
 */

/**
 * Gets all workflow_type_map.
 */
function workflow_get_workflow_type_map() {
  $results = db_query('SELECT type, wid FROM {workflow_type_map}');
  return $results->fetchAllKeyed();
}

/**
 * Getss workflow_type_map for a type. On no record, FALSE is returned.
 *
 * Currently this is a unique result but requests have been made to allow a node to have multiple
 * workflows. This is trickier than it sounds as a lot of our processing code will have to be
 * tweaked to account for multiple results.
 * ALERT: If a node type is *not* mapped to a workflow it will be listed as wid 0.
 * Hence, we filter out the non-mapped results.
 *
 * @see workflow_get_workflows_by_type()
 */
function workflow_get_workflow_type_map_by_type($type) {
  static $map = array();
  if (!isset($map[$type])) {
    $results = db_query('SELECT type, wid FROM {workflow_type_map} WHERE type = :type AND wid <> 0',
      array(':type' => $type));
    $map[$type] = $results->fetchObject();
  }
  return $map[$type];
}

/**
 * Given a wid, finds all node types mapped to it.
 */
function workflow_get_workflow_type_map_by_wid($wid) {
  static $map = array();
  if (!isset($map[$wid])) {
    $results = db_query('SELECT type, wid FROM {workflow_type_map} WHERE wid = :wid',
      array(':wid' => $wid));
    $map[$wid] = $results->fetchAll();
  }
  return $map[$wid];
}

/**
 * Deletes all type maps.
 *
 * @todo: why is this here instead of the admin_ui?
 */
function workflow_delete_workflow_type_map_all() {
  return db_delete('workflow_type_map')->execute();
}

/**
 * Given a wid, deletes the map for that workflow.
 */
function workflow_delete_workflow_type_map_by_wid($wid) {
  return db_delete('workflow_type_map')->condition('wid', $wid)->execute();
}

/**
 * Given a type, deletes the map for that workflow.
 */
function workflow_delete_workflow_type_map_by_type($type) {
  return db_delete('workflow_type_map')->condition('type', $type)->execute();
}

/**
 * Given information, inserts a new workflow_type_map. Returns data by ref. (like node_save).
 *
 * @todo: why is this here instead of the admin_ui?
 */
function workflow_insert_workflow_type_map($node_type, $wid) {
  $type_map = (object) array(
    'type' => $node_type,
    'wid' => $wid,
  );

  // Be sure we have a clean insert. There should never be more than one map for a type.
  workflow_delete_workflow_type_map_by_type($type_map->type);
  if ($type_map->wid) {
    drupal_write_record('workflow_type_map', $type_map);
  }
}
