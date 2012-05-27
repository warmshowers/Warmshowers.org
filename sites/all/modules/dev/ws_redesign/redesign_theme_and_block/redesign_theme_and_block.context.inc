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
  $context->tag = '';
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
        'wsuser-wsuser_search_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_search_block',
          'region' => 'sidebar_first',
          'weight' => 0,
        ),
        'wsuser-wsuser_member_block' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_block',
          'region' => 'highlight',
          'weight' => 0,
        ),
        'wsuser-wsuser_member_recommendation' => array(
          'module' => 'wsuser',
          'delta' => 'wsuser_member_recommendation',
          'region' => 'highlight',
          'weight' => 1,
        ),
        'follow-site' => array(
          'module' => 'follow',
          'delta' => 'site',
          'region' => 'highlight',
          'weight' => 2,
        ),
        'views-recent_forum_topics-block_1' => array(
          'module' => 'views',
          'delta' => 'recent_forum_topics-block_1',
          'region' => 'highlight',
          'weight' => 3,
        ),
        'wsmap-0' => array(
          'module' => 'wsmap',
          'delta' => 0,
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
  $export['authenticated_dashboard'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'everywhere';
  $context->description = '';
  $context->tag = '';
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
  $export['everywhere'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'unauthenticated_front';
  $context->description = 'front page for unauthenticated users';
  $context->tag = '';
  $context->conditions = array(
    'language' => array(
      'values' => array(
        'en-working' => 'en-working',
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
        'views-tutorial_videos-block_1' => array(
          'module' => 'views',
          'delta' => 'tutorial_videos-block_1',
          'region' => 'content',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('front page for unauthenticated users');
  $export['unauthenticated_front'] = $context;

  return $export;
}
