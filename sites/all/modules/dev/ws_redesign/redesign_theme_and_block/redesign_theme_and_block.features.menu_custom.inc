<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function redesign_theme_and_block_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: anon_main_menu
  $menus['anon_main_menu'] = array(
    'menu_name' => 'anon_main_menu',
    'title' => 'Anon Main Menu',
    'description' => 'Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)',
  );
  // Exported menu: footer_menu
  $menus['footer_menu'] = array(
    'menu_name' => 'footer_menu',
    'title' => 'Footer Menu',
    'description' => 'Menu for use in Warmshowers Footer',
  );
  // Exported menu: logged_in_main_menu
  $menus['logged_in_main_menu'] = array(
    'menu_name' => 'logged_in_main_menu',
    'title' => 'Logged In Main Menu',
    'description' => 'Main Menu for Warmshowers Logged-In User (Home/FAQ/Forums/etc)',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Anon Main Menu');
  t('Footer Menu');
  t('Logged In Main Menu');
  t('Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)');
  t('Main Menu for Warmshowers Logged-In User (Home/FAQ/Forums/etc)');
  t('Menu for use in Warmshowers Footer');


  return $menus;
}
