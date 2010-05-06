<?php
//$Id: views_plugin_style_xhtml.inc,v 1.1.2.2 2009/10/07 15:27:38 allisterbeharry Exp $
/**
 * @file
 * Implementation of views_plugin_style for views_xhtml
 */

/**
 * Implementation of views_plugin_style
 *
 */
class views_plugin_style_xhtml extends views_plugin_style {
  /**
   * Set default options
   */
  function options(&$options) {
    $options['format'] = 'hcard';
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
      '#title' => t('Microformat'),
      '#options' => array('hcard' => t('hCard'), 'hcalendar' => t('hCalendar')),
      '#default_value' => $this->options['format'],
    );
  }

}