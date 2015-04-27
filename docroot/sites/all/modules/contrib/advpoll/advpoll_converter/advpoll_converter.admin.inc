<?php
/*
 * @file
 * Advanced Poll Converter Module Admin Forms and Functions
 * 
 * Administrative form for converter.
 */

function advpoll_converter_form($form, $form_state) {
  $form = array(
    '#attributes' => array('class' => array('advpoll-converter-form')),
  );
  $form['info'] = array(
    '#title' => t('Advanced Poll Converter'),
    '#description' => t('Select core Drupal polls to convert to Advanced polls. As always with any conversion process, be sure to back up your data.'),
    '#type' => 'fieldset',
    '#tree' => FALSE,
  );
  $polls = _advpoll_converter_get_polls();
  if ($polls) {

    $options = array(0 => t('No'), 1 => t('Yes'));
    $form['info']['advpoll_converter_delete'] = array(
      '#title' => t('Delete source polls?'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => 1,
      '#description' => t('When the process is run, should original polls be deleted?'),
    );

    $form['info']['advpoll_converter_select'] = array(
      '#type' => 'select',
      '#title' => t('Select polls to convert.'),
      '#options' => $polls,
      '#multiple' => TRUE,
      '#description' => t('Select the polls to convert.'),
      '#size' => 10,
    );

    $form['info']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Convert'),
    );
  } 
  else {
    $form['info']['message'] = array(
      '#type' => 'item',
      '#markup' => t('There are currently no polls to convert.'),
    );
  }

  return $form;
}

/*
 * Submit for administrative form
 */

function advpoll_converter_form_submit($form, $form_state) {
  $nids = array();
  $delete = 0;

  if (isset($form_state['values']['advpoll_converter_select'])) {
    $delete = $form_state['values']['advpoll_converter_delete'];
    $selections = $form_state['values']['advpoll_converter_select'];

    if ($selections) {
      foreach ($selections as $nid) {
        $nids[] = $nid;
      }
      _advpoll_converter_create_nodes($nids);

      if ($delete) {
        node_delete_multiple($nids);
      }
    } 
    else {
      form_set_error('advpoll_converter_select', t('Please select at least one poll to convert.'));
    }
  }
}

/*
 * Fetch available polls
 */

function _advpoll_converter_get_polls() {
  $result = db_query("SELECT title, nid FROM {node} WHERE type='poll'");
  $options = array();
  foreach ($result as $record) {
    $options[$record->nid] = $record->title;
  }
  return $options;
}

/*
 * Convert selected polls to Advanced Polls
 */

function _advpoll_converter_create_nodes($nids) {
  $poll_nodes = db_query("SELECT n.nid, 
                                 n.title, 
                                 n.language, 
                                 n.created, 
                                 n.status, 
                                 n.uid,
                                 p.runtime, 
                                 p.active 
                                 FROM {node} n 
                                 JOIN {poll} p ON n.nid = p.nid 
                                 WHERE type='poll' AND n.nid IN (:ids)", 
                                 array(':ids' => $nids));

  foreach ($poll_nodes as $record) {
    //set up base Node
    $node = new stdClass();
    $node->type = 'advpoll';
    node_object_prepare($node);

    $node->language = $record->language;
    $node->title = $record->title;
    $node->uid = $record->uid;
    $node->status = $record->status;
    $record->language = LANGUAGE_NONE;

    // fetch poll's choices
    $poll_choices = db_query("SELECT pc.chid, 
                                     pc.chtext 
                                     FROM {poll_choice} pc 
                                     WHERE pc.nid = :nid", 
                                     array(':nid' => $record->nid));
    $choice_by_id = array();
    $choices = array();

    foreach ($poll_choices as $choice) {
      $id = dechex(time() * rand(5, 50));
      $choice_by_id[$choice->chid] = $id;
      $choices[] = array(
        'choice' => $choice->chtext, 
        'write_in' => 0, 
        'choice_id' => $id,
        );
    }

    $node->advpoll_choice[$record->language] = $choices;
    $node->advpoll_dates[$record->language][0]['value'] = date('Y-m-d 00:00:00', $record->created);

    $end_time = time();
    if ($record->runtime) {
      $end_time = $record->created + $record->runtime;
    } 
    else {
      $end_time = $record->created + 60 * 60 * 24 * 365;
    }

    if ($record->active) {
      $node->advpoll_closed[$record->language][0]['value'] = 'open';
    } 
    else {
      $node->advpoll_closed[$record->language][0]['value'] = 'close';
    }

    $node->advpoll_dates[$record->language][0]['value2'] = date('Y-m-d 00:00:00', $end_time);
    $node->advpoll_max_choices[$record->language][0]['value'] = 1;
    $node->advpoll_options[$record->language][0]['value'] = 'showvotes';
    $node->advpoll_mode[$record->language][0]['value'] = 'normal';
    $node->advpoll_results[$record->language][0]['value'] = 'aftervote';
    $node->advpoll_behavior[$record->language][0]['value'] = 'approval';
    node_save($node);

    // save to get nid of newly created node - needed to be able to record associated votes.
    $nid = $node->nid;

    // update associated votes
    $poll_votes = db_query("SELECT pv.chid, 
                                   pv.uid, 
                                   pv.hostname, 
                                   pv.timestamp 
                                   FROM {poll_vote} pv 
                                   WHERE pv.nid = :nid", 
                                   array(':nid' => $record->nid));

    $votes = array();
    foreach ($poll_votes as $row) {
      $votes = array(
        'entity_type' => 'advpoll',
        'entity_id' => $nid,
        'value' => 1,
        'tag' => $choice_by_id[$row->chid],
        'uid' => $row->uid,
        'vote_source' => $row->hostname,
        'timestamp' => $row->timestamp,
      );

      votingapi_set_votes($votes, $criteria = NULL);
    }
    drupal_set_message(t('A new advanced poll node was created for %title.', array('%title' => $record->title)));
  }
}