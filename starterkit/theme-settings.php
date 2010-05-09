<?php
// $Id$

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
function omega_starterkit_form_system_theme_settings_alter(&$form, &$form_state) {
  
  // Return the form
  return $form;
}
