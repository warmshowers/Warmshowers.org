<?php
/**
 * @file
 * Integration file for node.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function node_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'node') {
    // Node paths: node/$nid.
    if (isset($arg[1]) && is_numeric($arg[1]) && ($node = menu_get_object())) {

      $bool = (
        (
          $node->type == 'poll' &&
          isset($arg[2]) &&
          in_array($arg[2], array('edit', 'results'))
        ) ||
        !isset($arg[2])
      );

      if ($bool) {
        $result['type'] = NODEWORDS_TYPE_NODE;
        $result['id'] = $node->nid;
      }
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
