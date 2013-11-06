<?php

/**
 * @file
 * This is a Drush alias for a local Warmshowers.org development site.
 */

/**
 * This is an example of an alias for a local dev environment.
 *
 * You can learn quite a bit about Drush aliases by accessing
 * `drush docs-aliases`.
 */
$aliases['local'] = array(
  'root' => '/home/kosta/src/warmshowers.org',
  'uri' => 'https://local.warmshowers.org',
  'db-url' => 'mysql://root@localhost/warmshowers_local',
  'path-aliases' => array(
    // Under path aliases, you specify the full path to the rebuild manifest
    // for your local environment.
    '%rebuild' => '/home/kosta/.drush/warmshowers.rebuild.yaml',
  ),
  // In the rebuild section of your alias, you can define variables to replace
  // placeholders in your manifest file.
  //
  // For example, if you had variable[site_mail] = %email in your rebuild
  // manifest, then the value here would be swapped with the placeholder during
  // the rebuild.
  '#rebuild' => array(
    'email' => 'kosta@embros.org',
  ),
);
