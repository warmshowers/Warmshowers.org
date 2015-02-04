<?php
/**
 * Temporary script dedicated to creating roles based on old donortools dump
 * 4 Feb 2015
 * by rfay
 *
 * Note that you should DELETE existing items first
 */

// Map role to range of donation amount
$role_amounts = array(
  24 => array(0, 1000000), // Current member; all amounts
  18 => array(0, 1),
  19 => array(1, 10),
  20 => array(10, 25),
  21 => array(25, 50),
  22 => array(50, 100),
  23 => array(100, 1000000),
);

$uc_roles_query = "insert ignore into uc_roles_expirations (uid,rid,expiration) (select u.uid, %d, unix_timestamp(date_add(d.maxdate, interval 365 day)) AS expiration from users u, (select email, max(date) as maxdate, sum(amount) as sumamount, date from tmp_donations group by email) d where u.mail = d.email and u.uid>0 and d.sumamount >= %d and d.sumamount < %d)";
$roles_query = "insert ignore into users_roles (uid,rid) (select u.uid, %d from users u, (select email, max(date) as maxdate, sum(amount) as sumamount, date from tmp_donations group by email) d where u.mail = d.email and u.uid>0 and d.sumamount >=%d and d.sumamount < %d)";

// DANGER WILL ROBINSON: Clears these roles globally to start with!
$result = db_query("DELETE FROM uc_roles_expirations where rid >= 18 and rid <= 24");
$result = db_query("DELETE FROM users_roles where rid >= 18 and rid <= 24");

foreach ($role_amounts as $role => $range) {
  $result = db_query($uc_roles_query, array($role, $range[0], $range[1]));
  $result = db_query($roles_query, array($role, $range[0], $range[1]));
}
