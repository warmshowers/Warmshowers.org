<?php
// $Id: views_bonus_export.theme.inc,v 1.8 2010/06/16 14:47:51 neclimdul Exp $
/**
 * @file
 * Theme related functions for processing our output style plugins.
 *
 * Views bug: http://drupal.org/node/593336
 */

/**
 * Preprocess csv output template.
 */
function template_preprocess_views_bonus_export_csv(&$vars) {
  // TODO Replace items with themed_rows.
  _views_bonus_export_shared_preprocess($vars);

  $vars['seperator'] = $vars['options']['seperator'];

  // Special handling when quoted values are involved.
  if ($vars['options']['quote']) {
    $wrap = '"';
    $replace_value = '""';
  }
  else {
    $wrap = '';
    $replace_value = '';
  }

  // Format header values.
  foreach ($vars['header'] as $key => $value) {
    $output = decode_entities(strip_tags($value));
    if ($vars['options']['trim']) {
      $output = trim($output);
    }
    $vars['header'][$key] = $wrap . str_replace('"', $replace_value, $output) . $wrap;
  }

  // Format row values.
  foreach ($vars['themed_rows'] as $i => $values) {
    foreach ($values as $j => $value) {
      $output = decode_entities(strip_tags($value));
      if ($vars['options']['trim']) {
        $output = trim($output);
      }
      $vars['themed_rows'][$i][$j] = $wrap . str_replace('"', $replace_value, $output) . $wrap;
    }
  }
}

/**
 * Preprocess txt output template.
 */
function template_preprocess_views_bonus_export_txt(&$vars) {
  _views_bonus_export_shared_preprocess($vars);
}

/**
 * Preprocess doc output template.
 */
function template_preprocess_views_bonus_export_doc(&$vars) {
  _views_bonus_export_shared_preprocess($vars);
}

/**
 * Preprocess xls output template.
 */
function template_preprocess_views_bonus_export_xls(&$vars) {
  _views_bonus_export_shared_preprocess($vars);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_views_bonus_export_xml(&$vars) {
  _views_bonus_export_shared_preprocess($vars);

  foreach ($vars['themed_rows'] as $num => $row) {
    foreach ($row as $field => $content) {
      // Prevent double encoding of the ampersand. Look for the entities produced by check_plain().
      $content = preg_replace('/&(?!(amp|quot|#039|lt|gt);)/', '&amp;', $content);
      // Convert < and > to HTML entities.
      $content = str_replace(
        array('<', '>'),
        array('&lt;', '&gt;'),
        $content);
      $vars['themed_rows'][$num][$field] = $content;
    }
  }
}
