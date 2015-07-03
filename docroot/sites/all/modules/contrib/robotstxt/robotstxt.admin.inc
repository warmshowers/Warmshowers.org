<?php

/**
 * @file
 * Administrative page callbacks for the robotstxt module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function robotstxt_admin_settings() {
  $form['robotstxt'] = array(
    '#type' => 'textarea',
    '#title' => t('Contents of robots.txt'),
    '#default_value' => variable_get('robotstxt', ''),
    '#cols' => 60,
    '#rows' => 20,
    '#wysiwyg' => FALSE,
  );

  return system_settings_form($form);
}
