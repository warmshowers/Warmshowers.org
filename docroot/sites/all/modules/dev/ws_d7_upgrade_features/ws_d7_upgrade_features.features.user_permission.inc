<?php
/**
 * @file
 * ws_d7_upgrade_features.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ws_d7_upgrade_features_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'cancel account'.
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user',
  );

  return $permissions;
}
