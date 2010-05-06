<?php
//$Id: views_xml.views.inc,v 1.1.2.3 2009/09/08 16:37:48 allisterbeharry Exp $
/**
 * @file views_xml.views.inc 
 * Views style plugins to render nodes in the XML data format.
 * @see views-view-xml.tpl.php
 * @ingroup views_plugins
 */

/**
 * Implementation of hook_views_plugin
 *
 */
function views_xml_views_plugins() {
  return array(
    'module' => views_xml,
	'style' => array( //declare the views_xml_* style plugins
      'views_xml' => array(
        'title' => t('XML data document'),
        'theme' => 'views_view_xml',
        'help' => t('Displays nodes as XML.'),
        'handler' => 'views_plugin_style_xml',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',       
      ),
    ),
  );
}


/**
 * Theme preprocess function for views-view-xml.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_views_view_xml(&$vars) {
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
  $result = array();  
}

