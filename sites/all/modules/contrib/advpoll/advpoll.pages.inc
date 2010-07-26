<?php
// $Id: advpoll.pages.inc,v 1.1.2.2 2010/06/22 11:33:06 mirodietiker Exp $

/**
 * @file
 * Page callbacks for the advpoll module.
 */

/**
 * List all polls as a page.
 */
function advpoll_page() {
  // List all polls
  $sql = "SELECT n.nid, n.title, p.active, n.created, c.value AS votes FROM {node} n INNER JOIN {advpoll} p ON n.nid = p.nid INNER JOIN {votingapi_cache} c ON n.nid = c.content_id WHERE n.status = 1 AND c.tag = '_advpoll' AND c.function = 'total_votes' AND c.content_type = 'advpoll' GROUP BY n.nid, n.title, p.active, n.created, c.value ORDER BY n.created DESC";
  $sql = db_rewrite_sql($sql);
  $result = pager_query($sql, 15);
  $output = '<ul>';
  while ($node = db_fetch_object($result)) {
    $output .= '<li>'. l($node->title, 'node/'. $node->nid) .' - '. format_plural($node->votes, '1 vote', '@count votes') .' - '. (_advpoll_is_active($node) ? t('open') : t('closed')) .'</li>';
  }
  $output .= '</ul>';
  $output .= theme('pager', NULL, 15);
  return $output;
}

/**
 * Display the electoral list page.
 */
function advpoll_electoral_list_page($node) {
  drupal_set_title(check_plain($node->title));
  $output = '';
    
  if (user_access('administer polls')) {
    $output .= drupal_get_form('advpoll_electoral_list_form', $node->nid);
  }

  $output .= '<p>'. t('This table lists all the eligible voters for this poll.') .'</p>';

  $header[] = array('data' => t('Voter'), 'field' => 'u.name');

  $result = pager_query("SELECT u.uid, u.name FROM {advpoll_electoral_list} el LEFT JOIN {users} u ON el.uid = u.uid WHERE el.nid = %d". tablesort_sql($header), 20, 0, NULL, $node->nid);
  $table_rows = array();
  while ($voter = db_fetch_object($result)) {
    $row = array(
      theme('username', $voter),
    );

    if (user_access('administer polls')) {
      $row[] = l(t('remove'), 'node/'. $node->nid .'/remove/'. $voter->uid);
    }

    $table_rows[] = $row;
  }
  $output .= theme('table', $header, $table_rows);
  $output .= theme('pager', NULL, 20, 0);
  print theme('page', $output);
}

/**
 * Custom page to handle confirmation of clearing all votes.
 */
function advpoll_clear_votes_page($node) {

  // Add the votes tab to the menu.
  $breadcrumb = drupal_get_breadcrumb();
  $breadcrumb[] = l(t('Votes'), 'node/' . $node->nid . '/votes');
  drupal_set_breadcrumb($breadcrumb);

  $output = drupal_get_form('advpoll_clear_votes_confirm_form', $node->nid);

  // Set the title after we get the form so we overwrite confirm_form()'s title.
  drupal_set_title(check_plain($node->title));

  return $output;
}

/**
 * Callback for results tab.
 */
function advpoll_results_page($node) {
  drupal_set_title(check_plain($node->title));
  return node_show($node, 0);
}

/**
 * Display the votes page.
 */
