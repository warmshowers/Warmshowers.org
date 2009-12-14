<?php
// $Id: tracker.inc,v 1.1.4.5 2009/12/05 22:44:28 kiam Exp $

/**
 * @file
 * Integration file for tracker.module.
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
function tracker_nodewords_type_id($arg) {
  if ($arg[0] == 'tracker') {
    return array(NODEWORDS_TYPE_TRACKER, array(''));
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
