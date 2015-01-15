<?php

/**
 * @file
 * File menu items.
 */

/**
 * The number of bogus requests one IP address can make before being banned.
 */
define('UC_FILE_REQUEST_LIMIT', 50);

/**
 * Download file chunk.
 */
define('UC_FILE_BYTE_SIZE', 8192);

/**
 * Download statuses.
 */
define('UC_FILE_ERROR_OK'                     , 0);
define('UC_FILE_ERROR_NOT_A_FILE'             , 1);
define('UC_FILE_ERROR_TOO_MANY_BOGUS_REQUESTS', 2);
define('UC_FILE_ERROR_INVALID_DOWNLOAD'       , 3);
define('UC_FILE_ERROR_TOO_MANY_LOCATIONS'     , 4);
define('UC_FILE_ERROR_TOO_MANY_DOWNLOADS'     , 5);
define('UC_FILE_ERROR_EXPIRED'                , 6);
define('UC_FILE_ERROR_HOOK_ERROR'             , 7);

/**
 * Themes user file downloads page.
 *
 * @param $files
 *   Array of the files.
 *
 * @ingroup themeable
 */
function theme_uc_file_user_downloads($header, $files) {
  $rows = array();
  $row = 0;
  foreach ($files as $file) {
    $rows[] = array(
      array('data' => format_date($file['granted'], 'custom', variable_get('uc_date_format_default', 'm/d/Y')), 'class' => 'date-row', 'id' => 'date-'. $row),
      array('data' => $file['link'], 'class' => 'filename-row', 'id' => 'filename-'. $row),
      array('data' => $file['description'], 'class' => 'description-row', 'id' => 'description-'. $row),
      array('data' => $file['accessed'] .'/'. ($file['download_limit'] ? $file['download_limit'] : t('Unlimited')), 'class' => 'download-row', 'id' => 'download-'. $row),
      array('data' => count(unserialize($file['addresses'])) .'/'. ($file['address_limit'] ? $file['address_limit'] : t('Unlimited')), 'class' => 'addresses-row', 'id' => 'addresses-'. $row),
    );
    $row++;
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No downloads found'), 'colspan' => 5));
  }

  $output  = theme('table', $header, $rows);
  $output .= theme('pager', NULL, UC_FILE_PAGER_SIZE, 0);
  $output .= '<div class="form-item"><p class="description">'.
  t('Once your download is finished, you must refresh the page to download again. (Provided you have permission)') .
  '<br />'. t('Downloads will not be counted until the file is finished transferring, even though the number may increment when you click.') .
  '<br /><b>'. t('Do not use any download acceleration feature to download the file, or you may lock yourself out of the download.') .'</b>'.
  '</p></div>';
  return $output;
}

/**
 * Table builder for user downloads.
 */
function uc_file_user_downloads($account) {
  // Create a header and the pager it belongs to.
  $header = array(
    array('data' => t('Purchased'  ), 'field' => 'u.granted', 'sort' => 'desc'),
    array('data' => t('Filename'   ), 'field' => 'f.filename'),
    array('data' => t('Description'), 'field' => 'p.description'),
    array('data' => t('Downloads'  ), 'field' => 'u.accessed'),
    array('data' => t('Addresses'  )),
  );

  drupal_set_title(t('File downloads'));

  $files = array();

  $result = pager_query(
    "SELECT u.granted, f.filename, u.accessed, u.addresses, p.description, u.file_key, f.fid, u.download_limit, u.address_limit, u.expiration FROM {uc_file_users} as u ".
    "LEFT JOIN {uc_files} as f ON u.fid = f.fid ".
    "LEFT JOIN {uc_file_products} as p ON p.pfid = u.pfid WHERE uid = %d".
    tablesort_sql($header),
    UC_FILE_PAGER_SIZE,
    0,
    "SELECT COUNT(*) FROM {uc_file_users} WHERE uid = %d",
    $account->uid
  );

  $row = 0;
  while ($file = db_fetch_object($result)) {

    $download_limit = $file->download_limit;

    // Set the JS behavior when this link gets clicked.
    $onclick = array(
      'attributes' => array(
        'onclick' => 'uc_file_update_download('. $row .', '. $file->accessed .', '. ((empty($download_limit)) ? -1 : $download_limit) .');', 'id' => 'link-'. $row
      ),
    );

    // Expiration set to 'never'
    if ($file->expiration == FALSE) {
      $file_link = l(basename($file->filename), 'download/'. $file->fid .'/'. $file->file_key, $onclick);
    }

    // Expired.
    elseif (time() > $file->expiration) {
      $file_link = basename($file->filename);
    }

    // Able to be downloaded.
    else {
      $file_link = l(basename($file->filename), 'download/'. $file->fid .'/'. $file->file_key, $onclick) .' ('. t('expires on @date', array('@date' => format_date($file->expiration, 'custom', variable_get('uc_date_format_default', 'm/d/Y')))) .')';
    }

    $files[] = array(
      'granted' => $file->granted,
      'link' => $file_link,
      'description' => $file->description,
      'accessed' => $file->accessed,
      'download_limit' => $file->download_limit,
      'addresses' => $file->addresses,
      'address_limit' => $file->address_limit,
    );
    $row++;
  }

  $output = theme('uc_file_user_downloads', $header, $files);

  if (user_access('administer users')) {
    $output .= drupal_get_form('uc_file_user_form', $account);
  }

  return $output;
}

