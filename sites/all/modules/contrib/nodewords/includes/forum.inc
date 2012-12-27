<?php
/**
 * @file
 * Integration file for forum.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function forum_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'forum') {
    // Forum paths: forum/$tid , forum/.
    if (!isset($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_VOCABULARY;
      $result['id'] = variable_get('forum_nav_vocabulary', 0);
    }
    elseif (is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
