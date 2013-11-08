#!/usr/bin/env drush

<?php

/**
 * @file
 * Import the SQL DB from the Dropbox file.
 *
 * This script requires Drush 6.0.0 or later.
 */

// Get file name for sanitized dump.
$files = scandir(sprintf('/%s/Dropbox/ws_private', $_SERVER['HOME']));
$sql_dump = '';
foreach ($files as $file) {
  if (strpos($file, 'warmshowers_sanitized_') === 0) {
    $sql_dump = sprintf('%s/Dropbox/ws_private/%s', $_SERVER['HOME'], $file);
    break;
  }
}
// Fail early if dump isn't found.
if (!$sql_dump || !file_exists($sql_dump)) {
  return drush_set_error('NO_SQL_DUMP_FOUND', dt('Failed to find a SQL dump in Dropbox.'));
}
drush_log('Starting import of DB. This might take a while!', 'ok');
$self_record = drush_sitealias_get_record('@warmshowers.dev');
if (!$self_record) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load your @warmshowers.dev alias.'));
}
$database_name = $self_record['databases']['default']['default']['database'];
$database_username = $self_record['databases']['default']['default']['username'];
$database_password = '';
if ($self_record['databases']['default']['default']['password']) {
  $database_password = $self_record['databases']['default']['default']['password'];
}
$mysql_connect = sprintf('mysql -u%s', $database_username);
if ($database_password) {
  $mysql_connect .= sprintf(' -p%s', $database_password);
}
// Recreate database.
$cmd = sprintf('echo "DROP DATABASE %s" | %s', $database_name, $mysql_connect);
drush_log(dt('Dropping database !database', array('!database' => $database_name)));
drush_shell_exec($cmd);
$cmd = sprintf('echo "CREATE DATABASE %s" | %s', $database_name, $mysql_connect);
drush_shell_exec($cmd);
// Use gzip -dc to import the DB.
$cmd = sprintf('gzip -dc %s | %s %s', $sql_dump, $mysql_connect, $database_name);
drush_log(dt('Importing database with command: "!command"', array('!command' => $cmd)), 'ok');
return drush_shell_exec($cmd);
