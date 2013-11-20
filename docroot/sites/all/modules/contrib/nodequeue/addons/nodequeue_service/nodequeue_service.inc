<?php

/**
 * @file
 * Miscellaneous functions for Nodequeue Service.
 *
 * @author Greg Harvey - http://www.drupaler.co.uk
 */

/**
 * Returns an array of available nodequeue "channels" available.
 *
 * @return
 *   Array with key of nodequeue ids and all settings within.
 */
function nodequeue_service_get_nodequeues() {
  if (module_exists('nodequeue')) {
    // Fetch all of the queues.
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(0));
    if (empty($queues)) {
      return services_error(t('Nodequeue available, but no nodequeues exist'));
    } 
    else {
      $availablequeues = array();
      foreach ($queues as $queue) {
        $availablequeues[$queue->qid]['qid'] = $queue->qid;
        $availablequeues[$queue->qid]['title'] = $queue->title;
        $availablequeues[$queue->qid]['subqueue_title'] = $queue->subqueue_title;
        $availablequeues[$queue->qid]['size'] = $queue->size;
        $availablequeues[$queue->qid]['link'] = $queue->link;
        $availablequeues[$queue->qid]['link_remove'] = $queue->link_remove;
        $availablequeues[$queue->qid]['owner'] = $queue->owner;
        $availablequeues[$queue->qid]['show_in_ui'] = $queue->show_in_ui;
        $availablequeues[$queue->qid]['show_in_tab'] = $queue->show_in_tab;
        $availablequeues[$queue->qid]['show_in_links'] = $queue->show_in_links;
        $availablequeues[$queue->qid]['reference'] = $queue->reference;
        $availablequeues[$queue->qid]['reverse'] = $queue->reverse;
        $availablequeues[$queue->qid]['subqueues'] = $queue->subqueues;
        $availablequeues[$queue->qid]['types'] = $queue->types;
        $availablequeues[$queue->qid]['roles'] = $queue->roles;
        $availablequeues[$queue->qid]['count'] = $queue->count;
      }
    }
    return $availablequeues;
  }
  else {
    return services_error(t('Nodequeue module unavailable.'));
  }
}

/**
 * Returns an array of available subqueues by parent queue id.
 *
 * @param $qid
 *   Integer representing the queue id of the parent nodequeue.
 *
 * @return
 *   Array with key of subqueue ids and all settings within.
 */
function nodequeue_service_get_subqueues($qid) {
  if (module_exists('nodequeue')) {
    $subqueues = nodequeue_load_subqueues_by_queue($qid);
    if (!empty($subqueues)) {
      foreach ($subqueues as $subqueue) {
        $availablequeues[$subqueue->sqid]['sqid'] = $subqueue->sqid;
        $availablequeues[$subqueue->sqid]['qid'] = $qid;
        $availablequeues[$subqueue->sqid]['reference'] = $subqueue->reference;
        $availablequeues[$subqueue->sqid]['title'] = check_plain($subqueue->title);
        $availablequeues[$subqueue->sqid]['count'] = $subqueue->count;
      }
      return $availablequeues;
    }
    else {
      return services_error(t('Specified nodequeue has no subqueues.'));
    }
  }
  else {
    return services_error(t('Nodequeue module unavailable.'));
  }
}

function nodequeue_service_save_subqueue($subqueue) {
  $subqueue = (object) $subqueue;
  $is_basic = FALSE; // Whether the node queue is just a standard queue with a single subqueue
  if (isset($subqueue->queue_name)) {
    // We must get the qid by the machine name of the queue
    // We also need to know the owner of the queue to determine what the refernce value will refer to.
    $result = db_query("SELECT qid, owner FROM {nodequeue_queue} WHERE name = '%s'", $subqueue->queue_name);
    if ($row = db_fetch_array($result)) {
      $qid = $row['qid'];
      $is_basic = $row['owner'] == 'nodequeue';
    }
    else {
      return services_error(t('Specified node queue does not exist.'));
    }
  }
  elseif (isset($subqueue->qid)) {
    $qid = $subqueue->qid;
    $owner = db_result(db_query("SELECT owner from {nodequeue_queue} WHERE qid = %d", $qid));
    if (!$owner) {
      return services_error(t('Specified node queue does not exist.'));
    }
    $is_basic = $owner == 'nodequeue';
  }
  else {
    return services_error(t('No queue was specified.'));
  }

  // If this is a basic nodequeue, then we should use the local qid
  // as our reference when looking for the sqid.
  $ref = $is_basic ? $qid : $subqueue->reference;

  // Get the local sqid for this subqueue.
  $sqid = db_result(db_query_range("SELECT sqid FROM {nodequeue_subqueue} WHERE qid = %d AND reference = '%s'", $qid, $ref, 0, 1));
  if (!$sqid) {
    return services_error(t('Specified subqueue does not exist.'));
  }
  $saved = nodequeue_save_subqueue_order($subqueue->nodes, $qid, $sqid);
  if ($saved[0] != NODEQUEUE_OK) {
    return services_error(t('Invalid subqueue order, could not be saved.'));
  }
  return $sqid;
}

/**
 * Access callback function for the subqueue.save service.
 */
function nodequeue_service_save_subqueue_access() {
  if (user_access('manipulate all queues')) {
    return TRUE;
  }
}

/**
 * Access callback function for the nodequeue service.
 */
function nodequeue_service_get_nodequeues_access() {
  if (user_access('get nodequeues')) {
    return TRUE;
  }
}
