<?php

/**
 * Walk the file_managed (D7) table testing to see if image/jpeg files are
 * actually loadable by imagecreatefromjpeg().
 *
 * Run this from docroot with
 * drush scr ../resources/check_images.php
 */
$result = db_query('SELECT fm.fid, fm.uri, fm.filemime, ct.entity_id nid, ct.field_contest_photo_fid AS photo_fid, u.uid, u.picture FROM {file_managed} fm LEFT JOIN {field_data_field_contest_photo} ct ON (fm.fid = ct.field_contest_photo_fid) LEFT JOIN {users u} ON (fm.fid = u.picture)');

$count = 0;
if ($result) {
  while ($row = $result->fetchAssoc()) {
    if ($row['filemime'] == 'image/jpeg') {
//      print '.';
//      if (++$count % 120 == 0) {
//        print "\n";
//      }
      if (FALSE === imagecreatefromjpeg($row['uri'])) {
//        print "\nBad item fid=" . $row['fid'] . " path=" . $row['uri'] . '\n';
        // First check recommendation photos
        if (!empty($row['photo_fid'])) {
          print "nid|{$row['nid']}\n";
        }
        else if (!empty($row['uid'])) {
          print "uid|{$row['uid']}\n";
        }
        else {
          print "Failed to find bad object for file fid={$row['fid']} uri={$row['uri']}\n";
        }

      }
    }
  }
}
