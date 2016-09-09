<?php
/**
 * @file
 * Browscap data import functions.
 */

define('BROWSCAP_IMPORT_OK', 0);
define('BROWSCAP_IMPORT_VERSION_ERROR', 1);
define('BROWSCAP_IMPORT_NO_NEW_VERSION', 2);
define('BROWSCAP_IMPORT_DATA_ERROR', 3);

/**
 * Helper function to update the browscap data.
 *
 * @param bool $cron
 *   Optional import environment. If false, display status messages to the user
 *   in addition to logging information with the watchdog.
 *
 * @return int
 *   A code indicating the result:
 *   - BROWSCAP_IMPORT_OK: New data was imported.
 *   - BROWSCAP_IMPORT_NO_NEW_VERSION: No new data version was available.
 *   - BROWSCAP_IMPORT_VERSION_ERROR: Checking the current data version failed.
 *   - BROWSCAP_IMPORT_DATA_ERROR: The data could not be downloaded or parsed.
 */
function _browscap_import($cron = TRUE) {
  // Check the local browscap data version number.
  $local_version = variable_get('browscap_version', 0);

  watchdog('browscap', 'Checking for new browscap version...');

  // Retrieve the current browscap data version number using HTTP.
  $current_version = drupal_http_request('https://www.browscap.org/version-number');

  // Log an error if the browscap version number could not be retrieved.
  if (isset($current_version->error)) {
    // Log a message with the watchdog.
    watchdog('browscap', "Couldn't check version: %error", array('%error' => $current_version->error), WATCHDOG_ERROR);

    // Display a message to user if the update process was triggered manually.
    if ($cron == FALSE) {
      drupal_set_message(t("Couldn't check version: %error", array('%error' => $current_version->error)), 'error');
    }

    return BROWSCAP_IMPORT_VERSION_ERROR;
  }

  // Sanitize the returned version number.
  $current_version = check_plain(trim($current_version->data));

  // Compare the current and local version numbers to determine if the browscap
  // data requires updating.
  if ($current_version == $local_version) {
    // Log a message with the watchdog.
    watchdog('browscap', 'No new version of browscap to import');

    // Display a message to user if the update process was triggered manually.
    if ($cron == FALSE) {
      drupal_set_message(t('No new version of browscap to import'));
    }

    return BROWSCAP_IMPORT_NO_NEW_VERSION;
  }

  // Set options for downloading data with or without compression.
  if (function_exists('gzdecode')) {
    $options = array(
      'headers' => array('Accept-Encoding' => 'gzip'),
    );
  }
  else {
    // The download takes over ten times longer without gzip, and may exceed
    // the default timeout of 30 seconds, so we increase the timeout.
    $options = array('timeout' => 600);
  }

  // Retrieve the browscap data using HTTP.
  $browscap_data = drupal_http_request('https://www.browscap.org/stream?q=PHP_BrowsCapINI', $options);

  // Log an error if the browscap data could not be retrieved.
  if (isset($browscap_data->error) || empty($browscap_data)) {
    // Log a message with the watchdog.
    watchdog('browscap', "Couldn't retrieve updated browscap: %error", array('%error' => $browscap_data->error), WATCHDOG_ERROR);

    // Display a message to user if the update process was triggered manually.
    if ($cron == FALSE) {
      drupal_set_message(t("Couldn't retrieve updated browscap: %error", array('%error' => $browscap_data->error)), 'error');
    }

    return BROWSCAP_IMPORT_DATA_ERROR;
  }

  // Decompress the downloaded data if it is compressed.
  if (function_exists('gzdecode')) {
    $browscap_data->data = gzdecode($browscap_data->data);
  }

  // Parse the returned browscap data.
  // The parse_ini_string function is preferred but only available in PHP 5.3.0.
  if (version_compare(PHP_VERSION, '5.3.0', '>=')) {
    // Retrieve the browscap data.
    $browscap_data = $browscap_data->data;

    // Replace 'true' and 'false' with '1' and '0'
    $browscap_data = preg_replace(
      array(
        "/=\s*true\s*\n/",
        "/=\s*false\s*\n/",
      ),
      array(
        "=1\n",
        "=0\n",
      ),
      $browscap_data
    );

    // Parse the browscap data as a string.
    $browscap_data = parse_ini_string($browscap_data, TRUE, INI_SCANNER_RAW);
  }
  else {
    // Create a path and filename.
    $server = $_SERVER['SERVER_NAME'];
    $path = variable_get('file_temporary_path', '/tmp');
    $file = "$path/browscap_$server.ini";

    // Write the browscap data to a file.
    $browscap_file = fopen($file, "w");
    fwrite($browscap_file, $browscap_data->data);
    fclose($browscap_file);

    // Parse the browscap data as a file.
    $browscap_data = parse_ini_file($file, TRUE);
  }

  if ($browscap_data) {
    // Find the version information.
    // The version information is the first entry in the array.
    $version = array_shift($browscap_data);

    // Save parsed Browscap data.
    _browscap_save_parsed_data($browscap_data);

    // Clear the browscap data cache.
    cache_clear_all('*', 'cache_browscap', TRUE);

    // Update the browscap version and imported time.
    variable_set('browscap_version', $current_version);
    variable_set('browscap_imported', REQUEST_TIME);

    // Log a message with the watchdog.
    watchdog('browscap', 'New version of browscap imported: %version', array('%version' => $current_version));

    // Display a message to user if the update process was triggered manually.
    if ($cron == FALSE) {
      drupal_set_message(t('New version of browscap imported: %version', array('%version' => $current_version)));
    }

    return BROWSCAP_IMPORT_OK;
  }

  return BROWSCAP_IMPORT_DATA_ERROR;
}

