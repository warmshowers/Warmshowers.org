<?php
// $Id: taxonomy_service.inc,v 1.1.2.6.2.1 2009/12/05 01:42:39 heyrocker Exp $
/**
 * @file
 *  Link general taxonomy functionalities to services module.
 */

/**
 * Services interface to taxonomy_get_tree().
 *
 * @see
 *   taxonomy_get_tree()
 */
function taxonomy_service_get_tree($vid, $parent = 0, $max_depth = NULL) {
  return taxonomy_get_tree($vid, $parent, -1, $max_depth);
}

/**
 * Services interface to taxonomy_select_nodes().
 *
 * Note that where taxonomy_select_nodes() returns the results
 * of a db_query(), this function returns an array of node objects.
 *
 * @see
 *   taxonomy_select_nodes().
 * @return 
 *   An array of node objects.
 */
function taxonomy_service_select_nodes($tids = array(), $fields = array(), $operator = 'or', $depth = 0, $pager = TRUE, $order = 'n.sticky DESC, n.created DESC') {
  $result = taxonomy_select_nodes($tids, $operator, $depth, $pager, $order);
  while ($node = db_fetch_object($result)) {
    $nodes[] = services_node_load(node_load($node->nid), $fields);
  }

  return $nodes;
}


/**
 * Services interface to taxonomy_save_term().
 *
 * @see
 *   taxonomy_save_term().
 */
function taxonomy_service_save_term($term) {
  if (!is_array($term)) {
    $term = (array) $term;
  }
  return taxonomy_save_term($term);
}

/**
 * Services interface to taxonomy_save_vocabulary().
 *
 * @see
 *   taxonomy_save_vocabulary().
 */
function taxonomy_service_save_vocabulary($vocabulary) {
  if (!is_array($vocabulary)) {
    $vocabulary = (array) $vocabulary;
  }
  return taxonomy_save_vocabulary($vocabulary);
}
