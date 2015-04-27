<?php

/**
 * @file
 * Views plugins definition for views_atom
 */

/**
 * Implements hook_views_plugins().
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
    'uses options' => TRUE,
    'uses grouping' => FALSE,
    'type' => 'feed',
  );


  // @todo, where is this plugin used?
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

/**
 * Implements hook_views_data().
 */
function views_atom_views_data() {
  $data = array();
  $data['views_atom']['table']['group'] = t('Global');
  $data['views_atom']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );

  $data['views_atom']['views_atom_atom_tombstone'] = array(
    'title' => t('Views Atom Atom Tombstone'),
    'help' => t('Provide a tombstone for a given id'),
    'area' => array(
      'handler' => 'views_atom_handler_area_atom_tombstone',
    ),
  );

  return $data;
}
