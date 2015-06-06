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

  // Theme colorbox but with no gallery.
  // TODO: If it's really just to remove one html attribute then there must be a more efficient method???
  $hooks['colorbox_imagefield_no_gallery'] = array(
    'variables' => array(
        'namespace' => NULL,
        'path' => NULL,
        'alt' => NULL,
        'title' => NULL,
        'gid' => NULL,
        'field_name' => NULL,
        'attributes' => NULL
    ),
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
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function warmshowers_zen_preprocess_html(&$variables, $hook) {
  // TODO: Consider using https://www.drupal.org/project/metatag instead.
  // Until then these images should reside in the theme.
  // Also suggest a reasonable image for shares to facebook
  $ws_image = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image',
      'content' => drupal_get_path('theme', 'warmshowers_zen') . '/imv/ws-og-image.png',
    ),
  );
  $ws_image_secure = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image:secure_url',
      'content' => drupal_get_path('theme', 'warmshowers_zen') . '/img/ws-og-image.png',
    ),
  );
  drupal_add_html_head($ws_image, 'ws-image');
  drupal_add_html_head($ws_image_secure, 'ws-image-secure');

  // On front page, let users know about the iOS app
  if(drupal_is_front_page()) {
    $ios_app = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'al:ios:app_store_id',
        'content' => '359056872',
      ),
    );
    drupal_add_html_head($ios_app, 'apple-itunes-app');
  }
  $variables['head'] = drupal_get_html_head();

  /*
   * Add page classes depending on the following logic:
   */
  _warmshowers_zen_add_html_classes($variables);
}

/**
 * Helper function to add relevant classes to describe each page.
 *
 * @param array $variables
 */
function _warmshowers_zen_add_html_classes(&$variables) {
  global $user;
  $args = arg();

  // Add classes for all populated theme regions
  $regions = system_region_list('warmshowers_zen');
  if (isset($regions)) {
    foreach ($regions as $key=>$region) {
      if (isset($variables[$key])) {
        $variables['classes_array'][] = drupal_html_class("has-region-{$key}");
      }
    }
  }
  // Add classes for all roles a user has
  foreach ($user->roles as $role){
    $variables['classes_array'][] = drupal_html_class("user-has-role-{$role}");
  }
  // Add classes for page node type
  if ($arg[0] == 'user') {
    // @TODO: New logic.
  }
  if (($url_parts = explode("/", $_GET['q'])) && $url_parts[0] == 'user') {
    $variables['classes_array'][] = 'page-user-profile';
  }
}

/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function warmshowers_zen_preprocess_page(&$variables, $hook) {
  global $user;

  /*
   * Generate renderable menu arrays
   */
  _warmshowers_zen_generate_menus($variables);

  // Remove breadcrumb from profile pages, but don't remove from template for forums and perhaps other places.
  if (($url_parts = explode("/", $_GET['q'])) && $url_parts[0] == 'user') {
    unset($variables['breadcrumb']);
  }
  // Add links to login, or if logged in, add link to profile
  if (!$variables['logged_in']) {
    $variables['authentication_block'] =  l(t('Sign up'), 'user/register', array('attributes' => array('class' => array('signup')))) .
      l(t('Log in'), 'user', array('attributes' => array('class' => array('login'))));
  }
  else {
    $variables['authentication_block'] = t(
      "Logged in as !name | !logout",
      array(
        '!name' => l($user->data['fullname'], 'user/' . $user->uid),
        '!logout' => l(t('Log out'),'logout')
      )
    );
  }

}

/**
 * Helper function to generate menu arrays ready for rendering.
 *
 * @param array $variables
 */
function _warmshowers_zen_generate_menus(&$variables) {
  // Primary nav.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Build links.
    $variables['primary_nav'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Build links.
    $variables['secondary_nav'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function warmshowers_zen_preprocess_node(&$variables, $hook) {

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
  // TODO: Is this really needed if we're using better messages??
  $display = $variables['display'];
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
    $name = strip_tags(format_username($recipient));
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
        // TODO: this line is removing the picture that may potentially be there
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
