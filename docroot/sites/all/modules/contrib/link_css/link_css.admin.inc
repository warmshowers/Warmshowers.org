<?php
/**
 * @file
 * Admin form for Link CSS module.
 */

/**
 * Settings form.
 */
function link_css_admin() {
  $form = array();

  $form['link_css_skip_system'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip system links'),
    '#default_value' => variable_get('link_css_skip_system', TRUE),
    '#description' => t('Leave core CSS files loaded with @import. This helps
      avoid hitting IE\'s limit and saves any live refresh scripts monitoring
      files which are unlikely to change'),
  );

  $form['link_css_warn_ie_limit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warn if IE limit exceeded'),
    '#default_value' => variable_get('link_css_warn_ie_limit', TRUE),
    '#description' => t('Internet Explorer <=7 will not load more than 31 linked
      styelsheets. Display a warning if this limit is exceeded.'),
  );

  return system_settings_form($form);
}
