<?php
// $Id: ranking.inc,v 1.8.2.37.2.12 2010/06/25 20:12:41 mirodietiker Exp $

/**
 * @file
 * Handle ranking votes, e.g. choice A is preferred over choice B, which in turn is preferred over choice C.
 */

function advpoll_info_ranking() {
  return array(
    'name' => 'ranking',
    'name_label' => t('Ranking'),
    'description' => t('Rank a number of choices.'),
  );
}

function advpoll_algorithms_ranking() {
  return array(
    'borda_count' => t('Borda count'),
    'instant_runoff' => t('Instant runoff'),
  );
}

function advpoll_voting_ranking_form(&$form_state, &$node, $teaser, $page, $status) {
  static $ranking_form_count = 0; 
  
  $form = array(
    '#id' => 'advpoll_voting_ranking_form-'. $ranking_form_count++,
    '#attributes' => array('class' => 'advpoll-vote'),
    '#node' => $node,
  );
  
  // Add write-in select box if write-ins are enabled and user has permission.
  $handle_writeins = $node->writeins && user_access('add write-ins');

  $form['ajax'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => 'ajax'),
  );

  $form['#attributes']['class'] .= ' drag-and-drop';
  $form['js_order'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );
  if ($node->max_choices) {
    $max_choices = $node->max_choices;
  }
  else {
    $max_choices = count($node->choice) - $node->writein_choices;
    /*if ($handle_writeins) {
      $max_choices++;
    }
    */
  }
  $form['max_choices'] = array(
    '#type' => 'hidden',
    '#value' => $max_choices,
  );

  // TODO: figure out why this is here. shouldn't every poll have choices?
  if (isset($node->choice)) {
    $list = array();
    $num_choices = count($node->choice);

    // Generate the list of possible rankings
    $choices[0] = '--';
    for ($i = 1; $i <= $num_choices; $i++) {
      if ($i == 1) {
        $val = t('1st');
      }
      elseif ($i == 2) {
        $val = t('2nd');
      }
      elseif ($i == 3) {
        $val = t('3rd');
      }
      else {
        $val = t($i .'th');
      }
      $choices[$i] = $val;
    }
    // Fix to work around limitations in the current translation system. By 
    // listing the strings here they are made immediately available for 
    // translating. Listing up to 15 here, as it should be enough for most 
    // users. If more are needed they are made available for translating in 
    // Drupal when a poll with more than 15 choices has been created.
    // TODO: Find a better solution for this.
    array(t('4th'), t('5th'), t('6th'), t('7th'), t('8th'), t('9th'), t('10th'), t('11th'), t('12th'), t('13th'), t('14th'), t('15th'));

    // List of poll choices, to be populated.
    $form['choice'] = array('#tree' => TRUE);

    // If previewing check the format against the current users permissions.
    $check = $node->build_mode == NODE_BUILD_PREVIEW;
    foreach ($node->choice as $key => $choice) {
      // Don't show blank choices or write-in votes if the setting is disabled.
      if ($choice['label'] && ($node->show_writeins || !$choice['writein'])) {
        $form['choice'][$key] = array(
          '#type' => 'select',
          '#title' => _advpoll_choice_markup($choice['label'], $node->format, $check) . ($choice['writein'] ? ' '. t('(write-in)') : ''),
          '#options' => $choices,
        );
      }
    }

    if ($handle_writeins) {
      $form['choice'][$key + 1] = array(
        '#type' => 'select',
        '#title' => t('(write-in)'),
        '#options' => $choices,
        '#attributes' => array('class' => 'advpoll-writeins'),
      );
      // Key index of the write-in option.
      $form['writein_key'] = array(
        '#type' => 'value',
        '#value' => $key + 1,
      );
    }
  }

  // Add write-in text field.
  if ($handle_writeins) {
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
    static $ranking_vote_count = 0;
    $form['vote'] = array(
      '#type' => 'submit',
      '#value' => t('Vote'),
      '#id' => 'edit-vote-rank-'. $ranking_vote_count++,
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
    $form['message']['#value'] = $user->uid ? t('You are not eligible to vote in this poll.') : $login_message;
  }

  $form['#action'] = url('node/'. $node->nid);
  // Set form caching because we could have multiple forms on the page.
  // (from poll.module).
  $form['#cache'] = TRUE;
  
  return $form;
}

