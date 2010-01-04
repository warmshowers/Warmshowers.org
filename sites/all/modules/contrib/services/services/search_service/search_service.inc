<?php
// $Id: search_service.inc,v 1.1.2.8.2.3 2009/12/05 01:42:39 heyrocker Exp $
/**
 * @file
 *  Link general search functionalities to services module.
 */

/**
 * Callback for search.nodes service.
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
 * Callback for search.content service.
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
 * Callback for search.users service.
 */
function search_service_users($keys) {
  // invoke the search hook to generate results
  $results = module_invoke('user', 'search', 'search', $keys);
  if ($results && is_array($results) && count($results)) {
    return $results;
  }
  return services_error(t('Search returned no results.'), 401);
}
