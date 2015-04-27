<?php
/**
 * @file
 * Provide a url_to_path() function by refactoring and repurposing conf_path().
 *
 * @see http://drupal.org/node/1113588
 */

/**
 * Find the appropriate configuration directory for a given host and path.
 *
 * @param $http_host
 *   The hostname and optional port number, e.g. "www.example.com" or
 *   "www.example.com:8080".
 *
 * @param $script_name
 *   The part of the url following the hostname, including the leading slash.
 *
 * @return
 *   The path of the matching configuration directory.
 *
 * @see conf_path()
 */
function find_conf_path($http_host, $script_name, $require_settings = TRUE) {
  $confdir = 'sites';

  $sites = array();
  if (file_exists(DRUPAL_ROOT . '/' . $confdir . '/sites.php')) {
    // This will overwrite $sites with the desired mappings.
    include(DRUPAL_ROOT . '/' . $confdir . '/sites.php');
  }

  $uri = explode('/', $script_name);
  $server = explode('.', implode('.', array_reverse(explode(':', rtrim($http_host, '.')))));
  for ($i = count($uri) - 1; $i > 0; $i--) {
    for ($j = count($server); $j > 0; $j--) {
      $dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
      if (isset($sites[$dir]) && file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $sites[$dir])) {
        $dir = $sites[$dir];
      }
      if (file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $dir . '/settings.php') || (!$require_settings && file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $dir))) {
        $conf = "$confdir/$dir";
        return $conf;
      }
    }
  }
  $conf = "$confdir/default";
  return $conf;
}

/**
 * Returns the local relative path corresponding to a given URL, if possible.
 *
 * Finds the configuration directory matching the given URL, and compares it
 * to the current configuration directory. Returns a path relative to the drupal
 * install if they match, and FALSE if they don't.
 *
 * In a properly-configured multisite installation, this function helps answer
 * the question, "Could a given URL match a file or path within my site?"
 *
 * Whether the URL does in fact resolve to this site, or at all, cannot be
 * determined within Drupal itself.  This is only a sanity check.
 *
 * @param $url
 *   The internal path or external URL being linked to, such as
 *   "drupal/node/34" or "http://example.com/drupal/foo".
 * @return
 *   FALSE if $url contains a host/port/path that does not match the current
 *   site, or else a drupal path such as "node/34" or "foo".
 *
 * @see conf_path(), find_conf_path()
 */
function url_to_path($url) {
  global $base_path;
  $local = &drupal_static('__FUNCTION__', array());
  $base_len = strlen($base_path);

  $parts = parse_url($url);
  $http_host = isset($parts['host'])
    ? $parts['host'] : $_SERVER['HTTP_HOST'];
  if (isset($parts['port'])) {
    $http_host .= ':' . $parts['port'];
  }
  $script_name = '/';
  if (isset($parts['path'])) {
    $script_name .= ltrim($parts['path'], '/');
  }
  $path = $http_host . $script_name;
  if (!isset($local[$path])) {
    $local[$path] = (
      conf_path() == find_conf_path($http_host, $script_name)
      && !strncmp($script_name, $base_path, $base_len)
    );
  }
  return $local[$path]
    ? substr($script_name, $base_len) : FALSE;
}

/**
 * Returns an absolute local path corresponding to a given URL, if possible.
 *
 * For example, when looking for image URLs within an email message body for
 * possible conversion to inline attachments, the following code might be used:
 * @code
 *   if ( !url_is_external($url)
 *     && ($path = url_to_realpath($url))
 *     && file_exists($path) ) {
 *     // Attach $path and replace $url.
 *     ...
 *   }
 * @endcode
 *
 * @param $url
 *   The internal path or external URL being linked to, such as "node/34" or
 *   "http://example.com/drupal/foo".
 * @return
 *   FALSE if $url contains a host/port/path that does not match the current
 *   site, or else an absolute local path such as "/path/to/drupal/node/34".
 *
 * @see url_to_path()
 */
function url_to_realpath($url) {
  $path = url_to_path($url);
  if ($path) {
    $decoded_path = rawurldecode($path);
    return realpath(DRUPAL_ROOT . DIRECTORY_SEPARATOR . $decoded_path);
  }
  return FALSE;
}