/**
 * Saves parsed Browscap data.
 *
 * The purpose of this function is to perform the queries on the {browscap}
 * table as a transaction. This vastly improves performance with database
 * engines such as InnoDB and ensures that queries will work while new data
 * is being imported.
 *
 * @param array $browscap_data
 *   Browscap data that has been parsed with parse_ini_string() or
 *   parse_ini_file().
 */
function _browscap_save_parsed_data(&$browscap_data) {
  // Start a transaction. The variable is unused. That's on purpose.
  $transaction = db_transaction();

  // Delete all data from database.
  db_delete('browscap')->execute();

  // Prepare the data for insertion.
  $import_data = array();
  foreach ($browscap_data as $key => $values) {
    // Store the current value.
    $original_values = $values;

    // Replace '*?' with '%_'.
    $user_agent = strtr($key, '*?', '%_');

    // Remove trailing spaces to prevent "duplicate entry" errors. Databases
    // such as MySQL do not preserve trailing spaces when storing VARCHARs.
    $user_agent = rtrim($user_agent);

    // Change all array keys to lowercase.
    $original_values = array_change_key_case($original_values);

    // Add to array of data to import.
    $import_data[$user_agent] = $original_values;

    // Remove processed data to reduce memory usage.
    unset($browscap_data[$key]);
  }

  // Get user agents to insert, removing case-insensitive duplicates.
  $user_agents = array_keys($import_data);
  $user_agents = array_intersect_key($user_agents, array_unique(array_map('strtolower', $user_agents)));

  // Insert all data about user agents into database in chunks.
  $user_agents_chunks = array_chunk($user_agents, 1000);
  foreach ($user_agents_chunks as $user_agents_chunk) {
    $query = db_insert('browscap')
      ->fields(array('useragent', 'data'));
    foreach ($user_agents_chunk as $user_agent) {
      $values = $import_data[$user_agent];

      // Recurse through the available user agent information.
      $previous_parent = NULL;
      $parent = isset($values['parent']) ? $values['parent'] : FALSE;
      while ($parent && $parent !== $previous_parent) {
        $parent_values = isset($import_data[$parent]) ? $import_data[$parent] : array();
        $values = array_merge($parent_values, $values);
        $previous_parent = $parent;
        $parent = isset($parent_values['parent']) ? $parent_values['parent'] : FALSE;
      }

      $query->values(array(
        'useragent' => $user_agent,
        'data' => serialize($values),
      ));
    }
    $query->execute();
  }
}
