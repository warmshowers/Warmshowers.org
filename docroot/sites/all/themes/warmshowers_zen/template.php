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

  // Add classes for all populated theme regions to show which regions are on the page.
  $regions = system_region_list('warmshowers_zen');
  if (isset($regions)) {
    foreach ($regions as $key=>$region) {
      if (isset($variables[$key])) {
        $variables['classes_array'][] = drupal_html_class("has-region-{$key}");
      }
    }
  }
  // Add classes for all roles a user has.
  $roles_include = array(
      'anonymous user',
      'authenticated user',
      'donation-free',
      'current-member',
  );
  foreach ($user->roles as $role){
    if (in_array($role, $roles_include)) {
      $variables['classes_array'][] = drupal_html_class("user-has-role-{$role}");
    }
  }
  // Add a class for the user profile page if not already created by base theme.
  if (arg(0) == 'user' && is_numeric(arg(1)) && arg(2) == NULL) {
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
   *
   * @TODO: Not implemented yet, but consider replacing menu blocks with this.
   */
  _warmshowers_zen_generate_menus($variables);

  // Remove breadcrumb from profile pages, but don't remove from template for forums and perhaps other places.
  if (arg(0) == 'user' && is_numeric(arg(1))) {
    unset($variables['breadcrumb']);
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
 */
function warmshowers_zen_preprocess_node(&$variables) {

  //@TODO: Only in here so that I don't have to clear registry if I want to add some block output. Remove if empty.
}

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 */
function warmshowers_zen_preprocess_region(&$variables) {

  // Count how many blocks are in each region
  $blocks = (count(element_children($variables['elements'])));
  $variables['classes_array'][] = drupal_html_class("region-block-count--{$blocks}");
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 */
function warmshowers_zen_preprocess_block(&$variables) {

  $variables['edit_links'] = !empty($variables['edit_links']) ? $variables['edit_links'] : NULL;

  if (isset($variables['block']->delta)) {
    $variables['classes_array'][] = drupal_html_class("block-delta__{$variables['block']->delta}");
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
  // TODO: Is this really needed if we're using better messages??
  // TODO: this probably changed, it looks like messages are
  // empty so check out the function theme_status_message in D7
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
 * Overriding of drupal theme_form_element().
 *
 * @param $variables
 * @return string
 */
function warmshowers_zen_form_element($variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      if (!empty($element['#description'])) {
        $output .= '<div class="description">' . $element['#description'] . "</div>\n";
      }
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      if (!empty($element['#description'])) {
        $output .= '<div class="description">' . $element['#description'] . "</div>\n";
      }
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

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
function warmshowers_zen_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => 'form-required label-text',
    'title' => $t('This field is required.'),
  );
  $output = '<span' . drupal_attributes($attributes) . '>: ' . $attributes['title'] . '</span>';

  $attributes['class'] = 'form-required label-marker';
  $output .= '<span' . drupal_attributes($attributes) . '>*</span>';

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
  $recipient = $variables['recipient'];
  $options = $variables['options'];
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
    return theme('username', array('account' => $recipient));
  }
}

/**
 * Override username to present fullname instead. Experimental.
 * @param $variables
 * @return string
 */
function warmshowers_zen_username($variables) {
  $account = $variables['account'];
  $name = warmshowers_zen_sanitized_username($variables);

  if ($account->uid && $name) {
    // Shorten the name when it is too long or it will break many tables.
    if (drupal_strlen($name) > 22) {
      $name = drupal_substr($name, 0, 18) . '...';
    }

    if (user_access('access user profiles')) {
      $output = l($name, 'user/' . $account->uid, array('attributes' => array('title' => t('View user profile.'))));
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
 * @TODO: Must check this function thoroughly, a lot has changed regarding profile pictures.
 *
 * @param $variables
 */
function warmshowers_zen_preprocess_user_picture(&$variables) {
  $default = isset($variables['picture']) ? $variables['picture'] : NULL;

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

    if (!empty($account->picture->uri) && file_exists($account->picture->uri)) {
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

  $title = t('Thanks for your support');

  $product = array_shift(array_values($order->products));
  $model = $product->model;

  drupal_set_title($title);

  if ($order->order_total == 0) {
    switch ($model) {
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
