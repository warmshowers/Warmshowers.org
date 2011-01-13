<?php
// $Id: theme-settings.php,v 1.1.2.7 2010/06/14 13:17:05 himerus Exp $

// Include the definition of omega_settings() and omega_theme_get_default_settings().
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
  $subtheme_defaults = omega_theme_get_default_settings('omega_starterkit');
  // Merge the saved variables and their default values.
  $form = array();
  // Add the base theme's settings.
  $form += omega_settings($saved_settings, $subtheme_defaults);
  // Return the form
  return $form;
}
