<?php

/**
 * @file
 * Features integration for Ubercart product classes.
 */

/**
 * Implements hook_features_export_options().
 */
function uc_product_classes_features_export_options() {
  $classes = array();
  foreach (uc_product_node_info() as $type => $info) {
    $classes[$type] = $info['name'];
  }
  return $classes;
}

/**
 * Implements hook_features_export().
 */
function uc_product_classes_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['uc_product'] = 'uc_product';
  foreach ($data as $class) {
    $export['features']['uc_product_classes'][$class] = $class;
  }

  // Create a pipe for the CCK fields and variables to be exported.
  $pipe = node_features_export($data, $export, $module_name);

  // If strongarm is available, export the shippable and image settings.
  if (module_exists('strongarm')) {
    foreach ($data as $class) {
      $pipe['variable'][] = "uc_product_shippable_{$class}";
      $pipe['variable'][] = "uc_image_{$class}";
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uc_product_classes_features_export_render($module, $data, $export = NULL) {
  $output = node_features_export_render($module, $data, $export);
  return array('uc_product_default_classes' => $output['node_info']);
}

/**
 * Implements hook_features_revert().
 *
 * @see node_features_revert()
 */
function uc_product_classes_features_revert($module) {
  if ($default_types = features_get_default('uc_product_classes', $module)) {
    foreach ($default_types as $type_name => $type_info) {
      // We need to de-activate any missing fields. De-activating allows us to
      // preserve data. We de-activate by setting the widget_active flag to 0;
      // widget_active is incorrectly named, and really should be
      // instance_active
      if (module_exists('content')) {
        // Our existing fields ($fields) needs to be the first argument here,
        // so only fields that don't exist in code can be de-activated.
        if ($deleted_fields = array_diff(content_features_fields_normal($type_name), content_features_fields_default($type_name))) {
          foreach ($deleted_fields as $field_name) {
            db_update(content_instance_tablename())
              ->fields(array(
                  'widget_active' => 0,
                ))
              ->condition('field_name', $field_name)
              ->condition('type_name', $type_name)
              ->execute();
          }
        }
      }
      // Delete node types
      // We don't use node_type_delete() because we do not actually
      // want to delete the node type (and invoke hook_node_type()).
      // This can lead to bad consequences like CCK deleting field
      // storage in the DB.
      db_delete('node_type')
        ->condition('type', $type_name)
        ->execute();
      db_delete('uc_product_classes')
        ->condition('pcid', $type_name)
        ->execute();
    }
    uc_product_node_info(TRUE);
    node_types_rebuild();
    menu_rebuild();
  }
}
