<?php

/**
 * @file
 * User menu callbacks for privatemsg_filter module.
 */

/**
 * Return autocomplete results for tags.
 *
 * Most of this code has been lifted/modified from privatemsg_user_name_autocomplete().
 */
function privatemsg_filter_tags_autocomplete($string) {

  // 1: Parse $string and build a list of tags.
  $tags = array();
  $fragments = explode(',', $string);
  foreach ($fragments as $tag) {
    $tag = trim($tag);
    $tags[$tag] = $tag;
  }

  // 2: Find the next tag suggestion.
  $fragment = array_pop($tags);
  $matches = array();
  if (!empty($fragment)) {
    $query = _privatemsg_assemble_query(array('tags_autocomplete', 'privatemsg_filter'), $fragment, $tags);
    $prefix = count($tags) ? implode(", ", $tags) . ", " : '';
    // 3: Build proper suggestions and print.
    foreach ($query->execute() as $tag) {
      $matches[$prefix . $tag->tag . ", "] = $tag->tag;
    }
  }
  // convert to object to prevent drupal bug, see http://drupal.org/node/175361
  drupal_json_output((object)$matches);
}