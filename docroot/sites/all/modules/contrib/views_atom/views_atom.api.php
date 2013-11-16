<?php

/**
 * Prepare an entity to be rendered into Drupal-RDF.
 *
 * This hook allows modules to alter an entity to add additional data before it is
 * processed into an XML string in Drupal's custom RDF format.  Modifications made
 * to the entity will not be saved (at least not by the views_atom module).
 *
 * In Drupal 6 the entity will always be a node, but hopefully in Drupal 7 it can
 * become any entity.
 *
 * @param $entity
 *   The entity to prepare.
 */
function hook_views_atom_prerender($entity) {

}

/**
 * Prepare a node to be rendered into Drupal-RDF.
 *
 * This hook allows modules to alter a node to add additional data before it is
 * processed into an XML string in Drupal's custom RDF format.  Modifications made
 * to the node will not be saved (at least not by the views_atom module).
 *
 * @param $node
 *   The node to prepare.
 */
function hook_views_atom_prerender_node($node) {

}

/**
 * Render an entity into XML in Drupal-RDF format.
 *
 * This hook allows modules to add to the XML representation of an entity.
 *
 * @param $entity
 *   The entity that is being rendered.  In Drupal 6 this is always a node. In
 *   Drupal 7, hopefully, it could be any registered entity.
 * @param $xml
 *   A SimpleXML object representing the output XML.  It will be rendered down
 *   into a string later by views_atom.
 */
function hook_views_atom_render($entity, $xml) {

}

/**
 * Render a node into XML in Drupal-RDF format.
 *
 * This hook allows modules to add to the XML representation of a node.  It is
 * identical to hook_views_atom_render() except that it applies only to nodes.
 * In Drupal 6 that means nothing, but it's here for future extensibility.
 *
 * @param $node
 *   The node that is being rendered.
 * @param $xml
 *   A SimpleXML object representing the output XML.  It will be rendered down
 *   into a string later by views_atom.
 */
function hook_views_atom_render_node($node, $xml) {

}
