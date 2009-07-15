<?php
// $Id$
// Pull out some things from the page.tpl.php and make that code more consise.
// $header_first
global $theme_key;
$settings = theme_get_settings($theme_key);
$omega = omega_theme_get_default_settings('omega');
// Merge the saved variables and their default values.
$settings = array_merge($omega, $settings);
$vars['omega'] = $omega;
/**
 * Adding in the ability to combine the sidebars on certain paths in order to adjust 
 * layouts based on context of the page. My main purpose for this is in the admin 
 * section to help widen up the layout if two sidebars are being used.
 * 
 * All it does is combine the sidebar_first and sidebar_last into sidebar_first and 
 * unset the sidebar_last variable. This resets the layout to act like only one 
 * sidebar is present in certain paths
 */
 if ($settings['sidebar_contain_pages']) {
   $path = drupal_get_path_alias($_GET['q']);
   // Compare with the internal and path alias (if any).
   $page_match = drupal_match_path($path, $settings['sidebar_contain_pages']);
   if ($path != $_GET['q']) {
     $page_match = $page_match || drupal_match_path($_GET['q'], $settings['sidebar_contain_pages']);
   }
   $page_match = !($settings['sidebar_combine'] xor $page_match);
   if($page_match && substr($_GET['q'], 0, 17) != 'admin/build/block') {
     $vars['sidebar_first'] = $vars['sidebar_first']. $vars['sidebar_last'];
     unset($vars['sidebar_last']); 
   }
 }
/**
 * Header Region
 * Depends on the width of the logo and title region, which is 4 grids by default.
 * This leaves 12 (Grid-12 by default) for the maximum width of any one of the elements in this zone
 * If only one zone is included, it fills the maximum width, and if both zones are present, 
 * they will use the provided settings from the theme configuration interface.
 */
if ($vars['header_first']) {
  $vars['omega']['header_first_classes'] = ao($vars, array('header_first', 'header_last'), 'header_first', TRUE);
  $vars['header_first'] = '<div id="header-first" class="'.ns('grid-12', $vars['header_last'], $settings['omega_header_last_width']). $vars['omega']['header_first']. $vars['omega']['header_first_classes']. '">'. $vars['header_first']. '</div>';
}
// $header_last
if ($vars['header_last']) {
  $vars['omega']['header_last_classes'] = ao($vars, array('header_first', 'header_last'), 'header_last', TRUE);
  $vars['header_last'] = '<div id="header-last" class="'.ns('grid-12', $vars['header_first'], $settings['omega_header_first_width']). $vars['omega']['header_last_classes']. '">'. $vars['header_last']. '</div>';
}
/**
 * Preface Region
 * Same as above, preparing the preface regions to accept settings configurations
 */
if ($vars['preface_first']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['preface_first_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_first');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['preface_first'] = '<div id="preface-first" class="preface '.ns(
      'grid-16', 
        $vars['preface_middle'], $settings['omega_preface_middle_width'], 
        $vars['preface_last'], $settings['omega_preface_last_width'])
    . ' '.$vars['omega']['preface_first_classes'].'">' 
    .$vars['preface_first']. '</div>';
}
if ($vars['preface_middle']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['preface_middle_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_middle');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['preface_middle'] = '<div id="preface-middle" class="preface '.ns(
      'grid-16', 
        $vars['preface_first'], $settings['omega_preface_first_width'], 
        $vars['preface_last'], $settings['omega_preface_last_width'])
    . $vars['omega']['preface_middle_classes']. '">' 
    .$vars['preface_middle']. '</div>';
}
if ($vars['preface_last']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['preface_last_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_last');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['preface_last'] = '<div id="preface-last" class="preface '.ns(
      'grid-16', 
        $vars['preface_first'], $settings['omega_preface_first_width'], 
        $vars['preface_middle'], $settings['omega_preface_middle_width'])
    . $vars['omega']['preface_last_classes']. '">' 
    .$vars['preface_last']. '</div>';
}
/**
 * Body Region
 * Configuration of $sidebar_first, $sidebar_last, and the main content zone
 * 
 * This section needs MUCH cleanup as I know I can condense the switch
 * statement to be much shorter than completely redeclaring each zone
 * for each case that can be present.
 * 
 * @todo cleanup switch statement
 */
