<?php
/**
 * @file
 */

/**
 * Theme a donations thermometer.
 * */
function template_preprocess_donations_thermometer(&$vars) {

  $amount = $vars['amount'];
  $target = $vars['target'];
  $currency = $vars['currency'];
  $size = $vars['size'];

  drupal_add_js(drupal_get_path('module', 'donations_thermometer') . '/js/donations_thermometer.js');
  drupal_add_css(drupal_get_path('module', 'donations_thermometer') . '/css/donations_thermometer.css');

  $percent = ($amount / $target) * 100;
  if ($percent > 100) {
    $percent = 100;
  }
  $vars['percent'] = $percent;
}