<?php
// $Id: forum.inc,v 1.1.4.3 2009/11/19 10:17:11 kiam Exp $

/**
 * @file
 * Integration file for forum.module.
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
function forum_nodewords_type_id($arg) {
  if ($arg[0] == 'forum') {
    // Forum paths: forum/$tid , forum/
    if (!isset($arg[1])) {
      return array(NODEWORDS_TYPE_VOCABULARY, array(variable_get('forum_nav_vocabulary', 0)));
    }
    elseif (is_numeric($arg[1])) {
      return array(NODEWORDS_TYPE_TERM, array($arg[1]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
