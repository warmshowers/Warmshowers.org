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
    'path' => drupal_get_path('theme', 'warmshowers_zen') . '/templates',
  );
  // Theme colorbox but with no gallery.
  $hooks['colorbox_imagefield_no_gallery'] = array(
    'arguments' => array('namespace' => NULL, 'path' => NULL, 'alt' => NULL, 'title' => NULL, 'gid' => NULL, 'field_name' => NULL, 'attributes' => NULL),
  );
  return $hooks;
}

/**
 * Copies from theme_colorbox_imagefield(), but with no rel= that creates gallery.
 *
 * @param $presetname
 * @param $path
 * @param string $alt
 * @param string $title
 * @param string $gid
 * @param string $field_name
 * @param null $attributes
 * @return string
 */
function warmshowers_zen_colorbox_imagefield_no_gallery($presetname, $path, $alt = '', $title = '', $gid = '', $field_name = '', $attributes = NULL) {
  if (!empty($path)) {
    $image = theme('imagecache', $presetname, $path, $alt, $title, $attributes);
    if ($colorbox_presetname = variable_get('colorbox_imagecache_preset', 0)) {
      $link_path = imagecache_create_url($colorbox_presetname, $path);
    }
    else {
      $link_path = file_create_url($path);
    }
    $class = 'colorbox imagefield imagefield-imagelink imagefield-'. $field_name;

    return l($image, $link_path, array('html' => TRUE, 'attributes' => array('title' => $title, 'class' => $class)));
  }
}
/**
 * Implementation of hook_preprocess_page().
 */
function warmshowers_zen_preprocess_page(&$variables) {

  // On front page, let users know about the iOS app
  if(drupal_is_front_page()) {
    drupal_set_html_head('<meta name="apple-itunes-app" content="app-id=359056872" />');
    $variables['head'] = drupal_get_html_head();
  }

  if (!empty($variables['highlight'])) {
    $variables['classes_array'][] = 'with-highlight';
  }
  global $user;
  foreach ($user->roles as $role){
    $role = str_replace(" ","-",$role);
    $variables['classes_array'][] = 'role-'.$role;
  }
  $variables['classes_array'][] = 'spg-'.array_pop(explode("/", $_GET['q']));

  // Remove breadcrumb from profile pages, but don't remove from template for forums and perhaps other places.
  if (($url_parts = explode("/", $_GET['q'])) && $url_parts[0] == 'user') {
    unset($variables['breadcrumb']);
  }

  // Add links to login, or if logged in, add link to profile
  if (!$variables['logged_in']) {
    $variables['authentication_block'] =  l(t('Sign up'), 'user/register', array('attributes' => array('class' => 'signup'))) .
      l(t('Log in'), 'user', array('attributes' => array('class' => 'login')));
  }
  else {
   $variables['authentication_block'] = t("Logged in as !name | !logout",
     array('!name' => l($user->fullname, 'user/' . $user->uid), '!logout' => l(t('Log out'),'logout')));
  }
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
 * Override privatemsg theming of username.
 *
 * This actually adds a new option 'email', which is for when the name is
 * being viewed in email.
 *
 * @param $recipient
 * @param $options
 * @return mixed|string
 */
function warmshowers_zen_privatemsg_username($recipient, $options) {
  if (!isset($recipient->uid)) {
    $recipient->uid = $recipient->recipient;
  }

  if (!empty($options['email'])) {
    $name = $recipient->fullname;
    if (!empty($options['unique'])) {
      $name .= ' [user]';
    }
    return $name;
  }
  else if (!empty($options['plain'])) {
    $name = $recipient->name;
    if (!empty($options['unique'])) {
      $name .= ' [user]';
    }
    return $name;

  }
  else {
    return theme('username', $recipient);
  }
}

/**
 * Override username to present fullname instead. Experimental.
 * @param $object
 * @return string
 */
function warmshowers_zen_username($object) {

  $name = (!empty($object->fullname) && user_access('access user profiles')) ? $object->fullname : t("WS Member");

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
 * Copied from imagecache_profiles.module
 * (imagecache_profiles_preprocess_user_picture) and adjusted for colorbox.
 * Requires colorbox and imagecache_profiles modules.
 *
 * @param $variables
 */
function warmshowers_zen_preprocess_user_picture(&$variables) {
  $default = $variables['picture'];
  if (variable_get('user_pictures', 0)) {
    $account = $variables['account'];

    if (isset($account->imagecache_preset)) {
      // Manually set preset (e.g. Views)
      $preset = $account->imagecache_preset;
    }
    elseif (variable_get('user_picture_imagecache_profiles_default', '')) {
      // Default user picture preset.
      $preset = variable_get('user_picture_imagecache_profiles_default', '');
    }

    if (!empty($account->picture) && file_exists($account->picture)) {
      $picture = $account->picture;
    }
    elseif (variable_get('user_picture_default', '')) {
      $picture = variable_get('user_picture_default', '');
    }

    if (isset($picture)) {
      $name = (!empty($account->fullname) && user_access('access user profiles')) ? $account->fullname : t("WS Member");
      $alt = t("@user's picture", array('@user' => $name));
      if (isset($preset)) {
        $preset = is_numeric($preset) ? imagecache_preset($preset) : imagecache_preset_by_name($preset);
      }
      if (empty($preset)) {
        $variables['picture'] = $default; //theme('image', $picture, $alt, $alt, '', FALSE);
      }
      else {
        if (!empty($account->uid)) {
          $variables['picture'] = theme('colorbox_imagefield_no_gallery', $preset['presetname'], $picture, $alt, $alt);
        }
      }
    }
  }
}

/**
 * Preprocess the classes variable for certain flags
 */
function warmshowers_zen_preprocess_flag(&$variables) {
  // Add specific link attributes to our responsive buttons
  switch (@$variables['flag']->name){
    case "unresponsive_member":
    case "responsive_member":
      $variables['flag_classes'] .= " rounded green lgrounded";
    default: break;
  }
}
