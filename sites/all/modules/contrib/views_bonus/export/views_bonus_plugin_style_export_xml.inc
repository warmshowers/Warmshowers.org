<?php
// $Id: views_bonus_plugin_style_export_xml.inc,v 1.5 2009/05/11 06:58:06 neclimdul Exp $
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_export_xml extends views_bonus_plugin_style_export {
  /**
   * Initialize plugin.
   *
   * Set feed image for shared rendering later.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
    $this->feed_image = drupal_get_path('module', 'views_bonus_export') . '/images/xml.png';
  }

  /**
   * Set options fields and default values.
   *
   * @return
   * An array of options information.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['provide_file'] = array(
      'default' => TRUE,
      'translatable' => FALSE,
    );
    $options['filename'] = array(
      'default' => 'view-%view.xml',
      'translatable' => FALSE,
    );

    return $options;
  }

  /**
   * Options form mini callback.
   *
   * @param $form
   * Form array to add additional fields to.
   * @param $form_state
   * State of the form.
   * @return
   * None.
   */
  function options_form(&$form, &$form_state) {
    $form['provide_file'] = array(
      '#type' => 'checkbox',
      '#title' => t('Provide as file'),
      '#default_value' => $this->options['provide_file'],
      '#description' => t('By deselecting this, the xml file will be provided as a feed instead of a file for download.'),
      '#dependency' => array(
        'edit-style-options-override' => array(FALSE),
      ),
    );
    $form['filename'] = array(
      '#type' => 'textfield',
      '#title' => t('XML filename'),
      '#default_value' => $this->options['filename'],
      '#description' => t('The filename that will be suggested to the browser for downloading purposes. %view will be replaced with the view name.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-style-options-provide-file' => array(TRUE),
      ),
    );
  }
}
