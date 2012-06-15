<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function redesign_theme_and_block_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: anon-main-menu
  $menus['anon-main-menu'] = array(
    'menu_name' => 'anon-main-menu',
    'title' => 'Anon Main Menu',
    'description' => 'Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)',
  );
  // Exported menu: menu-footer-menu-en
  $menus['menu-footer-menu-en'] = array(
    'menu_name' => 'menu-footer-menu-en',
    'title' => 'Footer Menu (en)',
    'description' => 'Menu for use in Warmshowers Footer (English)',
  );
  // Exported menu: menu-footer-menu-es
  $menus['menu-footer-menu-es'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'title' => 'Footer Menu (es)',
    'description' => 'Menu for use in Warmshowers Footer (Spanish)',
  );
  // Exported menu: menu-logged-in-main-menu-en
  $menus['menu-logged-in-main-menu-en'] = array(
    'menu_name' => 'menu-logged-in-main-menu-en',
    'title' => 'Logged In Main Menu (en)',
    'description' => 'Main Menu for Warmshowers Logged-In User (Home/FAQ/Forums/etc)',
  );
  // Exported menu: menu-logged-in-main-menu-es
  $menus['menu-logged-in-main-menu-es'] = array(
    'menu_name' => 'menu-logged-in-main-menu-es',
    'title' => 'Logged In Main Menu (es)',
    'description' => 'Spanish logged-in main menu',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Anon Main Menu');
  t('Footer Menu (en)');
  t('Footer Menu (es)');
  t('Logged In Main Menu (en)');
  t('Logged In Main Menu (es)');
  t('Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)');
  t('Main Menu for Warmshowers Logged-In User (Home/FAQ/Forums/etc)');
  t('Menu for use in Warmshowers Footer (English)');
  t('Menu for use in Warmshowers Footer (Spanish)');
  t('Spanish logged-in main menu');


  return $menus;
}
