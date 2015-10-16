<?php

/**
 * @file
 * XML Site-map behavior.
 *
 * Updates node type in xmlsitemap table.
 */

/**
 * Implementation of node_convert_change().
 */
function xmlsitemap_node_convert_change($data, $op) {
  if ($op == 'insert') {
    $node = $data['node'];

    // Change the node type stored in the xmlsitemap table.
    $link = xmlsitemap_link_load('node', $node->nid);
    $link['subtype'] = $data['dest_node_type'];
    xmlsitemap_link_save($link);
  }
}
