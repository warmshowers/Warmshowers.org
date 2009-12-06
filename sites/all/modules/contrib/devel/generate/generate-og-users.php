<?php

// $Id: generate-og-users.php,v 1.3 2006/06/11 00:43:25 killes Exp $:

/**
 * This script assigns existing users to existing groups.
 * Requires Drupal 4.7 and OG module.
 *
 * Takes all groups, and assines a decreasing number of random users
 * as members.
 *
 * If n is the total number of registered users, the first group gets
 * all of them, the next gets half of them, etc. Admins are excluded.
 *
 * Sponsored by CivicSpace Labs
 */
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

function gou_get_groups() {
  $result = db_query("SELECT nid, uid FROM {og_uid} WHERE is_admin >= 1");
  $groups = array();
  while ($group = db_fetch_array($result)) {
    $groups[$group['nid']][] = $group['uid'];
  }

  return $groups;
}

function gou_count_users() {
  return db_result(db_query('SELECT COUNT(*) FROM {users} WHERE uid > 0'));
}

function gou_assign_users($nid, $admins, $limit = NULL) {
  $sql = 'INSERT INTO og_uid (nid, uid, is_active, mail_type) SELECT %d, uid, 1, 1 FROM users u WHERE u.uid NOT IN ('. str_pad('', count($admins) * 3 - 1, '%d,') .') ORDER BY RAND()';
  if ($limit) {
    $sql .= " LIMIT $limit";
  }
  db_query($sql, $nid, implode(',', $admins));
}


$users = gou_count_users();
$groups = gou_get_groups();

foreach ($groups as $nid => $group) {
  $node = node_load($nid);
  drupal_set_message(t('Assigned %n users to group %t.', array('%n' => $users, '%t' => theme('placeholder', $node->title))));
  gou_assign_users($nid, $group, $users);
  $users = floor($users / 2 + count($group));
}