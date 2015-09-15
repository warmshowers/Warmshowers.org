<?php
/**
 * @file
 * ws_permissions_features.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function ws_permissions_features_user_default_roles() {
  $roles = array();

  // Exported role: Board Extra - Like board member.
  $roles['Board Extra - Like board member'] = array(
    'name' => 'Board Extra - Like board member',
    'weight' => 0,
  );

  // Exported role: Board Member.
  $roles['Board Member'] = array(
    'name' => 'Board Member',
    'weight' => 0,
  );

  // Exported role: Geolocation User Administrator.
  $roles['Geolocation User Administrator'] = array(
    'name' => 'Geolocation User Administrator',
    'weight' => 0,
  );

  // Exported role: Translator.
  $roles['Translator'] = array(
    'name' => 'Translator',
    'weight' => 0,
  );

  // Exported role: Translator (extras).
  $roles['Translator (extras)'] = array(
    'name' => 'Translator (extras)',
    'weight' => 0,
  );

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 0,
  );

  // Exported role: comment posting user.
  $roles['comment posting user'] = array(
    'name' => 'comment posting user',
    'weight' => 0,
  );

  // Exported role: current-member.
  $roles['current-member'] = array(
    'name' => 'current-member',
    'weight' => 0,
  );

  // Exported role: developer.
  $roles['developer'] = array(
    'name' => 'developer',
    'weight' => 0,
  );

  // Exported role: donation-basic.
  $roles['donation-basic'] = array(
    'name' => 'donation-basic',
    'weight' => 0,
  );

  // Exported role: donation-founder.
  $roles['donation-founder'] = array(
    'name' => 'donation-founder',
    'weight' => 0,
  );

  // Exported role: donation-free.
  $roles['donation-free'] = array(
    'name' => 'donation-free',
    'weight' => 0,
  );

  // Exported role: donation-maintainer.
  $roles['donation-maintainer'] = array(
    'name' => 'donation-maintainer',
    'weight' => 0,
  );

  // Exported role: donation-obsolete-was-founder.
  $roles['donation-obsolete-was-founder'] = array(
    'name' => 'donation-obsolete-was-founder',
    'weight' => 0,
  );

  // Exported role: donation-supporter.
  $roles['donation-supporter'] = array(
    'name' => 'donation-supporter',
    'weight' => 0,
  );

  // Exported role: editor.
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => 0,
  );

  // Exported role: email viewer.
  $roles['email viewer'] = array(
    'name' => 'email viewer',
    'weight' => 0,
  );

  // Exported role: pre-authorized.
  $roles['pre-authorized'] = array(
    'name' => 'pre-authorized',
    'weight' => 0,
  );

  // Exported role: registrar.
  $roles['registrar'] = array(
    'name' => 'registrar',
    'weight' => 0,
  );

  // Exported role: super-admin.
  $roles['super-admin'] = array(
    'name' => 'super-admin',
    'weight' => 0,
  );

  return $roles;
}
