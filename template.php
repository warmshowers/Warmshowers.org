<?php
//$Id$
require_once './'. drupal_get_path('theme', 'omega') ."/theme-functions.inc";

/**
 * Preprocessor for page.tpl.php template file.
 */
function omega_preprocess_page(&$vars, $hook) {
	// Pull out some things from the page.tpl.php and make that code more consise.
  // $header_first
  global $theme_key;
  $settings = theme_get_settings($theme_key);
  //krumo($settings);
  $omega = array(
    // grid size/layout variable defaults
    // Header Regions
    'omega_header_first_width' => ovars($settings['omega_header_first_width'], 6),
    'omega_header_last_width' => ovars($settings['omega_header_last_width'], 6),
    // Preface Regions
    'omega_preface_first_width' => ovars($settings['omega_preface_first_width'], 6),
    'omega_preface_middle_width' => ovars($settings['omega_preface_middle_width'], 4),
    'omega_preface_last_width' => ovars($settings['omega_preface_last_width'], 6),
    // Postscript Regions
    'omega_postscript_one_width' => ovars($settings['omega_postscript_one_width'], 4),
    'omega_postscript_two_width' => ovars($settings['omega_postscript_two_width'], 4),
    'omega_postscript_three_width' => ovars($settings['omega_postscript_three_width'], 4),
    'omega_postscript_four_width' => ovars($settings['omega_postscript_four_width'], 4),
    // Main Content Regions
    'omega_content_layout' => ovars($settings['omega_content_layout'], 'first_content_last'),
    'omega_sidebar_first_width' => ovars($settings['omega_sidebar_first_width'], 4),
    'omega_content_main_width' => ovars($settings['omega_content_main_width'], 8),
    'omega_sidebar_last_width' => ovars($settings['omega_sidebar_last_width'], 4),
    // page title information
    'front_page_title_display' => ovars($settings['front_page_title_display'], 'title_slogan'),
	  'page_title_display_custom' => ovars($settings['page_title_display_custom'], ''),
	  'other_page_title_display' => ovars($settings['other_page_title_display'], 'ptitle_slogan'),
	  'other_page_title_display_custom' => ovars($settings['other_page_title_display_custom'], ''),
	  'configurable_separator' => ovars($settings['configurable_separator'], ' | '),
    
  );
  $vars['omega'] = $omega;
  /**
   * Header Region
   * Depends on the width of the logo and title region, which is 4 grids by default.
   * This leaves 12 (Grid-12 by default) for the maximum width of any one of the elements in this zone
   * If only one zone is included, it fills the maximum width, and if both zones are present, 
   * they will use the provided settings from the theme configuration interface.
   */
  if ($vars['header_first']) {
  	$vars['omega']['header_first_classes'] = ao($vars, array('header_first', 'header_last'), 'header_first', TRUE);
    $vars['header_first'] = '<div id="header-first" class="'.ns('grid-12', $vars['header_last'], $omega['omega_header_last_width']). $vars['omega']['header_first']. $vars['omega']['header_first_classes']. '">'. $vars['header_first']. '</div>';
  }
  // $header_last
  if ($vars['header_last']) {
  	$vars['omega']['header_last_classes'] = ao($vars, array('header_first', 'header_last'), 'header_last', TRUE);
    $vars['header_last'] = '<div id="header-last" class="'.ns('grid-12', $vars['header_first'], $omega['omega_header_first_width']). $vars['omega']['header_last_classes']. '">'. $vars['header_last']. '</div>';
  }
  /**
   * Preface Region
   * Same as above, preparing the preface regions to accept settings configurations
   */
  if ($vars['preface_first']) {
  	$vars['omega']['preface_first_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_first');
    $vars['preface_first'] = '<div id="preface-first" class="preface '.ns(
        'grid-16', 
          $vars['preface_middle'], $omega['omega_preface_middle_width'], 
          $vars['preface_last'], $omega['omega_preface_last_width'])
      . ' '.$vars['omega']['preface_first_classes'].'">' 
      .$vars['preface_first']. '</div>';
  }
  if ($vars['preface_middle']) {
  	$vars['omega']['preface_middle_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_middle');
    $vars['preface_middle'] = '<div id="preface-middle" class="preface '.ns(
        'grid-16', 
          $vars['preface_first'], $omega['omega_preface_first_width'], 
          $vars['preface_last'], $omega['omega_preface_last_width'])
      . $vars['omega']['preface_middle_classes']. '">' 
      .$vars['preface_middle']. '</div>';
  }
  if ($vars['preface_last']) {
  	$vars['omega']['preface_last_classes'] = ao($vars, array('preface_first', 'preface_middle', 'preface_last'), 'preface_last');
    $vars['preface_last'] = '<div id="preface-last" class="preface '.ns(
        'grid-16', 
          $vars['preface_first'], $omega['omega_preface_first_width'], 
          $vars['preface_middle'], $omega['omega_preface_middle_width'])
      . $vars['omega']['preface_last_classes']. '">' 
      .$vars['preface_last']. '</div>';
  }
  /**
   * Body Region
   * Configuration of $sidebar_first, $sidebar_last, and the main content zone
   */
  switch($settings['omega_content_layout']){
  	default:
  	case 'first_content_last':
	  // FIRST - CONTENT - LAST
	  $sl_max_width = $omega['omega_sidebar_first_width'] + $omega['omega_sidebar_last_width'];
	  $sl_pull = $omega['omega_content_main_width'] + $omega['omega_sidebar_last_width'];
	  if ($vars['sidebar_first']) {
	    $vars['sidebar_first_classes'] = 
	      ns('grid-'. $sl_max_width, 
	      $vars['sidebar_last'] || !$vars['sidebar_last'], $omega['omega_sidebar_last_width']
	      
	    ). ' '. ns('pull-'. $sl_pull,
	      $vars['sidebar_last'], $omega['omega_sidebar_last_width']
	    );
	  }
	  if ($vars['sidebar_last']) {
	    $vars['sidebar_last_classes'] = 
	      ns('grid-'. $sl_max_width, 
	      $vars['sidebar_first'] || !$vars['sidebar_first'], $omega['omega_sidebar_first_width']
	    );
	  }
	  if($vars['content']) {
	  	$vars['main_content_classes'] = 
	      ns('grid-16',  
	      $vars['sidebar_first'], $omega['omega_sidebar_first_width'],
	      $vars['sidebar_last'], $omega['omega_sidebar_last_width']
	    ). ' '. ns('push-'. $omega['omega_sidebar_first_width'],
	      !$vars['sidebar_first'], $omega['omega_sidebar_first_width']
	    );
	  }
	  break;
	  // CONTENT  - FIRST - LAST
  	case 'content_first_last':
    $sl_max_width = $omega['omega_sidebar_first_width'] + $omega['omega_sidebar_last_width'];
    $sl_pull = $omega['omega_content_main_width'] + $omega['omega_sidebar_last_width'];
    if ($vars['sidebar_first']) {
      $vars['sidebar_first_classes'] = 
        ns('grid-'. $sl_max_width, 
        $vars['sidebar_last'] || !$vars['sidebar_last'], $omega['omega_sidebar_last_width']
      );
    }
    if ($vars['sidebar_last']) {
      $vars['sidebar_last_classes'] = 
        ns('grid-'. $sl_max_width, 
        $vars['sidebar_first'] || !$vars['sidebar_first'], $omega['omega_sidebar_first_width']
      );
    }
    if($vars['content']) {
      $vars['main_content_classes'] = 
        ns('grid-16',  
        $vars['sidebar_first'], $omega['omega_sidebar_first_width'],
        $vars['sidebar_last'], $omega['omega_sidebar_last_width']
      );
    }
  	break;
  	// FIRST - LAST - CONTENT
  	case 'first_last_content':
    $sl_max_width = $omega['omega_sidebar_first_width'] + $omega['omega_sidebar_last_width'];
    $sl_pull = $omega['omega_content_main_width'] + $omega['omega_sidebar_last_width'];
    if ($vars['sidebar_first']) {
      $vars['sidebar_first_classes'] = 
        ns('grid-'. $sl_max_width, 
        $vars['sidebar_last'] || !$vars['sidebar_last'], $omega['omega_sidebar_last_width']
      ). ' '. ns('pull-'. $sl_pull,
        $vars['sidebar_last'], $omega['omega_sidebar_last_width']
      );
    }
    if ($vars['sidebar_last']) {
      $vars['sidebar_last_classes'] = 
        ns('grid-'. $sl_max_width, 
        $vars['sidebar_first'] || !$vars['sidebar_first'], $omega['omega_sidebar_first_width']
      ). ' '. ns('pull-'. $sl_pull,
        $vars['sidebar_first'], $omega['omega_sidebar_first_width']
      );
    }
    if($vars['content']) {
      $vars['main_content_classes'] = 
        ns('grid-16',  
        $vars['sidebar_first'], $omega['omega_sidebar_first_width'],
        $vars['sidebar_last'], $omega['omega_sidebar_last_width']
      ). ' '. ns('push-'. $sl_max_width,
        !$vars['sidebar_first'], $omega['omega_sidebar_first_width']
      );
    }
  	break;
  }
  
  /**
   * Postscript Region
   * Same as above, preparing the preface regions to accept settings configurations
   */
  if ($vars['postscript_one']) {
    $vars['omega']['postscript_one_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_one');
    $vars['postscript_one'] = '<div id="postscript-one" class="postscript '.ns(
        'grid-16', 
          $vars['postscript_two'], $omega['omega_postscript_two_width'], 
          $vars['postscript_three'], $omega['omega_postscript_three_width'], 
          $vars['postscript_four'], $omega['omega_postscript_four_width']) 
      . ' '.$vars['omega']['postscript_one_classes'].'">'. 
      $vars['postscript_one'] . '</div>';
  }
  if ($vars['postscript_two']) {
    $vars['omega']['postscript_two_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_two');
    $vars['postscript_two'] = '<div id="postscript-two" class="postscript '.ns(
        'grid-16', 
          $vars['postscript_one'], $omega['omega_postscript_one_width'], 
          $vars['postscript_three'], $omega['omega_postscript_three_width'], 
          $vars['postscript_four'], $omega['omega_postscript_four_width']) 
      . ' '.$vars['omega']['postscript_two_classes'].'">'. 
      $vars['postscript_two'] . '</div>';
  }
  if ($vars['postscript_three']) {
    $vars['omega']['postscript_three_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_three');
    $vars['postscript_three'] = '<div id="postscript-three" class="postscript '.ns(
        'grid-16', 
          $vars['postscript_one'], $omega['omega_postscript_one_width'], 
          $vars['postscript_two'], $omega['omega_postscript_two_width'], 
          $vars['postscript_four'], $omega['omega_postscript_four_width']) 
      . ' '.$vars['omega']['postscript_three_classes'].'">'. 
      $vars['postscript_three'] . '</div>';
  }
  if ($vars['postscript_four']) {
    $vars['omega']['postscript_four_classes'] = ao($vars, array('postscript_one', 'postscript_two', 'postscript_three', 'postscript_four'), 'postscript_four');
    $vars['postscript_four'] = '<div id="postscript-four" class="postscript '.ns(
        'grid-16', 
          $vars['postscript_one'], $omega['omega_postscript_one_width'], 
          $vars['postscript_two'], $omega['omega_postscript_two_width'], 
          $vars['postscript_three'], $omega['omega_postscript_three_width']) 
      . ' '.$vars['omega']['postscript_four_classes'].'">'. 
      $vars['postscript_four'] .'</div>';
  }
  
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
} // end preprocess_page
/** 
 * NINESIXTY - Contextually adds 960 Grid System classes.
 *
 * The first parameter passed is the *default class*. All other parameters must
 * be set in pairs like so: "$variable, 3". The variable can be anything available
 * within a template file and the integer is the width set for the adjacent box
 * containing that variable.
 *
 *  class="<?php print ns('grid-16', $var_a, 6); ?>"
 *
 * If $var_a contains data, the next parameter (integer) will be subtracted from
 * the default class. See the README.txt file.
 */
