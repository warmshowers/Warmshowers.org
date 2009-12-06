<?php
// $Id: memcache.inc,v 1.15.2.8.2.8 2009/08/04 20:41:12 jeremy Exp $

require_once 'dmemcache.inc';

/** Implementation of cache.inc with memcache logic included **/

/**
 * Return data from the persistent cache.
 * 
 * Data may be stored as either plain text or as serialized data.
 * cache_get() will automatically return unserialized objects and arrays.
 *
 * @param $cid
 *   The cache ID of the data to retrieve.
 * @param $table
 *   The table $table to store the data in. Valid core values are 'cache_filter',
 *   'cache_menu', 'cache_page', or 'cache' for the default cache.
 */
function cache_get($cid, $table = 'cache') {
  // Determine when the current table was last flushed.
  $cache_flush = variable_get("cache_flush_$table", 0);
  // Retrieve the item from the cache.
  $cache = dmemcache_get($cid, $table);
  if (is_object($cache)) {
    $cache_tables = isset($_SESSION['cache_flush']) ? $_SESSION['cache_flush'] : NULL;
    // Items cached before the cache was last flushed are no longer valid.
    $cache_lifetime = variable_get('cache_lifetime', 0);
    if ($cache_lifetime && $cache->created && $cache_flush &&
        ($cache->created < $cache_flush) &&
        ((time() - $cache->created >= $cache_lifetime)) ||
        (is_array($cache_tables) && $cache_tables[$table] &&
        $cache_tables[$table] > $cache->created)) {
      // Cache item expired, return NULL.
      return 0;
    }
    return $cache;
  }
  return 0;
}

/**
 * Store data in memcache.
 *
 * @param $cid
 *   The cache ID of the data to store.
 * @param $data
 *   The data to store in the cache. Complex data types will be automatically
 *   serialized before insertion. Strings will be stored as plain text and 
 *   not serialized.
 * @param $table
 *   The table $table to store the data in. Valid core values are 'cache_filter',
 *   'cache_menu', 'cache_page', or 'cache'.
 * @param $expire
 *   One of the following values:
 *   - CACHE_PERMANENT: Indicates that the item should never be removed unless
 *     explicitly told to using cache_clear_all() with a cache ID.
 *   - CACHE_TEMPORARY: Indicates that the item should be removed at the next
 *     general cache wipe.
 *   - A Unix timestamp: Indicates that the item should be kept at least until
 *     the given time, after which it behaves like CACHE_TEMPORARY.
 * @param $headers
 *   A string containing HTTP header information for cached pages.
 */
function cache_set($cid, $data, $table = 'cache', $expire = CACHE_PERMANENT, $headers = NULL) {
  $created = time();

  // Create new cache object.
  $cache = new stdClass;
  $cache->cid = $cid;
  $cache->data = is_object($data) ? memcache_clone($data) : $data;
  $cache->created = $created;
  $cache->headers = $headers;
  if ($expire == CACHE_TEMPORARY) {
    // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
    // until the next flush.
    $cache->expire = time() + 2591999;
  }
  // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
  else if ($expire != CACHE_PERMANENT && $expire < 2592000) {
    // Expire is expressed in seconds, convert to the proper future timestamp
    // as expected in dmemcache_get().
    $cache->expire = time() + $expire;
  }
  else {
    $cache->expire = $expire;
  }

  // We manually track the expire time in $cache->expire.  When the object
  // expires, we only allow one request to rebuild it to avoid cache stampedes.
  // Other requests for the expired object while it is still being rebuilt get
  // the expired object.
  dmemcache_set($cid, $cache, 0, $table);
}

/**
 *
 * Expire data from the cache. If called without arguments, expirable
 * entries will be cleared from the cache_page and cache_block tables.
 *
 * Memcache logic is simpler than the core cache because memcache doesn't have
 * a minimum cache lifetime consideration (it handles it internally), and 
 * doesn't support wildcards.  Wildcard flushes result in the entire table
 * being flushed.
 *
 * @param $cid
 *   If set, the cache ID to delete. Otherwise, all cache entries that can
 *   expire are deleted.
 *
 * @param $table
 *   If set, the table $table to delete from. Mandatory
 *   argument if $cid is set.
 *
 * @param $wildcard
 *   If set to TRUE, the $cid is treated as a substring
 *   to match rather than a complete ID. The match is a right hand
 *   match. If '*' is given as $cid, the table $table will be emptied.
 */
function cache_clear_all($cid = NULL, $table = NULL, $wildcard = FALSE) {
  // Default behavior for when cache_clear_all() is called without parameters
  // is to clear all of the expirable entries in the block and page caches.
  if (!isset($cid) && !isset($table)) {
    cache_clear_all('*', 'cache_block', TRUE);
    cache_clear_all('*', 'cache_page', TRUE);
    return;
  }
  if (empty($cid) || ($cid == '*' && $wildcard !== TRUE)) {
    // don't do anything if cid is unset. this matches the default drupal behavior...
    if ($wildcard && $cid != '*') {
      if (variable_get('memcache_debug', FALSE)) {
        // call watchdog, since you probably didn't want to flush the entire bin.
        watchdog('memcache', "illegal wildcard in cache_clear_all - not flushing entire bin. table: $table. cid: $cid", WATCHDOG_WARNING);
      }
    }
  }
  else if ($cid == '*' || $wildcard === TRUE) {
    if (variable_get('cache_lifetime', 0)) {
      // Update the timestamp of the last global flushing of this table.  When
      // retrieving data from this table, we will compare the cache creation
      // time minus the cache_flush time to the cache_lifetime to determine
      // whether or not the cached item is still valid.
      variable_set("cache_flush_$table", time());

      // We store the time in the current user's session which is saved into
      // the sessions table by sess_write().  We then simulate that the cache
      // was flushed for this user by not returning cached data to this user
      // that was cached before the timestamp.
      if (is_array($_SESSION['cache_flush'])) {
        $cache_tables = $_SESSION['cache_flush'];
      }
      else {
        $cache_tables = array();
      }
      $cache_tables[$table] = time();
      $_SESSION['cache_flush'] = $cache_tables;
    }
    else {
      dmemcache_flush($table);
    }
  }
  else {
    dmemcache_delete($cid, $table);
  }
}

/**
 * Provide a substitute clone() function for PHP4. This is a copy of drupal_clone
 * because common.inc isn't included early enough in the bootstrap process to
 * be able to depend on drupal_clone.
 */
function memcache_clone($object) {
  return version_compare(phpversion(), '5.0') < 0 ? $object : clone($object);
}
