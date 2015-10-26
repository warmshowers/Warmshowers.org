<?php

/**
 * Walk the users table to see if user picture is
 * actually loadable by imagecreatefromjpeg().
 * If not loadable, delete the file and remove from the user profile
 *
 * Run this from docroot with
 * drush scr ../resources/fix_user_images.php
 */

$count = 0;
$start = 1;
$batch_size = 40;
$result = db_query('SELECT u.uid, u.picture, fm.fid, fm.uri, fm.filename, fm.filemime FROM {users} u, {file_managed} fm WHERE u.picture = fm.fid AND picture > 0 AND u.uid >= :uid', array(':uid' => $start));


if ($result) {
  while ($row = $result->fetchAssoc()) {
    if ($row['filemime'] == 'image/jpeg') {

      if (++$i % $batch_size == 0) {
        print "\n";
      }
      print "{$row['uid']}.";

      if (FALSE === imagecreatefromjpeg($row['uri'])) {

        // Failed to load the jpeg, so let's get rid of it.
        $file = file_load($row['fid']);
        if ($file !== FALSE) {
          $account = user_load($row['uid']);
          $saved_account = user_save($account, array('picture' => 0));
          if ($saved_account === FALSE) {
            print "Failed to save account {$row['uid']}\n";
          } else {
            print "Removed picture for uid {$row['uid']}\n";
          }

          file_usage_delete($file, 'user');
          $rv = file_delete($file);

          if ($rv !== TRUE) {
            print "{$row['fid']}: file_delete({$row['fid']}) not complete - result was " . print_r($rv, TRUE) . '\n';
          }

        } else {
          print "Failed to load file_managed for fid={$row['fid']}\n";
        }

      }
    }
  }
}
