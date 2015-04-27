<?php

/**
 * @file
 * Theme functions for the uc_ups module.
 */

/**
 * Theme function to format the UPS service name and rate amount line-item
 * shown to the customer.
 *
 * @param $variables
 *   Associative array containing information needed to theme a quote.
 *   Contains two keys:
 *   - service: The UPS service name.
 *   - packages: Package information.
 *
 * @ingroup themeable
 */
function theme_uc_ups_option_label($variables) {
  $service = $variables['service'];
  $packages = $variables['packages'];

  // Start with logo as required by the UPS terms of service.
  $output  = theme('image', array(
    'path' => drupal_get_path('module', 'uc_ups') . '/uc_ups_logo.gif',
    'alt' => t('UPS logo'),
    'attributes' => array('class' => 'ups-logo')
  ));

  // Add the UPS service name.
  $output .= t('@service Rate', array('@service' => $service));

  // Add package information
  $output .= ' (' . format_plural(count($packages), '1 package', '@count packages') . ')';

  return $output;
}
