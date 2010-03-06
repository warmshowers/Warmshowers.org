<?php
// $Id: views_bonus_plugin_style_export_xls.inc,v 1.1 2010/01/07 22:56:47 neclimdul Exp $
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_export_xls extends views_bonus_plugin_style_export {
  var $feed_text = 'XLS';
  var $feed_file = 'view-%view.xls';

  /**
   * Initialize plugin.
   *
   * Set feed image for shared rendering later.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
    $this->feed_image = drupal_get_path('module', 'views_bonus_export') . '/images/xls.png';
  }
}
