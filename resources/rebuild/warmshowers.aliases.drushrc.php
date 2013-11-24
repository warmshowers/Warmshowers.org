<?php

/**
 * @file
 * This is a Drush alias for a local Warmshowers.org development site.
 */

/**
 * This is an example alias for a warmshowers.dev local environment.
 *
 * You can learn quite a bit about Drush aliases by accessing
 * `drush docs-aliases`.
 *
 * There are three TODOs listed below. Make sure you attend to all of them!
 */
// TODO: Edit the 'root' value to match the path to your local site.
// Don't add a trailing slash to the $site_root variable.
$aliases['dev'] = array(
  'root' => '/path/to/warmshowers-repo/docroot',
  'uri' => 'http://warmshowers.dev',
  // TODO: Make sure you have created a local database called 'warmshowers'.
  'db-url' => 'mysql://root@localhost/warmshowers',
  'path-aliases' => array(
    '%rebuild' => '/path/to/warmshowers-repo/resources/rebuild/warmshowers.rebuild.yaml',
  ),
  // TODO: Set your email address in the 'email' section below.
  // TODO: Optionally, set the path to Dropbox. You don't need to do this if
  // your Dropbox directory is at "~/Dropbox".
  '#rebuild' => array(
    'email' => 'youremailhere@localhost.com',
    'dropbox_root' => '/path/to/Dropbox',
  ),
);
