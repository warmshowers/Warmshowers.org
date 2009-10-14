<?php
// $Id$

// Include the definition of zen_settings() and zen_theme_get_default_settings().
include_once './' . drupal_get_path('theme', 'omega') . '/theme-settings.php';
/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function omega_starterkit_settings($saved_settings) {
  // Get the default values from the .info file.
  //krumo($saved_settings);
  $subtheme_defaults = omega_theme_get_default_settings('omega_starterkit');
  //krumo($defaults);
  // Merge the saved variables and their default values.
  //$settings = array_merge($defaults, $saved_settings);
  $form = array();
  // Add the base theme's settings.
  $form += omega_settings($saved_settings, $subtheme_defaults);
  // Return the form
  return $form;
}