function ns() {
  $args = func_get_args();
  $default = array_shift($args);
  // Get the type of class, i.e., 'grid', 'pull', 'push', etc.
  // Also get the default unit for the type to be procesed and returned.
  list($type, $return_unit) = explode('-', $default);

  // Process the conditions.
  $flip_states = array('var' => 'int', 'int' => 'var');
  $state = 'var';
  foreach ($args as $arg) {
    if ($state == 'var') {
      $var_state = !empty($arg);
    }
    elseif ($var_state) {
      $return_unit = $return_unit - $arg;
    }
    $state = $flip_states[$state];
  }

  $output = '';
  // Anything below a value of 1 is not needed.
  if ($return_unit > 0) {
    $output = $type . '-' . $return_unit;
  }
  return $output;
}
/**
 * NINESIXTY - This rearranges how the style sheets are included so the framework styles
 * are included first.
 *
 * Sub-themes can override the framework styles when it contains css files with
 * the same name as a framework style. This can be removed once Drupal supports
 * weighted styles.
 */
function omega_css_reorder($css) {
  global $theme_info, $base_theme_info;

  // Dig into the framework .info data.
  $framework = !empty($base_theme_info) ? $base_theme_info[0]->info : $theme_info->info;

  // Pull framework styles from the themes .info file and place them above all stylesheets.
  if (isset($framework['stylesheets'])) {
    foreach ($framework['stylesheets'] as $media => $styles_from_960) {
      // Setup framework group.
      if (isset($css[$media])) {
        $css[$media] = array_merge(array('framework' => array()), $css[$media]);
      }
      else {
        $css[$media]['framework'] = array();
      }
      foreach ($styles_from_960 as $style_from_960) {
        // Force framework styles to come first.
        if (strpos($style_from_960, 'framework') !== FALSE) {
          $framework_shift = $style_from_960;
          $remove_styles = array($style_from_960);
          // Handle styles that may be overridden from sub-themes.
          foreach ($css[$media]['theme'] as $style_from_var => $preprocess) {
            if ($style_from_960 != $style_from_var && basename($style_from_960) == basename($style_from_var)) {
              $framework_shift = $style_from_var;
              $remove_styles[] = $style_from_var;
              break;
            }
          }
          $css[$media]['framework'][$framework_shift] = TRUE;
          foreach ($remove_styles as $remove_style) {
            unset($css[$media]['theme'][$remove_style]);
          }
        }
      }
    }
  }
  return $css;
}


/**
 * OMEGA - A function to return the alpha and or omega classes based on context
 *
 * @param $vars
 * @param $elements
 * @param $current
 * @param $alpha
 * @param $omega
 * @return classes
 */
function ao($vars, $elements, $current, $alpha = FALSE, $omega = FALSE){
  $classes = array();
  $regions = array();
  // let's get rid of empty elements first
  foreach($elements AS $k => $r) {
    if($vars[$r]) {
        $regions[$k] = $r;
    }
  }
  // now we do another fast loop since we emptied out any blank zones to determine what
  // regions are alpha & omega
  foreach($regions AS $k => $r) {
    $classes[$r] = '';
    if(!$alpha){
      $alpha = TRUE;
      $classes[$r] .= ' alpha';
    }
    if(!$omega && ($k == count($regions) - 1 || count($regions) == 1 )) {
      $omega = TRUE;
      $classes[$r] .= ' omega';
    }
  }
  return $classes[$current];
}