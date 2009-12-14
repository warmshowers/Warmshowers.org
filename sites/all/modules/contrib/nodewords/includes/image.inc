<?php
// $Id: image.inc,v 1.1.4.3 2009/11/19 10:17:11 kiam Exp $

/**
 * @file
 * Integration file for image.module.
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
function image_nodewords_type_id($arg) {
  if ($arg[0] == 'image') {
    // Image gallery paths: image/ , image/???/$tid
    if (!isset($arg[1])) {
      return array(NODEWORDS_TYPE_VOCABULARY, array(variable_get('image_gallery_nav_vocabulary', 0)));
    }
    elseif (isset($arg[2]) && is_numeric($arg[2])) {
      return array(NODEWORDS_TYPE_TERM, array($arg[2]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
