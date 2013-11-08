#!/usr/bin/env drush

<?php

/**
 * @file
 * Does the same work as clean_up_languages.sh
 */

$dev_site_host = 'warmshowers.dev';
$www_prefix = '';
$alias = drush_sitealias_get_record('@warmshowers.dev');
if (!$alias) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load alias.'));
}
if (!file_exists($alias['root'] . '/' . 'assets/devsite_scripts/languages.sql')) {
  return drush_set_error('NO_LANGUAGES_SQL', dt('Could not find languages.sql'));
}
else {
  $languages_sql = $alias['root'] . '/' . 'assets/devsite_scripts/languages.sql';
}
drush_shell_exec(sprintf('bash %s', $alias['root'] . '/' . 'assets/rebuild/clean-up-languages-sed.sh'));
drush_invoke_process('@warmshowers.dev', 'cache-clear', array('all'));

// This is the "Clean up Variable" code from i18n.
if ($variables = i18n_variable()) {
  db_query("DELETE FROM {i18n_variable} WHERE name NOT IN (" . db_placeholders($variables, 'varchar') . ')', $variables);
}
