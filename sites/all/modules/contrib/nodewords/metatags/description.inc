<?php
// $Id: description.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Support file for DESCRIPTION meta tag.
 */

function nodewords_description_prepare($type, $ids, $value, $settings) {
  if ((!isset($value) || empty($value)) && $settings['use_teaser'] && count($ids) == 1) {
    switch ($type) {
      case 'node':
        $node = node_load($ids[0]);
        if ($node && node_access('view', $node)) {
          // HACK: see http://drupal.org/node/79315 (can't use php functions in
          // body of nodes). Note that this still won't work if you have a
          // CCK field with PHP format.
          if (filter_format_allowcache($node->format)) {
            // We would like to use node_view($node, TRUE, FALSE), unfortunately
            // this uses theme_node() which, by default, adds 'Posted by ...'
            // information (which we don't want). The code below calls all
            // functions node_view() does, without the theme.
            $node = node_build_content($node, TRUE, FALSE);
            $content = drupal_render($node->content);
            $node->teaser = $content;
            node_invoke_nodeapi($node, 'alter', TRUE, FALSE);
            $value = $node->teaser;
          }
        }
        break;

      case 'term':
        // TODO: probably we have to do a db_rewrite_sql() query here so access is restricted
        $term = taxonomy_get_term($ids[0]);
        if ($term) {
          $value = $term->description;
        }
        break;
      case 'vocabulary':
        // TODO: probably we have to do a db_rewrite_sql() query here so access is restricted
        $voc = taxonomy_vocabulary_load($ids[0]);
        if ($voc) {
          $value = $voc->description;
        }
        break;
    }
  }
  return $value;
}

function nodewords_description_form($type, $value, $settings) {
  return array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $value,
    '#cols' => 60,
    '#rows' => 6,
    '#description' => t('Enter a description for this page. Limit your description to about 20 words, with a maximum of %count characters. It should not contain any HTML tags or other formatting.', array('%count' => $settings['max_size'])) . ($settings['use_teaser'] ? ' '. t('When you leave this field empty, the teaser will be used as description.') : ''),
  );
}
