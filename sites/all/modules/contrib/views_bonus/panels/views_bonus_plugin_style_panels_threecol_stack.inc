<?php
// $Id: views_bonus_plugin_style_panels_threecol_stack.inc,v 1.3 2009/01/03 04:50:45 neclimdul Exp $
/**
 * @file
 * Plugin include file for a 3 column stacked style plugin.
 */

/**
 * Three column layout rendered by panels.
 *
 * @todo document
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_panels_threecol_stack extends views_plugin_style {
  /**
   * Set default options
   */
  function options(&$options) {
    parent::options($options);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }
}
