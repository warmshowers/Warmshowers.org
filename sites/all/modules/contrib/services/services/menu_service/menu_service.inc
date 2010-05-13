<?php
// $Id: menu_service.inc,v 1.1.2.4.2.3 2010/05/11 01:43:43 heyrocker Exp $
/**
 * @file
 * Adds a service type for a menu
 *
 * @note This file has a 80 character width limit for all lines
 */

/**
 * Get the menu from the database.
 *
 * @param $menu_id
 *   The named menu links to return. Defaults to 'primary-links'.
 * @param $fields
 *   An array of fields to be returned, for use in limiting the
 *   size of data returned to only that which is necessary.
 * @param $language
 *   The language code of the menu to return. Optional if site 
 *   has only one language (or i18n is not installed <- verify)
 *
 * @return
 *   An array of all child menu items from a given menu item.
 */
function menu_service_get($menu_id = NULL, $fields = array(), $language = '') {
  if ($menu_id === NULL || trim($menu_id) === '') {
    $menu_id = 'primary-links';
  }
  if (sizeof($fields) === 0) {
    $fields = FALSE;
  }
  $data = menu_tree_all_data($menu_id);
  return menu_service_process($data, $fields, $language);
}

/**
 * Recursively returns menu items, optionally limited by an array of fields.
 *
 * @param $data
 *   The menu item whose children should be returned
 * @param $fields
 *   An array of fields to be returned, for use in limiting the
 *   size of data returned to only that which is necessary.
 * @param $language
 *   The language code of the menu to return. Defaults to default language.
 *
 * @return
 *   Array. All children menu items from a given menu item.
 */
function menu_service_process($data, $fields, $language) {
  $out = array();
  if (is_array($data)) {
    foreach ($data as $item) {
      $tmp = new stdClass;
      if ($fields) {
        foreach ($fields as $field) {
          $tmp->{$field} = $item['link'][$field];
        }
      }
      else {
        $tmp = (object) $item['link'];
      }
      
      if (!$fields || in_array('path_alias', $fields, TRUE)) {
      	$tmp->path_alias = drupal_get_path_alias($item['link']['link_path'], $language);
      }
            
      $tmp->children = menu_service_process($item['below'], $fields, $language);
      $out[] = $tmp;
    }
  }
  return $out;
}
