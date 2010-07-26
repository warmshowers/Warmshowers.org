<?php
// $Id: views_xml.views.inc,v 1.1.2.9 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Views style plugins to render nodes in the XML data format.
 *
 * @see views_plugin_style_xml.inc
 * @ingroup views_plugins
 */


/**
 * Implementation of hook_views_plugin().
 */
function views_xml_views_plugins() {
  $path = drupal_get_path('module', 'views_xml'); 
	return array(
	   'module' => 'views_xml',
      'style'   => array(                 // declare the views_xml_* style plugins
        'views_xml_style' => array(
          'title'           => ('XML data document'),
	        'path'            => $path,
          'help'            => ('Display a view as an XML document.'),
          'handler'         => 'views_plugin_style_xml',
	        'theme'           => 'views_views_xml_style',
	        'theme file'      => 'views_views_xml_style.theme.inc',
	        'theme path'      => $path.'/theme',
	        'additional themes' => array ('views_views_xml_style_raw' => 'style', 'views_views_xml_style_opml' => 'style', 'views_views_xml_style_atom' => 'style'),
          'uses row plugin' => FALSE,
          'uses fields'     => TRUE,
          'uses options'    => TRUE,
	        'type'            => 'normal',
	        'help_topic'      => 'style-xml',
	        'even empty'      => TRUE
       ),
     ),
    );
}
