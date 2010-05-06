<?php
//$Id: views_plugin_style_json.inc,v 1.1.2.2 2009/10/07 15:27:38 allisterbeharry Exp $
/**
 * @file
 * Implementation of views_plugin_style for views_json
 */

/**
 * Implementation of views_plugin_style
 *
 */
class views_plugin_style_json extends views_plugin_style {
  
  /**
   * Set default options
   */
  function options(&$options) {
    $options['format'] = 'Simple';
  }
  
  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */  
  function options_form(&$form, &$form_state) {
    $form['format'] = array(
      '#type' => 'radios',
      '#title' => t('JSON data format'),
      '#options' => array('Simple' => t('Simple'), 'Exhibit' => t('MIT Simile/Exhibit')),
      '#default_value' => $this->options['format'],
    );
  }

}