<?php
// $Id$
// require_once for the functions that need to be available when we are outside
// of the omega theme in the administrative interface
include_once './' . drupal_get_path('theme', 'omega') . '/template.theme-registry.inc';
/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function omega_settings($saved_settings, $subtheme_defaults = array()) {
	// Add the form's CSS
  //drupal_add_css(drupal_get_path('theme', 'omega') . '/theme-settings.css', 'theme');

  // Add javascript to show/hide optional settings
  drupal_add_js(drupal_get_path('theme', 'omega'). '/js/omega_admin.js', 'theme');

  // Get the default values from the .info file.
  $defaults = omega_theme_get_default_settings('omega');
  // Allow a subtheme to override the default values.
  $defaults = array_merge($defaults, $subtheme_defaults);
  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);
		
	for($i=1;$i<=16;$i++){
		$grids[$i]= $i;
	}
  $form['omega_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Omega 960 settings'),
    '#description' => t('Core configuration options for the Omega theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
	  // General Settings
	  $form['omega_container']['omega_general'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('General Omega 960 Settings'),
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
		      '#default_value' => $saved_settings['front_page_title_display'],
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
		      '#default_value' => $saved_settings['page_title_display_custom'],
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
		      '#default_value' => $saved_settings['other_page_title_display'],
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
		      '#default_value' => $saved_settings['other_page_title_display_custom'],
		      '#description'   => t('Enter a custom page title for all other pages'),
		    );
		    // SEO configurable separator
		    $form['omega_container']['omega_general']['page_format_titles']['configurable_separator'] = array(
		      '#type' => 'textfield',
		      '#title' => t('Title separator'),
		      '#description' => t('Customize the separator character used in the page title'),
		      '#size' => 60,
		      '#default_value' => $saved_settings['configurable_separator'],
		    );
		  } else {
		      $form['omega_container']['omega_general']['page_format_titles']['#description'] = 'NOTICE: You currently have the "Page Title" module installed and enabled, so the Page titles theme settings have been disabled to prevent conflicts.  If you wish to re-enable the Page titles theme settings, you must first disable the "Page Title" module.';
		      $form['omega_container']['omega_general']['page_format_titles']['configurable_separator']['#disabled'] = 'disabled';
		  }
	  // Breadcrumb
	  $form['omega_container']['omega_general']['breadcrumb'] = array(
	    '#type'          => 'fieldset',
	    '#title'         => t('Breadcrumb settings'),
	    '#attributes'    => array('id' => 'omega-breadcrumb'),
	    '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
	  $form['omega_container']['omega_general']['breadcrumb']['omega_breadcrumb'] = array(
	    '#type'          => 'select',
	    '#title'         => t('Display breadcrumb'),
	    '#default_value' => $saved_settings['omega_breadcrumb'],
	    '#options'       => array(
	                          'yes'   => t('Yes'),
	                          'admin' => t('Only in admin section'),
	                          'no'    => t('No'),
	                        ),
	  );
	  $form['omega_container']['omega_general']['breadcrumb']['omega_breadcrumb_separator'] = array(
	    '#type'          => 'textfield',
	    '#title'         => t('Breadcrumb separator'),
	    '#description'   => t('Text only. Don’t forget to include spaces.'),
	    '#default_value' => $saved_settings['omega_breadcrumb_separator'],
	    '#size'          => 5,
	    '#maxlength'     => 10,
	  );
	  $form['omega_container']['omega_general']['breadcrumb']['omega_breadcrumb_home'] = array(
	    '#type'          => 'checkbox',
	    '#title'         => t('Show home page link in breadcrumb'),
	    '#default_value' => $saved_settings['omega_breadcrumb_home'],
	  );
	  $form['omega_container']['omega_general']['breadcrumb']['omega_breadcrumb_trailing'] = array(
	    '#type'          => 'checkbox',
	    '#title'         => t('Append a separator to the end of the breadcrumb'),
	    '#default_value' => $saved_settings['omega_breadcrumb_trailing'],
	    '#description'   => t('Useful when the breadcrumb is placed just before the title.'),
	  );
	  $form['omega_container']['omega_general']['breadcrumb']['omega_breadcrumb_title'] = array(
	    '#type'          => 'checkbox',
	    '#title'         => t('Append the content title to the end of the breadcrumb'),
	    '#default_value' => $saved_settings['omega_breadcrumb_title'],
	    '#description'   => t('Useful when the breadcrumb is not placed just before the title.'),
	  );
		  
	  
	  
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
          '#default_value' => $saved_settings['omega_jqueryui'],
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
			    '#default_value' => $saved_settings['mission_statement_pages'],
			    '#options'       => array(
			                          'home' => t('Display mission statement only on front page'),
			                          'all' => t('Display mission statement on all pages'),
			                        ),
			  );
		  

	  // Region Settings
	  $form['omega_container']['omega_regions'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('960gs Region Settings'),
	    '#description' => t('Configure how your regions are rendered. This area is currently a quick implementation of an interface to allow end users to quickly build out and adjust the default page layout. This feature will be improved over time, and include much more flexibility.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
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
          '#default_value' => $saved_settings['omega_header_first_width'],
          '#options' => $grids,
          '#description' => t('This number, paired with the Header Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Header First'),
          '#default_value' => $saved_settings['omega_header_last_width'],
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
          '#default_value' => $saved_settings['omega_preface_first_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the Preface Middle and Preface Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_middle_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Middle'),
          '#default_value' => $saved_settings['omega_preface_middle_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the Preface First and Preface Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Last'),
          '#default_value' => $saved_settings['omega_preface_last_width'],
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
          '#default_value' => $saved_settings['omega_content_layout'],
          '#options'       => array(
                               'first_content_last' => t('Sidebar First - Content - Sidebar Last'),
                               'content_first_last' => t('Content - Sidebar First - Sidebar Last'),
                               'first_last_content' => t('Sidebar First - Sidebar Last - Content'),
                              ),
        );
        $form['omega_container']['omega_regions']['main']['omega_sidebar_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Sidebar First'),
          '#default_value' => $saved_settings['omega_sidebar_first_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the Content Main and Sidebar Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['main']['omega_content_main_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Main Content Region'),
          '#default_value' => $saved_settings['omega_content_main_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the Sidebar First and Sidebar Last determine the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['main']['omega_sidebar_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Sidebar Last'),
          '#default_value' => $saved_settings['omega_sidebar_last_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the Sidebar First and Main Content determine the share of your grid for each element.'),
        );
        $options = array(t('Combine Sidebars on all except the listed pages.'), t('Combine Sidebars on only the listed pages.'));
        $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
		    $form['omega_container']['omega_regions']['main']['sidebar_combine'] = array(
		      '#type' => 'radios',
		      '#title' => t('Combine Sidebars'),
		      '#description' => t('This is useful for administrative pages, and in certain contexts. You may choose to in certain areas, combine the <strong>$sidebar_first</strong> and <strong>$sidebar_last</strong> to create one sidebar from the content of both.'),
		      '#options' => $options,
		      '#default_value' => $saved_settings['sidebar_combine'],
		    );
		    $form['omega_container']['omega_regions']['main']['sidebar_contain_pages'] = array(
		      '#type' => 'textarea',
		      '#title' => t('Pages'),
		      '#default_value' => $saved_settings['sidebar_contain_pages'],
		      '#description' => $description,
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
          '#default_value' => $saved_settings['omega_postscript_one_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_two_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 2'),
          '#default_value' => $saved_settings['omega_postscript_two_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_three_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 3'),
          '#default_value' => $saved_settings['omega_postscript_three_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_four_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 4'),
          '#default_value' => $saved_settings['omega_postscript_four_width'],
          '#options' => $grids,
          '#description' => t('This number, combined with the other Postscript content zones determines the share of your grid for each element.'),
        );
  // Return theme settings form
  return $form;
}  