/**
 * Handles file downloading and error states.
 *
 * @param $fid
 *   The fid of the file specified to download.
 * @param $key
 *   The hash key of a user's download.
 *
 * @see _uc_file_download_validate()
 */
function _uc_file_download($fid, $key) {

  global $user;

  // Error messages for various failed download states.
  $admin_message = t('Please contact the site administrator if this message has been received in error.');
  $error_messages = array(
    UC_FILE_ERROR_NOT_A_FILE              => t('The file you requested does not exist. '),
    UC_FILE_ERROR_TOO_MANY_BOGUS_REQUESTS => t('You have attempted to download an incorrect file URL too many times. '),
    UC_FILE_ERROR_INVALID_DOWNLOAD        => t("The following URL is not a valid download link. "),
    UC_FILE_ERROR_TOO_MANY_LOCATIONS      => t('You have downloaded this file from too many different locations. '),
    UC_FILE_ERROR_TOO_MANY_DOWNLOADS      => t('You have reached the download limit for this file. '),
    UC_FILE_ERROR_EXPIRED                 => t("This file download has expired. "),
    UC_FILE_ERROR_HOOK_ERROR              => t("A hook denied your access to this file. "),
  );

  $ip = ip_address();
  $file_download = uc_file_get_by_key($key);

  $file_download->full_path = uc_file_qualify_file($file_download->filename);

  // If it's ok, we push the file to the user.
  $status = _uc_file_download_validate($file_download, $user, $ip);
  if ($status == UC_FILE_ERROR_OK) {
    _uc_file_download_transfer($file_download, $ip);
  }

  // Some error state came back, so report it.
  else {
    drupal_set_message($error_messages[$status] . $admin_message, 'error');
  }

  // Kick 'em to the curb. >:)
  _uc_file_download_redirect($user->uid);
}

/**
 * Performs first-pass authorization. Call authorization hooks afterwards.
 *
 * Called when a user requests a file download, function checks download
 * limits then checks for any implementation of hook_download_authorize.
 * Passing that, the function _uc_file_download_transfer is called.
 *
 * @param $fid
 *   The fid of the file specified to download.
 * @param $key
 *   The hash key of a user's download.
 */
function _uc_file_download_validate($file_download, &$user, $ip) {

  $request_cache = cache_get('uc_file_'. $ip);
  $requests = ($request_cache) ? $request_cache->data + 1 : 1;

  $message_user = ($user->uid) ? t('The user %username ', array('%username' => $user->name)) : t('The IP address %ip ', array('%ip' => $ip));

  if ($requests > UC_FILE_REQUEST_LIMIT) {
    return UC_FILE_ERROR_TOO_MANY_BOGUS_REQUESTS;
  }

  // Must be a valid file.
  if (!$file_download || !is_readable($file_download->full_path)) {
    cache_set('uc_file_'. $ip, $requests, 'cache', time() + 86400);
    if ($requests == UC_FILE_REQUEST_LIMIT) {
      watchdog('uc_file', '%username has been temporarily banned from file downloads.', array('%username' => $message_user), WATCHDOG_WARNING);
    }

    return UC_FILE_ERROR_INVALID_DOWNLOAD;
  }

  $addresses = $file_download->addresses;

  // Check the number of locations.
  if (!empty($file_download->address_limit) && !in_array($ip, $addresses) && count($addresses) >= $file_download->address_limit) {
    watchdog('uc_file', '%username has been denied a file download by downloading it from too many IP addresses.', array('%username' => $message_user), WATCHDOG_WARNING);

    return UC_FILE_ERROR_TOO_MANY_LOCATIONS;
  }

  // Check the downloads so far.
  if (!empty($file_download->download_limit) && $file_download->accessed >= $file_download->download_limit) {
    watchdog('uc_file', '%username has been denied a file download by downloading it too many times.', array('%username' => $message_user), WATCHDOG_WARNING);

    return UC_FILE_ERROR_TOO_MANY_DOWNLOADS;
  }

  // Check if it's expired.
  if ($file_download->expiration && time() >= $file_download->expiration) {
    watchdog('uc_file', '%username has been denied an expired file download.', array('%username' => $message_user), WATCHDOG_WARNING);

    return UC_FILE_ERROR_EXPIRED;
  }

  // Check any if any hook_download_authorize calls deny the download
  foreach (module_implements('download_authorize') as $module) {
    $name = $module .'_download_authorize';
    $result = $name($user, $file_download);
    if (!$result) {
      return UC_FILE_ERROR_HOOK_ERROR;
    }
  }

  // Everything's ok!
  watchdog('uc_file', '%username has started download of the file %filename.', array('%username' => $message_user, '%filename' => basename($file_download->filename)), WATCHDOG_NOTICE);
}

