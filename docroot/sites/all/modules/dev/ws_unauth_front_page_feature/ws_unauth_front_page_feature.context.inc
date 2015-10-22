<?php
/**
 * @file
 * ws_unauth_front_page_feature.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function ws_unauth_front_page_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'redesign_unauthenticated_front';
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
        'warmshowers_site-ws_anonymous_welcome_message' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_anonymous_welcome_message',
          'region' => 'highlighted',
          'weight' => '-73',
        ),
        'warmshowers_site-ws_member_stats_block' => array(
          'module' => 'warmshowers_site',
          'delta' => 'ws_member_stats_block',
          'region' => 'highlighted',
          'weight' => '-72',
        ),
        'nodeblock-15521' => array(
          'module' => 'nodeblock',
          'delta' => '15521',
          'region' => 'highlighted',
          'weight' => '-10',
        ),
        'views-photo_contest-block_1' => array(
          'module' => 'views',
          'delta' => 'photo_contest-block_1',
          'region' => 'content',
          'weight' => '-10',
        ),
        'nodeblock-15526' => array(
          'module' => 'nodeblock',
          'delta' => '15526',
          'region' => 'content',
          'weight' => '-8',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('all');
  t('front page for unauthenticated users');
  $export['redesign_unauthenticated_front'] = $context;

  return $export;
}
