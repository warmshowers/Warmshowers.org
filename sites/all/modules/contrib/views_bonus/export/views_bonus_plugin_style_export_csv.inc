<?php
// $Id: views_bonus_plugin_style_export_csv.inc,v 1.10 2010/06/16 14:47:51 neclimdul Exp $
/**
 * @file
 * Plugin include file for export style plugin.
 */

/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class views_bonus_plugin_style_export_csv extends views_bonus_plugin_style_export {
  var $feed_text = 'CSV';
  var $feed_file = 'view-%view.csv';

  /**
   * Initialize plugin.
   *
   * Set feed image for shared rendering later.
   */
  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options = NULL);
    $this->feed_image = drupal_get_path('module', 'views_bonus_export') . '/images/csv.png';
    $this->feed_file = 'view-%view.csv';
    $this->feed_text = 'CSV';
  }

  /**
   * Set options fields and default values.
   *
   * @return
   * An array of options information.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['quote'] = array(
      'default' => TRUE,
      'translatable' => TRUE,
    );
    $options['seperator'] = array(
      'default' => ',',
      'translatable' => TRUE,
    );
    $options['header'] = array(
      'default' => TRUE,
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
    parent::options_form($form, $form_state);

    $form['seperator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => !empty($this->options['seperator']) ? $this->options['seperator'] : ',',
      '#description' => t('This is the seperator that is used to seperate fields. CSV implies comma seperated fields so this should not be changed unless you have specific requirements'),
    );
    $form['quote'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['quote']),
      '#title' => t('Quote values. Useful for output that might contain your seperator as part of one of the values.'),
    );
    $form['trim'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['trim']),
      '#title' => t('Trim whitespace from rendered fields. Can be useful for some themes where output results in extra newlines.'),
    );
    $form['header'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make first row a list of column headers.'),
      '#default_value' => !empty($this->options['header']),
    );
  }
}
