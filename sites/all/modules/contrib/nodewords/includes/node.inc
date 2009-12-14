<?php
// $Id: node.inc,v 1.1.4.5 2009/12/05 22:30:22 kiam Exp $

/**
 * @file
 * Integration file for node.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/*****************************************************************************
 * Drupal hooks.
 ****************************************************************************/

/**
 * Implementation of hook_nodewords_type_id().
 */
function node_nodewords_type_id($arg) {
  if ($arg[0] == 'node') {
    // Node paths: node/$nid
    if (isset($arg[1]) && is_numeric($arg[1])) {
      if (!($node = menu_get_object())) {
        return array(NODEWORDS_TYPE_NONE, '');
      }

      // If the meta tags for the content type are disabled, don't show them.
      if (!variable_get('nodewords_edit_metatags_' . $node->type, TRUE)) {
        return array(NODEWORDS_TYPE_NONE, '');
      }

      $bool = (
        (
          $node->type == 'poll' &&
          isset($arg[2]) &&
          in_array($arg[2], array('edit', 'results'))
        ) ||
        !isset($arg[2])
      );

      if ($bool) {
        return array(NODEWORDS_TYPE_NODE, array($node->nid));
      }
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
