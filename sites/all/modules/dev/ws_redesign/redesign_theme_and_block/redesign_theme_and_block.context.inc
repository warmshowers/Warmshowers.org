<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function redesign_theme_and_block_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'authenticated_dashboard';
  $context->description = 'Dashboard page for logged-in users';
  $context->tag = 'all languages';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wsmap-wsmap_map_behaviors' => array(
          'module' => 'wsmap',
          'delta' => 'wsmap_map_behaviors',
          'region' => 'sidebar_first',
          'weight' => 0,
        ),
        'wsuser-wsuser_search_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_search_block',
          'region' => 'sidebar_first',
          'weight' => 1,
        ),
        'wsuser-wsuser_member_welcome_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_welcome_block',
          'region' => 'highlight',
          'weight' => 0,
        ),
        'wsuser-wsuser_member_recommendation' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_recommendation',
          'region' => 'highlight',
          'weight' => 1,
        ),
        'warmshowers_site-warmshowers_site_social_links' => array(
          'module' => 'warmshowers_site',
          'delta' => 'warmshowers_site_social_links',
          'region' => 'highlight',
          'weight' => 2,
        ),
        'views-recent_forum_topics-block_1' => array(
          'module' => 'views',
          'delta' => 'recent_forum_topics-block_1',
          'region' => 'highlight',
          'weight' => 3,
        ),
        'wsmap-wsmap_map' => array(
          'module' => 'wsmap',
          'delta' => 'wsmap_map',
          'region' => 'content_top',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard page for logged-in users');
  t('all languages');
  $export['authenticated_dashboard'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere';
  $context->description = 'Everywhere items for all languages (Language Switcher)';
  $context->tag = 'all languages';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'locale-0' => array(
          'module' => 'locale',
          'delta' => 0,
          'region' => 'header',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere items for all languages (Language Switcher)');
  t('all languages');
  $export['everywhere'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_en';
  $context->description = 'Everywhere Anon (English) - Anon main menu, for example';
  $context->tag = 'en';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'en-working' => 'en-working',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-anon-main-menu-en' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-en',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (English) - Anon main menu, for example');
  t('en');
  $export['everywhere_anon_en'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_es';
  $context->description = 'Everywhere Anon (Spanish) - Anon main menu, for example';
  $context->tag = 'es';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'es' => 'es',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-anon-main-menu-es' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-es',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Spanish) - Anon main menu, for example');
  t('es');
  $export['everywhere_anon_es'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_fr';
  $context->description = 'Everywhere Anon (French) - Anon main menu, for example';
  $context->tag = 'fr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fr' => 'fr',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-anon-main-menu-fr' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-fr',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (French) - Anon main menu, for example');
  t('fr');
  $export['everywhere_anon_fr'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_pt';
  $context->description = 'Everywhere Anon (Portugese) - Anon main menu, for example';
  $context->tag = 'pt';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pt-br' => 'pt-br',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-anon-main-menu-pt' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-pt',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Portugese) - Anon main menu, for example');
  t('pt');
  $export['everywhere_anon_pt'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_en';
  $context->description = '';
  $context->tag = 'en';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'en-working' => 'en-working',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-logged-in-main-menu-en' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-en',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('en');
  $export['everywhere_auth_en'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_es';
  $context->description = 'Authenticated, all pages, Spanish';
  $context->tag = 'es';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'es' => 'es',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-logged-in-main-menu-es' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-es',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, Spanish');
  t('es');
  $export['everywhere_auth_es'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_fr';
  $context->description = 'Authenticated, all pages, French';
  $context->tag = 'fr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fr' => 'fr',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-logged-in-main-menu-fr' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-fr',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, French');
  t('fr');
  $export['everywhere_auth_fr'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_pt';
  $context->description = 'Authenticated, all pages, Portugese';
  $context->tag = 'pt';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pt-br' => 'pt-br',
      ),
    ),
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-logged-in-main-menu-pt' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-pt',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, Portugese');
  t('pt');
  $export['everywhere_auth_pt'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_en';
  $context->description = 'Everywhere (English)';
  $context->tag = 'en';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'en-working' => 'en-working',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-en' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-en',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (English)');
  t('en');
  $export['everywhere_en'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_es';
  $context->description = 'Everywhere (Spanish)';
  $context->tag = 'es';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'es' => 'es',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-es' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-es',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Spanish)');
  t('es');
  $export['everywhere_es'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_fr';
  $context->description = 'Everywhere (French)';
  $context->tag = 'fr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fr' => 'fr',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-fr' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-fr',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (French)');
  t('fr');
  $export['everywhere_fr'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_pt';
  $context->description = 'Everywhere (Portuguese)';
  $context->tag = 'pt';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pt-br' => 'pt-br',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-pt' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-pt',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Portuguese)');
  t('pt');
  $export['everywhere_pt'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'profile_page';
  $context->description = 'Profile page';
  $context->tag = 'all languages';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'authenticated user' => 'authenticated user',
      ),
    ),
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'all',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wsuser-wsuser_member_contact_location' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_contact_location',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
        'wsuser-wsuser_member_actions' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_actions',
          'region' => 'sidebar_second',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Profile page');
  t('all languages');
  $export['profile_page'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'unauthenticated_front';
  $context->description = 'front page for unauthenticated users';
  $context->tag = 'all';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'en-working' => 'en-working',
        'es' => 'es',
        'fr' => 'fr',
        'pt-br' => 'pt-br',
      ),
    ),
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'user' => array(
      'values' => array(
        'anonymous user' => 'anonymous user',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'nodeblock-15526' => array(
          'module' => 'nodeblock',
          'delta' => 15526,
          'region' => 'sidebar_first',
          'weight' => 0,
        ),
        'views-photo_contest-block_2' => array(
          'module' => 'views',
          'delta' => 'photo_contest-block_2',
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
        'nodeblock-15521' => array(
          'module' => 'nodeblock',
          'delta' => 15521,
          'region' => 'sidebar_second',
          'weight' => 1,
        ),
        'wsuser-wsuser_member_count_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_count_block',
          'region' => 'highlight',
          'weight' => 0,
        ),
        'views-map_slideshow-block_1' => array(
          'module' => 'views',
          'delta' => 'map_slideshow-block_1',
          'region' => 'content_top',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('all');
  t('front page for unauthenticated users');
  $export['unauthenticated_front'] = $context;

  return $export;
}
