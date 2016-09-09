<?php
/**
 * @file
 * Poormans cron functions.
 */

/**
 * Output buffer callback for poormans cron early page flush.
 *
 * @param string $content
 *   The content of the output buffer.
 *
 * @return string
 *   The content of the output buffer.
 */
function _ultimate_cron_poorman_page_flush($content) {
  if (empty($GLOBALS['ultimate_cron_page_flush'])) {
    return $content;
  }

  // Check output buffer handlers to determine if we can safely
  // set the content size in the http header.
  $handlers = ob_list_handlers();

  // Pop ourselves off...
  array_pop($handlers);

  // We can safe ignore default output handlers.
  $default = array_keys($handlers, 'default output handler');
  foreach ($default as $key) {
    unset($handlers[$key]);
  }

  // We can also safely ignore zlib output handlers, if we remember to
  // disable compression.
  if ($zlib = array_keys($handlers, 'zlib output compression')) {
    foreach ($zlib as $key) {
      unset($handlers[$key]);
    }
    ini_set('zlib.output_compression', 'Off');
  }

  // If there are any unknown handlers left, we cannot safely set the
  // Content-Length.
  if (empty($handlers)) {
    $size = strlen($content);
    header("Content-Length: $size");
  }

  // In any case, we can always tell the client to close the connection.
  header("Connection: close");

  return $content;
}

// Output buffering messes up Drush ...
if (ultimate_cron_poorman_capable()) {
  ob_start('_ultimate_cron_poorman_page_flush');
}

/**
 * Determines if poormans cron can run on this page.
 */
function ultimate_cron_poorman_capable() {
  static $capable;
  if (isset($capable)) {
    return $capable;
  }

  if (drupal_is_cli()) {
    $capable = FALSE;
  }
  elseif ($_SERVER['REQUEST_METHOD'] !== 'GET') {
    $capable = FALSE;
  }
  elseif (basename($_SERVER['PHP_SELF']) == 'cron.php') {
    $capable = FALSE;
  }
  elseif (isset($_GET['q']) && preg_match('!^system/files/!', $_GET['q'])) {
    $capable = FALSE;
  }
  else {
    $capable = TRUE;
  }

  return $capable;
}

/**
 * Implements hook_exit().
 */
function ultimate_cron_exit($dest = NULL) {
  // No need for poorman until site is installed.
  if (variable_get('install_task') != 'done') {
    return;
  }

  // Don't attempt poormans stuff until fully bootstrapped.
  if (drupal_bootstrap() < DRUPAL_BOOTSTRAP_FULL) {
    return;
  }

  // We're picky about when we run poormans cron, because
  // we may want to flush the output buffer.
  if (!ultimate_cron_poorman_capable()) {
    return;
  }

  $poorman = _ultimate_cron_plugin_load('settings', 'poorman');
  if (!$poorman) {
    return;
  }

  $settings = $poorman->getDefaultSettings();
  if (!$settings['launcher']) {
    return;
  }

  $launcher = _ultimate_cron_plugin_load('launcher', $settings['launcher']);
  if (!$launcher) {
    watchdog('ultimate_cron', 'Invalid poormans cron launcher: @name', array(
      '@name' => $settings['launcher'],
    ), WATCHDOG_ERROR);
    return;
  }

  if ($settings['early_page_flush']) {
    // Poormans cron needs to be the last that runs.
    // Run remaining exit hooks, and shutdown like core does.
    $modules = module_implements('exit');

    do {
      $module = array_shift($modules);
    } while ($modules && ($module !== 'ultimate_cron'));

    foreach ($modules as $module) {
      module_invoke($module, 'exit', $dest);
    }

    // Commit the user session, if needed.
    drupal_session_commit();

    if (variable_get('cache', 0) && ($cache = drupal_page_set_cache())) {
      drupal_serve_page_from_cache($cache);
    }

    _registry_check_code(REGISTRY_WRITE_LOOKUP_CACHE);
    drupal_cache_system_paths();
    module_implements_write_cache();

    _ultimate_cron_launch_poorman($launcher);
    exit;
  }
  else {
    // Wait until the very end before running cron.
    register_shutdown_function('_ultimate_cron_launch_poorman', $launcher, 3);
  }
}

/**
 * Postponable poormans cron launching.
 *
 * @param UltimateCronLauncher $launcher
 *   The Ultimate Cron launcher to use for poormans cron.
 * @param int $postpone
 *   The number of times to postpone via register_shutdown_function.
 */
function _ultimate_cron_launch_poorman($launcher, $postpone = 0) {
  if ($postpone > 0) {
    register_shutdown_function(__FUNCTION__, $launcher, $postpone - 1);
    return;
  }

  // Poormans cron should run as anonymous, just like regular cron does.
  global $user;
  $original_user = $user;
  $user = drupal_anonymous_user();
  $launcher->launchPoorman();
  $user = $original_user;
}

/**
 * Flush the page and end the client request.
 */
function ultimate_cron_poorman_page_flush() {
  @ignore_user_abort();

  $GLOBALS['ultimate_cron_page_flush'] = TRUE;
  while (ob_get_level() > 0) {
    ob_end_flush();
  }
  flush();

  // FastCGI may need another way of letting the client know, that
  // we're done with it.
  if (function_exists('fastcgi_finish_request')) {
    fastcgi_finish_request();
  }
}

/**
 * Page callback for triggering poormans cron.
 */
function ultimate_cron_poorman_page() {
  if (!isset($_GET['cron_key']) || variable_get('cron_key', 'drupal') != $_GET['cron_key']) {
    watchdog('cron', 'Cron could not run because an invalid key was used.', array(), WATCHDOG_NOTICE);
    drupal_access_denied();
  }
  drupal_page_is_cacheable(FALSE);
  drupal_exit();
}

/**
 * Send the actual HTTP request for launching threads.
 */
function ultimate_cron_poorman_trigger() {
  $url_options = variable_get('ultimate_cron_poorman_url_options', array());
  $http_options = variable_get('ultimate_cron_poorman_http_options', array());

  $url_options = array(
    'query' => array(
      'timestamp' => REQUEST_TIME,
      'cron_key' => variable_get('cron_key', 'drupal'),
    ),
    'absolute' => TRUE,
  ) + $url_options;

  $plugin = _ultimate_cron_plugin_require('settings', 'poorman');
  $settings = $plugin->getDefaultSettings();

  $http_options = array(
    'headers' => array(
      'User-Agent' => $settings['user_agent'],
    ),
  ) + $http_options;

  $url = url('admin/config/system/cron/poorman', $url_options);
  $response = drupal_http_request($url, $http_options);
  if (!empty($response->error)) {
    watchdog('ultimate_cron', 'Could not trigger poorman at @url. Error: @error', array(
      '@url' => $url,
      '@error' => $response->error,
    ), WATCHDOG_ERROR);
  }
}
