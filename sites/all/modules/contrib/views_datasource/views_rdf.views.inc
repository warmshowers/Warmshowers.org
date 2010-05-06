<?php
//$Id: views_rdf.views.inc,v 1.1.2.3 2009/09/08 16:37:48 allisterbeharry Exp $
/**
 * @file views_rdf.views.inc 
 * Views style plugin to render nodes in the RDF data format.
 * @see views-view-rdf.tpl.php
 * @ingroup views_plugins
 */

/**
 * Implementation of hook_views_plugin
 *
 */
function views_rdf_views_plugins() {
  return array(
    'module' => views_rdf,
	'style' => array( //declare the views_rdf style plugin
      'views_rdf' => array(
        'title' => t('RDF data document'),
        'theme' => 'views_view_rdf',
        'help' => t('Displays nodes using the RDF data format.'),
        'handler' => 'views_plugin_style_rdf',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',       
      ),
    ),
  );
}

/**
 * Theme preprocess function for views-view-rdf.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_views_view_rdf(&$vars) {
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
}