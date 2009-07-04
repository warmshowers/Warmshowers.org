<?php
// $Id$
// require_once for the functions that need to be available when we are outside
// of the omega theme in the administrative interface
require_once 'theme-functions.inc';
/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function omega_settings($saved_settings) {
	//krumo($saved_settings);
	// add in custom CSS & jQuery for the sliders to be filled with awesomeness and #moonfruit
	//drupal_add_css(drupal_get_path('theme', 'omega'). '/css/ui.slider.extras.css', 'module', 'all', TRUE);
	//drupal_add_css(drupal_get_path('theme', 'omega'). '/css/redmond/jquery-ui-1.7.1.custom.css', 'module', 'all', TRUE);
	//drupal_add_js(drupal_get_path('theme', 'omega'). '/js/jquery-1.3.2.min.js', 'module');
	//drupal_add_js(drupal_get_path('theme', 'omega'). '/js/jquery-ui-1.7.1.custom.min.js', 'module');
	//drupal_add_js(drupal_get_path('theme', 'omega'). '/js/selectToUISlider.jQuery.js', 'module');
	drupal_add_js(drupal_get_path('theme', 'omega'). '/js/omega_admin.js', 'module');
	for($i=1;$i<=16;$i++){
		$grids[$i]= $i;
	}
  $form['omega_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Omega 960 settings'),
    '#description' => t('Core configuration options for the Omega theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => false,
  );
	  // General Settings
	  $form['omega_container']['omega_general'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('General Settings'),
	    '#description' => t('Configure generic options on rendering content in this theme.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => FALSE,
	  );
	    $form['omega_container']['omega_general']['omega_jqueryui'] = array(
          '#type'          => 'radios',
	        '#description'   => t('The Omega theme provides jQueryUI functionality. You will need to turn this off if you are using the jQuery UI module.'),
          '#title'         => t('Include jQuery UI?'),
          '#default_value' => ovars($saved_settings['omega_jqueryui'], 1),
          '#options'       => array(
	                             t('Do NOT include jQueryUI'),
	                             t('DO include jQueryUI'),
                              ),
        );
		  // Mission Statement
		  $form['omega_container']['omega_general']['mission_statement'] = array(
		    '#type' => 'fieldset',
		    '#title' => t('Mission statement'),
		    '#collapsible' => TRUE,
		    '#collapsed' => TRUE,
		  );
			  $form['omega_container']['omega_general']['mission_statement']['mission_statement_pages'] = array(
			    '#type'          => 'radios',
			    '#title'         => t('Where should your mission statement be displayed?'),
			    '#default_value' => ovars($saved_settings['mission_statement_pages'], 'home'),
			    '#options'       => array(
			                          'home' => t('Display mission statement only on front page'),
			                          'all' => t('Display mission statement on all pages'),
			                        ),
			  );
		  // Breadcrumb
		  $form['omega_container']['omega_general']['breadcrumb'] = array(
		    '#type' => 'fieldset',
		    '#title' => t('Breadcrumb'),
		    '#collapsible' => TRUE,
		    '#collapsed' => TRUE,
		  );
			  $form['omega_container']['omega_general']['breadcrumb']['breadcrumb_display'] = array(
			    '#type' => 'checkbox',
			    '#title' => t('Display breadcrumb'),
			    '#default_value' => ovars($saved_settings['breadcrumb_display'], 1),
			  );
	  // Region Settings
	  $form['omega_container']['omega_regions'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Region Settings'),
	    '#description' => t('Configure how your regions are rendered.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => false,
	  );
		  $form['omega_container']['omega_regions']['headers'] = array(
		    '#type' => 'fieldset',
		    '#title' => t('Header Configuration'),
		    '#description' => t('Header region zones, including Primary & Secondary menus, Header first and Header Last.'),
		    '#collapsible' => TRUE,
		    '#collapsed' => false,
		  );
        $form['omega_container']['omega_regions']['headers']['omega_header_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Header First'),
          '#default_value' => ovars($saved_settings['omega_header_first_width'], 6),
          '#options' => $grids,
          '#description' => t('This number, paired with the Header Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Header First'),
          '#default_value' => ovars($saved_settings['omega_header_last_width'], 6),
          '#options' => $grids,
          '#description' => t('This number, paired with the Header First determine the share of your grid for each element.'),
        );
  // Return theme settings form
  return $form;
}  