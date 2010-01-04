<?php
// $Id: comment_service.inc,v 1.1.2.1.2.2 2009/12/05 01:42:38 heyrocker Exp $

/**
 * @file
 *  Link commenting functionality to services module.
 */

/**
 * Adds a new comment to a node and returns the cid.
 *
 * If a cid is specified in $comment->cid then that comment is edited.
 *
 * @param $comment
 *   An array matching the form values that would be submitted in the comment
 *   edit form.
 * @return
 *   Unique identifier for the comment (cid) or FALSE if there was a problem.
 */
function comment_service_save($comment) {
  // If the submitted comment does not contain a uid, set it to anonymous.
  if (isset($comment->uid)) {
    $uid = 0;
  }

  // If the submitted comment does not contain a nid, then return an error.
  if (!isset($comment->nid)) {
    return services_error(t("No node specified."));
  }

  // If a cid is present then the user is trying to edit an existing comment.
  if (isset($comment->cid) && ($comment->cid != 0)) {
    $initial_comment = _comment_load($comment->cid);
    $admin = user_access("administer comments");

    if ($initial_comment->uid == 0 && !$admin) {
      return services_error(t("Anonymous comments can't be edited"));
    }

    if (($initial_comment->uid != $comment->uid || comment_num_replies($comment->cid) != 0) && !$admin) {
      return services_error(t("User does not have permission to edit this comment"));
    }
  }

  // Can I just make a note here about how stupid it is that comment_load returns
  // an object but comment_save expects an array?
  return comment_save((array) $comment);
}

/**
 * Returns the comments of a specified node.
 *
 * @param $nid
 *   Unique identifier for the node.
 * @param $count
 *   Number of comments to return.
 * @param $start
 *   Which comment to start with. if present, $start and $count are used together
 *   to create a LIMIT clause for selecting comments. This could be used to do paging.
 * @return
 *   An array of comment objects.
 */
function comment_service_load_node_comments($nid, $count = 0, $start = 0) {
  $comments = array();
  $limit = ((int)$count > 0 ? ' LIMIT '. (int)$start .', '. (int)$count .' ' : '');

  $result = db_query("SELECT cid FROM {comments} WHERE nid = %d ORDER BY thread DESC". $limit, $nid);
  while ($comment = db_fetch_array($result)) {
    $comments[] = _comment_load($comment['cid']);
  }

  return $comments;
}

/**
 * Returns a specified comment
 *
 * @param $cid
 *   Unique identifier for the specified comment
 * @return
 *   The comment object
 */
function comment_service_load($cid) {
  $cid = db_result(db_query("SELECT cid FROM {comments} WHERE cid = %d", $cid));
  return _comment_load($cid);
}

/**
 * Returns the number of comments on a given node id.
 *
 * @param $nid
 *   Unique identifier for the specified node.
 * @return
 *   Number of comments that node has.
 */
function comment_service_node_comments_count_all($nid) {
  return comment_num_all($nid);
}

/**
 * Returns the number of new comments on a given node id since timestamp.
 *
 * @param $nid
 *   Unique identifier for the specified node.
 * @param $since
 *   Timestamp to indicate what nodes are new. Defaults to time of last user acces to node.
 * @return
 *   Number of comments that node has.
 */
function comment_service_node_comments_count_new($nid, $since = 0) {
  return comment_num_new($nid, $since);
}
