<?php
// $Id: theme-settings.php,v 1.8.2.11 2010/11/16 14:39:39 himerus Exp $

// require_once for the functions that need to be available when we are outside
// of the omega theme in the administrative interface
//include_once './' . drupal_get_path('theme', 'omega') . '/template.theme-registry.inc';
include_once './' . drupal_get_path('theme', 'omega') . '/theme-functions.inc';

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
  drupal_add_css(drupal_get_path('theme', 'omega'). '/css/omega_theme_settings.css', 'theme', 'all', TRUE);

  // Get the default values from the .info file.
  if (count($subtheme_defaults) > 0) {
    // Allow a subtheme to override the default values.
    $settings = array_merge($subtheme_defaults, $saved_settings);
  }
  else {
    // Merge the saved variables and their default values.
    $defaults = omega_theme_get_default_settings('omega');
    $settings = array_merge($defaults, $saved_settings);
  }

  for ($i = 1; $i <= 24; $i++){
    $grids[$i] = $i;
  }
  for ($i = 0; $i <= 23; $i++){
    $spacing[$i] = $i;
  }
  $containers = array(
    '12' => '12 column grid',
    '16' => '16 column grid',
    '24' => '24 column grid'
  );
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
    $form['omega_container']['omega_general']['optional_css'] = array(
      '#type' => 'fieldset',
      '#title' => t('Optional CSS Files'),
      '#description'   => t('Here, you may disable default theme CSS provided by the Omega base theme.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
      $form['omega_container']['omega_general']['optional_css']['reset_css'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable reset.css'),
        '#default_value' => $saved_settings['reset_css'],
        '#description'   => t('reset.css is the default CSS reset standard created by <a href="http://meyerweb.com/eric/tools/css/reset/">Eric Meyer</a>.'),
      );
      $form['omega_container']['omega_general']['optional_css']['text_css'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable text.css'),
        '#default_value' => $saved_settings['text_css'],
        '#description'   => t('text.css offers some generic typography to give the default text presenation a bit more love.'),
      );
      $form['omega_container']['omega_general']['optional_css']['regions_css'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable regions.css'),
        '#default_value' => $saved_settings['regions_css'],
        '#description'   => t('regions.css defines all the default regions of the Omega theme and its sub-themes. Currently there are no defining characteristics in this file, and it can be disabled without affecting any region presentation.'),
      );
      $form['omega_container']['omega_general']['optional_css']['defaults_css'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable defaults.css'),
        '#default_value' => $saved_settings['defaults_css'],
        '#description'   => t('defaults.css gives the Omega theme the majority of the spacing and alignment CSS for various elements.'),
      );
      $form['omega_container']['omega_general']['optional_css']['custom_css'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable custom.css'),
        '#default_value' => $saved_settings['custom_css'],
        '#description'   => t('custom.css provides some additional CSS that is module related, and not a part of core drupal. Can be disabled and used as a reference for certain items. Contributed CSS that is not directly related to core markup will be in this file.'),
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
      }
      else {
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
                                'home' => t('Display mission statement only on front page.'),
                                'all' => t('Display mission statement on all pages.'),
                                'none'=> t('Do not display the mission statement on any pages.'),
                              ),
        );

    // Region Settings
    $form['omega_container']['omega_regions'] = array(
      '#type' => 'fieldset',
      '#title' => t('960gs Region Settings'),
      '#description' => t('Configure how your regions are rendered. This area is currently a quick implementation of an interface to allow end users to quickly build out and adjust the default page layout. This feature will be improved over time, and include much more flexibility.'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $default_omega_layout = !empty($saved_settings['omega_fixed_fluid']) ? $saved_settings['omega_fixed_fluid'] : 'fixed';
      $form['omega_container']['omega_regions']['omega_fixed_fluid'] = array(
          '#type'          => 'radios',
          '#description'   => t('You may select fluid layout, or the default fixed width layout.'),
          '#title'         => t('Fixed / Fluid Layout'),
          '#default_value' => $default_omega_layout,
          '#options'       => array(
                               'fixed' => t('Fixed width (theme default)'),
                               'fluid' => t('Fluid width'),
                              ),
        );
      $form['omega_container']['omega_regions']['omega_default_container_width'] = array(
        '#type' => 'select',
        '#title' => t('Default container width'),
        '#default_value' => $saved_settings['omega_default_container_width'],
        '#options' => $containers,
        '#weight' => -50,
        '#description' => t('This width is used for regions like $help, $messages and other non-important regions in page.tpl.php'),
      );

      // Header Blocks
      $form['omega_container']['omega_regions']['headers'] = array(
        '#type' => 'fieldset',
        '#title' => t('Header Configuration'),
        '#description' => t('Header region zones, including Logo/Branding, Primary & Secondary menus, Header first and Header Last. By default, the logo and menu elements are designed to display inline. This is accomplished by making the width of the grid elements for the logo and menus to equal the container width for those items, however, to make them stack, you can make each element have the full amount of grids that the container allows.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['headers']['omega_branding_wrapper_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Logo/Navigation Elements'),
          '#default_value' => $saved_settings['omega_branding_wrapper_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the branding (logo) area and navigation menus.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_logo_width'] = array(
          '#type' => 'select',
          '#title' => t('Width for Logo/Branding area'),
          '#default_value' => $saved_settings['omega_header_logo_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the logo/branding area. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_menu_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Menu Elements'),
          '#default_value' => $saved_settings['omega_header_menu_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the primary/secondary menu elements. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_wrapper_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Header Elements'),
          '#default_value' => $saved_settings['omega_header_wrapper_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the header region areas.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Header First'),
          '#default_value' => $saved_settings['omega_header_first_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the first header region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_header_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Header Last'),
          '#default_value' => $saved_settings['omega_header_last_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the last header region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_internal_nav_wrapper_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Breadcrumb/Slogan/Search'),
          '#default_value' => $saved_settings['omega_internal_nav_wrapper_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the breadcrumb/search/slogan area.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_breadcrumb_slogan_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Breadcrumb/Slogan'),
          '#default_value' => $saved_settings['omega_breadcrumb_slogan_width'],
          '#options' => $grids,
          '#description' => t('Grid width for the slogan/breadcrumb area. By default, the slogan will only appear in the zone if there is no breadcrumb avaiable.'),
        );
        $form['omega_container']['omega_regions']['headers']['omega_search_width'] = array(
          '#type' => 'select',
          '#title' => t('Wrapper Area width for Search'),
          '#default_value' => $saved_settings['omega_search_width'],
          '#options' => $grids,
          '#description' => t('Grid width for the search zone, which appears inline with the breadcrumb/slogan zone.'),
        );

      // Preface Blocks
      $form['omega_container']['omega_regions']['preface'] = array(
        '#type' => 'fieldset',
        '#title' => t('Preface Configuration'),
        '#description' => t('Grid configuration for preface zones. You may use prefix and suffix here to allow extra spacing between regions. You can create all regions inline if the total of the grid elements are less than or equal to the container width defind above. You may stack these items easily by making the elements grid width be the full amount defined by the container.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['preface']['omega_preface_wrapper_grids'] = array(
          '#type' => 'select',
          '#title' => t('Preface Wrapper Container Grids'),
          '#default_value' => $saved_settings['omega_preface_wrapper_grids'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the preface regions.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface First'),
          '#default_value' => $saved_settings['omega_preface_first_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the first preface region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_first_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Preface First'),
          '#default_value' => $saved_settings['omega_preface_first_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['preface']['omega_preface_first_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Preface First'),
          '#default_value' => $saved_settings['omega_preface_first_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_middle_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Middle'),
          '#default_value' => $saved_settings['omega_preface_middle_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the middle preface region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_middle_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Preface Middle'),
          '#default_value' => $saved_settings['omega_preface_middle_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['preface']['omega_preface_middle_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Preface Middle'),
          '#default_value' => $saved_settings['omega_preface_middle_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Preface Last'),
          '#default_value' => $saved_settings['omega_preface_last_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the last preface region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['preface']['omega_preface_last_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Preface Last'),
          '#default_value' => $saved_settings['omega_preface_last_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['preface']['omega_preface_last_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Preface Last'),
          '#default_value' => $saved_settings['omega_preface_last_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
      // Main Body Regions
      $form['omega_container']['omega_regions']['main'] = array(
        '#type' => 'fieldset',
        '#title' => t('Content Layout Configuration'),
        '#description' => t('<p>Grid configurations for Content Zone, Sidebar First and Sidebar Last. The "main" regions here are the only true "smart" zone that will use the maximum container width to determine the appropriate width for elements in this zone based on which regions are displayed on the current page.</p><p>If your container grid is 16 grids, and you have a configuration of 4-8-4, which would imply two sidebars and the content zone, if all regions are present, this layout of 4-8-4 will be respected. However, if on a page, the first sidebar is empty of content, the content zone would then incorporate those leftover 4 grids, so your layout would be 12-4.'),
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
        $form['omega_container']['omega_regions']['main']['omega_content_container_width'] = array(
          '#type' => 'select',
          '#title' => t('Container width for content zone'),
          '#default_value' => $saved_settings['omega_content_container_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the main content regions. This includes the content_top, content_bottom, and primary content zone.'),
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

      // Postscript Blocks
      $form['omega_container']['omega_regions']['postscript'] = array(
        '#type' => 'fieldset',
        '#title' => t('Postscript Configuration'),
        '#description' => t('Grid configuration for postscript zones. You may use prefix and suffix here to allow extra spacing between regions. You can create all regions inline if the total of the grid elements are less than or equal to the container width defind above. You may stack these items easily by making the elements grid width be the full amount defined by the container.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_container_width'] = array(
          '#type' => 'select',
          '#title' => t('Container width for postscript regions'),
          '#default_value' => $saved_settings['omega_postscript_container_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the postscript regions.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_one_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 1'),
          '#default_value' => $saved_settings['omega_postscript_one_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the first postscript region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_one_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Postscript 1'),
          '#default_value' => $saved_settings['omega_postscript_one_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['postscript']['omega_postscript_one_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Postscript 1'),
          '#default_value' => $saved_settings['omega_postscript_one_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_two_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 2'),
          '#default_value' => $saved_settings['omega_postscript_two_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the second postscript region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_two_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Postscript 2'),
          '#default_value' => $saved_settings['omega_postscript_two_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['postscript']['omega_postscript_two_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Postscript 2'),
          '#default_value' => $saved_settings['omega_postscript_two_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_three_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 3'),
          '#default_value' => $saved_settings['omega_postscript_three_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the third postscript region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_three_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Postscript 3'),
          '#default_value' => $saved_settings['omega_postscript_three_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['postscript']['omega_postscript_three_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Postscript 3'),
          '#default_value' => $saved_settings['omega_postscript_three_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_four_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Postscript 4'),
          '#default_value' => $saved_settings['omega_postscript_four_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the fourth postscript region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['postscript']['omega_postscript_four_prefix'] = array(
          '#type' => 'select',
          '#title' => t('Prefix Spacing for Postscript 4'),
          '#default_value' => $saved_settings['omega_postscript_four_prefix'],
          '#options' => $spacing,
          '#prefix' => '<div class="prefix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding prefix grids to this element will add padding to the left side of the element, creating spacing between the previous element.'),
        );
         $form['omega_container']['omega_regions']['postscript']['omega_postscript_four_suffix'] = array(
          '#type' => 'select',
          '#title' => t('Suffix Spacing for Postscript 4'),
          '#default_value' => $saved_settings['omega_postscript_four_suffix'],
          '#options' => $spacing,
          '#prefix' => '<div class="suffix_config">',
          '#suffix' => '</div>',
          '#description' => t('Adding suffix grids to this element will add padding to the right side of the element, creating spacing between the next element.'),
        );
      // Footer Blocks
      $form['omega_container']['omega_regions']['footer'] = array(
        '#type' => 'fieldset',
        '#title' => t('Footer Configuration'),
        '#description' => t('Grid configuration for footer zones. You can create both regions inline if the total of the grid elements are less than or equal to the container width defind above. You may stack these items easily by making the elements grid width be the full amount defined by the container.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
        $form['omega_container']['omega_regions']['footer']['omega_footer_container_width'] = array(
          '#type' => 'select',
          '#title' => t('Container width for footer regions'),
          '#default_value' => $saved_settings['omega_footer_container_width'],
          '#options' => $containers,
          '#description' => t('Container Grid width for the footer regions.'),
        );
        $form['omega_container']['omega_regions']['footer']['omega_footer_first_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Footer First'),
          '#default_value' => $saved_settings['omega_footer_first_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the first footer region. This number should be less than or equal to the container width defined above.'),
        );
        $form['omega_container']['omega_regions']['footer']['omega_footer_last_width'] = array(
          '#type' => 'select',
          '#title' => t('Contextual Width for Footer Last'),
          '#default_value' => $saved_settings['omega_footer_last_width'],
          '#options' => $grids,
          '#description' => t('Grid width of the last footer region. This number should be less than or equal to the container width defined above.'),
        );
  // Return theme settings form
  return $form;
}