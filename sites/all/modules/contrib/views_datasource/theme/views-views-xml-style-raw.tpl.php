<?php
// $Id: views-views-xml-style-raw.tpl.php,v 1.1.2.6 2010/06/07 03:27:07 allisterbeharry Exp $
/**
 * @file views-views-xml-style-raw.tpl.php
 * Default template for the Views XML style plugin using the raw schema
 *
 * Variables
 * - $view: The View object.
 * - $rows: Array of row objects as rendered by _views_xml_render_fields 
 *
 * @ingroup views_templates
 * @see views_views_xml_style.theme.inc
 */	
	if ($view->override_path) {       // inside live preview
    print htmlspecialchars($xml);
  }
  else if ($options['using_views_api_mode']) {     // We're in Views API mode.
    print $xml;
  }
  else {
  	drupal_set_header("Content-Type: $content_type; charset=utf-8");
    print $xml;
    exit;
  }