/**
 * Process variables for advpoll-display-ranking-form.tpl.php.
 *
 * The variables array contains the following arguments:
 * - $form
 *
 * @see advpoll-display-ranking-form.tpl.php
 */
function advpoll_preprocess_advpoll_voting_ranking_form(&$variables) {
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

  // Add tabledrag JavaScript.
  drupal_add_tabledrag($form['#id'] . '-table', 'order', 'self', 'advpoll-choice-order', NULL, NULL, FALSE);

}

function advpoll_view_results_ranking($node, $teaser, $page) {
  $results = votingapi_select_results(array('content_type' => 'advpoll', 'content_id' => $node->nid));
  $round_table = '';

  // If no one has voted, $results = array() and thus is empty.
  if (!empty($results)) {
    // Temporary list of choices indexes for the ranking.
    $ranking_list = array();
    // Result object
    $ranking = array();
    $choices = array();
    $poll = array();
    $rounds = array();
    foreach ($results as $result) {
      $tag = $result['tag'];
      if ($tag == '_advpoll') {
        // Poll-wide cached value.
        $poll[$result['function']] = $result['value'];
      }
      else if (strstr($tag, '_rounds_')) {
        // Re-construct round data and extract the round from the tag.
        $round = str_replace('_rounds_', '', $tag);
        if (!isset($rounds[$round])) {
          $rounds[$round] = array();
        }
        // $result->function actually stores $choice.
        $rounds[$round][$result['function']] = $result['value'];
      }
      else if (isset($node->choice[$tag])) {
        // Note: choices that have been removed will not pass the previous
        // line's test even though their values are still in the vote table.

        // Choice-specific cached value.
        if ($result['function'] == 'ranking') {
          $ranking_list[$result['value']][] = $tag;
        }
        else if (!isset($node->choice[$result['function']])) {
          $choices[$tag][$result['function']] = $result['value'];
        }
      }
    }

    // Re-construct the rankings object.
    foreach ($ranking_list as $i => $choice_list) {
      $ranking[$i]->choices = array();
      foreach ($choice_list as $choice_i) {
        $ranking[$i]->choices[] = $choice_i;
        if (isset($choices[$choice_i]['view_score'])) {
          $ranking[$i]->view_score = $choices[$choice_i]['view_score'];
        }
        if (isset($choices[$choice_i]['raw_score'])) {
          $ranking[$i]->raw_score = $choices[$choice_i]['raw_score'];
        }
        if (isset($choices[$choice_i]['percentage'])) {
          $ranking[$i]->percentage = $choices[$choice_i]['percentage'];
        }    
      }
    }

    $output = '';

    if ($node->algorithm == 'borda_count') {
      for ($i = 0; $i < count($ranking); $i++) {
        $first_one = TRUE;
        $this_rank = '';

        // Loop through all choices with this ranking.
        foreach ($ranking[$i]->choices as $choice) {
          $label = isset($node->choice[$choice]) ? _advpoll_choice_markup($node->choice[$choice]['label'], $node->format, FALSE) . ($node->choice[$choice]['writein']? ' '. t('(write-in)') : '') : t('(deleted)');
          $this_rank .= ($first_one ? '' : ', ') . $label;
          $first_one = FALSE;
        }

        $percentage = round(100 * (isset($ranking[$i]->percentage) ? $ranking[$i]->percentage : 0), 0);
        $output .= theme('advpoll_bar', $this_rank, $percentage, $ranking[$i]->view_score);
      }
    }
    else {
      $output .= '<ol>';
      for ($i = 0; $i < count($ranking); $i++) {
        $output .= '<li> ';
        $first_one = TRUE;
        // If previewing check the format against the current users permissions.
        $check = $node->build_mode == NODE_BUILD_PREVIEW;

        // Loop through all choices with this ranking.
        foreach ($ranking[$i]->choices as $choice) {
          $label = isset($node->choice[$choice]) ? _advpoll_choice_markup($node->choice[$choice]['label'], $node->format, FALSE) . ($node->choice[$choice]['writein']? ' '. t('(write-in)') : '') : t('(deleted)');
          $output .= ($first_one ? '' : ', ') . $label;
          $first_one = FALSE;
        }

        // Show the ranking's score if it exists (depends on algorithm).
        if (isset($ranking[$i]->view_score)) {
          $output .= ' ('. $ranking[$i]->view_score .'%)';
        }
        $output .= '</li>';
      }
    }
    $output .= '</ol>';

    if (user_access('inspect all votes') && isset($rounds)) {
      if (count($rounds) > 0) {
        $header[0] = t('Rounds');
        $total_rounds = count($rounds);
        for ($i = 0; $i < count($rounds); $i++) {
          $choices = $rounds[$i];
          if ($i + 1 == $total_rounds) {
            // This is the last round.
            $header[$i + 1] = t('Final');
          }
          else {
            $header[$i + 1] = $i + 1;
          }
          if ($i == 0) {
            $rows = array();
          }
          foreach ($node->choice as $key => $data) {
            $rows[$key][0] = $data['label'];
            $rows[$key][$i + 1] = isset($choices[$key]) && $choices[$key]? $choices[$key] : '';
          }
        }
        $round_table = theme('table', $header, $rows, array(), t('Per-round breakdown of votes for each choice'));
      }
    }
  }
  $output .= $round_table;
  
  return array('results' => $output, 'votes' => $poll['total_votes']);
}

