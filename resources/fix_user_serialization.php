<?php

// Run this with drush -r /var/www/warmshowers_org/docroot scr fix_user_serialization.php

ini_set('memory_limit', -1);

$batch_size = 50;
$start = 1;

$result = db_query('SELECT uid, name, data FROM {users} WHERE uid >= :uid ORDER BY uid', array(':uid' => $start));

$i=0;
foreach ($result as $record) {

  if ($record->uid != 0) {
    $account = user_load($record->uid);
    $unserialized = unserialize($record->data);
    if ($unserialized === FALSE || $record->data == "b:0;") {
      print ++$i . ". {$account->uid}";
      if ($record->data == 'b:0;') {
        print " $record->data";
      }
      print "\n";
      $fixed_account = user_save($account);
      if ($fixed_account === FALSE) {
        print "Failed to resave $account->uid\n";
      }
      unset($account, $fixed_account);
      if ($i % $batch_size == 0) {
        break;
      }
    }
  }
}
