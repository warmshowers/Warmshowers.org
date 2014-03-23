#!/usr/bin/env drush
<?php

/**
 * @file
 * Sanitization script to remove user data.
 */

$self_record = drush_sitealias_get_record('@warmshowers.dev');
// Sanity check.
if (!$self_record) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load your @warmshowers.dev alias.'));
}

drush_log('Updating database to hide sensitive information', 'ok');
// Table containing users.
$table_user = 'users';
// Table containing user's location.
$table_location = 'user_location';
// Custom wsuser table.
$table_wsuser = 'wsuser';

$req = "UPDATE {$table_location} SET
street = CONCAT(oid, '_street'),
city = CONCAT(oid, '_city'),
postal_code = CONCAT(oid, '_postcode')";

drush_invoke_process('@warmshowers.dev', 'sql-query', array($req));

$req = "UPDATE {$table_wsuser} SET
  fullname = CONCAT(uid,'_fullname'),
  fax_number = case WHEN fax_number = '' THEN '' ELSE CONCAT(uid, '_fax_number') END,
  homephone = case WHEN homephone = '' THEN '' ELSE CONCAT(uid, '_homephone') END,
  mobilephone = case WHEN mobilephone = '' THEN '' ELSE CONCAT(uid, '_mobilephone') END,
  workphone = case WHEN workphone = '' THEN '' ELSE CONCAT(uid, '_workphone') END";

drush_invoke_process('@warmshowers.dev', 'sql-query', array($req));

$req = "UPDATE {$table_user} SET data = ''";
drush_invoke_process('@warmshowers.dev', 'sql-query', array($req));
