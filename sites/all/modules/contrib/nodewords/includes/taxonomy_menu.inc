<?php
// $Id: taxonomy_menu.inc,v 1.1.4.3 2009/11/19 10:17:11 kiam Exp $

/**
 * @file
 * Integration file for taxonomy_menu.module.
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
function taxonomy_menu_nodewords_type_id($arg) {
  if ($arg[0] == 'taxonomy_menu') {
    // Taxonomy menu paths: taxonomy_menu/$vid, taxonomy_menu/$vid/$tid
    if (isset($arg[2]) && is_numeric($arg[2])) {
      return array(NODEWORDS_TYPE_TERM, array($arg[2]));
    }
    elseif (isset($arg[1]) && is_numeric($arg[1])) {
      return array(NODEWORDS_TYPE_VOCABULARY, array($arg[1]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