/**
 * Calculate the results for a ranking poll based on the algorithm.
 *
 * @param $node
 *  The node object for the current poll
 *
 * @return 
 *   Should return an object that include the following attributes
 *   -results : 2d array listing the aggregate preference, including ties
 *   -rounds : 2d array listing the per-choice vote count for each round and
 *              a status message indicating who was eliminated
 *   -totalVoters : the total number of voters who participated
 */
function advpoll_calculate_results_ranking(&$cache, $node) {
  if ($node->algorithm == 'borda_count') {
    $results = _advpoll_calculate_bordacount($node);
  }
  else {
    $results = _advpoll_calculate_instantrunoff($node);
  }

  // Cache rankings.
  // API: $cache[$tag][$type][$function] = $value (0 is the default $type)
  if (isset($results->ranking)) {
    for ($i = 0; $i < count($results->ranking); $i++) {
      foreach ($results->ranking[$i]['choices'] as $choice) {
        $cache[$choice][0]['ranking'] = $i;
        if (isset($results->ranking[$i]['raw_score'])) {
          $cache[$choice][0]['raw_score'] = $results->ranking[$i]['raw_score'];
        }
        if (isset($results->ranking[$i]['view_score'])) {
          $cache[$choice][0]['view_score'] = $results->ranking[$i]['view_score'];
        }
        if (isset($results->ranking[$i]['percentage'])) {
          $cache[$choice][0]['percentage'] = $results->ranking[$i]['percentage'];
        }
      }
    }
  }

  // Cache round results.
  if (isset($results->matrix)) {
    foreach ($results->matrix as $i => $round) {
      $key = '_rounds_'. $i;
      $cache[$key] = array();
      foreach ($round as $choice => $votes) {
        $cache[$key][0][$choice] = count($votes);
      }
    }
  }

  // Cache total votes.
  $cache['_advpoll'][0]['total_votes'] = isset($results->total_votes) ? $results->total_votes : 0;

  // Cache total points (if it exists).
  if (isset($results->total_points)) {
    $cache['_advpoll'][0]['total_points'] = $results->total_points;
  }
}

