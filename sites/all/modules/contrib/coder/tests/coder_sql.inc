<?php
// $Id: coder_sql.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * This include file implements tests for the Drupal SQL as defined by
 * http://drupal.org/node/2497
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder.
 */

function coder_test_sql() {
  $sql = 'select * from {node}'; // lower case
  $sql = 'insert into {node} (changed) VALUES (%d)'; // lower case
  $sql = 'delete from {node}'; // lower case
  $sql = 'update {node} set changed = now()'; // lower case
  $var = t('select something from this');
  $var = t('update something');
  $var = t('insert something');
  $var = t('delete something');
  $sql = 'INSERT INTO node (changed) VALUES (1)';
  $sql = 'SELECT * FROM {node} LIMIT 10';
  $sql = "SELECT * FROM {node} WHERE nid=$nid"; // unsecure
  $sql = "SELECT * FROM {false_accounts} WHERE uids REGEXP '^%s,|,%s,|,%s$'"; // is OK
  $sql = "SELECT COUNT(n.nid) FROM {node} n INNER JOIN {node_revisions} r USING (nid, vid) WHERE n.type='%s' AND (r.title REGEXP '^[^[:alpha:]].*$')";
  $sql = "SELECT COUNT(n.nid) FROM {node} n INNER JOIN {node_revisions} r USING (nid, vid) WHERE n.type='%s' AND (r.title REGEXP '^[^[:alpha:]].*$') AND nid=$nid";
  $sql = "SELECT COUNT(n.nid) FROM {node} n INNER JOIN {node_revisions} r USING (nid, vid) WHERE n.type=$type AND (r.title REGEXP '^[^[:alpha:]].*$')";

  // Back tick usage tests.
  $sql = 'SELECT * FROM {node} WHERE title=`abc`';
  $sql = 'INSERT INTO {foo} (nid, title) VALUES ("1", `abc`)';
  $sql = 'INSERT INTO {foo} VALUES ("1", `abc`)';
  $sql = 'UPDATE {foo} SET nid="1", title=`abc`';
  $sql = 'DELETE FROM {foo} WHERE nid="1" AND title=`abc`';

  // Unquoted %s placeholder tests.
  $sql = 'SELECT * FROM {foo} WHERE name=%s';
  $sql = 'INSERT INTO {foo} (%s)';
  $sql = 'INSERT INTO {foo} (1,%s)';
  $sql = 'INSERT INTO {foo} (1, %s)';

  // Variables withing db_query queries.
  db_query("SELECT * FROM {foo} WHERE name=$name");
  db_query("INSERT INTO {foo} SET name='$name'");
  update_sql("INSERT INTO {foo} SET name='$name'");
}
