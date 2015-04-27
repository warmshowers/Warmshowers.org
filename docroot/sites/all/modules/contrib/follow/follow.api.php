<?php

/**
 * Alter the available networks to the Follow module.
 *
 * @param $networks
 *   Associative array of networks that are available.
 * @param $uid
 *   The User ID of the networks to be displayed. If 0 is provided, will be the
 *   networks for the website rather then an individual user.
 */
function hook_follow_networks_alter(&$networks, $uid = 0) {
  // Add a network.
  $networks[$uid]['mailinglist'] = array(
    'title' => t('Mailing List'),
    'domain' => 'mailinglist.domain.com',
  );

  // Replace Twitter with Identi.ca
  unset($networks[$uid]['twitter']);
  $networks[$uid]['identica'] = array(
    'title' => t('Identi.ca'),
    'domain' => 'identi.ca',
  );
}

/**
 * Alter the available icon styles.
 *
 * @param array $styles
 *   An array of icon styles.
 */
function hook_follow_icon_styles_alter(&$styles) {
  $styles['my-style'] = array(
    'name' => 'my-style',
    'label' => t('My Custom Icon Style'),
    // An array of CSS overrides. These  overrides will only apply to the
    // generic anchor (a.follow-link). If you would like something more
    // customized than this, it is recommended you add it to your own custom
    // stylesheet, or use the CSS Injector module. NOTE: if you change the CSS
    // overrides after the style is already in use, you will have to delete the
    // generated CSS file in your public files directory.
    'css-overrides' => array(
      'height: 44px;',
      'line-height: 40px;',
      'padding-left: 41px;',
      'padding-right: 5px;',
    ),
    // You can specify a custom path to your icons. Your icons must be named
    // appropriately in order for them to work. Please see the small or large
    // icons in the follow module directory for an example.
    'icon-path' => 'sites/all/themes/mytheme/follow-icons',
  );

}
