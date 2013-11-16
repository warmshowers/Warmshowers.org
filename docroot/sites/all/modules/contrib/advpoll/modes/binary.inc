<?php

/**
 * @file
 * Handle binary (true/false) votes.
 */

function advpoll_info_binary() {
  return array(
    'name' => 'binary',
    'name_label' => t('Binary'),
    'description' => t('Vote for or against a number of choices.'),
  );
}

function advpoll_algorithms_binary() {
  return array('plurality' => t('Plurality'));
}

/**
 * Implementation of the view_voting hook for the poll module.
 * 
 * This creates a list of choices to allow the user to vote on choices.
 */
function advpoll_voting_binary_form(&$form_state, $node, $teaser, $page, $status) {
  static $binary_form_count = 0; 
  $form = array(
    '#id' => 'advpoll-voting-binary-form-'. $binary_form_count++,
    '#attributes' => array('class' => 'advpoll-vote'),
  );
  $form['ajax'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => 'ajax'),
  );
  $form['#node'] = $node;

  if ($node->choice) {
    $list = array();
    // If previewing check the format against the current users permissions.
    $check = $node->build_mode == NODE_BUILD_PREVIEW;
    foreach ($node->choice as $i => $choice) {
      // Don't show blank choices or write-in votes if the setting is disabled.
      if ($choice['label'] && ($node->show_writeins || !$choice['writein'])) {
        $list[$i] = _advpoll_choice_markup($choice['label'], $node->format, $check) . ($choice['writein'] ? ' '. t('(write-in)') : '');
      }
    }
    // Add write-in checkbox/radio if write-ins are enabled and user has permission.
    if ($node->writeins && user_access('add write-ins')) {
      $list[$i + 1] = t('(write-in)');
      $form['writein_key'] = array(
        '#type' => 'value',
        '#value' => $i + 1,
      );
    }

    $form['choice'] = array(
      '#options' => $list,
      '#tree' => TRUE,
    );

    $max_choices = $node->build_mode == NODE_BUILD_PREVIEW ? $node->settings['max_choices'] : $node->max_choices; 

    if ($max_choices == 1) {
      // Plurality voting
      $form['choice']['#type'] = 'radios';
      $form['choice']['#default_value'] = -1;
    }
    else {
      // Approval voting
      $form['choice']['#type'] = 'checkboxes';
    }
  }

  // Add write-in text field if write-ins are enabled and user has permission.
  if ($node->writeins && user_access('add write-ins')) {
    $form['writein_choice'] = array(
      '#type' => 'textfield',
      '#title' => t('Write-in vote'),
      '#size' => 25,
    );
  }

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
    '#attributes' => array('class' => 'edit-nid'),
  );

  // Hide vote button if user can't vote and instead display appropriate message.
  if ($node->build_mode != NODE_BUILD_PREVIEW && advpoll_eligible($node) && $status == 'open') {
    static $binary_vote_count = 0;
    $form['vote'] = array(
      '#type' => 'submit',
      '#value' => t('Vote'),
      '#id' => 'edit-vote-binary-'. $binary_vote_count++,
      /* TODO: re-enable this.
      '#ahah' => array(
        'path' => 'advpoll/js_vote',
        'wrapper' => 'vote-choices',
        'method' => 'replace',
        'effect' => 'none',
      ),
      */
    );
  }
  elseif ($node->build_mode == NODE_BUILD_PREVIEW) {
    // Display nothing.
  }
  elseif ($status == 'pending') {
    $form['message']['#value'] = t('This poll opens @time.', array('@time' => format_date($node->start_date)));
  }
  else {
    global $user;
    $login_message = t('<a href="@login">Login</a> to vote in this poll.', array('@login' => url('user/login', array('query' => drupal_get_destination()))));
    $form['message']['#value'] = isset($user->uid) ? t('You are not eligible to vote in this poll.') : $login_message;
  }

  $form['#action'] = url('node/'. $node->nid);
  
  return $form;
}

