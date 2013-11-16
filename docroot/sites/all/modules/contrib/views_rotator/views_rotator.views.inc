<?php
// $Id: views_rotator.views.inc,v 1.4 2008/09/23 02:28:12 mfer Exp $
/**
 * @file
 * Provide a rotating item display style for Views. This file is autoloaded by views.
 */

/**
  * Implementation of hook_views_plugin().
  */
function views_rotator_views_plugins() {
  return array(
    'style' => array(
      'views_rotator' => array(
        'title' => t('Views Rotator'),
        'theme' => 'views_view_rotator',
        'help' => t('Display one node at a time while rotating through them.'),
        'handler' => 'views_rotator_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

