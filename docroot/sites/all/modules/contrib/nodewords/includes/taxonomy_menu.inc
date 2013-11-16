<?php
/**
 * @file
 * Integration file for taxonomy_menu.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function taxonomy_menu_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'taxonomy_menu') {
    // Taxonomy menu paths: taxonomy_menu/$vid, taxonomy_menu/$vid/$tid.
    if (isset($arg[2]) && is_numeric($arg[2])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[2];
    }
    elseif (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_VOCABULARY;
      $result['id'] = $arg[1];
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
