<?php
// $Id: search_service_admin.inc,v 1.1.2.1 2009/03/07 19:10:18 marcingy Exp $

/**
 * @file
 * Search Service settings
 */

/**
 * Form to choose which search services
 */
function search_service_admin() {

  // For each enabled module, look for hook_search
  foreach (module_list() as $name) {
    if (function_exists($name . '_search')) {
      $options[$name] = $name;
    }
  }

  // If search_service_options not initialized, then make sure that it is an array
  $default_value = variable_get('search_service_options', 'node');
  if (! is_array($default_value)) {
    $default_value = array($default_value);
  }
  $form['search_service_message'] = array(
    '#type'          => 'markup',
    '#value'         => t('Choose search hooks that return local results. Do not choose aggregated or distributed results, as this may cause recursion.'), 
  );
  // Display checkboxes of list of hook_search()
  $form['search_service_options'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Search content services, returns searches results using'),
    '#options'       => $options,
    '#default_value' => $default_value,
  );

  return system_settings_form($form);
}
