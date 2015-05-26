<?php

/**
 * @file: Advanced Poll administration.
 */

/**
 * Admin settings form.
 */
function advpoll_settings_form($form, &$form_state) {
  $form = array();

  $form['advpoll_percentage_precision'] = array(
    '#type' => 'textfield',
    '#title' => t('Percentage precision'),
    '#description' => t('Enter the number of decimal digits to display percentage tallies by (0 will display whole percentages only)'),
    '#default_value' => variable_get('advpoll_percentage_precision', 0),

  );

  $form['percentage_rounding_method'] = array(
    '#type' => 'radios',
    '#title' => t('Percentage rounding method'),
    '#description' => t('Select rounding method to use when displaying percentage vote counts. Round is usually the best choice.'),
    '#options' => array(
      'floor' => t('Floor: round percentages down'),
      'round' => t('Round: round percentages up if greater than .5'),
    ),
    '#default_value' => variable_get('percentage_rounding_method', 'round'),
  );

  return system_settings_form($form);
}

/**
 * Validate admin settings form input.
 */
function advpoll_settings_form_validate($form, &$form_state) {
  // Ensure precision is an integer.
  if(!empty($form_state['values']['advpoll_percentage_precision']) && !is_numeric($form_state['values']['advpoll_percentage_precision'])) {
    form_set_error('advpoll_percentage_precision', t('Percentage precision must be an integer.'));
  }
}
