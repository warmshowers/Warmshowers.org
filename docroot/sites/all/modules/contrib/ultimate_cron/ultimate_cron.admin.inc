<?php
/**
 * @file
 * Ultimate Cron!
 */

/**
 * Main plugin settings form.
 */
function ultimate_cron_plugin_form($form, &$form_state, $plugin_type) {
  ctools_include('plugins');
  $plugin_types = ctools_plugin_get_plugin_type_info();
  $plugin_info = $plugin_types['ultimate_cron'][$plugin_type];
  $static = $plugin_info['defaults']['static'];
  $class = $static['class'];
  $class::defaultSettingsForm($form, $form_state, $plugin_info);
  if (!empty($form['#submit'])) {
    array_push($form['#submit'], 'ultimate_cron_plugin_settings_clear_cache');
  }
  return $form;
}

/**
 * Settings form for plugins.
 */
function ultimate_cron_plugin_settings($form, &$form_state, $plugin_type, $name) {
  $plugin = _ultimate_cron_plugin_require($plugin_type, $name);
  $form['description'] = array(
    '#markup' => $plugin->description,
  );

  // Sanitize input values.
  if (!isset($form_state['values']['settings'][$plugin_type][$name])) {
    $form_state['values']['settings'][$plugin_type][$name] = array();
  }
  $form_state['values']['settings'][$plugin_type][$name] += $plugin->getDefaultSettings();
  $form_state['default_values']['settings'][$plugin_type][$name] = $form_state['values']['settings'][$plugin_type][$name];

  $form['settings']['#tree'] = TRUE;
  $form['plugin_type'] = array(
    '#type' => 'value',
    '#value' => $plugin_type,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );

  $plugin->settingsForm($form, $form_state);

  if (!empty($form['settings'][$plugin_type][$name]['no_settings'])) {
    return $form;
  }

  $plugin->fallbackalize(
    $form['settings'][$plugin_type][$name],
    $form_state['values']['settings'][$plugin_type][$name],
    $form_state['default_values']['settings'][$plugin_type][$name],
    TRUE
  );
  $form = system_settings_form($form);
  $form['#validate'] = empty($form['#validate']) ? array() : $form['#validate'];
  $form['#submit'] = empty($form['#submit']) ? array() : $form['#submit'];
  array_unshift($form['#validate'], 'ultimate_cron_plugin_settings_validate');
  array_unshift($form['#submit'], 'ultimate_cron_plugin_settings_submit');
  array_push($form['#submit'], 'ultimate_cron_plugin_settings_clear_cache');
  return $form;
}

/**
 * Validate handler for plugin settings.
 */
function ultimate_cron_plugin_settings_validate($form, &$form_state) {
  $plugin_type = $form_state['values']['plugin_type'];
  $name = $form_state['values']['name'];
  $plugin = _ultimate_cron_plugin_require($plugin_type, $name);
  $plugin->settingsFormValidate($form, $form_state);
}

/**
 * Submit handler for plugin settings.
 */
function ultimate_cron_plugin_settings_submit($form, &$form_state) {
  $plugin_type = $form_state['values']['plugin_type'];
  $name = $form_state['values']['name'];
  $plugin = _ultimate_cron_plugin_require($plugin_type, $name);
  $plugin->settingsFormSubmit($form, $form_state);

  // Since this is a system_settings_form() we need to move the submitted data
  // around a little.
  $form_state['values'][$plugin->key] = $form_state['values']['settings'][$plugin_type][$name];
  unset($form_state['values']['plugin_type']);
  unset($form_state['values']['name']);
  unset($form_state['values']['settings']);
}

/**
 * Clear the cron hook cache after changing default settings.
 */
function ultimate_cron_plugin_settings_clear_cache($form, &$form_state) {
  cache_clear_all('ultimate_cron_hooks', 'cache');
}
