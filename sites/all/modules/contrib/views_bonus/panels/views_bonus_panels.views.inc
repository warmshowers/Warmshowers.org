<?php
// $Id: views_bonus_panels.views.inc,v 1.4 2009/06/30 19:22:24 neclimdul Exp $
/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implementation of hook_views_plugins().
 */
function views_bonus_panels_views_plugins() {
  return array(
    'style' => array(
      'panels_threecol' => array(
        'title' => t('Panels: 3 columns'),
        'help' => t('Display views items devided into 3 columns.'),
        'handler' => 'views_bonus_plugin_style_panels_threecol',
        'theme' => 'views_bonus_panels_render',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
      'panels_threecol_stack' => array(
        'title' => t('Panels: 1 top + 3 columns'),
        'help' => t('Display views items devided into 3 columns with the first item above.'),
        'handler' => 'views_bonus_plugin_style_panels_threecol_stack',
        'theme' => 'views_bonus_panels_render',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
      'panels_twocol' => array(
        'title' => t('Panels: 2 columns'),
        'help' => t('Display views items devided into 2 columns.'),
        'handler' => 'views_bonus_plugin_style_panels_twocol',
        'theme' => 'views_bonus_panels_render',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
      'panels_twocol_stack' => array(
        'title' => t('Panels: 1 top + 2 columns'),
        'help' => t('Display views items devided into 2 columns with the first item above.'),
        'handler' => 'views_bonus_plugin_style_panels_twocol_stack',
        'theme' => 'views_bonus_panels_render',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
      'panels_threecol_term' => array(
        'title' => t('Panels: Teasers, By Term, 3 columns'),
        'help' => t('Not implemented.'),
        'handler' => 'views_bonus_plugin_style_panels_threecol_term',
        'theme' => 'views_bonus_panels_render',
        'uses row plugin' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
