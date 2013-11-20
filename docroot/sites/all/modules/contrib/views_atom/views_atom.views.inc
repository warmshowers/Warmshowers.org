<?php
// $Id: views_atom.views.inc,v 1.1 2010/09/21 21:36:52 crell Exp $

/**
 * @file
 * Views plugins definition for views_atom
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_atom_views_plugins() {

  $info['style']['atom_fields'] = array(
    'title' => t('Atom Feed - Fields'),
    'help' => t('Outputs a Atom formatted feed'),
    'handler' => 'views_plugin_style_atom_fields',
    'theme' => 'views_view_views_atom_fields',
    'theme file' => 'views_atom.theme.inc',
    'theme path' => drupal_get_path('module', 'views_atom'),
    'path' => drupal_get_path('module', 'views_atom'),
    'uses row plugin' => TRUE,
    'uses fields' => TRUE,
    'uses options' => TRUE,
    'uses grouping' => FALSE,
    'type' => 'feed',
  );

  $info['style']['atom_delete'] = array(
    'title' => t('Atom Feed - Deletion'),
    'help' => t('Outputs a Atom feed of deleted nodes'),
    'handler' => 'views_plugin_style_atom_delete',
    'theme' => 'views_view_views_atom_delete',
    'theme file' => 'views_atom.theme.inc',
    'theme path' => drupal_get_path('module', 'views_atom'),
    'path' => drupal_get_path('module', 'views_atom'),
    'uses row plugin' => FALSE,
    'uses fields' => FALSE,
    'uses options' => FALSE,
    'uses grouping' => FALSE,
    'type' => 'feed',
  );

  $info['row']['rdf'] = array(
    'title' => t('RDF'),
    'help' => t('Displays the fields with an optional template.'),
    'handler' => 'views_plugin_row_rdf',
    'theme' => 'views_view_rdf',
    'theme file' => 'views_atom.theme.inc',
    'theme path' => drupal_get_path('module', 'views_atom'),
    'uses fields' => TRUE,
    'uses options' => FALSE,
    'no ui' => TRUE,
    'type' => 'feed',
  );

  $info['row']['rdf_node'] = array(
    'title' => t('RDF (Nodes)'),
    'help' => t('Displays the fields with an optional template.'),
    'handler' => 'views_plugin_row_rdf_node',
    'theme' => 'views_view_rdf_node',
    'theme file' => 'views_atom.theme.inc',
    'theme path' => drupal_get_path('module', 'views_atom'),
    'uses fields' => TRUE,
    'uses options' => FALSE,
    'type' => 'feed',
    'parent' => 'rdf',
  );

  return $info;
}
