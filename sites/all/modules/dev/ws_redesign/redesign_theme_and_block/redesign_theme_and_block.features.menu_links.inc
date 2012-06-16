<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function redesign_theme_and_block_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-anon-main-menu-en:<front>
  $menu_links['menu-anon-main-menu-en:<front>'] = array(
    'menu_name' => 'menu-anon-main-menu-en',
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
  // Exported menu link: menu-anon-main-menu-en:faq
  $menu_links['menu-anon-main-menu-en:faq'] = array(
    'menu_name' => 'menu-anon-main-menu-en',
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
  // Exported menu link: menu-anon-main-menu-en:forum
  $menu_links['menu-anon-main-menu-en:forum'] = array(
    'menu_name' => 'menu-anon-main-menu-en',
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
  // Exported menu link: menu-anon-main-menu-en:user/register
  $menu_links['menu-anon-main-menu-en:user/register'] = array(
    'menu_name' => 'menu-anon-main-menu-en',
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
  // Exported menu link: menu-anon-main-menu-es:<front>
  $menu_links['menu-anon-main-menu-es:<front>'] = array(
    'menu_name' => 'menu-anon-main-menu-es',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Inicio',
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
  // Exported menu link: menu-anon-main-menu-es:faq
  $menu_links['menu-anon-main-menu-es:faq'] = array(
    'menu_name' => 'menu-anon-main-menu-es',
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
  // Exported menu link: menu-anon-main-menu-es:forum
  $menu_links['menu-anon-main-menu-es:forum'] = array(
    'menu_name' => 'menu-anon-main-menu-es',
    'link_path' => 'forum',
    'router_path' => 'forum',
    'link_title' => 'Foros',
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
  // Exported menu link: menu-anon-main-menu-es:user/register
  $menu_links['menu-anon-main-menu-es:user/register'] = array(
    'menu_name' => 'menu-anon-main-menu-es',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Crear Cuenta',
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
  // Exported menu link: menu-footer-menu-en:<front>
  $menu_links['menu-footer-menu-en:<front>'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
  // Exported menu link: menu-footer-menu-en:contact
  $menu_links['menu-footer-menu-en:contact'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-48',
  );
  // Exported menu link: menu-footer-menu-en:faq
  $menu_links['menu-footer-menu-en:faq'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-44',
  );
  // Exported menu link: menu-footer-menu-en:logout
  $menu_links['menu-footer-menu-en:logout'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-40',
  );
  // Exported menu link: menu-footer-menu-en:node/25
  $menu_links['menu-footer-menu-en:node/25'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-46',
  );
  // Exported menu link: menu-footer-menu-en:user/login
  $menu_links['menu-footer-menu-en:user/login'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-42',
  );
  // Exported menu link: menu-footer-menu-en:user/register
  $menu_links['menu-footer-menu-en:user/register'] = array(
    'menu_name' => 'menu-footer-menu-en',
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
    'weight' => '-38',
  );
  // Exported menu link: menu-footer-menu-es:<front>
  $menu_links['menu-footer-menu-es:<front>'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Inicio',
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
    'weight' => '-49',
  );
  // Exported menu link: menu-footer-menu-es:contact
  $menu_links['menu-footer-menu-es:contact'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'link_path' => 'contact',
    'router_path' => 'contact',
    'link_title' => 'Contactarnos',
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
  // Exported menu link: menu-footer-menu-es:logout
  $menu_links['menu-footer-menu-es:logout'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'link_path' => 'logout',
    'router_path' => 'logout',
    'link_title' => 'Cerrar sesión',
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
    'weight' => '-39',
  );
  // Exported menu link: menu-footer-menu-es:user/login
  $menu_links['menu-footer-menu-es:user/login'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Iniciar sesión',
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
    'weight' => '-41',
  );
  // Exported menu link: menu-footer-menu-es:user/register
  $menu_links['menu-footer-menu-es:user/register'] = array(
    'menu_name' => 'menu-footer-menu-es',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Crear Cuenta',
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
    'weight' => '0',
  );
  // Exported menu link: menu-logged-in-main-menu-en:<front>
  $menu_links['menu-logged-in-main-menu-en:<front>'] = array(
    'menu_name' => 'menu-logged-in-main-menu-en',
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
  // Exported menu link: menu-logged-in-main-menu-en:faq
  $menu_links['menu-logged-in-main-menu-en:faq'] = array(
    'menu_name' => 'menu-logged-in-main-menu-en',
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
  // Exported menu link: menu-logged-in-main-menu-en:forum
  $menu_links['menu-logged-in-main-menu-en:forum'] = array(
    'menu_name' => 'menu-logged-in-main-menu-en',
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
  // Exported menu link: menu-logged-in-main-menu-es:<front>
  $menu_links['menu-logged-in-main-menu-es:<front>'] = array(
    'menu_name' => 'menu-logged-in-main-menu-es',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Inicio',
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
    'weight' => '-49',
  );
  // Exported menu link: menu-logged-in-main-menu-es:faq
  $menu_links['menu-logged-in-main-menu-es:faq'] = array(
    'menu_name' => 'menu-logged-in-main-menu-es',
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
  // Exported menu link: menu-logged-in-main-menu-es:forum
  $menu_links['menu-logged-in-main-menu-es:forum'] = array(
    'menu_name' => 'menu-logged-in-main-menu-es',
    'link_path' => 'forum',
    'router_path' => 'forum',
    'link_title' => 'Foros',
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Cerrar sesión');
  t('Contact Us');
  t('Contactarnos');
  t('Crear Cuenta');
  t('Donate!');
  t('FAQ');
  t('Foros');
  t('Forums');
  t('Frequently-Asked Questions');
  t('Home');
  t('Iniciar sesión');
  t('Inicio');
  t('Log In');
  t('Log Out');
  t('Sign Up');


  return $menu_links;
}