function advpoll_view_results_binary($node, $teaser, $page) {
  $results = votingapi_select_results(array('content_type' => 'advpoll', 'content_id' => $node->nid));
  $votes = array();
  $output = '';
  foreach ($results as $result) {
    $vote_value = $result['tag'];
    if ($vote_value == '_advpoll') {
      if ($result['function'] == 'total_votes') {
        $total_votes = $result['value'];
      }
    }
    else if (isset($node->choice[$vote_value])) {
      if (!isset($votes[$vote_value])) {
        $votes[$vote_value] = 0;
      }
      $votes[$vote_value] = $result['value'];
    }
  }

  if ($node->choice && $total_votes > 0) {
    // Add in any choices that received no votes.
    foreach ($node->choice as $i => $choice) {
      if (!isset($votes[$i])) {
        $votes[$i] = 0;
      }
    }

    // Sort results by votes, descending.
    arsort($votes);

    // Display results for each possible choice
    foreach ($votes as $i => $count) {
      $choice = $node->choice[$i];
      $percentage = round(100 * $votes[$i] / $total_votes, 0);
      $output .= theme('advpoll_bar', _advpoll_choice_markup($choice['label'], $node->format, FALSE), $percentage, format_plural($count, '1 vote', '@count votes'), $choice);
    }
  }

  return array('results' => $output, 'votes' => $total_votes);
}

function advpoll_calculate_results_binary(&$cache, $node) {
  $result = db_query('SELECT uid, vote_source FROM {votingapi_vote} WHERE content_type = "advpoll" AND content_id = %d AND value_type = "option" GROUP BY uid, vote_source, timestamp', $node->nid);
  $total_votes = 0;
  while ($vote = db_fetch_object($result)) {
    $total_votes++;
  }
  votingapi_add_results(array(array('content_type' => 'advpoll', 'content_id' => $node->nid, 'value_type' => '', 'tag' => '_advpoll', 'function' => 'total_votes', 'value' => $total_votes)));
}

/**
 * Registers the vote as a key for this node using votingapi_set_vote().
 */
function advpoll_voting_binary_form_submit($form, &$form_state) {
  $votes = array();
  $node = $form['#node'];

  // Do submission specific to writeins.
  _advpoll_writeins_voting_form_submit($node, $form_state, $votes, 1);

  if ($node->max_choices == 1) {
    // Plurality voting
    // Ignore write-in choice that has already been taken care of.
    if (!$node->writeins || !isset($form_state['values']['choice'][$form_state['values']['writein_key']])) {
      $vote['value'] = 1;
      $vote['tag'] = $form_state['values']['choice'];
      $vote['value_type'] = 'option';
      $vote['content_id'] = $node->nid;
      $vote['content_type'] = 'advpoll';
      $votes[] = $vote;
    }
  }
  else {
    // Approval voting
    foreach ($form_state['values']['choice'] as $choice => $selected) {
      $vote = array();
      // Ignore write-in choice that has already been taken care of.
      if (!$node->writeins || $choice != $form_state['values']['writein_key']) {
        $vote['value'] = $choice;
        if ($selected) {
          $vote['value_type'] = 'option';
          $vote['tag'] = $choice;
          $vote['value'] = 1;
          $vote['content_id'] = $node->nid;
          $vote['content_type'] = 'advpoll';
          $votes[] = $vote;
        }
      }
    }
  }

  // Need to pass a blank array as the second parameter so that existing votes aren't deleted......
  $results = votingapi_set_votes($votes, array());
  _advpoll_vote_response($node, $form_state);
}

/**
 * Check if the submitted key exists, just to make sure the form is not bypassed.
 *
 * @returns boolean true if the form is valid
 */
