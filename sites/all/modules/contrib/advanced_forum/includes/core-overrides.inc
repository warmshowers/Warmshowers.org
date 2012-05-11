<?php

/**
 * @file
 * These are functions that are overriding core directly (not theme related.)
 */

/**
 * Menu callback; prints a forum listing.
 */
function advanced_forum_page($tid = 0) {
  if (!is_numeric($tid)) {
    return MENU_NOT_FOUND;
  }

  $tid = (int)$tid;

  _advanced_forum_add_files();

  $topics = '';
  $forum_per_page = variable_get('forum_per_page', 25);
  $sortby = variable_get('forum_order', 1);

  $forums = advanced_forum_get_forums($tid);

  $parents = taxonomy_get_parents_all($tid);
  if ($tid && !in_array($tid, variable_get('forum_containers', array()))) {
    $topics = advanced_forum_get_topics($tid, $sortby, $forum_per_page);
  }

  $vid = variable_get('forum_nav_vocabulary', '');
  $vocabulary = taxonomy_vocabulary_load($vid);

  // Breadcrumb navigation:
  $breadcrumb[] = l(t('Home'), NULL);
  if ($tid) {
    $breadcrumb[] = l($vocabulary->name, 'forum');
  }
  if ($parents) {
    foreach (array_reverse($parents) as $p) {
      if ($p->tid != $tid) {
        $breadcrumb[] = l($p->name, 'forum/'. $p->tid);
      }
      else {
        $title = $p->name;
      }
    }
  }

  if (empty($title)) {
    $title = $vocabulary->name;
  }

  if (!variable_get('advanced_forum_disable_breadcrumbs', FALSE)) {
    drupal_set_breadcrumb($breadcrumb);
  }

  drupal_set_title(check_plain($title));

  return theme('forums', $forums, $topics, $parents, $tid, $sortby, $forum_per_page);
}

/**
 * Returns a list of all forums for a given taxonomy id
 *
 * This is copied from the forum module and adapted.
 *
 * Forum objects contain the following fields
 * -num_topics Number of topics in the forum
 * -num_posts Total number of posts in all topics
 * -last_post Most recent post for the forum
 *
 * @param $tid
 *   Taxonomy ID of the vocabulary that holds the forum list.
 * @return
 *   Array of object containing the forum information.
 */
function advanced_forum_get_forums($tid = 0) {

  $forums = array();
  $vid = variable_get('forum_nav_vocabulary', '');
  $_forums = taxonomy_get_tree($vid, $tid);

  if (count($_forums)) {

    $counts = array();

    $sql = "
      SELECT r.tid AS tid, n.nid AS nid, l.comment_count AS nid_comment_count
        FROM {node} n
        INNER JOIN {node_comment_statistics} l ON n.nid = l.nid
        INNER JOIN {term_node} r ON n.vid = r.vid
        WHERE n.status = 1
        GROUP BY r.tid, n.nid, l.comment_count";
    $sql_rewritten = db_rewrite_sql($sql);
    if ($sql_rewritten == $sql) {
      $sql = "
        SELECT r.tid, COUNT(n.nid) AS topic_count, SUM(l.comment_count) AS comment_count
          FROM {node} n
          INNER JOIN {node_comment_statistics} l ON n.nid = l.nid
          INNER JOIN {term_node} r ON n.vid = r.vid
          WHERE n.status = 1
          GROUP BY r.tid";
      $sql = db_rewrite_sql($sql);
    }
    else {
      $sql = "
        SELECT tid, COUNT(nid) AS topic_count, SUM(nid_comment_count) AS comment_count
          FROM ($sql_rewritten) AS forum_content_list
          GROUP BY tid";
    }

    $_counts = db_query($sql);
    while ($count = db_fetch_object($_counts)) {
      $counts[$count->tid] = $count;
    }
  }

  foreach ($_forums as $forum) {
    // Check if this term is a container
    if (in_array($forum->tid, variable_get('forum_containers', array()))) {
      $forum->container = 1;
    }

    if (!empty($counts[$forum->tid])) {
      $forum->num_topics = $counts[$forum->tid]->topic_count;
      $forum->num_posts = $counts[$forum->tid]->topic_count + $counts[$forum->tid]->comment_count;
    }
    else {
      $forum->num_topics = 0;
      $forum->num_posts = 0;
    }

    // Find out from the style's .info how many posts per forum to collect.
    $info = advanced_forum_style_info();
    $post_count = isset($info['forum list post count']) ? intval($info['forum list post count']) : 1;

    // This query does not use full ANSI syntax since MySQL 3.x does not support
    // table1 INNER JOIN table2 INNER JOIN table3 ON table2_criteria ON table3_criteria
    // used to join node_comment_statistics to users.
    $sql = "SELECT n.nid,
                   n.title as node_title,
                   n.type,
                   ncs.last_comment_timestamp as timestamp,
                   IF (ncs.last_comment_uid != 0, u2.name, ncs.last_comment_name) AS name,
                   ncs.last_comment_uid as uid
            FROM {node} n
            INNER JOIN {users} u1 ON n.uid = u1.uid
            INNER JOIN {term_node} tn ON n.vid = tn.vid
            INNER JOIN {node_comment_statistics} ncs ON n.nid = ncs.nid
            INNER JOIN {users} u2 ON ncs.last_comment_uid=u2.uid
            WHERE n.status = 1 AND tn.tid = %d
            ORDER BY ncs.last_comment_timestamp DESC";
    $sql = db_rewrite_sql($sql);
    if ($post_count) {
      $result = db_query_range($sql, $forum->tid, 0, $post_count);
      while ($topic = db_fetch_object($result)) {
        if ($post_count > 1) {
          $forum->last_post[] = $topic;
        }
        else {
          $forum->last_post = $topic;
        }
      }
    }

    $forums[$forum->tid] = $forum;
  }

  return $forums;
}

/**
 * This is copied from the forum module and adapted.
 */
function advanced_forum_get_topics($tid, $sortby, $forum_per_page, $sort_form = TRUE) {
  $term = taxonomy_get_term($tid);
  drupal_add_feed(url('taxonomy/term/'. $tid .'/0/feed'), 'RSS - '. check_plain($term->name));

  // Views handles this page
  $view = views_get_view('advanced_forum_topic_list');
  $view->set_display('default');
  $view->set_arguments(array($tid));
  $view->sort_form = $sort_form;
  return $view->preview();
}
