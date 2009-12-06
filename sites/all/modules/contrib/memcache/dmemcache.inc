<?php
// $Id: dmemcache.inc,v 1.1.2.7.2.4 2009/01/26 21:12:04 jeremy Exp $

/*
 * Core dmemcache functions required by:
 *   memcache.inc
 *   memcache.db.inc
 *   session-memcache.inc
 *   session-memcache.db.inc
 */

global $_memcache_statistics;
$_memcache_statistics = array('get' => array(), 'set' => array(), 'hit' => array());

/*
 * A memcache API for Drupal.
 */

/**
 *  Place an item into memcache
 *
 * @param $key The string with with you will retrieve this item later.
 * @param $value The item to be stored.
 * @param $exp Parameter expire is expiration time in seconds. If it's 0, the item never expires
 *   (but memcached server doesn't guarantee this item to be stored all the time, it could be
 *   deleted from the cache to make place for other items).
 * @param $bin The name of the Drupal subsystem that is making this call. Examples could be
 *   'cache', 'alias', 'taxonomy term' etc. It is possible to map different $bin values to
 *   different memcache servers.
 *
 * @return bool
 */
function dmemcache_set($key, $value, $exp = 0, $bin = 'cache') {
  global $_memcache_statistics;
  $_memcache_statistics['set'][] = $key;
  $_memcache_statistics['bins'][] = $bin;
  if ($mc = dmemcache_object($bin)) {
    $full_key = dmemcache_key($key, $bin);
    if (!$mc->set($full_key, $value, MEMCACHE_COMPRESSED, $exp)) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Retrieve a value from the cache.
 *
 * @param $key The key with which the item was stored.
 * @param $bin The bin in which the item was stored.
 *
 * @return The item which was originally saved or FALSE
 */
function dmemcache_get($key, $bin = 'cache') {
  global $_memcache_statistics;
  $_memcache_statistics['get'][] = $key;
  $_memcache_statistics['bins'][] = $bin;
  if ($mc = dmemcache_object($bin)) {
    $full_key = dmemcache_key($key, $bin);
    $result = $mc->get($full_key);
    if ($result) {
      // We check $result->expire to see if the object has expired.  If so, we
      // try and grab a lock.  If we get the lock, we return FALSE instead of
      // the cached object which should cause it to be rebuilt.  If we do not
      // get the lock, we return the cached object.  The goal here is to avoid
      // cache stampedes. 
      // By default the cache stampede semaphore is held for 15 seconds.  This
      // can be adjusted by setting the memcache_stampede_semaphore variable.
      // TODO: Can we log when a sempahore expires versus being intentionally
      // freed to track when this is happening?
      if ($result->expire && $result->expire <= time() && $mc->add($full_key .'_semaphore', '', FALSE, variable_get('memcache_stampede_semaphore', 15))) {
        $result = FALSE;
      }
      else {
        $_memcache_statistics['hit'][] = $key;
      }
    }
    return $result;
  }
}

/**
 * Deletes an item from the cache.
 *
 * @param $key The key with which the item was stored.
 * @param $bin The bin in which the item was stored.
 *
 * @return Returns TRUE on success or FALSE on failure.
 */
function dmemcache_delete($key, $bin = 'cache') {
  if ($mc = dmemcache_object($bin)) {
    $full_key = dmemcache_key($key, $bin);
    return $mc->delete($full_key);
  }
  return FALSE;
}

/**
 * Immediately invalidates all existing items. dmemcache_flush doesn't actually free any
 * resources, it only marks all the items as expired, so occupied memory will be overwritten by
 * new items.
 *
 * @param $bin The bin to flush. Note that this will flush all bins mapped to the same server
 *   as $bin. There is no way at this time to empty just one bin.
 *
 * @return Returns TRUE on success or FALSE on failure.
 */
function dmemcache_flush($bin = 'cache') {
  if ($mc = dmemcache_object($bin)) {
    return $mc->flush();
  }
}

function dmemcache_stats($bin = 'cache', $type = '') {
  // resolve requests for 'default' type to ''
  if ($type == 'default') {
    $type = '';
  }
  // resolve requests for 'default' bin to 'cache'.
  if ($bin == 'default') {
    $bin = 'cache';
  }
  if ($mc = dmemcache_object($bin)) {
    return $mc->getExtendedStats($type);
  }
}

/**
 * Returns an Memcache object based on the bin requested. Note that there is
 * nothing preventing developers from calling this function directly to get the
 * Memcache object. Do this if you need functionality not provided by this API
 * or if you need to use legacy code. Otherwise, use the dmemcache (get, set,
 * delete, flush) API functions provided here.
 *
 * @param $bin The bin which is to be used.
 *
 * @param $flush Rebuild the bin/server/cache mapping.
 *
 * @return an Memcache object or FALSE.
 */
function dmemcache_object($bin = NULL, $flush = FALSE) {
  global $memcache_online;
  static $memcacheCache = array(), $memcache_servers, $memcache_bins;

  if ($flush) {
    foreach ($memcacheCache as $cluster) {
      $cluster->close();
    }
    $memcacheCache = array();
  }

  if (empty($memcacheCache) || empty($memcacheCache[$bin])) {
    // $memcache_servers and $memcache_bins originate from settings.php.
    // $memcache_servers_custom and $memcache_bins_custom get set by
    // memcache.module. They are then merged into $memcache_servers and
    // $memcache_bins, which are statically cached for performance.
    if (empty($memcache_servers)) {
      // Values from settings.php
      $memcache_servers = variable_get('memcache_servers', array('127.0.0.1:11211' => 'default'));
      $memcache_bins    = variable_get('memcache_bins', array('cache' => 'default'));
    }

    // If there is no cluster for this bin in $memcache_bins, cluster is 'default'.
    $cluster = empty($memcache_bins[$bin]) ? 'default' : $memcache_bins[$bin];

    // If this bin isn't in our $memcache_bins configuration array, and the
    // 'default' cluster is already initialized, map the bin to 'cache' because
    // we always map the 'cache' bin to the 'default' cluster.
    if (empty($memcache_bins[$bin]) && !empty($memcacheCache['cache'])) {
      $memcacheCache[$bin] = &$memcacheCache['cache'];
    }
    else {
      // Create a new Memcache object. Each cluster gets its own Memcache object.
      $memcache = new Memcache;
      // A variable to track whether we've connected to the first server.
      $init = FALSE;

      // Link all the servers to this cluster.
      foreach ($memcache_servers as $s => $c) {
        if ($c == $cluster) {
          list($host, $port) = explode(':', $s);

          // This is a server that belongs to this cluster.
          if (!$init) {
            // First server gets the connect.
            if (@$memcache->connect($host, $port)) {
              // Only set init to true if a connection was made.
              $init = TRUE;
              $memcache_online = TRUE;
            }
            else {
              $memcache_online = FALSE;
            }
          }
          else {
            // Subsequent servers gett addServer.
            @$memcache->addServer($host, $port);
          }
        }
      }

      if ($init) {
        // Map the current bin with the new Memcache object.
        $memcacheCache[$bin] = $memcache;

        // Now that all the servers have been mapped to this cluster, look for
        // other bins that belong to the cluster and map them too.
        foreach ($memcache_bins as $b => $c) {
          if ($c == $cluster && $b != $bin) {
            // Map this bin and cluster by reference.
            $memcacheCache[$b] = &$memcacheCache[$bin];
          }
        }
      }
    }
  }

  return empty($memcacheCache[$bin]) ? FALSE : $memcacheCache[$bin];
}

function dmemcache_key($key, $bin = 'cache') {
  static $prefix;
  // memcache_key_prefix can be set in settings.php to support site namespaces
  // in a multisite environment.
  if (empty($prefix)) {
    $prefix = variable_get('memcache_key_prefix', '');
  }
  $full_key = ($prefix ? $prefix. '-' : '') . $bin . '-' . $key;

  return urlencode($full_key);
}
