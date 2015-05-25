<?php

/**
 * Implementation of hook_features_api().
 */
function profile_features_api() {
  return array(
    'profile_field' => array(
      'name' => t('Profile fields'),
      'feature_source' => TRUE,
      'default_hook' => 'profile_default_profile_fields',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
    ),
  );
}

/**
 * Implementation of hook_features_export().
 */
function profile_field_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['profile'] = 'profile';
  foreach ($data as $field) {
    $export['features']['profile_field'][$field] = $field;
  }
  return array();
}

/**
 * Implementation of hook_features_export_options().
 */
function profile_field_features_export_options() {
  $fields = array();
  // Query DB directly instead of going through a Profile module function as
  // the Profile module doesn't provide a good function for this purpose.
  $db_result = db_query('SELECT name, title FROM {profile_fields}');
  while ($field = db_fetch_object($db_result)) {
    $fields[$field->name] = $field->title;
  }
  return $fields;
}

/**
 * Implementation of hook_features_export_render().
 */
function profile_field_features_export_render($module, $data) {
  $code = array();
  $code[] = '$profile_fields = array();';
  foreach ($data as $name) {
    $db_result = db_query("SELECT * FROM {profile_fields} WHERE name = '%s'", $name);
    if ($field = db_fetch_array($db_result)) {
      unset($field['fid']);
      $code[] = "\$profile_fields['{$name}'] = " . features_var_export($field) .";";
    }
  }
  $code[] = "return \$profile_fields;";
  $code = implode("\n", $code);
  return array('profile_default_profile_fields' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function profile_field_features_revert($module) {
  profile_field_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild().
 */
function profile_field_features_rebuild($module) {
  if ($defaults = features_get_default('profile_field', $module)) {
    foreach ($defaults as $field) {
      // If the field doesn't exist in the DB at the moment then insert it.
      if (db_result(db_query("SELECT COUNT(*) FROM {profile_fields} WHERE name = '%s'", $field['name'])) == 0) {
        drupal_write_record('profile_fields', $field);
      }
      // If the field exists, then update it.
      else {
        drupal_write_record('profile_fields', $field, 'name');
      }
    }
  }
}
