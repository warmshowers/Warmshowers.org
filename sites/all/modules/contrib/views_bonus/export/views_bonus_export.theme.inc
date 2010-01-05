<?php
// $Id: views_bonus_export.theme.inc,v 1.2 2009/06/24 17:27:53 neclimdul Exp $
/**
 * @file
 * Theme related functions for processing our output style plugins.
 */

/**
 * Preprocess csv output template.
 */
function template_preprocess_views_bonus_export_csv(&$vars) {
  drupal_set_header('Content-type: text/csv; charset=utf-8');

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

  foreach ($vars['themed_rows'] as $i => $values) {
    foreach ($values as $j => $value) {
      $vars['themed_rows'][$i][$j] = $wrap . str_replace('"', $replace_value, decode_entities(strip_tags($value))) . $wrap;
    }
  }
}

/**
 * Preprocess txt output template.
 */
function template_preprocess_views_bonus_export_txt(&$vars) {
  drupal_set_header('Content-Type: text/plain');
  _views_bonus_export_shared_preprocess($vars);
}

/**
 * Preprocess doc output template.
 */
function template_preprocess_views_bonus_export_doc(&$vars) {
  drupal_set_header('Content-Type: application/msword');
  _views_bonus_export_shared_preprocess($vars);
}

/**
 * Preprocess xml output template.
 */
function template_preprocess_views_bonus_export_xml(&$vars) {
  drupal_set_header('Content-Type: text/xml');
  if (!$vars['options']['provide_file']) {
    unset($vars['options']['filename']);
  }
  _views_bonus_export_shared_preprocess($vars);

  foreach ($vars['themed_rows'] as $num => $row) {
    foreach ($row as $field => $content) {
      $vars['themed_rows'][$num][$field] = str_replace(
        array('&', '<', '>'),
        array('&amp;', '&lt;', '&gt;'),
        $content);
    }
  }
}

/**
 * Shared helper function for export preprocess functions.
 */
function _views_bonus_export_shared_preprocess(&$vars) {
  $view     = $vars['view'];
  $fields   = &$view->field;

  drupal_set_header('Cache-Control: max-age=60, must-revalidate');
  $filename = strtr(
    $vars['options']['filename'],
    array('%view' => check_plain($view->name))
  );
  if ($filename) {
    drupal_set_header('Content-Disposition: attachment; filename="'. $filename .'"');
  }

  $rows = $vars['rows'];

  $vars['header'] = array();
  foreach ($fields as $key => $field) {
    if (empty($field->options['exclude'])) {
      $vars['header'][$key] = check_plain($field->label());
    }
  }

  $vars['themed_rows'] = array();
  foreach ($rows as $num => $row) {
    $items = array();

    foreach ($fields as $key => $field) {
      if (empty($field->options['exclude'])) {
        $items[$key] = $field->theme($row);
      }
    }
    $vars['themed_rows'][$num] = $items;
  }
}
