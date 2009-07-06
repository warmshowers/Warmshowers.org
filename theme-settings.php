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
	    '#collapsed' => TRUE,
	  );
		// Page titles
		  $form['omega_container']['omega_general']['page_format_titles'] = array(
		    '#type' => 'fieldset',
		    '#title' => t('Page titles'),
		    '#description'   => t('This is the title that displays in the title bar of your web browser. Your site title, slogan, and mission can all be set on your Site Information page. [NOTE: For more advanced page title functionality, consider using the "Page Title" module.  However, the Page titles theme settings do not work in combination with the "Page Title" module and will be disabled if you have it enabled.]'),
		    '#collapsible' => TRUE,
		    '#collapsed' => TRUE,
		  );
		  if (module_exists('page_title') == FALSE) {
		    // front page title
		    $form['omega_container']['omega_general']['page_format_titles']['front_page_format_titles'] = array(
		      '#type' => 'fieldset',
		      '#title' => t('Front page title'),
		      '#description'   => t('Your front page in particular should have important keywords for your site in the page title'),
		      '#collapsible' => TRUE,
		      '#collapsed' => TRUE,
		    );
		    $form['omega_container']['omega_general']['page_format_titles']['front_page_format_titles']['front_page_title_display'] = array(
		      '#type' => 'select',
		      '#title' => t('Set text of front page title'),
		      '#collapsible' => TRUE,
		      '#collapsed' => FALSE,
		      '#default_value' => ovars($saved_settings['front_page_title_display'], ''),
		      '#options' => array(
		                      'title_slogan' => t('Site title | Site slogan'),
		                      'slogan_title' => t('Site slogan | Site title'),
		                      'title_mission' => t('Site title | Site mission'),
		                      'custom' => t('Custom (below)'),
		                    ),
		    );
		    $form['omega_container']['omega_general']['page_format_titles']['front_page_format_titles']['page_title_display_custom'] = array(
		      '#type' => 'textfield',
		      '#title' => t('Custom'),
		      '#size' => 60,
		      '#default_value' => ovars($saved_settings['page_title_display_custom'], ''),
		      '#description'   => t('Enter a custom page title for your front page'),
		    );
		    // other pages title
		    $form['omega_container']['omega_general']['page_format_titles']['other_page_format_titles'] = array(
		      '#type' => 'fieldset',
		      '#title' => t('Other page titles'),
		      '#collapsible' => TRUE,
		      '#collapsed' => TRUE,
		    );
		    $form['omega_container']['omega_general']['page_format_titles']['other_page_format_titles']['other_page_title_display'] = array(
		      '#type' => 'select',
		      '#title' => t('Set text of other page titles'),
		      '#collapsible' => TRUE,
		      '#collapsed' => FALSE,
		      '#default_value' => ovars($saved_settings['other_page_title_display'], 'ptitle_slogan'),
		      '#options' => array(
		                      'ptitle_slogan' => t('Page title | Site slogan'),
		                      'ptitle_stitle' => t('Page title | Site title'),
		                      'ptitle_smission' => t('Page title | Site mission'),
		                      'ptitle_custom' => t('Page title | Custom (below)'),
		                      'custom' => t('Custom (below)'),
		                    ),
		    );
		    $form['omega_container']['omega_general']['page_format_titles']['other_page_format_titles']['other_page_title_display_custom'] = array(
		      '#type' => 'textfield',
		      '#title' => t('Custom'),
		      '#size' => 60,
		      '#default_value' => ovars($saved_settings['other_page_title_display_custom'], ''),
		      '#description'   => t('Enter a custom page title for all other pages'),
		    );
		    // SEO configurable separator
		    $form['omega_container']['omega_general']['page_format_titles']['configurable_separator'] = array(
		      '#type' => 'textfield',
		      '#title' => t('Title separator'),
		      '#description' => t('Customize the separator character used in the page title'),
		      '#size' => 60,
		      '#default_value' => ovars($saved_settings['configurable_separator'], ' | '),
		    );
		  } else {
		      $form['omega_container']['omega_general']['page_format_titles']['#description'] = 'NOTICE: You currently have the "Page Title" module installed and enabled, so the Page titles theme settings have been disabled to prevent conflicts.  If you wish to re-enable the Page titles theme settings, you must first disable the "Page Title" module.';
		      $form['omega_container']['omega_general']['page_format_titles']['configurable_separator']['#disabled'] = 'disabled';
		  }
	  
	  
	  
	  
	  
	   $form['omega_container']['omega_general']['jquery'] = array(
        '#type' => 'fieldset',
        '#title' => t('jQuery Configuration'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
	    $form['omega_container']['omega_general']['jquery']['omega_jqueryui'] = array(
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
	  // Header BLocks
		  $form['omega_container']['omega_regions']['headers'] = array(
        '#type' => 'fieldset',
        '#title' => t('Header Configuration'),
        '#description' => t('Header region zones, including Primary & Secondary menus, Header first and Header Last.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
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
      // Preface Blocks
      $form['omega_container']['omega_regions']['preface'] = array(
        '#type' => 'fieldset',
        '#title' => t('Preface Configuration'),
        '#description' => t('Preface region zones.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['preface']['omega_preface_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface First'),
          '#default_value' => ovars($saved_settings['omega_preface_first_width'], 6),
          '#options' => $grids,
          '#description' => t('This number, combined with the Preface Middle and Preface Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_middle_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Middle'),
          '#default_value' => ovars($saved_settings['omega_preface_middle_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the Preface First and Preface Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Last'),
          '#default_value' => ovars($saved_settings['omega_preface_last_width'], 6),
          '#options' => $grids,
          '#description' => t('This number, combined with the Preface First and Preface Middle determine the share of your grid for each element.'),
        );
      // Main Body Regions
      $form['omega_container']['omega_regions']['main'] = array(
        '#type' => 'fieldset',
        '#title' => t('Content Layout Configuration'),
        '#description' => t('Content Zone, Sidebar First and Sidebar Last.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['main']['omega_content_layout'] = array(
          '#type'          => 'radios',
          '#description'   => t('You may arrange the order and size of your sidebars and main content zones here.'),
          '#title'         => t('Content Zone Layout'),
          '#default_value' => ovars($saved_settings['omega_content_layout'], first_content_last),
          '#options'       => array(
                               'first_content_last' => t('Sidebar First - Content - Sidebar Last'),
                               'content_first_last' => t('Content - Sidebar First - Sidebar Last'),
                               'first_last_content' => t('Sidebar First - Sidebar Last - Content'),
                              ),
        );
        $form['omega_container']['omega_regions']['main']['omega_sidebar_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Sidebar First'),
          '#default_value' => ovars($saved_settings['omega_sidebar_first_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the Content Main and Sidebar Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['main']['omega_content_main_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Main Content Region'),
          '#default_value' => ovars($saved_settings['omega_content_main_width'], 8),
          '#options' => $grids,
          '#description' => t('This number, combined with the Sidebar First and Sidebar Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['main']['omega_sidebar_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Sidebar Last'),
          '#default_value' => ovars($saved_settings['omega_sidebar_last_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the Sidebar First and Main Content determine the share of your grid for each element.'),
        );
        
    // Preface Blocks
      $form['omega_container']['omega_regions']['postscript'] = array(
        '#type' => 'fieldset',
        '#title' => t('Postscript Configuration'),
        '#description' => t('Postscript region zones.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_one_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 1'),
          '#default_value' => ovars($saved_settings['omega_postscript_one_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_two_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 2'),
          '#default_value' => ovars($saved_settings['omega_postscript_two_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_three_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 3'),
          '#default_value' => ovars($saved_settings['omega_postscript_three_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_four_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 4'),
          '#default_value' => ovars($saved_settings['omega_postscript_four_width'], 4),
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
  // Return theme settings form
  return $form;
}  