<?php
// $Id: views_json.views.inc,v 1.1.2.10 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Views style plugin to render nodes in the JSON data format.
 *
 * @see views_plugin_style_json.inc
 * @ingroup views_plugins
 */


/**
 * Implementation of hook_views_plugin().
 */
function views_json_views_plugins() {
  $path = drupal_get_path('module', 'views_json');
	return array(
    'module'  => 'views_json',
      'style'   => array(                 // Declare the views_json style plugin.
        'views_json'      => array(
          'title'         => ('JSON data document'),
	        'path'          => $path,
	        'help'          => ('Displays nodes in the JSON data format.'), 
          'handler'         => 'views_plugin_style_json',
		      'theme'           => 'views_views_json_style',
	        'theme file'      => 'views_views_json_style.theme.inc',
	        'theme path'      => $path.'/theme',
	        'additional themes' => array ('views_views_json_style_simple' => 'style', 'views_views_json_style_exhibit' => 'style'),
          'uses row plugin' => FALSE,
          'uses fields'     => TRUE,
          'uses options'    => TRUE,
          'type'            => 'normal',
	        'help_topic'      => 'style-json',
	        'even empty'      => TRUE
      ),
    ),
  );
}
