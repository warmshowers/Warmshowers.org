<?php
//$Id: views_json.views.inc,v 1.1.2.3 2009/09/08 16:37:48 allisterbeharry Exp $
/**
 * @file views_json.views.inc 
 * Views style plugin to render nodes in the JSON data format.
 * @see views-view-json.tpl.php views-view-row-unformatted.tpl.php
 * @ingroup views_plugins
 */

/**
 * Implementation of hook_views_plugin
 *
 */
function views_json_views_plugins() {
  return array(
    'module' => 'views_json',
    'style' => array( //declare the views_json style plugin
      'views_json' => array(
        'title' => t('JSON data document'),
        'theme' => 'views_view_json',
        'help' => t('Displays nodes in the JSON data format.'),
        'handler' => 'views_plugin_style_json',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',       
      ),
    ),
  );
}

/**
 * Theme preprocess function for views-view-json.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_views_view_json(&$vars) {
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
}