switch($settings['omega_content_layout']){
  default:
  case 'first_content_last':
  // FIRST - CONTENT - LAST
  $sl_max_width = $omega['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  $sl_pull = $omega['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
      
    ). ' '. ns('pull-'. $sl_pull,
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_first'] || !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  if($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-16',  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('push-'. $settings['omega_sidebar_first_width'],
      !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  break;
  // CONTENT  - FIRST - LAST
  case 'content_first_last':
  $sl_max_width = $settings['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  $sl_pull = $settings['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_first'] || !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  if($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-16',  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  break;
  // FIRST - LAST - CONTENT
  case 'first_last_content':
  $sl_max_width = $settings['omega_sidebar_first_width'] + $settings['omega_sidebar_last_width'];
  $sl_pull = $settings['omega_content_main_width'] + $settings['omega_sidebar_last_width'];
  if ($vars['sidebar_first']) {
    $vars['sidebar_first_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_last'] || !$vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('pull-'. $sl_pull,
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    );
  }
  if ($vars['sidebar_last']) {
    $vars['sidebar_last_classes'] = 
      ns('grid-'. $sl_max_width, 
      $vars['sidebar_first'] || !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    ). ' '. ns('pull-'. $sl_pull,
      $vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  if($vars['content']) {
    $vars['main_content_classes'] = 
      ns('grid-16',  
      $vars['sidebar_first'], $settings['omega_sidebar_first_width'],
      $vars['sidebar_last'], $settings['omega_sidebar_last_width']
    ). ' '. ns('push-'. $sl_max_width,
      !$vars['sidebar_first'], $settings['omega_sidebar_first_width']
    );
  }
  break;
}

/**
 * Postscript Region
 * Same as above, preparing the preface regions to accept settings configurations
 */
if ($vars['postscript_one']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['postscript_one_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_one');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['postscript_one'] = '<div id="postscript-one" class="postscript '.ns(
      'grid-16', 
        $vars['postscript_two'], $settings['omega_postscript_two_width'], 
        $vars['postscript_three'], $settings['omega_postscript_three_width'], 
        $vars['postscript_four'], $settings['omega_postscript_four_width']) 
    . ' '.$vars['omega']['postscript_one_classes'].'">'. 
    $vars['postscript_one'] . '</div>';
}
if ($vars['postscript_two']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['postscript_two_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_two');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['postscript_two'] = '<div id="postscript-two" class="postscript '.ns(
      'grid-16', 
        $vars['postscript_one'], $settings['omega_postscript_one_width'], 
        $vars['postscript_three'], $settings['omega_postscript_three_width'], 
        $vars['postscript_four'], $settings['omega_postscript_four_width']) 
    . ' '.$vars['omega']['postscript_two_classes'].'">'. 
    $vars['postscript_two'] . '</div>';
}
if ($vars['postscript_three']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['postscript_three_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_three');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['postscript_three'] = '<div id="postscript-three" class="postscript '.ns(
      'grid-16', 
        $vars['postscript_one'], $settings['omega_postscript_one_width'], 
        $vars['postscript_two'], $settings['omega_postscript_two_width'], 
        $vars['postscript_four'], $settings['omega_postscript_four_width']) 
    . ' '.$vars['omega']['postscript_three_classes'].'">'. 
    $vars['postscript_three'] . '</div>';
}
if ($vars['postscript_four']) {
	// determine if this region needs an alpha or omega class applied
  $vars['omega']['postscript_four_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_four');
  // add the wrapper element, grid-x and alpha/omega classes 
  $vars['postscript_four'] = '<div id="postscript-four" class="postscript '.ns(
      'grid-16', 
        $vars['postscript_one'], $settings['omega_postscript_one_width'], 
        $vars['postscript_two'], $settings['omega_postscript_two_width'], 
        $vars['postscript_three'], $settings['omega_postscript_three_width']) 
    . ' '.$vars['omega']['postscript_four_classes'].'">'. 
    $vars['postscript_four'] .'</div>';
}
// ZEN - BODY CLASSES
// Classes for body element. Allows advanced theming based on context
// (home page, node of certain type, etc.)
$classes = split(' ', $vars['body_classes']);
// Remove the mostly useless page-ARG0 class.
if ($index = array_search(preg_replace('![^abcdefghijklmnopqrstuvwxyz0-9-_]+!s', '', 'page-'. drupal_strtolower(arg(0))), $classes)) {
  unset($classes[$index]);
}
if (!$vars['is_front']) {
  // Add unique class for each page.
  $path = drupal_get_path_alias($_GET['q']);
  $classes[] = omega_id_safe('page-' . $path);
  // Add unique class for each website section.
  list($section, ) = explode('/', $path, 2);
  if (arg(0) == 'node') {
    if (arg(1) == 'add') {
      $section = 'node-add';
    }
    elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
      $section = 'node-' . arg(2);
    }
  }
  $classes[] = omega_id_safe('section-' . $section);
}
$vars['body_classes_array'] = $classes;
$vars['body_classes'] = implode(' ', $classes); // Concatenate with spaces.
// NINESIXTY - For easy printing of variables.
$vars['logo_img']         = $vars['logo'] ? theme('image', substr($vars['logo'], strlen(base_path())), t('Home'), t('Home')) : '';
$vars['linked_logo_img']  = $vars['logo_img'] ? l($vars['logo_img'], '<front>', array('rel' => 'home', 'title' => t('Home'), 'html' => TRUE)) : '';
$vars['linked_site_name'] = $vars['site_name'] ? l($vars['site_name'], '<front>', array('rel' => 'home', 'title' => t('Home'))) : '';
$vars['main_menu_links']      = theme('links', $vars['primary_links'], array('class' => 'links main-menu'));
$vars['secondary_menu_links'] = theme('links', $vars['secondary_links'], array('class' => 'links secondary-menu'));
// NINESIXTY - Make sure framework styles are placed above all others.
$vars['css_alt'] = omega_css_reorder($vars['css']);
$vars['styles'] = drupal_get_css($vars['css_alt']);

// ACQUIA-MARINA
// Set site title, slogan, mission, page title & separator (unless using Page Title module)
if (!module_exists('page_title')) {
  $title = t(variable_get('site_name', ''));
  $slogan = t(variable_get('site_slogan', ''));
  $mission = t(variable_get('site_mission', ''));
  $page_title = t(drupal_get_title());
  $title_separator = theme_get_setting('configurable_separator');
  if (drupal_is_front_page()) {                                                // Front page title settings
    switch (theme_get_setting('front_page_title_display')) {
      case 'title_slogan':
        $vars['head_title'] = drupal_set_title($title . $title_separator . $slogan);
        break;
      case 'slogan_title':
        $vars['head_title'] = drupal_set_title($slogan . $title_separator . $title);
        break;
      case 'title_mission':
        $vars['head_title'] = drupal_set_title($title . $title_separator . $mission);
        break;
      case 'custom':
        if (theme_get_setting('page_title_display_custom') !== '') {
          $vars['head_title'] = drupal_set_title(t(theme_get_setting('page_title_display_custom')));
        }
    }
  }
  else {                                                                       // Non-front page title settings
    switch (theme_get_setting('other_page_title_display')) {
      case 'ptitle_slogan':
        $vars['head_title'] = drupal_set_title($page_title . $title_separator . $slogan);
        break;
      case 'ptitle_stitle':
        $vars['head_title'] = drupal_set_title($page_title . $title_separator . $title);
        break;
      case 'ptitle_smission':
        $vars['head_title'] = drupal_set_title($page_title . $title_separator . $mission);
        break;
      case 'ptitle_custom':
        if (theme_get_setting('other_page_title_display_custom') !== '') {
          $vars['head_title'] = drupal_set_title($page_title . $title_separator . t(theme_get_setting('other_page_title_display_custom')));
        }
        break;
      case 'custom':
        if (theme_get_setting('other_page_title_display_custom') !== '') {
          $vars['head_title'] = drupal_set_title(t(theme_get_setting('other_page_title_display_custom')));
        }
    }
  }
  $vars['head_title'] = strip_tags($vars['head_title']);                       // Remove any potential html tags
}
// implement mission statement settings
$vars['mission'] = t(variable_get('site_mission', ''));
if ($settings['mission_statement_pages'] == 'all' || $vars['is_front'] && $settings['mission_statement_pages'] == 'home') {
  $vars['mission'] = '<div id="mission" class=""><p>' .$vars['mission']. '</p></div>';
}
else {
  unset($vars['mission']);
}