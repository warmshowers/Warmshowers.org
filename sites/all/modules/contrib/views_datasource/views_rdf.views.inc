<?php
// $Id: views_rdf.views.inc,v 1.1.2.8 2010/06/06 00:14:24 allisterbeharry Exp $

/**
 * @file
 * Views style plugin to render nodes in the RDF data format.
 *
 * @see views-view-rdf.tpl.php
 * @ingroup views_plugins
 */


/**
 * Implementation of hook_views_plugin().
 */
function views_rdf_views_plugins() {
	$path = drupal_get_path('module', 'views_rdf');
  return array(
    'module'  => 'views_rdf',
    'style'   => array(                 // declare the views_rdf style plugin
      'views_rdf' => array(
        'title'           => ('RDF data document'),
        'path'            => $path,
        'help'            => ('Displays nodes in a view using the RDF data format.'),
        'handler'         => 'views_plugin_style_rdf',
        'theme'           => 'views_views_rdf_style',
  	    'theme file'      => 'views_views_rdf_style.theme.inc',
	      'theme path'      => $path.'/theme',
        'uses row plugin' => FALSE,
        'uses fields'     => TRUE,
        'uses options'    => TRUE,
        'type'            => 'normal',
  	    'help_topic'      => 'style-rdf',
	      'even empty'      => TRUE
      ),
    ),
  );
}