/**
 * Sends the file's binary data to a user via HTTP and updates the
 * uc_file_users table.
 *
 * @param $file_user
 *   The file_user object from the uc_file_users.
 * @param $ip
 *   The string containing the ip address the download is going to.
 */
function _uc_file_download_transfer($file_user, $ip) {

  // Check if any hook_file_transfer_alter calls alter the download.
  foreach (module_implements('file_transfer_alter') as $module) {
    $name = $module .'_file_transfer_alter';
    $file_user->full_path = $name($file_user, $ip, $file_user->fid, $file_user->full_path);
  }

  // This could get clobbered, so make a copy.
  $filename = $file_user->filename;

  // Gather relevant info about the file.
  $size = filesize($file_user->full_path);
  $mimetype = file_get_mimetype($filename);

  // Workaround for IE filename bug with multiple periods / multiple dots
  // in filename that adds square brackets to filename -
  // eg. setup.abc.exe becomes setup[1].abc.exe
  if (strstr($_SERVER['HTTP_USER_AGENT'], 'MSIE')) {
    $filename = preg_replace('/\./', '%2e', $filename, substr_count($filename, '.') - 1);
  }

  // Check if HTTP_RANGE is sent by browser (or download manager)
  $range = NULL;
  if (isset($_SERVER['HTTP_RANGE'])) {
    list($size_unit, $range_orig) = explode('=', $_SERVER['HTTP_RANGE'], 2);

    if ($size_unit == 'bytes') {
      // Multiple ranges could be specified at the same time,
      // but for simplicity only serve the first range
      // See http://tools.ietf.org/id/draft-ietf-http-range-retrieval-00.txt
      list($range, $extra_ranges) = explode(',', $range_orig, 2);
    }
  }

  // Figure out download piece from range (if set)
  if (isset($range)) {
    list($seek_start, $seek_end) = explode('-', $range, 2);
  }

  // Set start and end based on range (if set),
  // else set defaults and check for invalid ranges.
  $seek_end = intval((empty($seek_end)) ? ($size - 1) : min(abs(intval($seek_end)), ($size - 1)));
  $seek_start = intval((empty($seek_start) || $seek_end < abs(intval($seek_start))) ? 0 : max(abs(intval($seek_start)), 0));

  // Only send partial content header if downloading a piece of the file (IE
  // workaround).
  if ($seek_start > 0 || $seek_end < ($size - 1)) {
    drupal_set_header('HTTP/1.1 206 Partial Content');
  }

  // Standard headers, including content-range and length
  drupal_set_header('Pragma: public');
  drupal_set_header('Cache-Control: cache, must-revalidate');
  drupal_set_header('Accept-Ranges: bytes');
  drupal_set_header('Content-Range: bytes '. $seek_start .'-'. $seek_end .'/'. $size);
  drupal_set_header('Content-Type: '. $mimetype);
  drupal_set_header('Content-Disposition: attachment; filename="'. $filename .'"');
  drupal_set_header('Content-Length: '. ($seek_end - $seek_start + 1));

  // Last-Modified is required for content served dynamically
  drupal_set_header('Last-Modified: '. gmdate("D, d M Y H:i:s", filemtime($file_user->full_path)) ." GMT");

  // Etag header is required for Firefox3 and other managers
  drupal_set_header('ETag: '. md5($file_user->full_path));

  // Open the file and seek to starting byte
  $fp = fopen($file_user->full_path, 'rb');
  fseek($fp, $seek_start);

  // Start buffered download
  while (!feof($fp)) {

    // Reset time limit for large files
    set_time_limit(0);

    // Push the data to the client.
    print(fread($fp, UC_FILE_BYTE_SIZE));
    flush();

    // Not sure why ob_flush() is required here, but leaving it out breaks
    // some systems (clicking on link doesn't download anything without it).
    ob_flush();
  }

  // Finished serving the file, close the stream and log the download
  // to the user table.
  fclose($fp);

  _uc_file_log_download($file_user, $ip);

}

/**
 * Processes a file download.
 */
function _uc_file_log_download($file_user, $ip) {

  // Add the address if it doesn't exist.
  $addresses = $file_user->addresses;
  if (!in_array($ip, $addresses)) {
    $addresses[] = $ip;
  }
  $file_user->addresses = $addresses;

  // Accessed again.
  $file_user->accessed++;

  // Calculate hash
  $file_user->file_key = drupal_get_token(serialize($file_user));

  drupal_write_record('uc_file_users', $file_user, 'fuid');
}

/**
 * Send 'em packin.
 */
function _uc_file_download_redirect($uid = NULL) {

  // Shoo away anonymous users.
  if ($uid == 0) {
    drupal_access_denied();
  }
  // Redirect users back to their file page.
  else {
    if (!headers_sent()) {
      drupal_goto('user/'. $uid .'/purchased-files');
    }
  }
}
