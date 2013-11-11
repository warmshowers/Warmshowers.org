#!/usr/bin/env drush

<?php

/**
 * @file
 * Sanitizes user e-mails after importing the DB.
 */

drush_log("This script will give dummy emails to all but a few key accounts. It's intended for the database `warmshowers`.");

$self_record = drush_sitealias_get_record('@warmshowers.dev');
if (!$self_record) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load your @warmshowers.dev alias.'));
}
$query = "UPDATE users SET mail=CONCAT('user_', uid, '@localhost') WHERE uid NOT IN (1,1165, 12075, 8088, 36456, 18358, 36076);";
$result = drush_invoke_process('@warmshowers.dev', 'sql-query', array($query));
