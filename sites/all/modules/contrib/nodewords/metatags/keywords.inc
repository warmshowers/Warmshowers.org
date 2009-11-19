<?php
// $Id: keywords.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Support file for KEYWORDS meta tag.
 */

function nodewords_keywords_prepare($type, $ids, $value, $settings) {
  if ($type == 'node' && function_exists('taxonomy_node_get_terms') && count($ids) == 1) {
    $node = node_load($ids[0]);
    if (node_access('view', $node)) {
      foreach (taxonomy_node_get_terms($node) as $term) {
        if (in_array($term->vid, $settings['keywords_vids'])) {
          $value .= ','. $term->name;
        }
      }
    }
  }
  $value .= ','. $settings['global']['keywords'];
  $value = _nodewords_keywords_uniq($value);
  return $value;
}

function nodewords_keywords_form($type, $value, $settings) {
  return array(
    '#type' => 'textfield',
    '#title' => t('Keywords'),
    '#default_value' => $value,
    '#size' => 60,
    '#maxlength' => $settings['max_size'],
    '#description' => t('Enter a comma separated list of keywords for this page. Avoid duplication of words as this will lower your search engine ranking.'),
  );
}

/*
 * Uniq a string which is a seperated list to items, preserving
 * the order in which they appeared.
 */
function _nodewords_keywords_uniq($text, $max_items = 0, $seperator = ',') {
  if (empty($text)) {
    return $text;
  }

  $items = array_map('trim', explode($seperator, $text));
  $uniq_values = array();
  $uniq_lowers = array();

  foreach ($items as $item) {
    $lower = strtolower($item);
    if (!in_array($lower, $uniq_lowers) && $lower != '') {
      $uniq_values[] = $item;
      $uniq_lowers[] = $lower;
    }
  }

  if ($max_items > 0) {
    $uniq_values = array_slice($uniq_values, 0, $max_items);
  }

  return implode($seperator, $uniq_values);
}
