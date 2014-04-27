<?php

/**
 * @file
 * Administrative interface for environment indicator configuration.
 *
 * @author Tom Kirkpatrick (mrfelton), www.systemseed.com
 */

/**
 * Module settings form.
 */
function environment_indicator_settings() {
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'environment_indicator') .'/environment_indicator.admin.js');
  
  $form['environment_indicator_enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#default_value' => variable_get('environment_indicator_enabled', 1)? '1' : '0',
    '#options' => array(t('Disabled'), t('Enabled')),
    '#description' => t('Should the Environment Indicator display?'),
  );
  $form['environment_indicator_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#description' => t('Choose a position for the environment indicator.'),
    '#options' => array(
      'left' => t('Left'),
      'right' => t('Right'),
    ),
    '#default_value' => variable_get('environment_indicator_position', 'left'),
  );
  $form['environment_indicator_margin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Adjust margin'),
    '#default_value' => variable_get('environment_indicator_margin', 1),
    '#description' => t('If enabled, the site output is shifted to the left or right approximately 30 pixels to display the environment indicator. If disabled, some absolute- or fixed-positioned page elements may be covered by the environment indicator strip.'),
  );
  $form['environment_indicator_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text to display'),
    '#default_value' => variable_get('environment_indicator_text', 'ENVIRONMENT INDICATOR'),
    '#description' => t('Text to display in the environment indicator. Override this for each environment in settings.php.'),
  );
  $form['environment_indicator_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Colour'),
    '#default_value' => variable_get('environment_indicator_color', '#d00c0c'),
    '#description' => t('The colour of the environment indicator. Override this for each environment in settings.php.'),
    '#size' => 7,
  );
  $form['environment_indicator_color_picker'] = array(
    '#type' => 'markup',
    '#value' => '<div id="environment-indicator-color-picker"></div>',
  );

  $form['environment_indicator_suppress_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Turn off Environment Indicator on these pages'),
    '#default_value' => variable_get('environment_indicator_suppress_pages', "imagecrop/*\n"),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are '<em>blog</em>' for the blog page and '<em>blog/*</em>' for every personal blog. '<em>&lt;front&gt;</em>' is the front page."),
  );
  return system_settings_form($form);
}
