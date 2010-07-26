<?php
// $Id: views_xhtml.views.inc,v 1.1.2.8 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Views style plugin to render nodes as XHTML microformats.
 *
 * @see views_plugin_style_xhtml.inc
 * @ingroup views_plugins
 */


/**
 * Implementation of hook_views_plugin().
 */
function views_xhtml_views_plugins() {
	$path = drupal_get_path('module', 'views_xhtml');
  return array(
    'module'  => views_xhtml,
    'style'   => array(                 // declare the views_xhtml style plugin
      'views_xhtml' => array(
        'title'           => ('XHTML data document'),
        'path'            => $path,
        'help'            => ('Displays nodes as XHTML microformats.'),
        'handler'         => 'views_plugin_style_xhtml',
        'theme'           => 'views_views_xhtml_style',
	      'theme file'      => 'views_views_xhtml_style.theme.inc',
	      'theme path'      => $path.'/theme',
        'additional themes' => array ('views_views_xhtml_style_hcard' => 'style', 'views_views_xhtml_style_hcalendar' => 'style'), 
        'uses row plugin' => FALSE,
        'uses fields'     => TRUE,
        'uses options'    => TRUE,
        'type'            => 'normal',
        
      ),
    ),
  );
}

