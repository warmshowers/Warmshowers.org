<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function redesign_theme_and_block_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: anon_main_menu:<front>
  $menu_links['anon_main_menu:<front>'] = array(
    'menu_name' => 'anon_main_menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: anon_main_menu:faq
  $menu_links['anon_main_menu:faq'] = array(
    'menu_name' => 'anon_main_menu',
    'link_path' => 'faq',
    'router_path' => 'faq',
    'link_title' => 'FAQ',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: anon_main_menu:forum
  $menu_links['anon_main_menu:forum'] = array(
    'menu_name' => 'anon_main_menu',
    'link_path' => 'forum',
    'router_path' => 'forum',
    'link_title' => 'Forums',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-47',
  );
  // Exported menu link: anon_main_menu:user/register
  $menu_links['anon_main_menu:user/register'] = array(
    'menu_name' => 'anon_main_menu',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Sign Up',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Exported menu link: footer_menu:<front>
  $menu_links['footer_menu:<front>'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: footer_menu:contact
  $menu_links['footer_menu:contact'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'contact',
    'router_path' => 'contact',
    'link_title' => 'Contact Us',
    'options' => array(
      'attributes' => array(
        'title' => 'Contact the site administrators if you have a problem with the site or need other help',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Exported menu link: footer_menu:faq
  $menu_links['footer_menu:faq'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'faq',
    'router_path' => 'faq',
    'link_title' => 'Frequently-Asked Questions',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-47',
  );
  // Exported menu link: footer_menu:logout
  $menu_links['footer_menu:logout'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'logout',
    'router_path' => 'logout',
    'link_title' => 'Log Out',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-45',
  );
  // Exported menu link: footer_menu:node/25
  $menu_links['footer_menu:node/25'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'node/25',
    'router_path' => 'node/%',
    'link_title' => 'Donate!',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: footer_menu:user/login
  $menu_links['footer_menu:user/login'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Log In',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: footer_menu:user/register
  $menu_links['footer_menu:user/register'] = array(
    'menu_name' => 'footer_menu',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Sign Up',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-44',
  );
  // Exported menu link: logged_in_main_menu:<front>
  $menu_links['logged_in_main_menu:<front>'] = array(
    'menu_name' => 'logged_in_main_menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: logged_in_main_menu:faq
  $menu_links['logged_in_main_menu:faq'] = array(
    'menu_name' => 'logged_in_main_menu',
    'link_path' => 'faq',
    'router_path' => 'faq',
    'link_title' => 'FAQ',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Exported menu link: logged_in_main_menu:forum
  $menu_links['logged_in_main_menu:forum'] = array(
    'menu_name' => 'logged_in_main_menu',
    'link_path' => 'forum',
    'router_path' => 'forum',
    'link_title' => 'Forums',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: logged_in_main_menu:user
  $menu_links['logged_in_main_menu:user'] = array(
    'menu_name' => 'logged_in_main_menu',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => 'Me',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Contact Us');
  t('Donate!');
  t('FAQ');
  t('Forums');
  t('Frequently-Asked Questions');
  t('Home');
  t('Log In');
  t('Log Out');
  t('Me');
  t('Sign Up');


  return $menu_links;
}
