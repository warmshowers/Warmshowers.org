<?php
// $Id: user.inc,v 1.1.4.6 2009/12/30 18:00:51 kiam Exp $

/**
 * @file
 * Integration file for user.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function user_nodewords_type_id($arg) {
  if ($arg[0] == 'user') {
    // User page paths: user/$uid.
    if (isset($arg[1]) && is_numeric($arg[1])) {
      if (isset($arg[2]) && $arg[2] == 'track') {
        return array(NODEWORDS_TYPE_TRACKER, array(0));
      }

      return array(NODEWORDS_TYPE_USER, array($arg[1]));
    }
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
