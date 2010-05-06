<?php
//$Id: views_plugin_style_xml.inc,v 1.1.2.2 2009/10/07 15:27:38 allisterbeharry Exp $
/**
 * @file
 * Implementation of views_plugin_style for views_xml
 */


/**
 * Implementation of views_plugin_style
 *
 */
class views_plugin_style_xml extends views_plugin_style {
  
  /**
   * Set default options
   */
  function options(&$options) {
    $options['schema'] = 'raw';
  }
  
  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */  
  function options_form(&$form, &$form_state) {
    $form['schema'] = array(
      '#type' => 'radios',
      '#title' => t('XML schema'),
      '#options' => array('raw' => t('Raw XML'), 'opml' => t('OPML'), 'atom' => t('Atom')),
      '#default_value' => $this->options['schema'],
    );
  }
  
}