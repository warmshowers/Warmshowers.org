<?php
/**
 * @file
 * random_member_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function random_member_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'random_member';
  $view->description = 'A view to randomly select individual members.';
  $view->tag = '';
  $view->base_table = 'users';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '300';
  $handler->display->display_options['cache']['output_lifespan'] = '300';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = 1;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Picture */
  $handler->display->display_options['fields']['picture']['id'] = 'picture';
  $handler->display->display_options['fields']['picture']['table'] = 'users';
  $handler->display->display_options['fields']['picture']['field'] = 'picture';
  $handler->display->display_options['fields']['picture']['exclude'] = TRUE;
  $handler->display->display_options['fields']['picture']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['picture']['imagecache_preset'] = 'random_member_photo';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  /* Field: Warmshowers: Full Name */
  $handler->display->display_options['fields']['fullname']['id'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['table'] = 'wsuser';
  $handler->display->display_options['fields']['fullname']['field'] = 'fullname';
  $handler->display->display_options['fields']['fullname']['exclude'] = TRUE;
  /* Field: Warmshowers: City */
  $handler->display->display_options['fields']['city']['id'] = 'city';
  $handler->display->display_options['fields']['city']['table'] = 'user_location';
  $handler->display->display_options['fields']['city']['field'] = 'city';
  $handler->display->display_options['fields']['city']['exclude'] = TRUE;
  /* Field: Warmshowers: Country */
  $handler->display->display_options['fields']['country']['id'] = 'country';
  $handler->display->display_options['fields']['country']['table'] = 'user_location';
  $handler->display->display_options['fields']['country']['field'] = 'country';
  $handler->display->display_options['fields']['country']['exclude'] = TRUE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<div class="random-member-photo">
[picture]
<a class="random-member-link" href="/user/[uid]">[fullname] - [city], [country]</a>
</div>';
  $handler->display->display_options['fields']['nothing']['hide_alter_empty'] = TRUE;
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = array(
    9 => '9',
  );
  $handler->display->display_options['filters']['rid']['group'] = '0';
  $handler->display->display_options['filters']['rid']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => 0,
  );
  $handler->display->display_options['filters']['uid']['group'] = '0';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User: Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: User: Picture */
  $handler->display->display_options['filters']['picture']['id'] = 'picture';
  $handler->display->display_options['filters']['picture']['table'] = 'users';
  $handler->display->display_options['filters']['picture']['field'] = 'picture';
  $handler->display->display_options['filters']['picture']['value'] = '1';
  $handler->display->display_options['filters']['picture']['group'] = '0';
  $handler->display->display_options['filters']['picture']['expose']['operator'] = FALSE;

  /* Display: Random member block */
  $handler = $view->new_display('block', 'Random member block', 'random_block');
  $handler->display->display_options['display_description'] = 'A small block to display a single member at random.';
  $handler->display->display_options['block_caching'] = '8';
  $translatables['random_member'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Picture'),
    t('Name'),
    t('Full Name'),
    t('City'),
    t('Country'),
    t('Uid'),
    t('<div class="random-member-photo">
[picture]
<a class="random-member-link" href="/user/[uid]">[fullname] - [city], [country]</a>
</div>'),
    t('User: Name'),
    t('Random member block'),
    t('A small block to display a single member at random.'),
  );
  $export['random_member'] = $view;

  return $export;
}
