<?php

/**
 * @file
 * Browscap Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function browscap_drush_command() {
  $items['browscap-import'] = array(
    'description' => 'Imports/updates browscap data via HTTP.',
  );
  $items['browscap-get-browser'] = array(
    'description' => dt('Show browser information for a given user agent.'),
    'arguments' => array(
      'user agent' => 'Quote-enclosed user agent',
    ),
    'required-arguments' => TRUE,
    'examples' => array("drush browscap-get-browser 'Mozilla/5.0 (BlackBerry; U; BlackBerry 9700; en) AppleWebKit/534.8+ (KHTML, like Gecko) Version/6.0.0.448 Mobile Safari/534.8+'"),
    'outputformat' => array(
      'default' => 'json',
    ),
  );
  return $items;
}

/**
 * Imports/updates browscap data via HTTP.
 */
function drush_browscap_import() {
  drush_log(dt('Importing Browscap data...'), 'ok');
  switch (_browscap_import()) {
    case BROWSCAP_IMPORT_VERSION_ERROR:
      drush_set_error('BROWSCAP', dt('Could not check current available Browscap data version.'));
      break;

    case BROWSCAP_IMPORT_NO_NEW_VERSION:
      drush_log(dt('No new version of Browscap data to import.'), 'ok');
      break;

    case BROWSCAP_IMPORT_DATA_ERROR:
      drush_set_error('BROWSCAP', dt('Failed to retrieve or load Browscap data.'));
      break;

    case BROWSCAP_IMPORT_OK:
      drush_log(dt('Imported Browscap data successfully.'), 'ok');
      break;
  }
}

/**
 * Gets browser information for Drush output.
 *
 * @param string $user_agent
 *   User agent.
 *
 * @return array
 *   An array of information about the user agent.
 */
function drush_browscap_get_browser($user_agent) {
  return browscap_get_browser($user_agent);
}
