<?php
// $Id: views-view-row-unformatted.tpl.php,v 1.1.2.1 2008/07/08 19:43:38 allisterbeharry Exp $
/**
 * @file views-view-row-unformatted.tpl.php
 * Simple view template to view unformatted fields from the views query.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 * @see views_json.views.inc
 */

//print ('template');
//print_r($fields);
//print_r($row);
//print_r($options);
//foreach ($fields as $id => $field)  
// print $id.":".$field->content."\n";

$field_separator = filter_xss($options['separator']);
foreach($row as $field_label => $field_value) { 
  $label = str_replace(':', '#colon#', $field_label);
  $value = filter_xss(str_replace(':', '#colon#', $field_value));
  $row_unformatted.=  $label.":".(!is_null($value) ? $value : "").$field_separator;
}
print rtrim($row_unformatted, $field_separator).PHP_EOL;

  