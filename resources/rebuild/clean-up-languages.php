#!/usr/bin/env drush

<?php

/**
 * @file
 * Does the same work as clean_up_languages.sh
 */

$alias = drush_sitealias_get_record('@warmshowers.dev');
if (!$alias) {
  return drush_set_error('NO_ALIAS_FOUND', dt('Failed to load alias.'));
}

$query = "
  UPDATE languages SET domain=REPLACE(domain, 'warmshowers.org', '$alias');
  UPDATE languages SET domain=REPLACE(domain, 'https://', 'http://');
  UPDATE languages SET domain=REPLACE(domain, 'www.{$alias}', '$alias');
";

drush_invoke_process('@warmshowers.dev', 'sql-query', array($query));

// This is the "Clean up Variable" code from i18n.
$variables = i18n_variable();
if ($variables) {
  db_query("DELETE FROM {i18n_variable} WHERE name NOT IN (" . db_placeholders($variables, 'varchar') . ')', $variables);
}

drush_invoke_process('@warmshowers.dev', 'vdel', array('language_default'));

drush_invoke_process('@warmshowers.dev', 'cache-clear', array('all'));
drush_invoke_process('@warmshowers.dev', 'cache-clear', array('all'));
