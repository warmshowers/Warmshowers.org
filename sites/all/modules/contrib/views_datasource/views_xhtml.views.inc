<?php
//$Id: views_xhtml.views.inc,v 1.1.2.3 2009/09/08 16:37:48 allisterbeharry Exp $
/**
 * @file views_xhtml.views.inc 
 * Views style plugin to render nodes as XHTML microformats.
 * @see views-view-xhtml.tpl.php
 * @ingroup views_plugins
 */

/**
 * Implementation of hook_views_plugin
 *
 */
function views_xhtml_views_plugins() {
  return array(
    'module' => views_xhtml,
	'style' => array( //declare the views_xhtml style plugin
      'views_xhtml' => array(
        'title' => t('XHTML data document'),
        'theme' => 'views_view_xhtml',
        'help' => t('Displays nodes as XHTML microformats.'),
        'handler' => 'views_plugin_style_xhtml',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',       
      ),
    ),
  );
}

/**
 * Theme preprocess function for views-view-xhtml.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_views_view_xhtml(&$vars) {
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
}