/**
 * Calculate the results using borda count.
 * 
 * @param $node
 *  The node object for the current poll.
 *
 * @return 
 *   Should return an object that include the following attributes
 *   -results : 2d array listing the aggregate preference, including ties
 *   -rounds : 2d array listing the per-choice vote count for each round and
 *              a status message indicating who was eliminated
 *   -totalVoters : the total number of voters who participated
 */
function _advpoll_calculate_bordacount($node) {
  $votes = array();
  // ORDER BY value ASC lets us ensure no gaps.
  $result = db_query("SELECT * FROM {votingapi_vote} v WHERE content_type = '%s' AND content_id = %d ORDER BY value ASC", 'advpoll', $node->nid);
  while ($vobj = db_fetch_object($result)) {
    $votes[] = $vobj;
  }

  if (count($votes) == 0) {
    // No votes yet.
    return array();
  }

  // Aggregate votes by user (uid if logged in, IP if anonymous)
  // in ascending order of value.
  $user_votes = array();

  foreach ($votes as $vote) {
    if ($vote->uid == 0) {
      // Anonymous user.
      $key = $vote->vote_source;
    }
    else {
      // Logged-in user.
      $key = $vote->uid;
    }
    $user_votes[$key][$vote->value] = $vote->tag;
  }

  $choice_votes = array();
  $total_choices = count($node->choice);
  $total_points = 0;
  // Loop through each user's vote
  foreach ($user_votes as $uid => $user_vote) {
    foreach ($user_vote as $ranking => $choice) {
      // Negative values are possible if choices were removed after vote
      $vote_value = max($total_choices - $ranking, 0);
      isset($choice_votes[$choice]) ? $choice_votes[$choice] += $vote_value : $choice_votes[$choice] = $vote_value;
      $total_points += $vote_value;
    }
  }

  // Add any remaining choices that received no votes.
  foreach ($node->choice as $i => $choice) {
    if (!isset($choice_votes[$i])) {
      // Didn't receive any votes
      $choice_votes[$i] = 0;
    }
  }

  // Sort descending (although there may be ties).
  arsort($choice_votes);

  // Figure out the final ranking.
  $ranking = array();
  $previous_total = -1;
  $cur_result = -1;

  foreach ($choice_votes as $choice => $total) {
    if ($total != $previous_total) {
      // Didn't tie with the previous score.
      $cur_result++;
    }
    $ranking[$cur_result]['choices'][] = $choice;
    $ranking[$cur_result]['raw_score'] = $total;
    $ranking[$cur_result]['view_score'] = format_plural($total, '1 point', '@count points');
    $ranking[$cur_result]['percentage'] = $total_points ? $total / $total_points : 0;
    $previous_total = $total;
  }

  $total_votes = count($user_votes);

  $result_obj->ranking = $ranking;
  $result_obj->total_votes = $total_votes;
  $result_obj->total_points = $total_points;
  return $result_obj;
}


/**
 * Calculate the results using instant-runoff voting.
 * 
 * @param $node
 *  The node object for the current poll.
 *
 * @return 
 *   Should return an object that include the following attributes.
 *   -results : 2d array listing the aggregate preference, including ties
 *   -rounds : 2d array listing the per-choice vote count for each round and
 *              a status message indicating who was eliminated
 *   -totalVoters : the total number of voters who participated
 */
