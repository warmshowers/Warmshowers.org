<?php
// $Id: memcache-session.inc,v 1.1.2.2.2.10 2009/07/14 22:07:11 jeremy Exp $

require_once dirname(__FILE__) . '/dmemcache.inc';

/**
 * @file
 * User session handling functions.
 *
 * An alternative to includes/session.inc.
 */

function sess_open($save_path, $session_name) {
  return TRUE;
}

function sess_close() {
  return TRUE;
}

function sess_read($key) {
  global $user;

  // Write and Close handlers are called after destructing objects since PHP 5.0.5
  // Thus destructors can use sessions but session handler can't use objects.
  // So we are moving session closure before destructing objects.
  register_shutdown_function('session_write_close');

  // Handle the case of first time visitors and clients that don't store 
  // cookies (eg. web crawlers).
  if (!isset($_COOKIE[session_name()])) {
    $user = drupal_anonymous_user();
    return '';
  }

  // Otherwise, if the session is still active, we have a record of the 
  // client's session in memcache.
  $session = dmemcache_get($key, 'session');
  
  $user = sess_user_load($session);

  // Record whether this session contains data so that in sess_write() it can 
  // be determined whether to skip a write.
  $user->session_data_present_at_load = !empty($session->session);

  return $user->session;
}

/**
 * Write a session to session storage.
 *
 * We have the following cases to handle.
 * 1. Anonymous user
 *   1a. Without session data.
 *   1b. With session data.
 *   1c. Session saving has been turned off programatically
 *       (see session_save_session()).
 *   1d. Without session data but had session data at the beginning of the request
 *       (thus a write must be made to clear stored session data).
 * 2. Authenticated user.
 *   2a. Without session data.
 *   2b. With session data.
 *   2c. Session saving has been turned off programatically
 *       (see session_save_session()).
 *
 * @param $key
 *   The session ID.
 * @param $value
 *   Any data to store in the session.
 * @return
 *   TRUE.
 */
function sess_write($key, $value) {
  global $user;

  // If the client doesn't have a session, and one isn't being created ($value),
  // do nothing. If session saving has been turned off, do nothing.
  // Cases 1a, 1c, and 2c are covered here.
  if ((empty($_COOKIE[session_name()]) && empty($value)) || !session_save_session()) {
    return TRUE;
  }

  // Prepare the information to be saved.
  $session = new stdClass;
  $session->sid = $key;
  $session->uid = $user->uid;
  $session->cache = isset($user->cache) ? $user->cache : '';
  $session->hostname = ip_address();
  $session->session = $value;
  $session->timestamp = time();

  // If this is an authenticated user, or there is something to save in the
  // session, or this is an anonymous user who currently has nothing in the 
  // session but did have something in session storage, write it to memcache.
  // If $user->session_data_present_at_load is not set, the current user
  // was created during this request and it's safest to do a write. 
  // Cases 1b, 1d, 2a, and 2b are covered here.
  if ($user->uid || !empty($value) || empty($value) && (!isset($user->session_data_present_at_load) || $user->session_data_present_at_load)) {
    dmemcache_set($key, $session, ini_get('session.gc_maxlifetime'), 'session');
    if ($user->uid && $session->timestamp - $user->access > variable_get('session_write_interval', 360)) {
      db_query('UPDATE {users} SET access = %d WHERE uid = %d', $session->timestamp, $user->uid);
      // Update the user access time so that the dmemcache_set() call
      // caches the updated time.
      $user->access = $session->timestamp;
    }
    // Data stored in session is stored in session memcache; no need
    // to duplicate it in users memcache.
    unset($user->session);
    unset($user->session_data_present_at_load);
    // Store the session id so we can locate the session with the user id.
    $user->sid = $key;
    
    dmemcache_set($user->uid, $user, ini_get('session.gc_maxlifetime'), 'users');
  }

  return TRUE;
}

function sess_regenerate() {
  // We code around http://bugs.php.net/bug.php?id=32802 by destroying
  // the session cookie by setting expiration in the past (a negative
  // value).  This issue only arises in PHP versions before 4.4.0,
  // regardless of the Drupal configuration.
  // TODO: remove this when we require at least PHP 4.4.0
  if (isset($_COOKIE[session_name()])) {
    setcookie(session_name(), '', time() - 42000, '/');
  }

  // Store the current (anonymous) session id.
  $old_session_id = session_id();

  // Generate the new (authenticated) session id.
  session_regenerate_id();
  $key = session_id();

  // Grab the user's information that is cached with the anonymous key.
  $info = dmemcache_get($old_session_id, 'session');

  // Update it.
  $info->sid = $key;

  // Store it with the new key.
  dmemcache_set($key, $info, ini_get('session.gc_maxlifetime'), 'session');

  // Clear the old data from the cache.
  dmemcache_delete($old_session_id, 'session');
}

