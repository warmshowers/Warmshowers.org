<?php
// $Id: views-view-json.tpl.php,v 1.1.2.7 2009/09/08 16:45:08 allisterbeharry Exp $
/**
 * @file views-view-json.tpl.php
 * View template to render view fields as JSON. Supports simple JSON and the Exhibit format.
 *
 * - $view: The view in use.
 * - $rows: The raw result objects from the query, with all data it fetched.
 * - $options: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see views_json.views.inc
 */
 
if ($options['format'] == 'Simple') json_simple_render($view);
if ($options['format'] == 'Exhibit') json_exhibit_render($view);

function json_simple_render($view) {
  define('EXHIBIT_DATE_FORMAT', '%Y-%m-%d %H:%M:%S');	
	$json = "{\n".'  "nodes":'."\n".str_repeat(" ", 4)."[\n";
	$total_view_result_count = count((array)($view->result)); //cast the result object to an array so we can count how many properties in itt;
	$view_result_count = 0;
	foreach ($view->result as $node) {
		$json.= str_repeat(" ", 6)."{\n";
		$total_field_count = count((array)$node); //cast the node object to an array so we can count how many properties in itt
		$field_count = 0;
		foreach($node as $field_label => $field_value) {
		  $label = trim(views_json_strip_illegal_chars(views_json_encode_special_chars($field_label)));
          $value = views_json_encode_special_chars(trim(views_json_is_date($field_value)));
          if ((is_null($value)) || ($value == '')) continue;
//          if (preg_match('/\d/', $value)) {
//            if (strtotime($value))
//              $value = gmstrftime(EXHIBIT_DATE_FORMAT, strtotime($value));
//          }
          $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
          $json.=str_repeat(" ", 8).'"'.$label.'"'. " ".": ".'"'.$value.'"'.((++$field_count == $total_field_count) ? "":",")."\n";
		}
		$json.=str_repeat(" ", 6)."}".((++$view_result_count == $total_view_result_count) ? "":",")."\n";	
	}
	$json.=str_repeat(" ", 4)."]\n}";
	
  if ($view->override_path) { //inside a live preview so just output the text
    print $json; 
  }
  else { //real deal so switch the content type and stop further processing of the page
    drupal_set_header('Content-Type: text/javascript');
    print $json;
    module_invoke_all('exit');
    exit;
 }
	
}


function json_exhibit_render($view) {
  define('EXHIBIT_DATE_FORMAT', '%Y-%m-%d %H:%M:%S');
  $json = "{\n".'  "items":'."\n".str_repeat(" ", 4)."[\n";
  $total_view_result_count = count((array)($view->result)); //cast the result object to an array so we can count how many properties in itt;
  $view_result_count = 0;
  foreach ($view->result as $node) { 
  	$json.=str_repeat(" ", 6)."{\n";
    $json.=str_repeat(" ", 8).'"type" '. " ".": ".'"'.'##type##'.'",'."\n";
    $json.=str_repeat(" ", 8).'"label" '. " "." : ".'"'.'##label##'.'",'."\n";
  	$total_field_count = count((array)$node); //cast the node object to an array so we can count how many properties in itt
	$field_count = 0;
    foreach($node as $field_label => $field_value) {
      $label = trim(views_json_strip_illegal_chars(views_json_encode_special_chars($field_label)));
      $value = views_json_encode_special_chars(trim(views_json_is_date($field_value)));
      if ((is_null($value)) || ($value == '')) continue;
//      if (preg_match('/\d/', $value)) {
//        if (strtotime($value))
//          $value = gmstrftime(EXHIBIT_DATE_FORMAT, strtotime($value));
//      }
      $label = str_replace('_value', '', str_replace("profile_values_profile_", '', $label)); //strip out Profile: from profile fields
      if ($label == 'type') $json = str_replace('##type##', $value, $json);
      elseif ($label == 'label') $json = str_replace('##label##', $value, $json);
      else $json.=str_repeat(" ", 8).'"'.$label.'"'. " ".": ".'"'.$value.'"'.((++$field_count == $total_field_count) ? "":",")."\n";
  	}
    if (strpos($json, '##type##') !== false) 
   	$json = str_replace('##type##', (isset($node->type) ? $node->type : 'Item'), $json);
    if (strpos($json, '##label##') !== false) 
    $json = str_replace('##label##', (isset($node->title) ? $node->title : 'none'), $json);
  	$json.=str_repeat(" ", 6)."}".((++$view_result_count == $total_view_result_count) ? "":",")."\n";
  }
  $json.=str_repeat(" ", 4)."]\n}";
  
  if ($view->override_path) { //inside a live preview so just output the text
  	print $json; 
  }
  else { //real deal so switch the content type and stop further processing of the page
    drupal_set_header('Content-Type: text/javascript');
    print $json;
    module_invoke_all('exit');
    exit;
 }

}
