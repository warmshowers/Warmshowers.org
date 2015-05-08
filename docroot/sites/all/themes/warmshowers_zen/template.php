<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */


/**
 * Implements hook_theme().
 */
function warmshowers_zen_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  $hooks['user_login_block'] = array(
    'template' => 'user-login-block',
    'variables' => array('form' => NULL),
    'path' => drupal_get_path('theme', 'warmshowers_zen') . '/templates',
  );
  // Theme colorbox but with no gallery.
  $hooks['colorbox_imagefield_no_gallery'] = array(
    'variables' => array('namespace' => NULL, 'path' => NULL, 'alt' => NULL, 'title' => NULL, 'gid' => NULL, 'field_name' => NULL, 'attributes' => NULL),
  );
  return $hooks;
}

/**
 * Copies from theme_colorbox_imagefield(), but with no rel= that creates gallery.
 *
 * @param $variables
 * @return string
 */
function warmshowers_zen_colorbox_imagefield_no_gallery($variables) {
  $path = $variables['path'];
  if (!empty($path)) {
    $image = theme('imagecache', $variables['presetname'], $variables['path'], $variables['alt'], $variables['title'], $variables['attributes']);
    if ($colorbox_presetname = variable_get('colorbox_imagecache_preset', 0)) {
      $link_path = imagecache_create_url($colorbox_presetname, $path);
    }
    else {
      $link_path = file_create_url($path);
    }
    $class = 'colorbox imagefield imagefield-imagelink imagefield-'. $variables['field_name'];

    return l($image, $link_path, array('html' => TRUE, 'attributes' => array('title' => $variables['title'], 'class' => $class)));
  }
}

/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  warmshowers_zen_preprocess_html($variables, $hook);
  warmshowers_zen_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_html(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function warmshowers_zen_preprocess_page(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Suggest a reasonable image for shares to facebook
  // @annetee it looks like drupal_add_html_head has changed to expect an
  // array, so this doesn't show up in the source of the page
  drupal_add_html_head('<meta property="og:image" content="https://www.warmshowers.org/files/ws-icon-v1_0.png" />
  <meta property="og:image:secure_url" content="https://www.warmshowers.org/files/ws-icon-v1_0.png" />');

  // On front page, let users know about the iOS app
  // @annetee it looks like drupal_add_html_head has changed to expect an
  // array, so this doesn't show up in the source of the page
  if(drupal_is_front_page()) {
    drupal_add_html_head('<meta name="apple-itunes-app" content="app-id=359056872" />');
  }
  $variables['head'] = drupal_get_html_head();

  if (!empty($variables['highlighted'])) {
    $variables['classes_array'][] = 'with-highlight';
  }
  global $user;
  foreach ($user->roles as $role){
    $role = str_replace(" ","-",$role);
    $variables['classes_array'][] = 'role-'.$role;
  }
  $variables['classes_array'][] = 'spg-'.array_pop(explode("/", $_GET['q']));

  // Set page-user-profile type if we're on profile page.
  if ($variables['menu_item']['path'] == 'user/%') {
    $variables['classes_array'][] = drupal_html_class('page-user-profile');
  }

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
   // @annetee $user->fullname variable is not properly loaded by
   // guessing wsuser custom module, so it's empty
     array('!name' => l($user->fullname, 'user/' . $user->uid), '!logout' => l(t('Log out'),'logout')));
  }

}

/**
 * This is a basic copy of theme_status_message. We add a div to help us with
 * our new layout of icons and different background colors.
 *
 * Return a themed set of status and/or error messages. The messages are grouped
 * by type.
 *
 * @param $variables
 *   (optional) Set to 'status' or 'error' to display only messages of that type.
 *
 * @return string
 *   A string containing the messages.
 */
