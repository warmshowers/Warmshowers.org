<?php

/** Implementation of cache.inc with memcache logic included **/

require_once 'dmemcache.inc';
require_once variable_get('memcache_extra_include', 'database.inc');

/**
 * Defines the period after which wildcard clears are not considered valid.
 */
define('MEMCACHE_WILDCARD_INVALIDATE', 86400 * 28);

/**
 * Define a unique string for registering content flushes.
 *
 * This must not be confused with wildcard flushes or actual cids, so needs
 * to be relatively unique.
 */
define('MEMCACHE_CONTENT_FLUSH', 'MEMCACHE_CONTENT_FLUSH');

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
  // Handle excluded bins first.
  $bins = variable_get('memcache_bins', array());
  if (!is_null($table) && isset($bins[$table]) && $bins[$table] == 'database') {
    return _cache_get($cid, $table);
  }

  // Retrieve the item from the cache.
  $cache = dmemcache_get($cid, $table);

  // Set up common variables.
  $cache_flush = variable_get("cache_flush_$table", 0);
  $cache_content_flush = variable_get("cache_content_flush_$table", 0);
  $cache_tables = isset($_SESSION['cache_flush']) ? $_SESSION['cache_flush'] : NULL;
  $cache_lifetime = variable_get('cache_lifetime', 0);
  $wildcard_flushes = variable_get('memcache_wildcard_flushes', array());
  $wildcard_invalidate = variable_get('memcache_wildcard_invalidate', MEMCACHE_WILDCARD_INVALIDATE);

  if (is_object($cache)) {
    // Items that have expired are invalid.
    if (isset($cache->expire) && $cache->expire !== CACHE_PERMANENT && $cache->expire <= $_SERVER['REQUEST_TIME']) {
      // If the memcache_stampede_protection variable is set, allow one process
      // to rebuild the cache entry while serving expired content to the
      // rest. Note that core happily returns expired cache items as valid and
      // relies on cron to expire them, but this is mostly reliant on its
      // use of CACHE_TEMPORARY which does not map well to memcache.
      // @see http://drupal.org/node/534092
      if (variable_get('memcache_stampede_protection', FALSE)) {
        // The process that acquires the lock will get a cache miss, all
        // others will get a cache hit.
        if (lock_acquire("memcache_$cid:$table", variable_get('memcache_stampede_semaphore', 15))) {
          $cache = FALSE;
        }
      }
      else {
        $cache = FALSE;
      }
    }
    // Items created before the last full wildcard flush against this bin are
    // invalid.
    elseif ($cache->created <= $cache_flush) {
      $cache = FALSE;
    }
    // Items created before the last content flush on this bin i.e.
    // cache_clear_all() are invalid.
    elseif ($cache->expire != CACHE_PERMANENT && $cache->created + $cache_lifetime <= $cache_content_flush) {
      $cache = FALSE;
    }
    // Items cached before the cache was last flushed by the current user are
    // invalid.
    elseif ($cache->expire != CACHE_PERMANENT && is_array($cache_tables) && isset($cache_tables[$table]) && $cache_tables[$table] >= $cache->created) {
      // Cache item expired, return FALSE.
      $cache = FALSE;
    }
    // Finally, check for wildcard clears against this cid.
    else {
      $flushes = isset($cache->flushes) ? (int)$cache->flushes : 0;
      $recorded_flushes = memcache_wildcard_flushes($cid, $table);
      if ($flushes < $recorded_flushes) {
        $cache = FALSE;
      }
      // If wildcards are cleared by a partial memcache flush or eviction
      // then it is possible for $cache->flushes to be greater than the return
      // of memcache_wildcard_flushes().
      if ($flushes > $recorded_flushes) {
        // Delete the cache item entirely, it will be set again with the correct
        // number of flushes.
        dmemcache_delete($cid, $table);
        $cache = FALSE;
      }
    }
  }
  else {
    $cache = FALSE;
  }

  // On cache misses, attempt to avoid stampedes when the
  // memcache_stampede_protection variable is enabled.
  if (!$cache) {
    if (variable_get('memcache_stampede_protection', FALSE) && !lock_acquire("memcache_$cid:$table", variable_get('memcache_stampede_semaphore', 15))) {
      // Prevent any single request from waiting more than three times due to
      // stampede protection. By default this is a maximum total wait of 15
      // seconds. This accounts for two possibilities - a cache and lock miss
      // more than once for the same item. Or a cache and lock miss for
      // different items during the same request.
      // @todo: it would be better to base this on time waited rather than
      // number of waits, but the lock API does not currently provide this
      // information. Currently the limit will kick in for three waits of 25ms
      // or three waits of 5000ms.
      static $lock_count = 0;
      $lock_count++;
      if ($lock_count <= variable_get('memcache_stampede_wait_limit', 3)) {
        // The memcache_stampede_semaphore variable was used in previous releases
        // of memcache, but the max_wait variable was not, so by default divide
        // the sempahore value by 3 (5 seconds).
        lock_wait("memcache_$cid:$table", variable_get('memcache_stampede_wait_time', 5));
        return cache_get($cid, $table);
      }
    }
  }

  // Clean up $_SESSION['cache_flush'] variable array if it is older than
  // the minimum cache lifetime, since after that the $cache_flush variable
  // will take over.
  if (is_array($cache_tables) && !empty($cache_tables) && $cache_lifetime) {
    // Expire the $_SESSION['cache_flush'] variable array if it is older than
    // the minimum cache lifetime, since after that the $cache_flush variable
    // will take over.
    if (max($cache_tables) < ($_SERVER['REQUEST_TIME'] - $cache_lifetime)) {
      unset($_SESSION['cache_flush']);
      $cache_tables = NULL;
    }
  }

  return $cache;
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
  // Handle database fallback first.
  $bins = variable_get('memcache_bins', array());
  if (!is_null($table) && isset($bins[$table]) && $bins[$table] == 'database') {
    return _cache_set($cid, $data, $table, $expire, $headers);
  }

  // The created time should always be set as late as possible, this is
  // especially true immediately after a full bin flush, so use time() here
  // instead of request time.
  $created = time();

  // Create new cache object.
  $cache = new stdClass;
  $cache->cid = $cid;
  $cache->data = is_object($data) ? memcache_clone($data) : $data;
  $cache->created = $created;
  $cache->headers = $headers;
  // Record the previous number of wildcard flushes affecting our cid.
  $cache->flushes = memcache_wildcard_flushes($cid, $table);
  if ($expire == CACHE_TEMPORARY) {
    // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
    // until the next flush.
    $cache->expire = $_SERVER['REQUEST_TIME'] + 2591999;
  }
  // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
  else if ($expire != CACHE_PERMANENT && $expire < 2592000) {
    // Expire is expressed in seconds, convert to the proper future timestamp
    // as expected in dmemcache_get().
    $cache->expire = $_SERVER['REQUEST_TIME'] + $expire;
  }
  else {
    $cache->expire = $expire;
  }

  // We manually track the expire time in $cache->expire.  When the object
  // expires, we only allow one request to rebuild it to avoid cache stampedes.
  // Other requests for the expired object while it is still being rebuilt get
  // the expired object.
  dmemcache_set($cid, $cache, 0, $table);
  if (isset($GLOBALS['locks']["memcache_$cid:$table"])) {
    lock_release("memcache_$cid:$table");
  }
}

/**
 *
 * Expire data from the cache. If called without arguments, expirable
 * entries will be cleared from the cache_page and cache_block tables.
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
  // Handle database fallback first.
  $bins = variable_get('memcache_bins', array());
  if (!is_null($table) && isset($bins[$table]) && $bins[$table] == 'database') {
    return _cache_clear_all($cid, $table, $wildcard);
  }

  if (!isset($cid) && isset($table)) {
    // cache_clear_all(NULL, $table) is for garbage collection only, memcache
    // does not need this due to design, so make these calls a no-op.
    return;
  }
  // Default behavior for when cache_clear_all() is called without parameters
  // is to clear all of the expirable entries in the block and page caches.
  elseif (!isset($cid) && !isset($table)) {
    cache_clear_all(MEMCACHE_CONTENT_FLUSH, 'cache_block');
    cache_clear_all(MEMCACHE_CONTENT_FLUSH, 'cache_page');
    return;
  }
  // Treat '*' and empty strings the same.
  elseif (($cid == '*' || $cid == '') && $wildcard) {
    // Since memcache works with a single memcache bin, full wildcard flushes
    // are tracked in a variable rather than flushing the bin.
    memcache_variable_set("cache_flush_$table", time());
  }
  elseif ($cid == MEMCACHE_CONTENT_FLUSH) {
    // Update the timestamp of the last global flushing of this table.  When
    // retrieving data from this table, we will compare the cache creation
    // time minus the cache_flush time to the cache_lifetime to determine
    // whether or not the cached item is still valid.
    memcache_variable_set("cache_content_flush_$table", time());
    if (variable_get('cache_lifetime', 0)) {
      // We store the time in the current user's session which is saved into
      // the sessions table by sess_write().  We then simulate that the cache
      // was flushed for this user by not returning cached data to this user
      // that was cached before the timestamp.
      if (isset($_SESSION['cache_flush'])) {
        $cache_tables = $_SESSION['cache_flush'];
      }
      else {
        $cache_tables = array();
      }
      // Use time() rather than request time here for correctness.
      $cache_tables[$table] = time();
      $_SESSION['cache_flush'] = $cache_tables;
    }
  }
  elseif ($wildcard) {
    // Register a wildcard flush for current cid
    memcache_wildcards($cid, $table, TRUE);
  }
  else {
    dmemcache_delete($cid, $table);
  }
}

/**
 * Sum of all matching wildcards.  Checking any single cache item's flush value
 * against this single-value sum tells us whether or not a new wildcard flush
 * has affected the cached item.
 */
function memcache_wildcard_flushes($cid, $table) {
  return array_sum(memcache_wildcards($cid, $table));
}

/**
 * Utilize multiget to retrieve all possible wildcard matches, storing
 * statically so multiple cache requests for the same item on the same page
 * load doesn't add overhead.
 */
function memcache_wildcards($cid, $table, $flush = FALSE) {
  static $wildcards = array();
  $matching = array();
  $length = strlen($cid);

  $wildcard_flushes = variable_get('memcache_wildcard_flushes', array());
  $wildcard_invalidate = variable_get('memcache_wildcard_invalidate', MEMCACHE_WILDCARD_INVALIDATE);

  if (isset($wildcard_flushes[$table]) && is_array($wildcard_flushes[$table])) {
    // Wildcard flushes per table are keyed by a substring equal to the
    // shortest wildcard clear on the table so far. So if the shortest
    // wildcard was "links:foo:", and the cid we're checking for is
    // "links:bar:bar", then the key will be "links:bar:".
    $keys = array_keys($wildcard_flushes[$table]);
    // All keys are the same length, so just get the length of the first one.
    $wildcard_length = strlen(reset($keys));
    $wildcard_key = substr($cid, 0, $wildcard_length);

    // Determine which lookups we need to perform to determine whether or not
    // our cid was impacted by a wildcard flush.
    $lookup = array();

    // Find statically cached wildcards, and determine possibly matching
    // wildcards for this cid based on a history of the lengths of past valid
    // wildcard flushes in this bin.
    if (isset($wildcard_flushes[$table][$wildcard_key])) {
      foreach ($wildcard_flushes[$table][$wildcard_key] as $flush_length => $timestamp) {
        if ($length >= $flush_length && $timestamp >= ($_SERVER['REQUEST_TIME'] - $wildcard_invalidate)) {
          $key = '.wildcard-' . substr($cid, 0, $flush_length);
          $wildcard = dmemcache_key($key, $table);
          if (isset($wildcards[$table][$wildcard])) {
            $matching[$wildcard] = $wildcards[$table][$wildcard];
          }
          else {
            $lookup[$wildcard] = $key;
          }
        }
      }
    }

    // Do a multi-get to retrieve all possibly matching wildcard flushes.
    if (!empty($lookup)) {
      $values = dmemcache_get_multi($lookup, $table);
      if (is_array($values)) {
        // Build an array of matching wildcards.
        $matching = array_merge($matching, $values);
        if (isset($wildcards[$table])) {
          $wildcards[$table] = array_merge($wildcards[$table], $values);
        }
        else {
          $wildcards[$table] = $values;
        }
        $lookup = array_diff_key($lookup, $values);
      }

      // Also store failed lookups in our static cache, so we don't have to
      // do repeat lookups on single page loads.
      foreach ($lookup as $wildcard => $key) {
        $wildcards[$table][$wildcard] = 0;
      }
    }
  }
  if ($flush) {
    // Avoid too many calls to variable_set() by only recording a flush for a
    // fraction of the wildcard invalidation variable, per cid length.  Defaults
    // to 28 / 4, or one week.
    $length = strlen($cid);
    if (isset($wildcard_flushes[$table])) {
      $wildcard_flushes_keys = array_keys($wildcard_flushes[$table]);
      $key_length = strlen(reset($wildcard_flushes_keys));
    }
    else {
      $key_length = $length;
    }
    $key = substr($cid, 0, $key_length);
    if (!isset($wildcard_flushes[$table][$key][$length]) || ($_SERVER['REQUEST_TIME'] - $wildcard_flushes[$table][$key][$length] > $wildcard_invalidate / 4)) {

      // If there are more than 50 different wildcard keys for this table
      // shorten the key by one, this should reduce variability by
      // an order of magnitude and ensure we don't use too much memory.
      if (isset($wildcard_flushes[$table]) && count($wildcard_flushes[$table]) > 50) {
        $key = substr($cid, 0, $key_length - 1);
        $length = strlen($key);
      }

      // If this is the shortest key length so far, we need to remove all
      // other wildcards lengths recorded so far for this table and start
      // again. This is equivalent to a full cache flush for this table, but
      // it ensures the minimum possible number of wildcards are requested
      // along with cache consistency.
      if ($length < $key_length) {
        $wildcard_flushes[$table] = array();
      }
      $key = substr($cid, 0, $key_length);
      $wildcard_flushes[$table][$key][$length] = $_SERVER['REQUEST_TIME'];
      memcache_variable_set('memcache_wildcard_flushes', $wildcard_flushes);
    }
    $wildcard = dmemcache_key('.wildcard-' . $cid, $table);
    if (isset($wildcards[$table][$wildcard]) && $wildcards[$table][$wildcard] != 0) {
      $mc = dmemcache_object($table);
      if ($mc) {
        $mc->increment($wildcard);
      }
      $wildcards[$table][$wildcard]++;
    }
    else {
      $wildcards[$table][$wildcard] = 1;
      dmemcache_set('.wildcard-' . $cid, '1', 0, $table);
    }
  }
  return $matching;
}

/**
 * Provide a substitute clone() function for PHP4. This is a copy of drupal_clone
 * because common.inc isn't included early enough in the bootstrap process to
 * be able to depend on drupal_clone.
 */
function memcache_clone($object) {
  return version_compare(phpversion(), '5.0') < 0 ? $object : clone($object);
}

/**
 * Re-implementation of variable_set() that writes through instead of clearing.
 */
function memcache_variable_set($name, $value) {
  global $conf;

  $serialized_value = serialize($value);
  db_query("UPDATE {variable} SET value = '%s' WHERE name = '%s'", $serialized_value, $name);
  if (!db_affected_rows()) {
    @db_query("INSERT INTO {variable} (name, value) VALUES ('%s', '%s')", $name, $serialized_value);
  }
  // If the variables are cached, get a fresh copy, update with the new value
  // and set it again.
  if ($cached = cache_get('variables', 'cache')) {
    $variables = $cached->data;
    $variables[$name] = $value;
    cache_set('variables', $variables);
  }
  // If the variables aren't cached, there's no need to do anything.
  $conf[$name] = $value;
}
