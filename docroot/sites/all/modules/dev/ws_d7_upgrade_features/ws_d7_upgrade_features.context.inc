<?php
/**
 * @file
 * ws_d7_upgrade_features.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function ws_d7_upgrade_features_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'admin_review';
  $context->description = 'Admin user and comment review';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'admin/user/review' => 'admin/user/review',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-comments_review-block_1' => array(
          'module' => 'views',
          'delta' => 'comments_review-block_1',
          'region' => 'sidebar_first',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin user and comment review');
  $export['admin_review'] = $context;

  $context = new stdClass();
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
        'wsuser-wsuser_member_welcome_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_welcome_block',
          'region' => 'highlighted',
          'weight' => '-10',
        ),
        'views-a0e2a8ab6066d8f20b8c3c1b25cf9735' => array(
          'module' => 'views',
          'delta' => 'a0e2a8ab6066d8f20b8c3c1b25cf9735',
          'region' => 'highlighted',
          'weight' => '-7',
        ),
        'donations_thermometer-thermometer' => array(
          'module' => 'donations_thermometer',
          'delta' => 'thermometer',
          'region' => 'highlighted',
          'weight' => '-6',
        ),
        'views-recent_feedback-recent_feedback' => array(
          'module' => 'views',
          'delta' => 'recent_feedback-recent_feedback',
          'region' => 'highlighted',
          'weight' => '-8',
        ),
        'wsmap-wsmap_map' => array(
          'module' => 'wsmap',
          'delta' => 'wsmap_map',
          'region' => 'content',
          'weight' => '-10',
        ),
        'wsmap-wsmap_map_behaviors' => array(
          'module' => 'wsmap',
          'delta' => 'wsmap_map_behaviors',
          'region' => 'sidebar_first',
          'weight' => '0',
        ),
        'wsuser-wsuser_search_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_search_block',
          'region' => 'sidebar_first',
          'weight' => '1',
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

  $context = new stdClass();
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
        'warmshowers_site-ws_user_account_menu' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_user_account_menu',
          'region' => 'header',
          'weight' => '-9',
        ),
        'warmshowers_site-ws_anonymous_user_menu' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_anonymous_user_menu',
          'region' => 'header',
          'weight' => '-5',
        ),
        'lang_dropdown-language' => array(
          'module' => 'lang_dropdown',
          'delta' => 'language',
          'region' => 'header',
          'weight' => '-7',
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_cn';
  $context->description = 'Everywhere Anon (Chinese) - Anon main menu';
  $context->tag = 'cn';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'zh-hans' => 'zh-hans',
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
        'menu-menu-anon-main-menu-cn' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-cn',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Chinese) - Anon main menu');
  t('cn');
  $export['everywhere_anon_cn'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_cs';
  $context->description = 'Everywhere Anon (Czech) - Anon main menu';
  $context->tag = 'cs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'cs' => 'cs',
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
        'menu-menu-anon-main-menu-cs' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-cs',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Czech) - Anon main menu');
  t('cs');
  $export['everywhere_anon_cs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_de';
  $context->description = 'Everywhere Anon (German) - Anon main menu, for example';
  $context->tag = 'de';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'de' => 'de',
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
        'menu-menu-anon-main-menu-de' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-de',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (German) - Anon main menu, for example');
  t('de');
  $export['everywhere_anon_de'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_fa';
  $context->description = 'Everywhere Anon (Farsi) - Anon main menu, for example';
  $context->tag = 'fa';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fa' => 'fa',
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
        'menu-menu-anon-main-menu-fa' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-fa',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Farsi) - Anon main menu, for example');
  t('fa');
  $export['everywhere_anon_fa'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_it';
  $context->description = 'Everywhere Anon (Italian) - Anon main menu, for example';
  $context->tag = 'it';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'it' => 'it',
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
        'menu-menu-anon-main-menu-it' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-it',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Italian) - Anon main menu, for example');
  t('it');
  $export['everywhere_anon_it'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_ja';
  $context->description = 'Everywhere Anon (Japanese) - Anon main menu, for example';
  $context->tag = 'ja';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ja' => 'ja',
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
        'menu-menu-anon-main-menu-ja' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-ja',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Japanese) - Anon main menu, for example');
  t('ja');
  $export['everywhere_anon_ja'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_pl';
  $context->description = 'Everywhere Anon (Polish) - Anon main menu';
  $context->tag = 'pl';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pl' => 'pl',
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
        'menu-menu-anon-main-menu-pl' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-pl',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Polish) - Anon main menu');
  t('pl');
  $export['everywhere_anon_pl'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_ro';
  $context->description = 'Everywhere Anon (Romanian) - Anon main menu';
  $context->tag = 'ro';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ro' => 'ro',
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
        'menu-menu-anon-main-menu-ro' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-ro',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Romanian) - Anon main menu');
  t('ro');
  $export['everywhere_anon_ro'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_rs';
  $context->description = 'Everywhere Anon (Serbian) - Anon main menu';
  $context->tag = 'rs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sr' => 'sr',
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
        'menu-menu-anon-main-menu-rs' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-rs',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Serbian) - Anon main menu');
  t('rs');
  $export['everywhere_anon_rs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_ru';
  $context->description = 'Everywhere Anon (Russian) - Anon main menu';
  $context->tag = 'ru';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ru' => 'ru',
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
        'menu-menu-anon-main-menu-ru' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-ru',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Russian) - Anon main menu');
  t('ru');
  $export['everywhere_anon_ru'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_sv';
  $context->description = 'Everywhere Anon (Swedish) - Anon main menu, for example';
  $context->tag = 'sv';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sv' => 'sv',
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
        'menu-menu-anon-main-menu-sv' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-sv',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Swedish) - Anon main menu, for example');
  t('sv');
  $export['everywhere_anon_sv'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_anon_tr';
  $context->description = 'Everywhere Anon (Turkish) - Anon main menu';
  $context->tag = 'tr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'tr' => 'tr',
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
        'menu-menu-anon-main-menu-tr' => array(
          'module' => 'menu',
          'delta' => 'menu-anon-main-menu-tr',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere Anon (Turkish) - Anon main menu');
  t('tr');
  $export['everywhere_anon_tr'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_cn';
  $context->description = '';
  $context->tag = 'cn';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'zh-hans' => 'zh-hans',
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
        'menu-menu-logged-in-main-menu-cn' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-cn',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('cn');
  $export['everywhere_auth_cn'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_cs';
  $context->description = '';
  $context->tag = 'cs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'cs' => 'cs',
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
        'menu-menu-logged-in-main-menu-cs' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-cs',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('cs');
  $export['everywhere_auth_cs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_de';
  $context->description = '';
  $context->tag = 'de';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'de' => 'de',
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
        'menu-menu-logged-in-main-menu-de' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-de',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('de');
  $export['everywhere_auth_de'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_fa';
  $context->description = '';
  $context->tag = 'fa';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fa' => 'fa',
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
        'menu-menu-logged-in-main-menu-fa' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-fa',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('fa');
  $export['everywhere_auth_fa'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_it';
  $context->description = '';
  $context->tag = 'it';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'it' => 'it',
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
        'menu-menu-logged-in-main-menu-it' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-it',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('it');
  $export['everywhere_auth_it'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_ja';
  $context->description = '';
  $context->tag = 'ja';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ja' => 'ja',
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
        'menu-menu-logged-in-main-menu-ja' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-ja',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('ja');
  $export['everywhere_auth_ja'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_pl';
  $context->description = '';
  $context->tag = 'pl';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pl' => 'pl',
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
        'menu-menu-logged-in-main-menu-pl' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-pl',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('pl');
  $export['everywhere_auth_pl'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_ro';
  $context->description = 'Authenticated, all pages, Romanian';
  $context->tag = 'ro';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ro' => 'ro',
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
        'menu-menu-logged-in-main-menu-ro' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-ro',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, Romanian');
  t('ro');
  $export['everywhere_auth_ro'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_rs';
  $context->description = '';
  $context->tag = 'rs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sr' => 'sr',
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
        'menu-menu-logged-in-main-menu-rs' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-rs',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('rs');
  $export['everywhere_auth_rs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_ru';
  $context->description = 'Authenticated, all pages, Russian';
  $context->tag = 'ru';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ru' => 'ru',
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
        'menu-menu-logged-in-main-menu-ru' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-ru',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, Russian');
  t('ru');
  $export['everywhere_auth_ru'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_sv';
  $context->description = 'Authenticated, all pages, Swedish';
  $context->tag = 'sv';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sv' => 'sv',
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
        'menu-menu-logged-in-main-menu-sv' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-sv',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Authenticated, all pages, Swedish');
  t('sv');
  $export['everywhere_auth_sv'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_auth_tr';
  $context->description = '';
  $context->tag = 'tr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'tr' => 'tr',
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
        'menu-menu-logged-in-main-menu-tr' => array(
          'module' => 'menu',
          'delta' => 'menu-logged-in-main-menu-tr',
          'region' => 'navigation',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('tr');
  $export['everywhere_auth_tr'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_cn';
  $context->description = 'Everywhere (Chinese)';
  $context->tag = 'cn';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'zh-hans' => 'zh-hans',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-cn' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-cn',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Chinese)');
  t('cn');
  $export['everywhere_cn'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_cs';
  $context->description = 'Everywhere (Czech)';
  $context->tag = 'cs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'cs' => 'cs',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-cs' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-cs',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Czech)');
  t('cs');
  $export['everywhere_cs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_de';
  $context->description = 'Everywhere (German)';
  $context->tag = 'de';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'de' => 'de',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-de' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-de',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (German)');
  t('de');
  $export['everywhere_de'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_fa';
  $context->description = 'Everywhere (Farsi)';
  $context->tag = 'fa';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'fa' => 'fa',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-fa' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-fa',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Farsi)');
  t('fa');
  $export['everywhere_fa'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_it';
  $context->description = 'Everywhere (Italian)';
  $context->tag = 'it';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'it' => 'it',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-it' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-it',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Italian)');
  t('it');
  $export['everywhere_it'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_ja';
  $context->description = 'Everywhere (Japanese)';
  $context->tag = 'ja';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ja' => 'ja',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-ja' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-ja',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Japanese)');
  t('ja');
  $export['everywhere_ja'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_pl';
  $context->description = 'Everywhere (Polish)';
  $context->tag = 'pl';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'pl' => 'pl',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-pl' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-pl',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Polish)');
  t('pl');
  $export['everywhere_pl'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_ro';
  $context->description = 'Everywhere (Romanian)';
  $context->tag = 'ro';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ro' => 'ro',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-ro' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-ro',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Romanian)');
  t('ro');
  $export['everywhere_ro'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_rs';
  $context->description = 'Everywhere (Serbian)';
  $context->tag = 'rs';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sr' => 'sr',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-rs' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-rs',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Serbian)');
  t('rs');
  $export['everywhere_rs'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_ru';
  $context->description = 'Everywhere (Russian)';
  $context->tag = 'ru';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'ru' => 'ru',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-ru' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-ru',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Russian)');
  t('ru');
  $export['everywhere_ru'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_sv';
  $context->description = 'Everywhere (Swedish)';
  $context->tag = 'sv';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'sv' => 'sv',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-sv' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-sv',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Swedish)');
  t('sv');
  $export['everywhere_sv'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere_tr';
  $context->description = 'Everywhere (Turkish)';
  $context->tag = 'tr';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'tr' => 'tr',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-menu-footer-menu-tr' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu-tr',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere (Turkish)');
  t('tr');
  $export['everywhere_tr'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'feedback_add_page';
  $context->description = 'Feedback Add Page';
  $context->tag = 'all languages';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'node/add/trust-referral' => 'node/add/trust-referral',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'nodeblock-88691' => array(
          'module' => 'nodeblock',
          'delta' => 88691,
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Feedback Add Page');
  t('all languages');
  $export['feedback_add_page'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'forums';
  $context->description = '';
  $context->tag = 'forums';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'forum' => 'forum',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'forum' => 'forum',
        'forum/*' => 'forum/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'search-0' => array(
          'module' => 'search',
          'delta' => 0,
          'region' => 'sidebar_second',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('forums');
  $export['forums'] = $context;

  $context = new stdClass();
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
        'wsuser-wsuser_member_profile_highlight' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_profile_highlight',
          'region' => 'highlighted',
          'weight' => '-10',
        ),
        'wsuser-wsuser_member_contact_location' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_contact_location',
          'region' => 'sidebar_second',
          'weight' => '0',
        ),
        'wsuser-wsuser_member_actions' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_actions',
          'region' => 'sidebar_second',
          'weight' => '1',
        ),
        'wsuser-wsuser_member_donate' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_donate',
          'region' => 'sidebar_second',
          'weight' => '2',
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'search_results';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'search' => 'search',
        'search/*' => 'search/*',
        'search/wsuser/*' => 'search/wsuser/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'wsuser-wsuser_search_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_search_block',
          'region' => 'sidebar_first',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;
  $export['search_results'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'thermometer';
  $context->description = 'Temporary thermometer page';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'node/89333' => 'node/89333',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'donations_thermometer-0' => array(
          'module' => 'donations_thermometer',
          'delta' => 0,
          'region' => 'content_bottom',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Temporary thermometer page');
  $export['thermometer'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'unauthenticated_front';
  $context->description = 'front page for unauthenticated users';
  $context->tag = 'all';
  $context->conditions = array(
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
        'warmshowers_site-ws_member_stats_block' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_member_stats_block',
          'region' => 'highlighted',
          'weight' => '-72',
        ),
        'warmshowers_site-ws_anonymous_welcome_message' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_anonymous_welcome_message',
          'region' => 'highlighted',
          'weight' => '-71',
        ),
        'nodeblock-15526' => array(
          'module' => 'nodeblock',
          'delta' => '15526',
          'region' => 'sidebar_first',
          'weight' => '0',
        ),
        'views-photo_contest-block_2' => array(
          'module' => 'views',
          'delta' => 'photo_contest-block_2',
          'region' => 'sidebar_second',
          'weight' => '0',
        ),
        'nodeblock-15521' => array(
          'module' => 'nodeblock',
          'delta' => '15521',
          'region' => 'sidebar_second',
          'weight' => '1',
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
