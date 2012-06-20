<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to warmshowers_zen_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: warmshowers_zen_breadcrumb()
 *
 *   where warmshowers_zen is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
function warmshowers_zen_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  $hooks['user_login_block'] = array(
    'template' => 'user-login-block',
    'arguments' => array('form' => NULL),
  );
  return $hooks;
}

/**
 * Implementation of hook_preprocess_page().
 */
function warmshowers_zen_preprocess_page(&$variables) {
  if (!empty($variables['highlight'])) {
    $variables['classes_array'][] = 'with-highlight';
  }
  global $user;
  foreach ($user->roles as $role){
    $role = str_replace(" ","-",$role);
    $variables['classes_array'][] = 'role-'.$role;
  }

  // Remove breadcrumb from profile pages, but don't remove from template for forums and perhaps other places.
  if (($url_parts = explode("/", $_GET['q'])) && $url_parts[0] == 'user') {
    unset($variables['breadcrumb']);
  }
}

/**
 * Implementation of hook_preprocess_user_profile().
 */
function warmshowers_zen_preprocess_user_profile(&$variables) {

  drupal_add_css(drupal_get_path('theme','warmshowers_zen') . '/css/profile.css', 'theme');

}

/**
 * This is a basic copy of theme_status_message.  We add a div to help us with our new layout of icons and different
 * background colors.
 * Return a themed set of status and/or error messages. The messages are grouped
 * by type.
 *
 * @param $display
 *   (optional) Set to 'status' or 'error' to display only messages of that type.
 *
 * @return
 *   A string containing the messages.
 */
function warmshowers_zen_status_messages($display = NULL) {
  $output = '';
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= "<div class=\"messages $type\"><div class=\"message\">\n";
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>'. $message ."</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div></div>\n";
  }
  return $output;
}

/**
 * Replace theme('form_element') to put the description ahead of the form element.
 *
 * @param $element
 * @param $value
 * @return string
 */
function warmshowers_zen_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  $output = '<div class="form-item"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . $t('This field is required.') . '">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="' . $element['#id'] . '">' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
    else {
      $output .= ' <label>' . $t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
  }
  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }
  $output .= " $value\n";
  $output .= "</div>\n";

  return $output;
}


/**
 * Override username to present fullname instead. Experimental.
 * @param $object
 * @return string
 */
function warmshowers_zen_username($object) {

  $name = !empty($object->fullname) ? $object->fullname : $object->name;

  if ($object->uid && $name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($name) > 20) {
      $name = drupal_substr($name, 0, 15) . '...';
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/' . $object->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      $output = check_plain($name);
    }
  }
  else {
    $output = check_plain(variable_get('unregistered', t('Unregistered')));
  }

  return $output;
}

/**
 * Override template_preprocess_user_picture().
 *
 * Copied from imagecache_profiles.module and adjusted for thickbox.
 * Requires thickbox and imagecache_profiles modules.
 *
 * @param $variables
 */
function warmshowers_zen_preprocess_user_picture(&$variables) {
  $default = $variables['picture'];
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];
    // Determine imagecache preset to use for user profile picture
    // First let's determine if we have a default imagecache preset
    if (variable_get('user_picture_imagecache_profiles_default', 0)) {
      // Define default user picture size
      $size = variable_get('user_picture_imagecache_profiles_default', 0);
    }
    // If on user profile page.
    if (arg(0) == 'user') {
      // Only show profile image for profile page, and edit account form,
      // not user/123/relationships or other module define pages.
      if (arg(2) == NULL || arg(2) == 'edit') {
        if (is_numeric(arg(1)) || (module_exists('me') && arg(1) == me_variable_get('me_alias'))) {
          if (variable_get('user_picture_imagecache_profiles', 0)) {
            $size = variable_get('user_picture_imagecache_profiles', 0);
          }
        }
      }
    }
    // If viewing a comment
    if (is_object($account) && array_key_exists('cid', get_object_vars($account))) {
      if (variable_get('user_picture_imagecache_comments', 0)) {
        $size = variable_get('user_picture_imagecache_comments', 0);
      }
    }

    // If views set an imagecache preset
    if (isset($account->imagecache_preset)) {
      $size = $account->imagecache_preset;
    }

    if (!empty($account->picture) && file_exists($account->picture)) {
      $picture = $account->picture;
    }
    else if (variable_get('user_picture_default', '')) {
      $picture = variable_get('user_picture_default', '');
    }

    if (isset($picture)) {
      $alt = t("@user's picture", array('@user' => $account->name ? $account->name : variable_get('anonymous', t('Anonymous'))));
      $preset = is_numeric($size) ? imagecache_preset($size) : imagecache_preset_by_name($size);
      if (empty($preset)) {
        $variables['picture'] = $default; //theme('image', $picture, $alt, $alt, '', FALSE);
      }
      else {
        if (!empty($account->uid) && user_access('access user profiles')) {
          $title = check_plain($account->fullname);
          $attributes = array('attributes' => array('title' => $title), 'html' => TRUE);
          $image = theme('imagefield_image_imagecache_thickbox', $preset['presetname'], $picture, $alt, $title);
          $variables['picture'] = l($image, "user/$account->uid", $attributes);
        }
        else {
          $variables['picture'] = theme('imagefield_image_imagecache_thickbox', $preset['presetname'], $picture, $alt, $alt);
        }
      }
    }
  }
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
function warmshowers_zen_preprocess(&$vars, $hook) {
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
function warmshowers_zen_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
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
function warmshowers_zen_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // warmshowers_zen_preprocess_node_page() or warmshowers_zen_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
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
function warmshowers_zen_preprocess_comment(&$vars, $hook) {
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
function warmshowers_zen_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */
