<?php
/**
 * @file
 * ws_permissions_features.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ws_permissions_features_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration menu'.
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(
      'administrator' => 'administrator',
      'registrar' => 'registrar',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access all views'.
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'views',
  );

  // Exported permission: 'access better messages'.
  $permissions['access better messages'] = array(
    'name' => 'access better messages',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'better_messages',
  );

  // Exported permission: 'access comments'.
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'pre-authorized' => 'pre-authorized',
      'registrar' => 'registrar',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'pre-authorized' => 'pre-authorized',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access contextual links'.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'contextual',
  );

  // Exported permission: 'access devel information'.
  $permissions['access devel information'] = array(
    'name' => 'access devel information',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'access environment indicator'.
  $permissions['access environment indicator'] = array(
    'name' => 'access environment indicator',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'environment_indicator',
  );

  // Exported permission: 'access environment indicator default_environment'.
  $permissions['access environment indicator default_environment'] = array(
    'name' => 'access environment indicator default_environment',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'environment_indicator',
  );

  // Exported permission: 'access environment indicator overwritten_environment'.
  $permissions['access environment indicator overwritten_environment'] = array(
    'name' => 'access environment indicator overwritten_environment',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'environment_indicator',
  );

  // Exported permission: 'access map'.
  $permissions['access map'] = array(
    'name' => 'access map',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'pre-authorized' => 'pre-authorized',
    ),
    'module' => 'wsmap',
  );

  // Exported permission: 'access map detail'.
  $permissions['access map detail'] = array(
    'name' => 'access map detail',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'wsmap',
  );

  // Exported permission: 'access rules debug'.
  $permissions['access rules debug'] = array(
    'name' => 'access rules debug',
    'roles' => array(),
    'module' => 'rules',
  );

  // Exported permission: 'access site in maintenance mode'.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access site reports'.
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access site-wide contact form'.
  $permissions['access site-wide contact form'] = array(
    'name' => 'access site-wide contact form',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'pre-authorized' => 'pre-authorized',
      'registrar' => 'registrar',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'access user contact forms'.
  $permissions['access user contact forms'] = array(
    'name' => 'access user contact forms',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer actions'.
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer attributes'.
  $permissions['administer attributes'] = array(
    'name' => 'administer attributes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_attribute',
  );

  // Exported permission: 'administer better messages'.
  $permissions['administer better messages'] = array(
    'name' => 'administer better messages',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'better_messages',
  );

  // Exported permission: 'administer blocks'.
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'block',
  );

  // Exported permission: 'administer browscap'.
  $permissions['administer browscap'] = array(
    'name' => 'administer browscap',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'browscap',
  );

  // Exported permission: 'administer cart links'.
  $permissions['administer cart links'] = array(
    'name' => 'administer cart links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_cart_links',
  );

  // Exported permission: 'administer comment notify'.
  $permissions['administer comment notify'] = array(
    'name' => 'administer comment notify',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'comment_notify',
  );

  // Exported permission: 'administer comments'.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'administer contact forms'.
  $permissions['administer contact forms'] = array(
    'name' => 'administer contact forms',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'contact',
  );

  // Exported permission: 'administer content types'.
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer contexts'.
  $permissions['administer contexts'] = array(
    'name' => 'administer contexts',
    'roles' => array(),
    'module' => 'context_ui',
  );

  // Exported permission: 'administer credit cards'.
  $permissions['administer credit cards'] = array(
    'name' => 'administer credit cards',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_credit',
  );

  // Exported permission: 'administer css injection'.
  $permissions['administer css injection'] = array(
    'name' => 'administer css injection',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'css_injector',
  );

  // Exported permission: 'administer environment indicator settings'.
  $permissions['administer environment indicator settings'] = array(
    'name' => 'administer environment indicator settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'environment_indicator',
  );

  // Exported permission: 'administer faq'.
  $permissions['administer faq'] = array(
    'name' => 'administer faq',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'faq',
  );

  // Exported permission: 'administer faq order'.
  $permissions['administer faq order'] = array(
    'name' => 'administer faq order',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'faq',
  );

  // Exported permission: 'administer features'.
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'administer filters'.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'administer flags'.
  $permissions['administer flags'] = array(
    'name' => 'administer flags',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'administer forums'.
  $permissions['administer forums'] = array(
    'name' => 'administer forums',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'forum',
  );

  // Exported permission: 'administer honeypot'.
  $permissions['administer honeypot'] = array(
    'name' => 'administer honeypot',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'honeypot',
  );

  // Exported permission: 'administer image styles'.
  $permissions['administer image styles'] = array(
    'name' => 'administer image styles',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'image',
  );

  // Exported permission: 'administer js_injector'.
  $permissions['administer js_injector'] = array(
    'name' => 'administer js_injector',
    'roles' => array(),
    'module' => 'js_injector',
  );

  // Exported permission: 'administer languages'.
  $permissions['administer languages'] = array(
    'name' => 'administer languages',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'administer mailsystem'.
  $permissions['administer mailsystem'] = array(
    'name' => 'administer mailsystem',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'mailsystem',
  );

  // Exported permission: 'administer menu'.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'menu',
  );

  // Exported permission: 'administer modules'.
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: 'administer nodes'.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer order workflow'.
  $permissions['administer order workflow'] = array(
    'name' => 'administer order workflow',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'administer own product features'.
  $permissions['administer own product features'] = array(
    'name' => 'administer own product features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_product',
  );

  // Exported permission: 'administer pathauto'.
  $permissions['administer pathauto'] = array(
    'name' => 'administer pathauto',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'pathauto',
  );

  // Exported permission: 'administer permissions'.
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer persistent login'.
  $permissions['administer persistent login'] = array(
    'name' => 'administer persistent login',
    'roles' => array(),
    'module' => 'persistent_login',
  );

  // Exported permission: 'administer privatemsg settings'.
  $permissions['administer privatemsg settings'] = array(
    'name' => 'administer privatemsg settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'administer product attributes'.
  $permissions['administer product attributes'] = array(
    'name' => 'administer product attributes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_attribute',
  );

  // Exported permission: 'administer product classes'.
  $permissions['administer product classes'] = array(
    'name' => 'administer product classes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_product',
  );

  // Exported permission: 'administer product features'.
  $permissions['administer product features'] = array(
    'name' => 'administer product features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_product',
  );

  // Exported permission: 'administer product options'.
  $permissions['administer product options'] = array(
    'name' => 'administer product options',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_attribute',
  );

  // Exported permission: 'administer products'.
  $permissions['administer products'] = array(
    'name' => 'administer products',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_product',
  );

  // Exported permission: 'administer recurring fees'.
  $permissions['administer recurring fees'] = array(
    'name' => 'administer recurring fees',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_recurring',
  );

  // Exported permission: 'administer reroute email'.
  $permissions['administer reroute email'] = array(
    'name' => 'administer reroute email',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'reroute_email',
  );

  // Exported permission: 'administer rules'.
  $permissions['administer rules'] = array(
    'name' => 'administer rules',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'rules',
  );

  // Exported permission: 'administer search'.
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'search',
  );

  // Exported permission: 'administer services'.
  $permissions['administer services'] = array(
    'name' => 'administer services',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'administer site configuration'.
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer software updates'.
  $permissions['administer software updates'] = array(
    'name' => 'administer software updates',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: 'administer store'.
  $permissions['administer store'] = array(
    'name' => 'administer store',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_store',
  );

  // Exported permission: 'administer taxonomy'.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'administer themes'.
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: 'administer unit tests'.
  $permissions['administer unit tests'] = array(
    'name' => 'administer unit tests',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'simpletest',
  );

  // Exported permission: 'administer url aliases'.
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'path',
  );

  // Exported permission: 'administer users'.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'Geolocation User Administrator' => 'Geolocation User Administrator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer views'.
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'views',
  );

  // Exported permission: 'allow disabling privatemsg'.
  $permissions['allow disabling privatemsg'] = array(
    'name' => 'allow disabling privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'block IP addresses'.
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(),
    'module' => 'system',
  );

  // Exported permission: 'bypass honeypot protection'.
  $permissions['bypass honeypot protection'] = array(
    'name' => 'bypass honeypot protection',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'honeypot',
  );

  // Exported permission: 'bypass node access'.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'bypass rules access'.
  $permissions['bypass rules access'] = array(
    'name' => 'bypass rules access',
    'roles' => array(),
    'module' => 'rules',
  );

  // Exported permission: 'cancel account'.
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: 'change own username'.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'user',
  );

  // Exported permission: 'choose htmlmail_plaintext'.
  $permissions['choose htmlmail_plaintext'] = array(
    'name' => 'choose htmlmail_plaintext',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'htmlmail',
  );

  // Exported permission: 'context ajax block access'.
  $permissions['context ajax block access'] = array(
    'name' => 'context ajax block access',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'context_ui',
  );

  // Exported permission: 'create block content'.
  $permissions['create block content'] = array(
    'name' => 'create block content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create blog content'.
  $permissions['create blog content'] = array(
    'name' => 'create blog content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create contest_photo content'.
  $permissions['create contest_photo content'] = array(
    'name' => 'create contest_photo content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create faq content'.
  $permissions['create faq content'] = array(
    'name' => 'create faq content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create forum content'.
  $permissions['create forum content'] = array(
    'name' => 'create forum content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create orders'.
  $permissions['create orders'] = array(
    'name' => 'create orders',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'create page content'.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create private message tags'.
  $permissions['create private message tags'] = array(
    'name' => 'create private message tags',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg_filter',
  );

  // Exported permission: 'create product content'.
  $permissions['create product content'] = array(
    'name' => 'create product content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create sitebuilding content'.
  $permissions['create sitebuilding content'] = array(
    'name' => 'create sitebuilding content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create story content'.
  $permissions['create story content'] = array(
    'name' => 'create story content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create tip content'.
  $permissions['create tip content'] = array(
    'name' => 'create tip content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create trust_referral content'.
  $permissions['create trust_referral content'] = array(
    'name' => 'create trust_referral content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'path',
  );

  // Exported permission: 'create video content'.
  $permissions['create video content'] = array(
    'name' => 'create video content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any block content'.
  $permissions['delete any block content'] = array(
    'name' => 'delete any block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any blog content'.
  $permissions['delete any blog content'] = array(
    'name' => 'delete any blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any contest_photo content'.
  $permissions['delete any contest_photo content'] = array(
    'name' => 'delete any contest_photo content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any faq content'.
  $permissions['delete any faq content'] = array(
    'name' => 'delete any faq content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any forum content'.
  $permissions['delete any forum content'] = array(
    'name' => 'delete any forum content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any page content'.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any product content'.
  $permissions['delete any product content'] = array(
    'name' => 'delete any product content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any sitebuilding content'.
  $permissions['delete any sitebuilding content'] = array(
    'name' => 'delete any sitebuilding content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any story content'.
  $permissions['delete any story content'] = array(
    'name' => 'delete any story content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any tip content'.
  $permissions['delete any tip content'] = array(
    'name' => 'delete any tip content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any trust_referral content'.
  $permissions['delete any trust_referral content'] = array(
    'name' => 'delete any trust_referral content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any video content'.
  $permissions['delete any video content'] = array(
    'name' => 'delete any video content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete orders'.
  $permissions['delete orders'] = array(
    'name' => 'delete orders',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'delete own block content'.
  $permissions['delete own block content'] = array(
    'name' => 'delete own block content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own blog content'.
  $permissions['delete own blog content'] = array(
    'name' => 'delete own blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own contest_photo content'.
  $permissions['delete own contest_photo content'] = array(
    'name' => 'delete own contest_photo content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own faq content'.
  $permissions['delete own faq content'] = array(
    'name' => 'delete own faq content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own forum content'.
  $permissions['delete own forum content'] = array(
    'name' => 'delete own forum content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own page content'.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own product content'.
  $permissions['delete own product content'] = array(
    'name' => 'delete own product content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own sitebuilding content'.
  $permissions['delete own sitebuilding content'] = array(
    'name' => 'delete own sitebuilding content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own story content'.
  $permissions['delete own story content'] = array(
    'name' => 'delete own story content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own tip content'.
  $permissions['delete own tip content'] = array(
    'name' => 'delete own tip content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own trust_referral content'.
  $permissions['delete own trust_referral content'] = array(
    'name' => 'delete own trust_referral content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own video content'.
  $permissions['delete own video content'] = array(
    'name' => 'delete own video content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete payments'.
  $permissions['delete payments'] = array(
    'name' => 'delete payments',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_payment',
  );

  // Exported permission: 'delete privatemsg'.
  $permissions['delete privatemsg'] = array(
    'name' => 'delete privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'delete revisions'.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in vocabulary_1'.
  $permissions['delete terms in vocabulary_1'] = array(
    'name' => 'delete terms in vocabulary_1',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_2'.
  $permissions['delete terms in vocabulary_2'] = array(
    'name' => 'delete terms in vocabulary_2',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_3'.
  $permissions['delete terms in vocabulary_3'] = array(
    'name' => 'delete terms in vocabulary_3',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_4'.
  $permissions['delete terms in vocabulary_4'] = array(
    'name' => 'delete terms in vocabulary_4',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_5'.
  $permissions['delete terms in vocabulary_5'] = array(
    'name' => 'delete terms in vocabulary_5',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in vocabulary_6'.
  $permissions['delete terms in vocabulary_6'] = array(
    'name' => 'delete terms in vocabulary_6',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'display drupal links'.
  $permissions['display drupal links'] = array(
    'name' => 'display drupal links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'edit any block content'.
  $permissions['edit any block content'] = array(
    'name' => 'edit any block content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'email viewer' => 'email viewer',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any blog content'.
  $permissions['edit any blog content'] = array(
    'name' => 'edit any blog content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any contest_photo content'.
  $permissions['edit any contest_photo content'] = array(
    'name' => 'edit any contest_photo content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any faq content'.
  $permissions['edit any faq content'] = array(
    'name' => 'edit any faq content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any forum content'.
  $permissions['edit any forum content'] = array(
    'name' => 'edit any forum content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any page content'.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any product content'.
  $permissions['edit any product content'] = array(
    'name' => 'edit any product content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any sitebuilding content'.
  $permissions['edit any sitebuilding content'] = array(
    'name' => 'edit any sitebuilding content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'email viewer' => 'email viewer',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any story content'.
  $permissions['edit any story content'] = array(
    'name' => 'edit any story content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any tip content'.
  $permissions['edit any tip content'] = array(
    'name' => 'edit any tip content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any trust_referral content'.
  $permissions['edit any trust_referral content'] = array(
    'name' => 'edit any trust_referral content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any video content'.
  $permissions['edit any video content'] = array(
    'name' => 'edit any video content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit orders'.
  $permissions['edit orders'] = array(
    'name' => 'edit orders',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'edit own block content'.
  $permissions['edit own block content'] = array(
    'name' => 'edit own block content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own blog content'.
  $permissions['edit own blog content'] = array(
    'name' => 'edit own blog content',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(),
    'module' => 'comment',
  );

  // Exported permission: 'edit own contest_photo content'.
  $permissions['edit own contest_photo content'] = array(
    'name' => 'edit own contest_photo content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own faq content'.
  $permissions['edit own faq content'] = array(
    'name' => 'edit own faq content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own forum content'.
  $permissions['edit own forum content'] = array(
    'name' => 'edit own forum content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own page content'.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own product content'.
  $permissions['edit own product content'] = array(
    'name' => 'edit own product content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own sitebuilding content'.
  $permissions['edit own sitebuilding content'] = array(
    'name' => 'edit own sitebuilding content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own story content'.
  $permissions['edit own story content'] = array(
    'name' => 'edit own story content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own tip content'.
  $permissions['edit own tip content'] = array(
    'name' => 'edit own tip content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own trust_referral content'.
  $permissions['edit own trust_referral content'] = array(
    'name' => 'edit own trust_referral content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own video content'.
  $permissions['edit own video content'] = array(
    'name' => 'edit own video content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in vocabulary_1'.
  $permissions['edit terms in vocabulary_1'] = array(
    'name' => 'edit terms in vocabulary_1',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_2'.
  $permissions['edit terms in vocabulary_2'] = array(
    'name' => 'edit terms in vocabulary_2',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_3'.
  $permissions['edit terms in vocabulary_3'] = array(
    'name' => 'edit terms in vocabulary_3',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_4'.
  $permissions['edit terms in vocabulary_4'] = array(
    'name' => 'edit terms in vocabulary_4',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_5'.
  $permissions['edit terms in vocabulary_5'] = array(
    'name' => 'edit terms in vocabulary_5',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in vocabulary_6'.
  $permissions['edit terms in vocabulary_6'] = array(
    'name' => 'edit terms in vocabulary_6',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'execute php code'.
  $permissions['execute php code'] = array(
    'name' => 'execute php code',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'filter private messages'.
  $permissions['filter private messages'] = array(
    'name' => 'filter private messages',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg_filter',
  );

  // Exported permission: 'flush caches'.
  $permissions['flush caches'] = array(
    'name' => 'flush caches',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'full member lists'.
  $permissions['full member lists'] = array(
    'name' => 'full member lists',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'wsuser',
  );

  // Exported permission: 'generate features'.
  $permissions['generate features'] = array(
    'name' => 'generate features',
    'roles' => array(),
    'module' => 'features',
  );

  // Exported permission: 'get a system variable'.
  $permissions['get a system variable'] = array(
    'name' => 'get a system variable',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'get any binary files'.
  $permissions['get any binary files'] = array(
    'name' => 'get any binary files',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'get own binary files'.
  $permissions['get own binary files'] = array(
    'name' => 'get own binary files',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'get private messages from remote'.
  $permissions['get private messages from remote'] = array(
    'name' => 'get private messages from remote',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg_services',
  );

  // Exported permission: 'maintain nodeblock'.
  $permissions['maintain nodeblock'] = array(
    'name' => 'maintain nodeblock',
    'roles' => array(),
    'module' => 'nodeblock',
  );

  // Exported permission: 'manage features'.
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'manual payments'.
  $permissions['manual payments'] = array(
    'name' => 'manual payments',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_payment',
  );

  // Exported permission: 'notify of path changes'.
  $permissions['notify of path changes'] = array(
    'name' => 'notify of path changes',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'pathauto',
  );

  // Exported permission: 'perform unlimited index queries'.
  $permissions['perform unlimited index queries'] = array(
    'name' => 'perform unlimited index queries',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'post comments'.
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'process credit cards'.
  $permissions['process credit cards'] = array(
    'name' => 'process credit cards',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_credit',
  );

  // Exported permission: 'read all private messages'.
  $permissions['read all private messages'] = array(
    'name' => 'read all private messages',
    'roles' => array(
      'administrator' => 'administrator',
      'registrar' => 'registrar',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'read privatemsg'.
  $permissions['read privatemsg'] = array(
    'name' => 'read privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'rename features'.
  $permissions['rename features'] = array(
    'name' => 'rename features',
    'roles' => array(),
    'module' => 'features',
  );

  // Exported permission: 'reply only privatemsg'.
  $permissions['reply only privatemsg'] = array(
    'name' => 'reply only privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'revert revisions'.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'save file information'.
  $permissions['save file information'] = array(
    'name' => 'save file information',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'search content'.
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'search',
  );

  // Exported permission: 'select account cancellation method'.
  $permissions['select account cancellation method'] = array(
    'name' => 'select account cancellation method',
    'roles' => array(),
    'module' => 'user',
  );

  // Exported permission: 'select text format for privatemsg'.
  $permissions['select text format for privatemsg'] = array(
    'name' => 'select text format for privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'privatemsg',
  );

  // Exported permission: 'send link to login one time'.
  $permissions['send link to login one time'] = array(
    'name' => 'send link to login one time',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'login_one_time',
  );

  // Exported permission: 'send private messages from remote'.
  $permissions['send private messages from remote'] = array(
    'name' => 'send private messages from remote',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg_services',
  );

  // Exported permission: 'set a system variable'.
  $permissions['set a system variable'] = array(
    'name' => 'set a system variable',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'services',
  );

  // Exported permission: 'show format selection for comment'.
  $permissions['show format selection for comment'] = array(
    'name' => 'show format selection for comment',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for node'.
  $permissions['show format selection for node'] = array(
    'name' => 'show format selection for node',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for privatemsg_message'.
  $permissions['show format selection for privatemsg_message'] = array(
    'name' => 'show format selection for privatemsg_message',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for rules_config'.
  $permissions['show format selection for rules_config'] = array(
    'name' => 'show format selection for rules_config',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for taxonomy_term'.
  $permissions['show format selection for taxonomy_term'] = array(
    'name' => 'show format selection for taxonomy_term',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for uc_order'.
  $permissions['show format selection for uc_order'] = array(
    'name' => 'show format selection for uc_order',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for uc_order_product'.
  $permissions['show format selection for uc_order_product'] = array(
    'name' => 'show format selection for uc_order_product',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format selection for user'.
  $permissions['show format selection for user'] = array(
    'name' => 'show format selection for user',
    'roles' => array(),
    'module' => 'better_formats',
  );

  // Exported permission: 'show format tips'.
  $permissions['show format tips'] = array(
    'name' => 'show format tips',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'better_formats',
  );

  // Exported permission: 'show more format tips link'.
  $permissions['show more format tips link'] = array(
    'name' => 'show more format tips link',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'better_formats',
  );

  // Exported permission: 'skip comment approval'.
  $permissions['skip comment approval'] = array(
    'name' => 'skip comment approval',
    'roles' => array(
      'administrator' => 'administrator',
      'comment posting user' => 'comment posting user',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'subscribe to comments'.
  $permissions['subscribe to comments'] = array(
    'name' => 'subscribe to comments',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment_notify',
  );

  // Exported permission: 'switch users'.
  $permissions['switch users'] = array(
    'name' => 'switch users',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'tag private messages'.
  $permissions['tag private messages'] = array(
    'name' => 'tag private messages',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg_filter',
  );

  // Exported permission: 'translate admin strings'.
  $permissions['translate admin strings'] = array(
    'name' => 'translate admin strings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'i18n_string',
  );

  // Exported permission: 'translate content'.
  $permissions['translate content'] = array(
    'name' => 'translate content',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
    ),
    'module' => 'translation',
  );

  // Exported permission: 'translate interface'.
  $permissions['translate interface'] = array(
    'name' => 'translate interface',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'translate user-defined strings'.
  $permissions['translate user-defined strings'] = array(
    'name' => 'translate user-defined strings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'i18n_string',
  );

  // Exported permission: 'unconditionally delete orders'.
  $permissions['unconditionally delete orders'] = array(
    'name' => 'unconditionally delete orders',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'use PHP for title patterns'.
  $permissions['use PHP for title patterns'] = array(
    'name' => 'use PHP for title patterns',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'auto_nodetitle',
  );

  // Exported permission: 'use advanced search'.
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
    ),
    'module' => 'search',
  );

  // Exported permission: 'use ctools import'.
  $permissions['use ctools import'] = array(
    'name' => 'use ctools import',
    'roles' => array(),
    'module' => 'ctools',
  );

  // Exported permission: 'use flag import'.
  $permissions['use flag import'] = array(
    'name' => 'use flag import',
    'roles' => array(),
    'module' => 'flag',
  );

  // Exported permission: 'use link to login one time'.
  $permissions['use link to login one time'] = array(
    'name' => 'use link to login one time',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'login_one_time',
  );

  // Exported permission: 'use text format 1'.
  $permissions['use text format 1'] = array(
    'name' => 'use text format 1',
    'roles' => array(
      'Board Extra - Like board member' => 'Board Extra - Like board member',
      'Board Member' => 'Board Member',
      'Geolocation User Administrator' => 'Geolocation User Administrator',
      'Translator' => 'Translator',
      'Translator (extras)' => 'Translator (extras)',
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'comment posting user' => 'comment posting user',
      'current-member' => 'current-member',
      'developer' => 'developer',
      'donation-basic' => 'donation-basic',
      'donation-founder' => 'donation-founder',
      'donation-free' => 'donation-free',
      'donation-maintainer' => 'donation-maintainer',
      'donation-obsolete-was-founder' => 'donation-obsolete-was-founder',
      'donation-supporter' => 'donation-supporter',
      'editor' => 'editor',
      'email viewer' => 'email viewer',
      'pre-authorized' => 'pre-authorized',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 3'.
  $permissions['use text format 3'] = array(
    'name' => 'use text format 3',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
      'super-admin' => 'super-admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 4'.
  $permissions['use text format 4'] = array(
    'name' => 'use text format 4',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
      'registrar' => 'registrar',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 5'.
  $permissions['use text format 5'] = array(
    'name' => 'use text format 5',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 6'.
  $permissions['use text format 6'] = array(
    'name' => 'use text format 6',
    'roles' => array(
      'administrator' => 'administrator',
      'developer' => 'developer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format 7'.
  $permissions['use text format 7'] = array(
    'name' => 'use text format 7',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'developer' => 'developer',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use tokens in privatemsg'.
  $permissions['use tokens in privatemsg'] = array(
    'name' => 'use tokens in privatemsg',
    'roles' => array(),
    'module' => 'privatemsg',
  );

  // Exported permission: 'view all orders'.
  $permissions['view all orders'] = array(
    'name' => 'view all orders',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'view all role expirations'.
  $permissions['view all role expirations'] = array(
    'name' => 'view all role expirations',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_roles',
  );

  // Exported permission: 'view cart links report'.
  $permissions['view cart links report'] = array(
    'name' => 'view cart links report',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_cart_links',
  );

  // Exported permission: 'view cc details'.
  $permissions['view cc details'] = array(
    'name' => 'view cc details',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_credit',
  );

  // Exported permission: 'view customers'.
  $permissions['view customers'] = array(
    'name' => 'view customers',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'view faq page'.
  $permissions['view faq page'] = array(
    'name' => 'view faq page',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'faq',
  );

  // Exported permission: 'view own invoices'.
  $permissions['view own invoices'] = array(
    'name' => 'view own invoices',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'view own orders'.
  $permissions['view own orders'] = array(
    'name' => 'view own orders',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'uc_order',
  );

  // Exported permission: 'view own recurring fees'.
  $permissions['view own recurring fees'] = array(
    'name' => 'view own recurring fees',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'uc_recurring',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view payments'.
  $permissions['view payments'] = array(
    'name' => 'view payments',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uc_payment',
  );

  // Exported permission: 'view reports'.
  $permissions['view reports'] = array(
    'name' => 'view reports',
    'roles' => array(),
    'module' => 'uc_store',
  );

  // Exported permission: 'view revisions'.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'Translator' => 'Translator',
      'administrator' => 'administrator',
      'developer' => 'developer',
      'editor' => 'editor',
      'registrar' => 'registrar',
    ),
    'module' => 'system',
  );

  // Exported permission: 'write privatemsg'.
  $permissions['write privatemsg'] = array(
    'name' => 'write privatemsg',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'privatemsg',
  );

  return $permissions;
}
