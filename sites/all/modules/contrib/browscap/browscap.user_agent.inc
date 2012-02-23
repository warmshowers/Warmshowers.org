<?php
/**
 * @file
 * Displays user agent information.
 */

/**
 * Page callback to show details about a specific user agent.
 *
 * @param $user_agent
 *   A user agent string.
 */
function browscap_useragent_properties($user_agent = NULL) {
  // Attempt to find an entry in the database about the given user agent
  $user_agent_data = db_fetch_object(db_query("SELECT * FROM {browscap} WHERE useragent = '%s'", $user_agent));

  // Create an array to hold user agent properties
  $rows = array();

  // If an entry in the database about the current user agent exists
  // unserialize the entry and find all of the user agent properties and
  // values contained within the entry
  if ($user_agent_data) {
    // Unserialize the database entry
    $user_agent_properties = unserialize($user_agent_data->data);

    // Find all of the user agent properties and values
    foreach ($user_agent_properties as $property => $value) {
      $rows[] = array(check_plain($property), check_plain($value));
    }
  }

  // Create an array of table headers
  $header = array(
    t('Property'),
    t('Value'),
  );

  // Build the table of user agent properties
  $build = theme('table', $header, $rows);

  return $build;
}
