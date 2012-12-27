<?php
/**
 * @file
 * Hooks for handling Migrate integration
 */

/**
 * Implementation of hook_migrate_prepare_node().
 *
 * Converts all fields of the type nodewords_META => VALUE to a new array:
 *   nodewords => META => array ('value' = VALUE)
 */
function nodewords_migrate_prepare_node(&$node, $tblinfo, $row) {
  $errors = array();

  $node->nodewords = array();

  foreach (nodewords_migrate_fields_node($node->type) as $source_tag => $label) {
    if (!empty($node->$source_tag)) {
      $tag = preg_replace('/nodewords_/', '', $source_tag);
      $node->nodewords[$tag]['value'] = $node->$source_tag;
    }
  }

  return $errors;
}

/**
 * Implementation of hook_migrate_fields_node().
 *
 * @return
 *   An array of destination fields to be exposed to the Migrate content
 *   set for the passed content type.
 */
function nodewords_migrate_fields_node($type) {
  // Grab a list of all content types.
  $types = (array) content_types();

  // This will be the returned array of fields to expose.
  $fields = array();

  // Sanity checking: see if the passed content type exists.
  if (isset($types[$type])) {
    // Convenience variable.
    $content_type = $types[$type];

    // Check if nodewords is allowed for the content type.
    if (variable_get('nodewords_edit_metatags_' . $content_type, TRUE)) {
      // Need to elimate the 0 value entries.
      $tags = array_filter(variable_get('nodewords_edit', array()));

      foreach ($tags as $tag) {
        $fields['nodewords_' . $tag] = t('Nodewords: ' . ucfirst($tag));
      }
    }
  }

  return $fields;
}
