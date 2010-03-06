<?php
// $Id: uc_catalog.inc,v 1.1.2.3 2009/12/29 08:35:13 kiam Exp $

/**
 * @file
 * Integration file for uc_catalog.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function uc_catalog_nodewords_type_id($arg) {
  if ($arg[0] == 'catalog') {
    if (isset($arg[1]) && is_numeric($arg[1])) {
      return array(NODEWORDS_TYPE_TERM, array($arg[1]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
