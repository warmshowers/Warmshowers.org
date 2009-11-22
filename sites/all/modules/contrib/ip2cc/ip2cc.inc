<?php
// $Id: ip2cc.inc,v 1.1.2.4 2009/09/27 09:55:28 mrfelton Exp $

/**
 * @file
 * Include file for IP to Country module.
 */

function _ip2cc_inet_aton($a) {
  $ip = gethostbyname($a);
  return sprintf("%u", ip2long($ip));
}

function _ip2cc_batch_insert() {
  $file = drupal_get_path('module', 'ip2cc') .'/ip-to-country.csv';
  if (!$file || !file_exists($file)) {
    drupal_set_message(t("The ip2cc database file @file is missing. ", array('@file' => $file)));
    return FALSE;
  }
  if ($fd = fopen($file, 'r')) {
    // flush the existing entries
    db_query("DELETE FROM {ip2cc}");
    $count = 0;
    while ($line = fgets($fd)) {
      if (preg_match('/^"([0-9]+)","([0-9]+)","(.{2})","(.{3})","(.*)"$/', trim($line), $matches)) {
        db_query("INSERT INTO {ip2cc} VALUES('%s', '%s', '%s', 'Unknown', 0)", $matches[1], $matches[2], $matches[3]);
        $count++;
      }
    }
    fclose($fd);

    variable_set('ip2cc_last_update', time());
    return $count;
  }
  drupal_set_message(t("Could not open the ip2cc database (@file) for reading. Please check it's permissions.", array('@file' => $file)));
  return FALSE;
}

/**
 * Get the total count of IP ranges in database
 */
function _ip2cc_get_count() {
  $sql   = "SELECT COUNT(*) FROM {ip2cc}";
  $count = db_result(db_query($sql));
  return (int) $count;
}
