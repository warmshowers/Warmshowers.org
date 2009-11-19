<?php
// $Id: deadwood.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * The default directory to store modules to be converted. Relative to file_directory_path().
 */
define('DEADWOOD_IN', 'deadwood');

/**
 * The default directory to store converted modules. Relative to file_directory_path().
 */
define('DEADWOOD_OUT', 'goodwood');

/**
 * The values for the availability status of conversion code for a category.
 * NOTE: This should be for an item not a category.
 */
define('DEADWOOD_CONVERSION_CODE_NOT_AVAILABLE', 0);
define('DEADWOOD_CONVERSION_CODE_AVAILABLE', 1);
define('DEADWOOD_CONVERSION_CODE_PARTIALLY_AVAILABLE', 2);
define('DEADWOOD_CONVERSION_CODE_REQUIRES_MANUAL', 3);
define('DEADWOOD_CONVERSION_CODE_NOTIFY_ONLY', 4);
define('DEADWOOD_CONVERSION_CODE_NOT_NEEDED', 5);

/**
 * Pass a string through t() and wrap the result in html entity <p>.
 */
function tp($string, $args = array()) {
  return '<p>' . t($string, $args) . '</p>';
}

/**
 * Convert a category's code availability status (integer) to a string.
 *
 * @param integer $type Availability status code.
 * @return string Representation of availability status code.
 */
function deadwood_get_code_availability_status($type) {
  switch ($type) {
    case DEADWOOD_CONVERSION_CODE_AVAILABLE:
      return t('Available');
    case DEADWOOD_CONVERSION_CODE_PARTIALLY_AVAILABLE:
      return t('Partially available');
    case DEADWOOD_CONVERSION_CODE_REQUIRES_MANUAL:
      return t('Manual intervention');
    case DEADWOOD_CONVERSION_CODE_NOTIFY_ONLY:
      return t('Notification only');
    case DEADWOOD_CONVERSION_CODE_NOT_NEEDED:
      return t('Not needed');
    default:
      return t('Not available');
  }
}

/**
 * Scan a specified directory and find all first-level directories beneath it.
 *
 * TODO Replace this with a call to file_scan_directory in include/files.inc.
 *
 * @param string $path Directory path.
 * @return Array of directory names.
 */
function deadwood_scan_directory($path) {
  static $ignore = array('.', '..', '.svn');
  $dirs = array();

  $path = $path .'/';
  $files = scandir($path);
  foreach ($files as $file) {
    $file_path = $path . $file;
    if (is_dir($file_path) && !in_array(basename($file_path), $ignore)) {
      $dirs[] = $file;
    }
  }
  return $dirs;
}

/**
 * Remove all files from specified directory and optionally remove the directory.
 *
 * @param string $path Directory path.
 */
function deadwood_clean_directory($path, $remove_me = FALSE) {
  $path = $path .'/';
  $files = scandir($path);
  foreach ($files as $file) {
    if ($file != '.' && $file != '..') {
      $file_path = $path . $file;
      if (is_dir($file_path)) {
        deadwood_clean_directory($file_path, TRUE);
      }
      else {
        file_delete($file_path);
      }
    }
  }
  if ($remove_me) {
    rmdir($path);
  }
}