function advpoll_voting_binary_form_validate($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);
  $ajax = $form_state['values']['ajax'];
  
  // Check if user is eligible to vote
  if (!advpoll_eligible($node)) {
    _advpoll_form_set_error('choice[', t('You are not allowed to vote in this poll.'), $ajax);
  }
  
  // Check if poll is active
  if (!_advpoll_is_active($node)) {
    _advpoll_form_set_error('choice[', t('This poll is closed.'), $ajax);
  }

  // Whether the write-in option is selected. This is calculated differently for
  // radio buttons and checkboxes.
  $writein_option = FALSE;
  $writein_text = isset($form_state['values']['writein_key']) ? $form_state['values']['writein_choice'] : '';

  // Check if user has already voted
  list($voted, $cancel_vote) = _advpoll_user_voted($node->nid);
  if ($voted) {
    _advpoll_form_set_error('choice[', t('You have already voted in this poll.'), $ajax);
    // Redirect to the current poll node to view the poll result instead of the voting form. This is only
    // initiated for non-Ajax voting.
    drupal_goto('node/'. $node->nid);
  }

  if ($node->max_choices == 1) {
    // Plurality voting
    // Write-ins are enabled, user has permission, and it's the write-in option.
    if ($node->writeins && user_access('add write-ins') && ($form_state['values']['choice'] == $form_state['values']['writein_key'])) {
      // Set the flag to true for additional checks.
      $writein_option = TRUE;
    }
    // The choice is invalid 
    elseif (!isset($node->choice[$form_state['values']['choice']])) {
      // Nothing is selected.
      _advpoll_form_set_error('choice[', t('At least one choice must be selected.'), $ajax);
    }
  }
  else {
    // Approval voting.
    $num_choices = 0;
    foreach ($node->choice as $i => $val) {
      // See if the box is checked.
      if ($form_state['values']['choice'][$i]) {
        $num_choices++;
      }
    }

    // Write-ins are enabled, user has permission, and the write-in box is checked.
    if ($node->writeins && user_access('add write-ins') && $form_state['values']['choice'][$form_state['values']['writein_key']]) {
      // Add one to number of choices for check on min/max boxes checked.
      $num_choices++;
      // Set the flag to true for additional checks.
      $writein_option = TRUE;
    }
  
    // Too many choices ranked.
    if ($node->max_choices != 0 && $num_choices > $node->max_choices) {
      $message = t('%num choices were selected but only %max are allowed.', array('%num' => $num_choices, '%max' => $node->max_choices));
      _advpoll_form_set_error('choice[', $message, $ajax);
    }

    // Not enough choices ranked.
    $min_choices = 1;
    if ($num_choices < $min_choices) {
      _advpoll_form_set_error('choice[', t('At least one choice must be selected.'), $ajax);
    }
  }

  // Do validation specific to writeins.
  _advpoll_writeins_voting_form_validate($node, $writein_option, $writein_text, $ajax);
}

/**
 * Process variables for advpoll-display-binary-form.tpl.php.
 *
 * The variables array contains the following arguments:
 * - $form
 *
 * @see advpoll-display-binary-form.tpl.php
 */
function advpoll_preprocess_advpoll_voting_binary_form(&$variables) {
  $form = &$variables['form'];
  $variables['message'] = drupal_render($form['message']);
  
  // If write-ins are used on this form.
  if (isset($form['writein_choice'])) {
    $variables['writein_choice'] = drupal_render($form['writein_choice']);
  }
  $variables['form_id'] = $form['#id'];

  // List of available choices in the poll.
  $variables['choice_list'] = drupal_render($form['choice']);
  // Take off the annoying colon & endlines that Drupal adds to each title.
  $variables['choice_list'] = preg_replace('/[\n\r]*: <\/label>[\n\r]*/i', '</label>', $variables['choice_list']);
  // All remaining form elements.
  $variables['form_submit'] = drupal_render($form);

}

/**
 * Hook to handle a cancelled vote for a binary poll.
 */
function advpoll_cancel_binary($node, $user_vote) {
  // Remove choice if this was the last vote for a write-in.
  if ($node->writeins) {
    $recalculate = FALSE;
    foreach ($user_vote as $vote) {
      if ($node->choice[$vote->tag]['writein']) {
        // Check if there are any other votes for this write-in.
        $count = db_result(db_query('SELECT COUNT(1) FROM {votingapi_vote} WHERE content_id = %d AND tag = %d', $node->nid, $vote->tag));
        if ($count == 0) {
          // Delete the write-in because no one else voted for it.
          db_query('DELETE FROM {advpoll_choices} WHERE cid = %d', $vote->tag);
          $recalculate = TRUE;
        }
      }
    }
    if ($recalculate) {
      votingapi_recalculate_results('advpoll', $node->nid);
    }
  }
}


function advpoll_votingapi_views_formatters($field = array()) {
  if ($field->field == 'value') {
    return array(
      'advpoll_binary_views_tags_formatter' => t('Advpoll Binary: choice label'),
    );
  }
}

// Display the name of the candidate, not the value of the vote
function advpoll_binary_views_tags_formatter($value, $field, $columns) {
  $node_type = isset($columns->node_type) ? $columns->node_type : db_result(db_query("SELECT type FROM {node} WHERE nid = %d", $columns->nid));
  $result = "";
  if ($node_type == 'advpoll_binary')
    $result = db_result(db_query("SELECT label FROM {advpoll_choices} WHERE nid = %d AND cid = %d", $columns->nid, $columns->votingapi_vote_node_tag));
  return $result;    
}