<?php
// $Id: emfield.views.inc,v 1.1.2.1 2009/09/30 15:11:01 aaron Exp $

/**
 * @file
 * Interface between emfield.module and views.module.
 */

// Include the files defining the classes we extend.
module_load_include('inc', 'content', 'includes/views/content.views');

/**
 * Implementation of hook_views_handlers().
 */
function emfield_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'emfield') . '/includes/views/handlers',
    ),
    'handlers' => array(

      // argument handlers
//       'emfield_handler_argument' => array(
//         'parent' => 'views_handler_argument',
//       ),
      'emfield_handler_argument_provider' => array(
        'parent' => 'content_handler_argument_string',
      ),
//       'emfield_handler_argument_numeric' => array(
//         'parent' => 'views_handler_argument_numeric',
//       ),
//       'emfield_handler_argument_reference' => array(
//         'parent' => 'emfield_handler_argument_numeric',
//       ),
//       'emfield_handler_argument_many_to_one' => array(
//         'parent' => 'views_handler_argument_many_to_one',
//       ),

      // field handlers
      'emfield_handler_field_provider' => array(
        'parent' => 'content_handler_field_multiple',
      ),
//       'emfield_handler_field_multiple' => array(
//         'parent' => 'emfield_handler_field',
//       ),

      // filter handlers
//       'emfield_handler_filter_string' => array(
//         'parent' => 'views_handler_filter_string',
//       ),
//       'emfield_handler_filter_numeric' => array(
//         'parent' => 'views_handler_filter_numeric',
//       ),
//       'emfield_handler_filter_float' => array(
//         'parent' => 'views_handler_filter_float',
//       ),
//       'emfield_handler_filter_many_to_one' => array(
//         'parent' => 'views_handler_filter_many_to_one',
//       ),

      // relationship handlers
//       'emfield_handler_relationship' => array(
//         'parent' => 'views_handler_relationship',
//       ),

      // sort handlers
//       'emfield_handler_sort' => array(
//         'parent' => 'views_handler_sort',
//       ),
    ),
  );
}

function theme_emfield_handler_field_provider($item, $field, $node, $values, $format) {
  $output = '';
  if (!empty($item) || $item == '0') {
    $provider = $item['provider'];
    $module = $field['module'];
    $info = emfield_include_invoke($module, $provider, 'info');
    switch ($format) {
      case 'provider':
        $output = check_plain($info['name']);
        break;
      case 'provider_raw':
        $output = check_plain($info['provider']);
        break;
      case 'provider_link':
        $output = l($info['name'], $info['url']);
        break;
      case 'provider_url':
        $output = url($info['url']);
        break;
      case 'provider_original_link':
        $output = l($info['name'], emfield_include_invoke($module, $provider, 'embedded_link', $item['value']));
        break;
      case 'provider_original_url':
        $output = url(emfield_include_invoke($module, $provider, 'embedded_link', $item['value']));
        break;
    }
  }
  return $output;
}
