<?php
// $Id: views_bonus_plugin_style_panels_twocol.inc,v 1.3 2009/01/03 04:50:45 neclimdul Exp $
/**
 * @file
 * Plugin include file for a 2 column style plugin.
 */

/**
 * Two column panels rendered output.
 *
 * @todo document
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_panels_twocol extends views_plugin_style {
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