function _advpoll_calculate_instantrunoff($node) {
  $votes = array();
  // ORDER BY value ASC lets us ensure no gaps.
  $result = db_query("SELECT * FROM {votingapi_vote} v WHERE content_type = '%s' AND content_id = %d ORDER BY value ASC", 'advpoll', $node->nid);
  while ($vobj = db_fetch_object($result)) {
    $votes[] = $vobj;
  }

  if (count($votes) == 0) {
    // No votes yet.
    return array();
  }

  // Aggregate votes by user (uid if logged in, IP if anonymous)
  // in ascending order of value.
  $user_votes = array();

  foreach ($votes as $vote) {
    if ($vote->uid == 0) {
      // Anonymous user.
      $key = $vote->vote_source;
    }
    else {
      // Logged-in user.
      $key = $vote->uid;
    }

    // Note: relies on ORDER BY value ASC in vote-getting SQL query.
    // Otherwise a later vote might have a lower value.
    $user_votes[$key][] = $vote->tag;
  }

  $total_votes = count($user_votes);

  // Log of 1st-place votes per choice in each round.
  $round_log = array();

  // Gradually append candidates as they are eliminated; end with the winner.
  $reverse_ranking = array();
  
  // If we eliminate one choice per round and have n choices, we should
  // not be able to do more than n - 1 rounds.
  $max_rounds = count($node->choice); 
  for ($round = 0; $round < $max_rounds; $round++) {

    // Initialize cur_round.
    $cur_round = array();
    $total_choices = count($node->choice);

    foreach ($node->choice as $key => $data) {
      $cur_round[$key] = array();
    }

    // Loop through each user.
    foreach ($user_votes as $key => $user_vote) {
      // $user_vote[0] contains the user's first remaining preference.
      $cur_round[$user_vote[0]][] = $key;
    }

    if ($round == 0) {
      // This is the first round.
      // Any choices with no first-place votes are considered eliminated.
      foreach ($cur_round as $key => $choice_votes) {
        if (count($choice_votes) == 0) {
          unset($cur_round[$key]);
          $reverse_ranking[0]['choices'][] = $key;
        }
      }
    }

    // Add the current round to the matrix.
    $round_log[] = $cur_round;

    // Calculate the min and max number of votes.
    $min_votes = -1;
    $max_votes = 0;

    // Number of choices that have already been discarded.
    $num_discarded = 0;

    // Examine the number of votes each choice received this round.
    foreach ($cur_round as $ch => $choice_votes) {
      $num_votes = count($choice_votes);

      if ($num_votes > $max_votes) {
        $max_votes = $num_votes;
        // Store current winner in case it has a majority.
        $cur_winner = $ch;
      }

      // This choice has already been eliminated (theoretically)
      // so don't count it as the minimum.
      if ($num_votes == 0) {
        $num_discarded++; // XXX: Probably don't need this variable any more
      }
      else if ($num_votes != 0 && ($num_votes < $min_votes || $min_votes == -1)) {
        $min_votes = $num_votes;
      }
    }

    // If one choice has a majority of remaining users it wins.
    // Note: we use count($user_votes) because some users may have incomplete
    // ballots and may have already had all of their choices eliminated.
    if ($max_votes > count($user_votes) / 2) {
    
      // Prune out the winning choice if it's still in there.
      if (isset($cur_round[$cur_winner])) {
          unset($cur_round[$cur_winner]);
      }

      // Keep computing until we figure out all final rankings.
      while (count($cur_round)  > 0) {
        // Loop through non-winning choices.
        $current_place = array();
        $min = -1;
        foreach ($cur_round as $ch => $choice_votes) {
          // Choice has already been eliminated, just unset it.
          if (count($choice_votes) == 0) {
            unset($cur_round[$ch]);
          }
          else if ($min == -1
              || count($choice_votes) < $min) {
            // New minimum.
            $current_place = array($ch);
            $min = count($choice_votes);
          }
          else if (count($choice_votes) == $min) {
            // Tied for minimum.
            $current_place[] = $ch;
          }
        }

        // current_place will be empty the first iteration if some
        // choices had no first-place votes and were eliminated
        // at the beginning.
        if (count($current_place) > 0) {
          $reverse_ranking[]['choices'] = $current_place;  
          // Remove all choices that had the minimum.
          foreach ($current_place as $ch_key) {
            unset($cur_round[$ch_key]);
          }
        }
      }

      // Save a reversed version of the round log to help compute winnerPercent.
      $revmat = array_reverse($round_log);

      // The winner finally gets added
      $reverse_ranking[]['choices'] = array($cur_winner);
      $index = count($reverse_ranking) - 1;
      $reverse_ranking[$index]['raw_score'] = round(count($revmat[0][$cur_winner]) * 100 / count($user_votes), 1);
      $reverse_ranking[$index]['view_score'] = $reverse_ranking[$index]['raw_score'] .'%';

      $result_obj->matrix = $round_log;
      $result_obj->total_votes = $total_votes;
      $result_obj->ranking = array_reverse($reverse_ranking);
      return $result_obj;
    }
    
    // Since we're still here, no one has won, so eliminate one of the
    // choices with the lowest number of votes.

     // Find all choices with the minimum number of votes
    $min_choices = array();
    foreach ($cur_round as $ch => $choice_votes) {
      if (count($choice_votes) == $min_votes) {
        $min_choices[] = $ch;
      }
    }

    // Randomly select the choice to eliminate out of the available choices.
    // TODO: due to the randomness, this result must be cached after each vote.
    $round_loser = array_rand($min_choices);

    $reverse_ranking[]['choices'] = array($min_choices[$round_loser]);
    
    // Loop through the users who voted for the loser and redistribute.
    foreach ($cur_round[$min_choices[$round_loser]] as $user_key) {
      // Remove their current first preference.
      array_shift($user_votes[$user_key]);

      // Keep eliminating first preference until we run out or find an choice
      // that hasn't been eliminated.
      while ($cur_round[$user_votes[$user_key][0]] == array() && count($user_votes[$user_key]) > 0) {
        array_shift($user_votes[$user_key]);
      }

      // If they have no more preferences, remove from list for simplicity.
      if (count($user_votes[$user_key]) == 0) {
        unset($user_votes[$user_key]);
      }
    }
  }
  // Loop detected. Signal user and record.
  drupal_set_message(t('Could not find a solution within @rounds iterations.', array('@rounds' => $max_rounds)));
  $result_obj->matrix = $round_log;
  $result_obj->total_votes = $total_votes;
  return $result_obj;
}

