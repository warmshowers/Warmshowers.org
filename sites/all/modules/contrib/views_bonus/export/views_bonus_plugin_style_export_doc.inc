<?php
// $Id: views_bonus_plugin_style_export_doc.inc,v 1.6 2009/07/07 07:59:40 neclimdul Exp $
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_export_doc extends views_bonus_plugin_style_export {
  var $feed_text = 'Word Document';
  var $feed_file = 'view-%view.doc';

  /**
   * Initialize plugin.
   *
   * Set feed image for shared rendering later.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
    $this->feed_image = drupal_get_path('module', 'views_bonus_export') . '/images/doc.png';
  }
}
