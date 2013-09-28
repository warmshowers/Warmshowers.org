<?php
// $Id: views_plugin_row_rdf_node.inc,v 1.1 2010/09/21 21:36:52 crell Exp $

/**
 * @file
 * Contains the RDF row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object
 * and formats it as an RSS item.
 */
class views_plugin_row_rdf_node extends views_plugin_row_rdf {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  function render($row) {
    // For the most part, this code is taken from node_feed() in node.module
    global $base_url;

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    $node = node_load($nid);

    // Allow other modules to modify the node before we start serializing it.
    module_invoke_all('views_atom_prerender', $node);
    module_invoke_all('views_atom_prerender_node', $node);

    // We need to start with a DOMDocument because SimpleXML's namespace handling
    // is abysmal.  Fortunately we can swap back and forth as needed.
    // @todo: Add more common namespaces here if needed.
    $dom = new DOMDocument('1.0');
    $rdf_xml = $dom->createElementNS('http://www.w3.org/1999/02/22-rdf-syntax-ns', 'rdf:RDF');
    $dom->appendChild($rdf_xml);

    // Get a SimpleXML version of the object for serious manipulation, since
    // its API is much easier to work with.
    $xml = simplexml_import_dom($dom);

    // Create the base entity element that wraps all Drupal data objects.
    $entity_xml = $xml->addChild('entity', NULL, 'http://drupal.org/');

    // Node type is more generically "bundle", at least in Drupal 7 speak.
    $entity_xml->addAttribute('type', 'node');
    $entity_xml->addAttribute('bundle', check_plain($node->type));

    $entity_xml->addChild('title', views_atom_sanitize($node->title));

    // Properties are primitive data associated with the entity.  These are
    // standard by entity type.  Those all get wrapped in a <properties> element.
    $properties_xml = $entity_xml->addChild('properties');


    // Since we don't have a complete map of available properties, we'll just
    // treat any primitive values on the node object as properties.  That will
    // get more than we need, but it works well enough.
    foreach ($node as $property => $value) {
      if (is_scalar($value) && !empty($value)) {
        // We want to map the date fields to Atom/RDF format properly.  The others
        // we'll just send along as is.
        if (in_array($property, array('created', 'changed'))) {
          $properties_xml->addChild($property, date('c', $value));
        }
        else {
          $properties_xml->addChild($property, views_atom_sanitize($value));
        }
      }
    }

    // Allow other modules the opportunity to get in on the XML conversion action.
    module_invoke_all('views_atom_render', $node, $entity_xml);
    module_invoke_all('views_atom_render_node', $entity_xml);

    drupal_alter('views_atom_render_rdf', $entity_xml, $node);

    // Force pretty formatting to make debugging easier.  It doesn't actually
    // affect parsing.  Note that because all objects in PHP are references
    // this $dom variable still has all of the changes made above using SimpleXML.
    // That's neat.
    $dom->formatOutput = TRUE;

    // This is stupid and bad, but I don't know of another way to supress the
    // <?xml crap.
    // @todo Find a better way to not get the XML declaration in the output.
    $output = $dom->saveXML();
    return str_replace('<?xml version="1.0"?>' . "\n", '', $output);
  }
}
