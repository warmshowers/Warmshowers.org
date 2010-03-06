<?php
// $Id: views_bonus_spy.views.inc,v 1.1 2009/02/16 15:20:38 manuelgarcia Exp $
/**
 * @file
 * Provide an spy style plugin for Views. This file is autoloaded by views.
 */

/**
  * Implementation of hook_views_plugin().
  */
function views_bonus_spy_views_plugins() {
  return array(
    'style' => array(
      'views_spy' => array(
        'title' => t('Views Spy'),
        'theme' => 'views_bonus_spy',
        'help' => t('Display a JQuery spy with the results.'),
        'handler' => 'views_bonus_plugin_style_spy',
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

