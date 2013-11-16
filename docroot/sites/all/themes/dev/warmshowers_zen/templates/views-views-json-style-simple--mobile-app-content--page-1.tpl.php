<?php
/**
 * @file views-views-json-style-simple.tpl.php
 * Default template for the Views JSON style plugin using the simple format
 *
 * Variables:
 * - $view: The View object.
 * - $rows: Hierachial array of key=>value pairs to convert to JSON
 * - $options: Array of options for this style
 *
 * @ingroup views_templates
 */

$jsonp_prefix = $options['jsonp_prefix'];

// HACK because I don't seem to be able to theme the date. This data is
// to be plain json, but the date should be a unix timestamp, and with
// 'raw' on for the json, the regular theming does not get respected.
// It gets executed, but the result gets discarded. So doing this transformation
// from 2012-09-01T05:00 to a timestamp here.
foreach ($rows['recommendations'] as &$item) {
  if (!empty($item['recommendation']['field_hosting_date_value'])) {
    $item['recommendation']['field_hosting_date_value'] = strtotime($item['recommendation']['field_hosting_date_value']);
  }
}

if ($view->override_path) {
	// We're inside a live preview where the JSON is pretty-printed.
	$json = _views_json_encode_formatted($rows);
	if ($jsonp_prefix) $json = "$jsonp_prefix($json)";
	print "<code>$json</code>";
}
else {
  $json = json_encode($rows);
  if ($jsonp_prefix) $json = "$jsonp_prefix($json)";
  if ($options['using_views_api_mode']) {
    // We're in Views API mode.
    print $json;
  }
  else {
    // We want to send the JSON as a server response so switch the content
    // type and stop further processing of the page.
    $content_type = ($options['content_type'] == 'default') ? 'application/json' : $options['content_type'];
    drupal_set_header("Content-Type: $content_type; charset=utf-8");
    print $json;
    //Don't think this is needed in .tpl.php files: module_invoke_all('exit');
    exit;
  }
}

