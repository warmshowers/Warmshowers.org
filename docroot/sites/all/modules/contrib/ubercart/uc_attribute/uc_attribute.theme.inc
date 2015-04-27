<?php

/**
 * @file
 * Theme functions for the uc_attribute module.
 */

/**
 * Displays an attribute option with an optional total or adjustment price.
 *
 * @param $variables
 *   An associative array containing:
 *   - option: The option name.
 *   - price: The price total or adjustment, if any.
 *
 * @see _uc_attribute_alter_form()
 * @ingroup themeable
 */
function theme_uc_attribute_option($variables) {
  $output = $variables['option'];
  if ($variables['price']) {
    $output .= ', ' . $variables['price'];
  }
  return $output;
}

/**
 * Displays the attribute selection form elements.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see _uc_attribute_alter_form()
 * @ingroup themeable
 */
function theme_uc_attribute_add_to_cart($variables) {
  $form = $variables['form'];

  $output = '<div id="' . $form['#id'] . '" class="attributes">';
  $stripes = array('even' => 'odd', 'odd' => 'even');
  $parity = 'even';
  foreach (element_children($form) as $aid) {
    $parity = $stripes[$parity];
    $classes = array('attribute', 'attribute-' . $aid, $parity);
    $output .= '<div class="' . implode(' ', $classes) . '">';
    $output .= drupal_render($form[$aid]);
    $output .= '</div>';
  }

  $output .= drupal_render_children($form) . '</div>';

  return $output;
}