/**
 * Implementation of the vote hook for the runoff module.
 *
 * This takes care of registering the vote in runoff nodes.
 */
function advpoll_voting_ranking_form_submit($form, &$form_state) {
  $votes = array();
  $node = $form['#node'];

  // Do submission specific to writeins.
  if (isset($form_state['values']['writein_key'])) {
    _advpoll_writeins_voting_form_submit($node, $form_state, $votes, $form_state['values']['choice'][$form_state['values']['writein_key']]);
  }

  foreach ($form_state['values']['choice'] as $choice => $rank) {
    $vote = array();
    // Ignore write-in choice that has already been taken care of.
    if (!$node->writeins || !isset($form_state['values']['writein_key']) || $choice != $form_state['values']['writein_key']) {
      $vote['value'] = $rank;
      // A zero value indicates they didn't rank that choice.
      if ($vote['value'] != 0) {
        $vote['value_type'] = 'option';
        $vote['tag'] = $choice;
        $vote['content_type'] = 'advpoll';
        $vote['content_id'] = $node->nid;
        $votes[] = $vote;
      }
    }
  }

  votingapi_set_votes($votes, array());
  _advpoll_vote_response($node, $form_state);
}

/**
 * Implementation of the vote validation hook for the runoff module.
 *
 * This checks if the submitted values are within range, if they are
 * not empty, and if they are not repeated.
 *
 * @returns boolean false on invalid forms, true otherwise.
 */
