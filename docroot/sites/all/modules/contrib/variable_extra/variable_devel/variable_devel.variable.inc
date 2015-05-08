<?php
/**
 * @file
 * Provides variable definitions for some devel modules.
 */
function variable_devel_variable_info($options) {
  $variable['devel_query_display'] = array(
    'type' => 'boolean',
    'title' => t('Display query log', array(), $options),
    'default' => 0,
  );

  $variable['devel_xhprof_enabled'] = array(
    'type' => 'boolean',
    'title' => t('Enable profiling of all page views and <a href="http://drush.ws">drush</a> requests.', array(), $options),
    'default' => '',
    'module' => 'devel',
  );

  $variable['devel_api_url'] = array(
    'type' => 'string',
    'title' => t('API Site', array(), $options),
    'default' => 'api.drupal.org',
    'module' => 'devel',
  );

  $variable['dev_timer'] = array(
    'type' => 'boolean',
    'title' => t('Display page timer', array(), $options),
    'default' => 1,
    'module' => 'devel',
  );

  $variable['dev_mem'] = array(
    'type' => 'boolean',
    'title' => t('Display memory usage', array(), $options),
    'default' => 1,
    'module' => 'devel',
  );

  $variable['devel_redirect_page'] = array(
    'type' => 'boolean',
    'title' => t('Display redirection page', array(), $options),
    'default' => 0,
    'module' => 'devel',
  );

  $variable['devel_page_alter'] = array(
    'type' => 'boolean',
    'title' => t('Display $page array', array(), $options),
    'default' => 0,
    'module' => 'devel',
  );

  $variable['devel_raw_names'] = array(
    'type' => 'boolean',
    'title' => t('Display machine names of permissions and modules', array(), $options),
    'default' => 0,
    'module' => 'devel',
  );

  $variable['devel_error_handlers'] = array(
    'type' => 'options',
    'options' => array(
      0 => t('None'),
      1 => t('Standard Drupal'),
      4 => t('Krumo backtrace in the message area'),
      2 => t('Krumo backtrace above the rendered page'),
    ),
    'title' => t('Error handlers', array(), $options),
    'default' => array(
      1 => '1',
    ),
    'module' => 'devel',
  );

  $variable['devel_krumo_skin'] = array(
    'type' => 'select',
    'options' => array(
      'default' => 'default',
      'blue' => 'blue',
      'green' => 'green',
      'orange' => 'orange',
      'white' => 'white',
      'disabled' => 'disabled',
    ),
    'title' => t('Krumo display', array(), $options),
    'default' => 'default',
    'module' => 'devel',
  );

  $variable['devel_rebuild_theme_registry'] = array(
    'type' => 'boolean',
    'title' => t('Rebuild the theme registry on every page load', array(), $options),
    'default' => 0,
    'module' => 'devel',
  );

  $variable['devel_use_uncompressed_jquery'] = array(
    'type' => 'boolean',
    'title' => t('Use uncompressed jQuery', array(), $options),
    'default' => 0,
    'module' => 'devel',
  );

  $variable['admin_menu_display'] = array(
    'type' => 'select',
    'options' => array(
      0 => 'None',
      'mlid' => 'Menu link ID',
      'weight' => 'Weight',
      'plid' => 'Parent link ID',
    ),
    'title' => t('Display additional data for each menu item', array(), $options),
    'default' => 'plid',
    'group' => 'admin_menu',
  );

  $variable['admin_menu_show_all'] = array(
    'type' => 'boolean',
    'title' => t('Display all menu items', array(), $options),
    'default' => 0,
    'group' => 'admin_menu',
  );
  return $variable;
}