/**
 * Counts how many users have sessions. Can count either anonymous sessions, authenticated sessions, or both.
 * Would be insane slow with memcached as we would need to retrieve at least the stats of all object.
 * Not implemented.
 */
function sess_count($timestamp = 0, $anonymous = true) {
}

/**
 * Called by PHP session handling with the PHP session ID to end a user's session.
 *
 * @param  string $sid
 *   the session id
 */
function sess_destroy_sid($sid) {
  dmemcache_delete($sid, 'session');
}

/**
 * End a specific user's session.
 */
function sess_destroy_uid($uid) {
  $user = dmemcache_get($uid, 'users');
  if (is_object($user) && isset($user->sid)) {
    dmemcache_delete($user->sid, 'session');
  }
  dmemcache_delete($uid, 'users');
}

function sess_gc($lifetime) {
  // Automatic with memcached.
  // Be sure to adjust 'php_value session.gc_maxlifetime' to a large enough
  // value. For example, if you want user sessions to stay in your database
  // for three weeks before deleting them, you need to set gc_maxlifetime
  // to '1814400'. At that value, only after a user doesn't log in after
  // three weeks (1814400 seconds) will his/her session be removed.
  return TRUE;
}

/**
 * Determine whether to save session data of the current request.
 *
 * This function allows the caller to temporarily disable writing of session data,
 * should the request end while performing potentially dangerous operations, such as
 * manipulating the global $user object.  See http://drupal.org/node/218104 for usage
 *
 * @param $status
 *   Disables writing of session data when FALSE, (re-)enables writing when TRUE.
 * @return
 *   FALSE if writing session data has been disabled. Otherwise, TRUE.
 */
function session_save_session($status = NULL) {
  static $save_session = TRUE;
  if (isset($status)) {
    $save_session = $status;
  }
  return ($save_session);
}

/**
 * Create the user object.
 *
 * @param $session
 *   The session object (see sess_write() for the structure).
 * @return $user
 *   The user object.
 */
function sess_user_load($session) {
  // We found the client's session record and they are an authenticated user.
  if ($session && $session->uid != 0) {
    $user = dmemcache_get($session->uid, 'users');
    // If the 'users' memcache bin is unavailable, $user will be NULL.
    // If the cached user was not found in the 'users' memcache bin, $user will
    // be FALSE.
    // In either of these cases, the user must be retrieved from the database.
    if (!$user->uid && isset($session->uid) && $session->uid != 0) {
      $user = db_fetch_object(db_query('SELECT u.* FROM {users} u WHERE u.uid = %d', $session->uid));

      $user = drupal_unpack($user);
      // Normally we would join the session and user tables. But we already
      // have the session information. So add that in.
      $user->sid = $session->sid;
      $user->cache = $session->cache;
      $user->hostname = $session->hostname;
      $user->timestamp = $session->timestamp;
      $user->session = empty($session->session) ? '' : $session->session;

      // Add roles element to $user
      $user->roles = array();
      $user->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
      $result = db_query("SELECT r.rid, r.name FROM {role} r INNER JOIN {users_roles} ur ON ur.rid = r.rid WHERE ur.uid = %d", $user->uid);
      while ($role = db_fetch_object($result)) {
        $user->roles[$role->rid] = $role->name;
      }
    }
    else if ($user->uid) {
      // Got a user object from 'users' memcache bin. Mark it in case modules
      // want to know that this user was created from memcache.
      $user->from_cache = TRUE;
      $user->session = empty($session->session) ? '' : $session->session;
    }
    else {
      // We will only get here when the session has a nonzero uid, a user object
      // was successfully retrieved from the 'users' bin, and that user
      // object's uid is 0. Not sure why this would ever happen. Leaving former
      // comment in:
      // This is a rare case that we have a session cached, but no session user object cached.
      // This usually only happens if you kill memcached and restart it.
      $user = drupal_anonymous_user($session->session);
    }
  }
  // We didn't find the client's record (session has expired), or they are an
  // anonymous user.
  else  {
    $session = isset($session->session) ? $session->session : '';
    $user = drupal_anonymous_user($session);
  }

  return $user;
}
