<?php
// $Id: views_bonus_paged_feed.views.inc,v 1.1 2010/01/07 21:19:05 neclimdul Exp $

/**
 * @file
 * Views API hooks for views_bonus_paged_feed.
 */

/**
 * Implementation of hook_views_plugins
 */
function views_bonus_paged_feed_views_plugins() {

  return array(
    'display' => array(
      'paged_feed' => array(
        'title' => t('Paged Feed'),
        'help' => t('Display the view as a feed, such as an RSS feed.'),
        'handler' => 'views_bonus_plugin_display_paged_feed',
        'parent' => 'feed', // so it knows to load the page plugin .inc file
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => TRUE,
        'accept attachments' => FALSE,
        'admin' => t('Paged Feed'),
        // @TODO - Should probably provide some help.
        // 'help topic' => 'display-feed',
      ),
    ),
  );

  return $plugins;
}
