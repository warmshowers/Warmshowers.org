<?php
// $Id: search_service.inc,v 1.1.2.8.2.4 2010/02/22 01:53:33 heyrocker Exp $
/**
 * @file
 *  Link general search functionalities to services module.
 */

/**
 * Return search results for nodes.
 *
 * @param $keys
 *   The keywords to search for within the nodes.
 * @param $simple
 *   When set to TRUE, only the fields indicated in $stdkeys will be returned.
 *   This can be helpful to limit the size of the search results.
 * @param $fields
 *   An array of the node properties that should be returned. When $simple
 *   is not set, a full node object is returned with each result. You can
 *   limit the properties of these objects to only the ones you need by
 *   specifying them in this array. Again, this gives the opportunity to
 *   limit your result set.
 *
 * @return
 *   An array of search results. If $simple is TRUE, this array will contain
 *   only results and no node objects. If $simple is FALSE the array will
 *   contain both results and full node objects, possibly limited by the
 *   properties indicated in $fields.
 *
 * @see hook_search()
 * @see node_search()
 */
function search_service_nodes($keys, $simple = FALSE, $fields = array()) {
  // define standard keys for simple set
  $stdkeys = array('node', 'link', 'type', 'title', 'user', 'date', 'snippet');

  // invoke the search hook to generate results
  $results = module_invoke('node', 'search', 'search', $keys);
  if ($results && is_array($results) && count($results)) {
    // if simple results requested, remove extra data
    if ($simple) {
      $num = count($results);
      for ($i = 0; $i<$num; $i++) {
        $keys = array_keys($results[$i]);
        foreach ($keys as $key) {
          if ($key == 'node') {
            $results[$i][$key] = $results[$i]['node']->nid;
          }          
          if (!in_array($key, $stdkeys)) {
            unset($results[$i][$key]);
          }
        }
      }
    }
    else {
      foreach ($results as $key => $result) {
        $result['node'] = services_node_load($result['node'], $fields);
        $results[$key] = $result;
      }
    }
    return $results;
  }
  return services_error(t('Search returned no results.'), 404);
}

/**
 * Return site content specified by a system setting.
 *
 * This service returns search results across the site, as specified by the
 * user at admin/settings/search_service. This can combine results from any
 * module that implements hook_search().
 *
 * @param $keys
 *   The keywords to search for within the content.
 * @param $simple
 *   When set to TRUE, only the fields indicated in $stdkeys will be returned.
 *   This can be helpful to limit the size of the search results.
 *
 * @return
 *   An array of search results. If $simple is TRUE, this array will contain
 *   only results and no content objects. If $simple is FALSE the array will
 *   contain both results and full content objects.
 *
 * @see hook_search()
 */
function search_service_content($keys, $simple = FALSE) {
  // define standard keys for simple set
  $stdkeys = array('link', 'type', 'title', 'user', 'date', 'snippet');

  // invoke the search hook to generate results
  $results = array();
  $search_hooks = variable_get('search_service_options', array('node' => 'node'));

  watchdog('search_service', t('search.content invoked for !keys using hooks !hooks', array('!keys' => $keys, '!hooks' => implode(', ', $search_hooks))));

  // run through only select hook_search() as defined in /admin/settings/search_service
  foreach ($search_hooks as $hook) {
    if (! empty($hook)) {
      $search_results = module_invoke($hook, 'search', 'search', $keys);
      if (! empty($search_results) ) {
        $results = array_merge($results, $search_results);
      }
    }
  }

  watchdog('search_service', t('search.content returned !count results for !keys using hooks !hooks', array('!count' => count($results), '!keys' => $keys, '!hooks' => implode($search_hooks, ', '))));

  if ($results and is_array($results) and count($results)) {
    // if simple results requested, remove extra data
    if ($simple) {
      $num = count($results);
      for ($i = 0; $i<$num; $i++) {
        $keys = array_keys($results[$i]);
        foreach ($keys as $key) {
          if (!in_array($key, $stdkeys)) {
            unset($results[$i][$key]);
          }
        }
      }
    }
    return $results;
  }
  return services_error(t('Search returned no results.'), 401);
}

/**
 * Return search results for users.
 *
 * @param $keys
 *   The keywords to search for within the users.
 *
 * @return
 *   An array of search results. 
 *
 * @see hook_search()
 * @see user_search()
 */
function search_service_users($keys) {
  // invoke the search hook to generate results
  $results = module_invoke('user', 'search', 'search', $keys);
  if ($results && is_array($results) && count($results)) {
    return $results;
  }
  return services_error(t('Search returned no results.'), 401);
}
