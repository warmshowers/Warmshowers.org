<?php
/**
 * @file
 * d7_theme_upgrade.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function d7_theme_upgrade_context_default_contexts() {
  $export = array();

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
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Everywhere items for all languages (Language Switcher)');
  t('all languages');
  $export['everywhere'] = $context;

  return $export;
}
