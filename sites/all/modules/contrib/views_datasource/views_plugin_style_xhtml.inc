<?php
// $Id: views_plugin_style_xhtml.inc,v 1.1.2.10 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Implementation of views_plugin_style for views_xhtml
 */


/**
 * Implementation of views_plugin_style
 */
class views_plugin_style_xhtml extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'hcard', 'translatable' => FALSE);
    $options['field_output'] = array('default' => 'normal', 'translatable' => FALSE);
    $options['plaintext_output'] = array('default' => TRUE, 'translatable' => FALSE);
    /*$options['skip_empty_fields'] = array('default' => TRUE, 'translatable' => FALSE);*/
    $options['escape_as_CDATA'] = array('default' => 'no', 'translatable' => FALSE);
    $options['content_type'] = array('default' => 'text/xhtml', 'translatable' => FALSE);
    $options['header'] = array('default' => NULL, 'translatable' => FALSE);
    $options['author'] = array('default' => $user->name, 'translatable' => FALSE);
    $options['using_views_api_mode'] = array('default' => FALSE, 'translatable' => FALSE);
    //$options['action_on_missing_field'] = array('default' => 'none', 'translatable' => FALSE);
    return $options;
  }

  /**
   * Provide a form for setting options.
   *
   * @param $form
   * @param $form_state
   */
  function options_form(&$form, &$form_state) {
    $form['format'] = array(
      '#type' => 'radios',
      '#title' => ('Microformat'),
      '#options' => array('hcard' => ('hCard'), 'hcalendar' => ('hCalendar')),
      '#default_value' => $this->options['format'],
    );
   $form['field_output']  = array(
      '#type' => 'radios',
      '#title' => 'Field output',
      '#description' => 'For each row in the view, fields can be output as either the field rendered by Views, or by the raw content of the field.',  
      '#options' => array('normal' => ('Normal'), 'raw' => ('Raw')),
      '#default_value' => $this->options['field_output'],
    );
    $form['plaintext_output'] = array(
      '#type'           => 'checkbox',
      '#title'          => ('Plaintext output'),
      '#default_value'  => $this->options['plaintext_output'],
      '#description'    => ('For each row in the view, strip all markup from the field output.'),
    );
    /*
    $form['skip_empty_fields'] = array(
      '#type'           => 'checkbox',
      '#title'          => ('Skip adding fields that are empty.'),
      '#default_value'  => $this->options['skip_empty_fields'],
      '#description'    => ('For each row in the view, skip adding a field if the field value is NULL or blank.'),
    );*/    
    $form['escape_as_CDATA']  = array(
      '#type' => 'radios',
      '#title' => 'Escape row content as CDATA',
      '#description' => 'For each row in the view, escape the row field content as XML CDATA sections.',
      '#options' => array('yes' => ('Yes'), 'no' => ('No')),
      '#default_value' => $this->options['escape_as_CDATA'],
      //'#process' => array('views_process_dependency'),
      //'#dependency' => array('edit-style-options-element-output-options' => array('nested')),
    );
    $form['header'] = array (
      '#type' => 'textarea',
      '#title' => 'XML document header',
      '#description' => 'You can use this to place any additional lines you want after the document declaration (like !DOCTYPE.)',
      '#default_value' => $this->options['header'], 
      );
    $form['content_type'] = array(
      '#type'           => 'radios',
      '#title'          => ('Content-Type'),
      '#options'        => array(
         'default'      => ("Default: text/xhtml"),  
         'text/xml'     => 'text/xml',
         'text/html'    => 'text/html',
       ),
      '#default_value'  => $this->options['content_type'],
      '#description'    => ('The Content-Type header that will be sent with the XHTML output.')
    );
    $form['author'] = array (
      '#type' => 'textfield',
      '#title' => 'View author',
      '#description' => 'The Drupal user name of the person who authored this view.',
      '#default_value' => $this->options['author'], 
      );     
    $form['using_views_api_mode'] = array(
      '#type'           => 'checkbox',
      '#title'          => ('Views API mode'),
      '#default_value'  => $this->options['using_views_api_mode'],
      '#description'    => ('Not using View API mode means the JSON gets output directly and the server ceases normal page processing.  Using it means the server does not cease processing after outputting the JSON.  This allows the Views API to be used with the view without having to prematurely terminate page processing.'),
    );    
  }

  /**
   * Implementation of views_style_plugin::theme_functions(). Returns an array of theme functions to use.
   * for the current style plugin
   * @return array 
   */
  function theme_functions() {
    $options = $this->options;
    if ($options['format'] == 'hcard') $hook = 'views_views_xhtml_style_hcard';
	  if ($options['format'] == 'hcalendar') $hook = 'views_views_xhtml_style_hcalendar'; 
  	return views_theme_functions($hook, $this->view, $this->display);
  }

/**
 * Implementation of views_style_plugin::additional_theme_functions(). Returns empty array.
 * @return array
 */  
  function additional_theme_functions() {
  	return array();
  }
    
  /**
   * Implementation of view_style_plugin::render()
   */
  function render() {
  	$view = $this->view;
  	$options = $this->options;
  	$field = $view->field;

 	  $rows = array();
 	 
    foreach ($view->result as $row) {
      $rows[] = _views_xhtml_render_fields($view, $row);
    }
    
 	  return theme($this->theme_functions(), $this->view, $this->options, $rows); 	  
 	}  
}
