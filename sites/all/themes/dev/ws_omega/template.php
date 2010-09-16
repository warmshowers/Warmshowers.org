<?php
// $Id$

/*
 * Add any conditional stylesheets you will need for this sub-theme.
 *
 * To add stylesheets that ALWAYS need to be included, you should add them to
 * your .info file instead. Only use this section if you are including
 * stylesheets based on certain conditions.
 */
/* -- Delete this line if you want to use and modify this code
// Example: optionally add a fixed width CSS file.
if (theme_get_setting('ws_omega_fixed')) {
  drupal_add_css(path_to_theme() . '/layout-fixed.css', 'theme', 'all');
}
// */


/**
 * Implementation of HOOK_theme().
 */
function ws_omega_theme(&$existing, $type, $theme, $path) {
  $hooks = omega_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function ws_omega_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function ws_omega_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function ws_omega_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function ws_omega_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function ws_omega_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */


/**
 * Create a string of attributes form a provided array.
 *
 * @param $attributes
 * @return string
 */
function ws_omega_render_attributes($attributes) {
	return omega_render_attributes($attributes);
}


function ws_omega_welcome_page_pics() {
  $limit = 10;   // Number of items to get in the query
  $numboxes = 5; // Number of boxes on the page
  $count = 0;
  $cache_timeout = 30 * 60;   // Number of seconds before we dump cache
  $anon = $GLOBALS['user']->uid ? "loggedin" : "anon";

  if ($cache = cache_get("warmshowerspb_header_pictures.$anon")) {
    $html = $cache->data;
  } else {
    $imagecache_enabled = module_exists('imagecache');
    $max_uid  = db_result(db_query("SELECT MAX(uid) FROM {users}"));
    $start_uid = rand(1000,$max_uid-100);

    $result1 = db_query_range("SELECT u.uid,w.fullname, u.picture
      FROM {users} u, {wsuser} w
      WHERE u.picture != '' and u.uid=w.uid and u.status
      and !w.isstale
      and !w.isunreachable
      and u.uid > %d
     ", $start_uid, 0, $limit);

    $html = '<ul class="mempics">';
     while ( ($pic = db_fetch_object($result1)) && $count <$numboxes) {
       if (!file_exists($pic->picture)) {
         continue;
       }
                $item = theme('image',$pic->picture, $pic->fullname, $pic->fullname);
       if (user_access('access user profiles')) {
         $item = l($item,"user/$pic->uid",array('html'=>TRUE));
       }
       $html .= "<li>$item</li>\n";
       $count++;

       if ($count >= $numboxes) {
         break;
       }
     }
     $html .= "</ul>";

     // Cache this html
     cache_set("warmshowerspb_header_pictures.$anon",$html,'cache', time() + $cache_timeout);
  }
  return $html;
}