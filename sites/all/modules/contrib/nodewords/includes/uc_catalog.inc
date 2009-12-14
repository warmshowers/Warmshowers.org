<?php
// $Id: uc_catalog.inc,v 1.1.2.1 2009/11/19 10:17:11 kiam Exp $

/**
 * @file
 * Integration file for uc_catalog.module.
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
function uc_catalog_nodewords_type_id($arg) {
  if ($arg[0] == 'catalog') {
    // Taxonomy menu paths: taxonomy_menu/$vid, taxonomy_menu/$vid/$tid
    if (isset($arg[1]) && is_numeric($arg[1])) {
      return array(NODEWORDS_TYPE_TERM, array($arg[1]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