function warmshowers_zen_status_messages($variables) {
  // @annetee, this probably changed, it looks like messages are
  // empty so check out the function theme_status_message in D7
  $display = $variables ['display'];
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
 * @param $variables
 * @return string
 */
function warmshowers_zen_form_element($variables) {
  $element = $variables ['element'];
  $value = $variables ['value'];
  // This looks just awful on checkbox, so use classic theming for it.
  if ($element['#type'] == 'checkbox') {
    return theme_form_element($element, $value);
  }
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
 * @param $variables
 * @return mixed|string
 */
function warmshowers_zen_privatemsg_username($variables) {
  $recipient = $variables ['recipient'];
  $options = $variables ['options'];
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
 * @param $variables
 * @return string
 */
function warmshowers_zen_username($variables) {
  $object = $variables ['object'];
  $name = warmshowers_zen_sanitized_username($object);

  if ($object->uid && $name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($name) > 22) {
      $name = drupal_substr($name, 0, 18) . '...';
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
 * Custom function to sanitize username
 *
 * We want to use fullname generally for *member* access. But it's not always
 * populated, in that case use username. But it might have an email address in it;
 * in which case use the user part of the email address.
 *
 * For unauth access, we'll just use 'WS Member'
 *
 * @param $variables
 *   User object
 * @return name to use
 */
function warmshowers_zen_sanitized_username($variables) {
  $account = $variables['account'];
  $name = t('WS Member');
  if (user_access('access user profiles')) {
    if (!empty($account->fullname)) {
      $name = $account->fullname;
    }
    else {
      // Some members use email as username, we don't want to display.
      list($name) = preg_split('/@/', $account->name);
    }
  }
  // Otherwise, no access to profiles, so just use 'WS member', the default.
  return $name;
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
      $name = (!empty($account->name) && user_access('access user profiles')) ? $account->name : t("WS Member");
      list($name) = preg_split('/@/', $name);
      $alt = t("@user's picture", array('@user' => $name));
      if (isset($preset)) {
        $preset = is_numeric($preset) ? imagecache_preset($preset) : imagecache_preset_by_name($preset);
      }
      if (empty($preset)) {
        // @annetee this line is removing the picture that may potentially be there
        // It's probably because there is no preset and there should be
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


/**
 * Override theming of donations thermometer
 *
 * @param $variables
 * @return string
 */
function warmshowers_zen_donations_thermometer($variables) {
  $amount = $variables['amount'];
  $target = $variables['target'];
  $currency = $variables['currency'];
  // TODO: Set default value to 'large'
  $size = $variables['size'];

  drupal_add_js(drupal_get_path('module', 'donations_thermometer') .'/donations_thermometer.js');
  drupal_add_css(drupal_get_path('module', 'donations_thermometer') .'/donations_thermometer.css');

  $account = user_load($GLOBALS['user']->uid);

  $percent = ($amount/$target)*100;
  $text = '<div class="donations_thermometer">


    <div class="gauge-' . $size . '">
    <div class="current-value" id="campaign-progress-current" style="height:'. $percent .'%;">
    <p>'. $percent .'% </p>
    </div>
    </div>
    <p class="donations-text-status">
    <span class="donations_header">' . t('Membership Donations') . '</span>
    <span class="donations_thermometer-label"> ' . t('Raised so far') . ':</span><span class="donations_thermometer-amount"> '. $currency . number_format($amount) .'</span><br/><span class="donations_thermometer-label">' . t('Goal') . ':</span><span class="donations_thermometer-amount"> '. $currency . number_format($target) .'</span><br/>';

  // Sorry to do logic here... but it keeps from forking donations_thermometer :-)
  if (wsuser_is_current_donor_member($GLOBALS['user'])) {
    $text .= t('Thanks for your generous contribution, @fullname', array('@fullname' => $account->fullname));
  }
  else if (wsuser_is_nondonor_member($account)){
    $text .= t('Thanks for choosing a membership level, @fullname!', array('@fullname' => $account->fullname));
  } else {
    $text .= l(t('Choose Membership Level and Donate'), 'donate', array('attributes' => array('class' => 'linkbutton rounded light')));
    $text .= '<br/>' . l(t('Membership FAQs'), 'faq/donations-and-membership-levels');
  }

  $text .=' </p></div>';

  return $text;
}


/**
 * Customize the message on the checkout-complete page, since we have
 * free orders that shouldn't say "donate"
 *
 * @param $variables
 * @return string
 */
function warmshowers_zen_uc_cart_complete_sale($variables) {
  $message = $variables['message'];
  $order = $variables['order'];
  $x = 1;

  $title = t('Thanks for your support');

  $product = $order->products[0]->model;

  drupal_set_title($title);

  if ($order->order_total == 0) {
    switch ($product) {
      case 'membership_hostingonly':
        $message = t('You incredible hosts are the backbone of our community, the ones that really make it happen. Thanks so much for showing your support by selecting the hosting-only donation level.');
        break;
      case 'membership_free':
      case 'membership_trial':
        $message = t('Thank you for being part of the Warm Showers community!  Your participation in hosting, riding, and giving valuable feedback is the fuel that keeps the community going.  If in the future you are able, please consider contributing a monetary donation as well, to help offset the growing costs associated with a global hospitality program.');
        break;

    }
  }
  return $message;

}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // warmshowers_zen_preprocess_node_page() or warmshowers_zen_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function warmshowers_zen_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */
