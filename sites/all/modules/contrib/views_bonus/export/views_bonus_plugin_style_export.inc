<?php
// $Id: views_bonus_plugin_style_export.inc,v 1.6 2009/05/05 19:47:09 neclimdul Exp $
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_export extends views_plugin_style {

  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    $url_options = array('html' => true);
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }
    $image = theme('image', $this->feed_image);
    $this->view->feed_icon .= l($image, $this->view->get_url(NULL, $path), $url_options);
  }
}
