<?php

/**
 * Implementation of hook_default_fe_block_settings().
 */
function redesign_theme_and_block_default_fe_block_settings() {
  $export = array();

  // warmshowers_zen
  $theme = array();

  $theme['block-anon_left_content'] = array(
    'module' => 'block',
    'status' => 0,
    'weight' => '0',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '<none>',
    'cache' => '-1',
    'machine_name' => 'anon_left_content',
  );

  $theme['follow-site'] = array(
    'module' => 'follow',
    'delta' => 'site',
    'status' => 0,
    'weight' => '-24',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '',
    'cache' => '1',
  );

  $theme['locale-0'] = array(
    'module' => 'locale',
    'delta' => '0',
    'status' => 0,
    'weight' => '-27',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '<none>',
    'cache' => '-1',
  );

  $theme['menu-anon_main_menu'] = array(
    'module' => 'menu',
    'delta' => 'anon_main_menu',
    'status' => 0,
    'weight' => '8',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '<none>',
    'cache' => '-1',
  );

  $theme['menu-footer_menu'] = array(
    'module' => 'menu',
    'delta' => 'footer_menu',
    'status' => 0,
    'weight' => '9',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '<none>',
    'cache' => '-1',
  );

  $theme['menu-logged_in_main_menu'] = array(
    'module' => 'menu',
    'delta' => 'logged_in_main_menu',
    'status' => 0,
    'weight' => '10',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '<none>',
    'cache' => '-1',
  );

  $theme['wsuser-wsuser_member_count_block'] = array(
    'module' => 'wsuser',
    'delta' => 'wsuser_member_count_block',
    'status' => 0,
    'weight' => '14',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '',
    'cache' => '8',
  );

  $theme['wsuser-wsuser_member_recommendation'] = array(
    'module' => 'wsuser',
    'delta' => 'wsuser_member_recommendation',
    'status' => 0,
    'weight' => '15',
    'region' => '',
    'custom' => '0',
    'throttle' => '0',
    'visibility' => '0',
    'pages' => '',
    'theme' => 'warmshowers_zen',
    'title' => '',
    'cache' => '1',
  );

  $export['warmshowers_zen'] = $theme;


  $theme_default = variable_get('theme_default', 'garland');
  $themes = list_themes();
  foreach ($export as $theme_key => $settings) {
    if ($theme_key != $theme_default && empty($themes[$theme_key]->status)) {
      unset($export[$theme_key]);
    }
  }
  return $export;
}
