<?php
//$Id: views_plugin_style_rdf.inc,v 1.1.2.3 2009/10/07 15:27:38 allisterbeharry Exp $
/**
 * @file
 * Implementation of views_plugin_style for views_rdf
 */

/**
 * Implementation of views_plugin_style
 *
 */
class views_plugin_style_rdf extends views_plugin_style {
  
  /**
   * Set default options
   */
  function options(&$options) {
    $options['vocabulary'] = 'FOAF';
  }
  
  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */  
  function options_form(&$form, &$form_state) {
    $form['vocabulary'] = array(
      '#type' => 'radios',
      '#title' => t('RDF vocabulary'),
      '#options' => array('FOAF' => t('FOAF'), 'SIOC' => t('SIOC')),
      '#default_value' => $this->options['vocabulary'],
    );
  }

}