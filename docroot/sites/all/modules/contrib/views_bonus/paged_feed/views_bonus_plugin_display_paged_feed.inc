<?php
// $Id: views_bonus_plugin_display_paged_feed.inc,v 1.2 2010/01/07 21:58:40 neclimdul Exp $

/**
 * @file
 * Contains the pager feed display plugin.
 */

/**
 * The plugin that handles a feed, such as RSS or atom and supports paging through data.
 *
 * @see views_plugin_display_feed
 *
 * @ingroup views_display_plugins
 */
class views_bonus_plugin_display_paged_feed extends views_plugin_display_feed {
  function get_style_type() { return 'feed'; }

  // We actually don't need to do a thing here because we want to behave
  // exactly like the core feed plugin. We just want a different definition so
  // pager plugins can interact with the feed query.
}
