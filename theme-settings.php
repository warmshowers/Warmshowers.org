<?php
// $Id$
// require_once for the functions that need to be available when we are outside
// of the omega theme in the administrative interface
require_once './'. drupal_get_path('theme', 'omega') ."/theme-functions.inc";
/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function omega_settings($saved_settings) {
	$defaults = array(
    'user_notverified_display'              => 1,
    'breadcrumb_display'                    => 1,
    'mission_statement_pages'               => 'home',
    'rebuild_registry'                      => 0,
  );
	$defaults = array_merge($defaults, theme_get_settings());
	$settings = array_merge($defaults, $saved_settings);
  $form['omega_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Omega 960 settings'),
    '#description' => t('Core configuration options for the Omega theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => false,
  );
  // General Settings
  $form['omega_container']['omega_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#description' => t('Configure generic options on rendering content in this theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
	  // Mission Statement
	  $form['omega_container']['omega_general']['mission_statement'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Mission statement'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
	  );
	  $form['omega_container']['omega_general']['mission_statement']['mission_statement_pages'] = array(
	    '#type'          => 'radios',
	    '#title'         => t('Where should your mission statement be displayed?'),
	    '#default_value' => omega_vars('mission_statement_pages', 'home'),
	    '#options'       => array(
	                          'home' => t('Display mission statement only on front page'),
	                          'all' => t('Display mission statement on all pages'),
	                        ),
	  );
	  // Breadcrumb
	  $form['omega_container']['omega_general']['breadcrumb'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Breadcrumb'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
	  );
	  $form['omega_container']['omega_general']['breadcrumb']['breadcrumb_display'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Display breadcrumb'),
	    '#default_value' => omega_vars('breadcrumb_display', 1),
	  );
  // Region Settings
  $form['omega_container']['omega_regions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Region Settings'),
    '#description' => t('Configure how your regions are rendered.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['omega_container']['omega_regions']['headers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Header Configuration'),
    '#description' => t('Header region zones, including Primary & Secondary menus, Header first and Header Last.'),
    '#collapsible' => TRUE,
    '#collapsed' => false,
  );
  
  // Return theme settings form
  return $form;
}  