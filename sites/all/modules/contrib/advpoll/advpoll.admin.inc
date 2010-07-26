<?php
// $Id: advpoll.admin.inc,v 1.1.2.3 2010/06/24 22:21:30 mirodietiker Exp $

/**
 * @file
 * Page callbacks for the advpoll module.
 */

/**
 * Remove an individual voter from the electoral list.
 */
function advpoll_remove_voter() {
  $nid = arg(1);
  $uid = arg(3);
  if ($uid && $node = node_load($nid)) {
    $result = db_query('SELECT name FROM {users} WHERE uid = %d', $uid);
    if ($user = db_fetch_object($result)) {
      db_query('DELETE FROM {advpoll_electoral_list} WHERE nid = %d AND uid = %d', $nid, $uid);
      drupal_set_message(t('%user removed from the electoral list.', array('%user' => $user->name)));
    }
    else {
      drupal_set_message(t('No user found with a uid of %uid.', array('%uid' => $uid)));
    }
  }
  drupal_goto('node/'. $node->nid .'/electoral_list');
}


/**
 * Callback for canceling a vote.
 */
function advpoll_cancel($node) {
  global $user;
  if (isset($node->type) && $node->voted && _advpoll_is_active($node)) {
    if ($user->uid && count($user_votes = votingapi_select_votes(array('content_type' => 'advpoll', 'content_id' => $node->nid, 'uid' => $user->uid))) > 0) {
      votingapi_delete_votes($user_votes);
    }
    else {
      $user_votes = votingapi_select_votes(array('content_type' => 'advpoll', 'content_id' => $node->nid, 'vote_source' => ip_address()));
      votingapi_delete_votes($user_votes);
    }

    $mode = _advpoll_get_mode($node->type);
    $function = 'advpoll_cancel_'. $mode;
    if (function_exists($function)) {
      $function($node, $user_votes);
    }
    votingapi_recalculate_results('advpoll', $node->nid);
    drupal_set_message(t('Your vote was canceled.'));
  }
  else {
    drupal_set_message(t('You are not allowed to cancel an invalid choice.'), 'error');
  }
  drupal_goto('node/'. $node->nid);
}