function advpoll_votes_page($node) {

  drupal_set_title(check_plain($node->title));

  if ($node->show_votes || user_access('administer polls')) {
    $output = t('This table lists all the recorded votes for this poll. If anonymous users are allowed to vote, they will be identified by the IP address of the computer they used when they voted.');

    $header[] = array('data' => t('Visitor'), 'field' => 'u.name');
    $header[] = array('data' => t('Date'), 'field' => 'v.timestamp');
    $header[] = array('data' => t('Vote'), '');

    // added pager query because of pre-grouping as an intermediate fix.
    $result = pager_query("
      SELECT v.uid, v.vote_source, v.timestamp, u.name
      FROM {votingapi_vote} v
      LEFT JOIN {users} u
        ON v.uid = u.uid
      WHERE v.content_type = 'advpoll'
        AND v.content_id = %d
      GROUP BY v.uid, v.vote_source, v.timestamp, u.name"
      .tablesort_sql($header),
      20, 0, "
      SELECT COUNT( DISTINCT v.uid, v.vote_source, v.timestamp, u.name )
      FROM {votingapi_vote} v
      LEFT JOIN {users} u
        ON v.uid = u.uid
      WHERE v.content_type = 'advpoll'
        AND v.content_id =%d",
      $node->nid
    );

    $uids = array();
    $hostnames = array();
    $timestamp = array();
    while ($vote = db_fetch_object($result)) {
      $uids[$vote->uid] = $vote->uid;
      $hostnames[$vote->vote_source] = $vote->vote_source;
      $timestamps[$vote->timestamp] = $vote->timestamp;
    }

    $rows = array();
    if (count($uids) > 0) {
      // Use db_query()'s placeholder syntax to prevent any potential SQL
      // injection attacks.
      $uid_placeholders = array_fill(0, count($uids), '%d');
      $host_placeholders = array_fill(0, count($hostnames), "'%s'");
      $time_placeholders = array_fill(0, count($timestamps), '%d');

      // Here we have to select votes based on their uid+hostname+timestamp
      // combination because there no unique id for the set of rankings that
      // corresponds to a user's full vote on a given poll.
      $query = "
        SELECT v.vote_id, v.tag, v.uid, v.vote_source, v.timestamp, v.value, u.name
        FROM {votingapi_vote} v
        LEFT JOIN {users} u
          ON v.uid = u.uid
        WHERE v.content_id = %d
          AND v.uid IN(". implode(', ', $uid_placeholders) .")
          AND v.vote_source IN(". implode(', ', $host_placeholders) .")
          AND v.timestamp IN(". implode(', ', $time_placeholders) .")"
        . tablesort_sql($header);
      $parameters = array_merge(array($query, $node->nid), array_values($uids), array_values($hostnames), array_values($timestamps));
      $result = call_user_func_array('db_query', $parameters);
      while ($vote = db_fetch_object($result)) {
        $key = $vote->uid ? $vote->uid : $vote->vote_source .'-'. $vote->timestamp;
        $rows[$key]['name'] = $vote->name ? theme('username', $vote) : check_plain($vote->vote_source);
        $rows[$key]['timestamp'] = $vote->timestamp;
        if ($node->type == 'advpoll_ranking') {
          // Need two dimensional results (if equal rankings are allowed).
          $rows[$key]['votes'][$vote->value][] = _advpoll_choice_markup($node->choice[$vote->tag]['label'], $node->format, FALSE);
        }
        else {
          // Just need one dimensional results.
          $rows[$key]['votes'][] = _advpoll_choice_markup($node->choice[$vote->tag]['label'], $node->format, FALSE);
        }
      }
    }
    
    $separators = array('advpoll_ranking' => ' > ', 'advpoll_binary' => ', ');

    // Create strings out of each vote.
    $results = array();

    foreach ($rows as $key => $container) {
      $ranking = $container['votes'];
      asort($ranking);
      $rankings = array();
      if ($node->type == 'advpoll_ranking') {
        // Include support for multiple choices having the same ranking.
        foreach ($ranking as $vote => $choices) {
          $rankings[$vote] = implode(' = ', $choices);
        }
      }
      else {
        // Just copy the previous array.
        $rankings = $ranking;
      }
      ksort($rankings);
      $results[$key]['name'] = $rows[$key]['name'];
      $results[$key]['timestamp'] = format_date($rows[$key]['timestamp'], 'small');
      $results[$key]['vote'] = implode($separators[$node->type], $rankings);
    }

    $output .= theme('table', $header, $results);
    $output .= theme('pager', NULL, 20, 0);
  }
  else {
    $output = '<div class="advpoll-cant-show-votes">'. t('This poll is configured to not display individual votes.') . '</div>';
  }

  // Only display the button if the user has access.
  if (_advpoll_clear_votes_access($node)) {
    $output .= drupal_get_form('advpoll_clear_votes_form', $node->nid);
  } 
  print theme('page', $output);
}

function advpoll_writeins_page($node) {
  $output = '';
  drupal_set_title(check_plain($node->title));
  $output .= drupal_get_form('advpoll_writein_promote_form', $node);
  $output .= drupal_get_form('advpoll_writein_merge_form', $node);
  echo theme('page', $output);
}

