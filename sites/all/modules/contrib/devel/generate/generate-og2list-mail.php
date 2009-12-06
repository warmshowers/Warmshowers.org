<?php

// $Id: generate-og2list-mail.php,v 1.4 2006/06/27 21:51:29 killes Exp $:

/**
 * This script creates bogus mails from existing users to existing groups.
 * Requires Drupal 4.7, OG2list and OG module.
 *
 * If n is the total number of members of a group, this script will
 * generate n*(n-1) mails. Use with caution.
 *
 * Sponsored by CivicSpace Labs
 */
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

function gom_get_groups() {
  $result = db_query("SELECT g.nid, g.recipient, u.uid, u.name, u.mail FROM {og2list_groups} g INNER JOIN {og_uid} o ON o.nid = g.nid INNER JOIN {users} u ON o.uid = u.uid WHERE u.status >= 1");
  $groups = array();
  while ($group = db_fetch_array($result)) {
    $groups[$group['nid']]['recipient'] = $group['recipient'];
    $groups[$group['nid']]['users'][] = array(
      'uid' => $group['uid'],
      'mail' => $group['mail'],
      'name' => $group['name'],
      );
  }

  return $groups;
}

function gom_create_content($groups) {
  foreach ($groups as $nid => $group) {
    foreach ($group['users'] as $user) {
      $msg_id = '<'. time() .'.'. mt_rand() .'@'. strtolower(variable_get('og2list_domain', $_SERVER['SERVER_NAME'])) .'>';

      $subject = 'Test mail from '. $user['name'] .' to '. $group['recipient'];
      db_query("INSERT INTO {og2list_incoming_content} (from_address,from_name,subject,msgid,content_type,body) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')", $user['mail'], $user['name'], $subject, $msg_id, 'text/plain', "totally irrelevant message body, sent at ". format_date(time(), 'custom', t('Y/m/d - G:i')));
      db_query("INSERT INTO {og2list_incoming_groups} SET mid=(SELECT mid FROM {og2list_incoming_content} WHERE msgid='%s'), oid=(SELECT nid FROM {og2list_groups} WHERE recipient='%s')", $msg_id, $group['recipient']);
    }
  }
}


$groups = gom_get_groups();

gom_create_content($groups);
