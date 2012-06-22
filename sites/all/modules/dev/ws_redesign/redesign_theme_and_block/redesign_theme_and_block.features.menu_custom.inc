<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function redesign_theme_and_block_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-anon-main-menu-en
  $menus['menu-anon-main-menu-en'] = array(
    'menu_name' => 'menu-anon-main-menu-en',
    'title' => 'Anon Main Menu (en)',
    'description' => 'Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)',
  );
  // Exported menu: menu-anon-main-menu-es
  $menus['menu-anon-main-menu-es'] = array(
    'menu_name' => 'menu-anon-main-menu-es',
    'title' => 'Anon Main Menu (es)',
    'description' => 'Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc) (Spanish)',
  );
  // Exported menu: menu-anon-main-menu-fr
  $menus['menu-anon-main-menu-fr'] = array(
    'menu_name' => 'menu-anon-main-menu-fr',
    'title' => 'Anon Main Menu (fr)',
    'description' => 'Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)',
  );
  // Exported menu: menu-anon-main-menu-pt
  $menus['menu-anon-main-menu-pt'] = array(
    'menu_name' => 'menu-anon-main-menu-pt',
    'title' => 'Anon Main Menu (pt)',
    'description' => 'Main Menu for Warmshowers Portugese Anon User (Home/Signup/FAQ/etc)',
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
  // Exported menu: menu-footer-menu-fr
  $menus['menu-footer-menu-fr'] = array(
    'menu_name' => 'menu-footer-menu-fr',
    'title' => 'Footer Menu (fr)',
    'description' => '',
  );
  // Exported menu: menu-footer-menu-pt
  $menus['menu-footer-menu-pt'] = array(
    'menu_name' => 'menu-footer-menu-pt',
    'title' => 'Footer Menu (pt)',
    'description' => '',
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
  // Exported menu: menu-logged-in-main-menu-fr
  $menus['menu-logged-in-main-menu-fr'] = array(
    'menu_name' => 'menu-logged-in-main-menu-fr',
    'title' => 'Logged In Main Menu (fr)',
    'description' => '',
  );
  // Exported menu: menu-logged-in-main-menu-pt
  $menus['menu-logged-in-main-menu-pt'] = array(
    'menu_name' => 'menu-logged-in-main-menu-pt',
    'title' => 'Logged In Main Menu (pt)',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Anon Main Menu (en)');
  t('Anon Main Menu (es)');
  t('Anon Main Menu (fr)');
  t('Anon Main Menu (pt)');
  t('Footer Menu (en)');
  t('Footer Menu (es)');
  t('Footer Menu (fr)');
  t('Footer Menu (pt)');
  t('Logged In Main Menu (en)');
  t('Logged In Main Menu (es)');
  t('Logged In Main Menu (fr)');
  t('Logged In Main Menu (pt)');
  t('Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc)');
  t('Main Menu for Warmshowers Anon User (Home/Signup/FAQ/etc) (Spanish)');
  t('Main Menu for Warmshowers Logged-In User (Home/FAQ/Forums/etc)');
  t('Main Menu for Warmshowers Portugese Anon User (Home/Signup/FAQ/etc)');
  t('Menu for use in Warmshowers Footer (English)');
  t('Menu for use in Warmshowers Footer (Spanish)');
  t('Spanish logged-in main menu');


  return $menus;
}
