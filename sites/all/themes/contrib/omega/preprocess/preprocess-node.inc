<?php
// $Id: preprocess-node.inc,v 1.2.2.3 2009/10/24 14:59:26 himerus Exp $
/**
 * Implementation of theme_preprocess_HOOK().
 * Passes varables to the node templates.
 *
 * @return $vars
 */
// Prepare the arrays to handle the classes and ids for the node container.
$vars['node_attributes'] = array();
// Add an id to allow the styling of a specific node.
$vars['node_attributes']['id'] = 'node-' . $vars['type'] . '-' . $vars['nid'];

// Add the default .node class
$vars['node_attributes']['class'][] = 'node';

// Add a class to allow styling of nodes of a specific type.
$vars['node_attributes']['class'][] = $vars['type'];

// Add a class to allow styling based on publish status.
if ($vars['status'] > 0) {
  $vars['node_attributes']['class'][] = 'published';
}
else {
  $vars['node_attributes']['class'][] = 'unpublished';
}

// Add a class to allow styling based on promotion.
if ($vars['promote'] > 0) {
  $vars['node_attributes']['class'][] = 'promoted';
}
else {
  $vars['node_attributes']['class'][] = 'not-promoted';
}

// Add a class to allow styling based on sticky status.
if ($vars['sticky']) {
  $vars['node_attributes']['class'][] = 'sticky';
}
else {
  $vars['node_attributes']['class'][] = 'not-sticky';
}

// Add a class to allow styling based on if a node is showing a teaser or the 
// whole thing.
if ($vars['teaser']) {
  $vars['node_attributes']['class'][] = 'teaser';
}
else {
  $vars['node_attributes']['class'][] = 'full-view';
}

// Add a class to allow styling of nodes being viewed by the author of the 
// node in question.
if ($vars['uid'] == $vars['user']->uid) {
  $vars['node_attributes']['class'][] = 'self-posted';
}

// Add a class to allow styling based on the node author.
$vars['node_attributes']['class'][] = 'author-' . strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $vars['node']->name));

// Add a class to allow styling for zebra striping.
$vars['node_attributes']['class'][] = $vars['zebra'];

// Add a class to make the node container self clearing.
$vars['node_attributes']['class'][] = 'clear-block';

// Crunch all the attributes together into a single string to be applied to 
// the node container.
$vars['attributes'] = theme('render_attributes', $vars['node_attributes']);