function advpoll_voting_ranking_form_validate($form, &$form_state) {
  $node = $form['#node'];
  $ajax = $form_state['values']['ajax'];
  
  // Check if user is eligible to vote.
  if (!advpoll_eligible($node)) {
    _advpoll_form_set_error('choice[', t('You are not allowed to vote in this poll.'), $ajax);
  }
  
  // Check if poll is active.
  if (!_advpoll_is_active($node)) {
    _advpoll_form_set_error('choice[', t('This poll is closed.'), $ajax);
  }

  // Whether the write-in option is selected.
  $writein_option = FALSE;
  $writein_text = isset($form_state['values']['writein_key']) ? $form_state['values']['writein_choice'] : '';

  // Check if user has already voted.
  list($voted, $cancel_vote) = _advpoll_user_voted($node->nid);
  if ($voted) {
    _advpoll_form_set_error('choice[', t('You have already voted in this poll.'), $ajax);
    // Redirect to the current poll node to view the poll result instead of the voting form. This is only
    // initiated for non-Ajax voting.
    drupal_goto('node/'. $node->nid);
  }

  // Array used to check which values are set.
  $set_values = array();

  $num_choices = 0;

  // Write-ins are enabled, user has permission, and the write-in box is checked.
  if ($node->writeins && user_access('add write-ins') && $form_state['values']['choice'][$form_state['values']['writein_key']]) {
    $num_choices++;
    // Set a flag for additional checks.
    $writein_option = TRUE;
  }

  foreach ($node->choice as $key => $choice) {
    // Count the number of choices that are ranked.
    if ($form_state['values']['choice'][$key]) {
      $num_choices++;
    }
    $int_value = intval($form_state['values']['choice'][$key]);
    // Mark this value as seen
    isset($set_values[$int_value]) ? $set_values[$int_value]++ : $set_values[$int_value] = 1;
    // Check range
    if ($int_value > count($node->choice) || $int_value < 0) {
      // TODO: clean up this error message
      $message = "Illegal rank for choice $key: $int_value (min: 1, max: ". count($node->choice) .')';
      _advpoll_form_set_error('choice]['. $key, $message, $ajax);
    }
  }

  // Write-ins are enabled, user has permission, and the write-in box is checked.
  if ($writein_option) {
    $int_value = intval($form_state['values']['choice'][$form_state['values']['writein_key']]);
    // Mark this value as seen
    $set_values[$int_value]++;
    // Check range
    if ($int_value > count($node->choice) || $int_value < 0) {
      // TODO: clean up this error message
      $message = "Illegal rank for the write-in choice: $int_value (min: 1, max: ". count($node->choice) .')';
      _advpoll_form_set_error('choice]['. $form_state['values']['writein_key'], $message, $ajax);
    }
  }

  // Do validation specific to writeins.
  _advpoll_writeins_voting_form_validate($node, $writein_option, $writein_text, $ajax);

  // Too many choices ranked.
  if ($node->max_choices != 0 && $num_choices > $node->max_choices) {
    $message = t('%num choices were selected but only %max are allowed.', array('%num' => $num_choices, '%max' => $node->max_choices));
    _advpoll_form_set_error('choice', $message, $ajax);
  }

  // Not enough choices ranked.
  $min_choices = 1;
  if ($num_choices < $min_choices) {
    _advpoll_form_set_error('choice', t('At least one choice must be selected.'), $ajax);
  }

  // Check that multiple choices are not set to the same value.
  foreach ($set_values as $val => $count) {
    if ($val != 0 && $count > 1) {
      $message = t('Multiple choices given the rank of %value.', array('%value' => $val));
      _advpoll_form_set_error('choice', $message, $ajax);
    }
  }
}


/**
 * Hook to handle a cancelled vote for a ranking poll.
 */
function advpoll_cancel_ranking($node, $user_vote) {
  // Remove choice if this was the last vote for a write-in.
  if ($node->writeins) {
    $recalculate = FALSE;
    foreach ($user_vote as $vote) {
      if ($node->choice[$vote['tag']]['writein']) {
        // Check if there are any other votes for this write-in.
        $count = db_result(db_query('SELECT COUNT(1) FROM {votingapi_vote} WHERE content_id = %d AND tag = %d', $node->nid, $vote['tag']));
        if ($count == 0) {
          // Delete the write-in because no one else voted for it.
          db_query('DELETE FROM {advpoll_choices} WHERE cid = %d', $vote['tag']);
          $recalculate = TRUE;
          watchdog('content', 'Removed write-in choice %choice after the last vote was cancelled.', array('%choice' => $node->choice[$vote['tag']]['label']));
        }
      }
    }
    if ($recalculate) {
      votingapi_recalculate_results('advpoll', $node->nid);
    }
  }
}
