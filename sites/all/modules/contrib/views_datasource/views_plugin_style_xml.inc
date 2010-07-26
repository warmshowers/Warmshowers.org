<?php
// $Id: views_plugin_style_xml.inc,v 1.1.2.15 2010/07/15 07:34:48 allisterbeharry Exp $

/**
 * @file
 * Implementation of views_plugin_style for views_xml
 */


/**
 * Implementation of views_plugin_style
 */
class views_plugin_style_xml extends views_plugin_style {

  /** 
   * Implementation of views_plugin_style::option_definition 
   */ 
	function option_definition() {
		global $user;
    $options = parent::option_definition();
    $options['schema'] = array('default' => 'raw', 'translatable' => FALSE);
    $options['root_element'] = array('default' => 'node', 'translatable' => FALSE);
    $options['top_child_object'] = array('default' => 'node', 'translatable' => FALSE);
    $options['field_output'] = array('default' => 'normal', 'translatable' => FALSE);
    $options['plaintext_output'] = array('default' => TRUE, 'translatable' => FALSE);
    /*$options['skip_empty_fields'] = array('default' => TRUE, 'translatable' => FALSE);*/
    $options['element_output'] = array('default' => 'nested', 'translatable' => FALSE);
    $options['escape_as_CDATA'] = array('default' => 'no', 'translatable' => FALSE);
    $options['content_type'] = array('default' => 'text/xml', 'translatable' => FALSE);
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
  	$form['schema'] = array(
      '#type' => 'radios',
      '#title' => ('XML schema'),
      '#description' => ('The schema or format of the XML data document.'),    
      '#options' => array('raw' => ('Raw XML'), 'opml' => ('OPML'), 'atom' => ('Atom')),
      '#default_value' => $this->options['schema'],
    );
    $form['root_element'] = array(
      '#type' => 'textfield',
      '#title' => ('Root element name'),
      '#default_value' => $this->options['root_element'],
      '#description' => ('The name of the root element in a raw XML document.'),
      //'#process' => array('views_process_dependency'),
      //'#dependency' => array('edit-style-options-schema' => array('raw')),  
    );
  	$form['top_child_object'] = array(
    '#type' => 'textfield',
    '#title' => ('Top-level child element name'),
    '#default_value' => $this->options['top_child_object'],
    '#description' => ('The name of each top-level child object in a raw XML document. e.g node or user or forum_post'),
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
    $form['element_output']  = array(
      '#type' => 'radios',
      '#title' => 'Element output',
      '#description' => 'For each row in the view, fields can be output as either nested XML child elements, or attributes of the raw XML element.',
      '#options' => array('nested' => ('Nested'), 'attributes' => ('Attributes')),
      '#default_value' => $this->options['element_output'],
    );
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
         'default'      => ("Default: text/xml for raw documents, text/html for OPML, application/atom+xml for Atom."),  
         'text/xml'     => 'text/xml',
         'text/html'    => 'text/html',
         'text/x-opml'  => 'text/x-opml'
         ),
      '#default_value'  => $this->options['content_type'],
      '#description'    => ('The Content-Type header that will be sent with the XML output.')
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
      '#description'    => ('Not using View API mode means the XML gets output directly and the server ceases normal page processing.  Using it means the server does not cease processing after outputting the JSON.  This allows the Views API to be used with the view without having to prematurely terminate page processing.'),
    );
    /*
    $form['action_on_missing_field'] = array(
      '#type'           => 'radios',
      '#title'          => ('Action on missing field'),
      '#options' => array('none' => ('Take no action if a required field is missing.'), 'warn' => ('Warn about the missing fields in preview mode.'), 'stop'=>t('Stop page processing and send an error message if a required field is missing.')),
      '#default_value'  => $this->options['action_on_missing_field'],
      '#description'    => ('Action to take if no field can be mapped to a required element or schema for the selected XML schema.'),
    );*/      
    
  } 
 	
  /**
   * Implementation of views_style_plugin::theme_functions(). Returns an array of theme functions to use.
   * for the current style plugin
   * @return array 
   */
  function theme_functions() {
    $options = $this->options;
    if ($options['schema'] == 'raw') $hook = 'views_views_xml_style_raw';
	  if ($options['schema'] == 'opml') $hook = 'views_views_xml_style_opml';
	  if ($options['schema'] == 'atom') $hook = 'views_views_xml_style_atom'; 
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
    	$rows[] = _views_xml_render_fields($view, $row);        
    }
    
    return theme($this->theme_functions(), $this->view, $this->options, $rows);    	  
 	}
 
}