<?php
// $Id: services_admin_browse.inc,v 1.5.2.45.2.7 2010/02/27 05:43:16 heyrocker Exp $
/**
 * @file
 *  Browser thru all services and servers.
 */

/**
 * Page callback to list all enabled services and servers.
 *
 * @return 
 *   List of services and servers with links to test them.
 */
function services_admin_browse_index() {
  $methods = services_get_all();

  // Show enable server modules
  $servers = module_implements('server_info');
  $output = '<h2>'. t('Servers') .'</h2>';

  if (!empty($servers)) {
    $output .= '<ul>';
    foreach ($servers as $module) {
      $info = module_invoke($module, 'server_info');
      services_strip_hashes($info);

      $name = $info['name'];
      $path = 'services/'. $info['path'];
      $output .= '<li class="leaf">'. l($name .' - /'. $path, $path) .'</li>';
    }
    $output .= '</ul>';
  }
  else {
    $output .= '<p>'. t('You must enable at least one server module to be able to connect remotely. Visit the <a href="@url">modules</a> page to enable server modules.', array('@url' => url('admin/build/modules'))) .'</p>';
  }


  $output .= '<h2>'. t('Services') .'</h2>';

  // group namespaces
  $services = array();
  foreach ($methods as $method) {
    $namespace = drupal_substr($method['method'], 0, strrpos($method['method'], '.'));
    $services[$namespace][] = $method;
  }

  if (count($services)) {
    foreach ($services as $namespace => $methods) {
      $output .= '<h3>'. $namespace .'</h3>';
      $output .= '<ul>';
      foreach ($methods as $method) {
        $output .= '<li class="leaf">'. l($method['method'], 'admin/build/services/browse/'. $method['method']) .'</li>';
      }
      $output .= '</ul>';
    }
  }
  else {
    $output .= t('No services have been enabled.');
  }

  return $output;
}

/**
 * Display a form for the testing of a single service method in the browser.
 *
 * @param $method
 *   The method to be tested
 * @return
 *   Form for testing and method parameter info.
 */
function services_admin_browse_method($method) {
  global $_services_admin_browse_test_submit_result;

  $output = '';

  $output .= '<h3>'. $method['method'] .'</h3>';
  $output .= '<p>'. $method['help'] .'</p>';

  // List arguments.
  $output .= '<h3>'. t('Arguments') .' ('. count($method['args']) .')</h3>';
  $output .= '<dl id="service-browser-arguments">';
  $count = 0;
  foreach ($method['args'] as $arg) {
    $count++;
    $output .= '<dt><em class="type">'. $arg['type'] .'</em><strong class="name">'.
      $arg['name'] .'</strong> ('. (($arg['optional']) ? t('optional') : t('required')) .')</dt>';
    $output .= '<dd>'. $arg['description'] .'</dd>';
  }

  $output .= '</dl>';

  // Allow testing of methods
  $output .= '<h3>'. t('Call method') .'</h3>';
  $output .= drupal_get_form('services_admin_browse_test');

  // Display results
  if ($_services_admin_browse_test_submit_result) {
    $output .= '<div id="output">';
    $output .= '<h3>'. t('Result') .'</h3>';
    $output .= '<code>'. $_services_admin_browse_test_submit_result .'</code>';
    $output .= '</div>';
  }

  return $output;
}

/**
 * Build the form used for testing a service in the browser
 *
 * @return
 *   FAPI array
 * @ingroup forms
 * @see services_admin_browse_test_submit() 
 */
function services_admin_browse_test() {
  $form = array();
  $method = services_method_get(arg(4));

  $form['arg'] = array('#tree' => TRUE);
  $form['format'] = array('#tree' => TRUE);

  foreach ($method['args'] as $key => $arg) {
    $form['name'][$key]         = array(
      '#value' => $arg['name']
    );
    $form['optional'][$key]     = array(
      '#value' => ($arg['optional']) ? t('optional') : t('required')
    );

    if (isset($arg['size']) && $arg['size'] == 'big') {
      $form['arg'][$key] = array(
        '#type'           => 'textarea'
      );
    }
    else {
      $form['arg'][$key] = array(
        '#type'           => 'textfield'
      );
    }

    $format_opt = array();
    switch ($arg['type']) {
      case 'array':
        $format_opt['cdel'] = t('Comma delimited');
      case 'struct':
        $format_opt['json'] = t('JSON');
        $format_opt['sphp'] = t('Serialized PHP');
        break;
    }
    if (!empty($format_opt)) {
      $form['format'][$key] = array(
        '#type' => 'select',
        '#options' => $format_opt,
      );
    }
    else {
      $form['format'][$key] = array(
        '#type' => 'value',
        '#value' => '',
      );
    }
  }

  services_auth_invoke('alter_browse_form', $form, $method);

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t('Call method')
  );

  $form['#redirect'] = FALSE;
  return $form;
}

/**
 * Submit callback for services_admin_browse_test().
 */
function services_admin_browse_test_submit($form, $form_state) {
  global $_services_admin_browse_test_submit_result;
  $method = services_method_get(arg(4));
  $args = services_admin_browse_test_unserialize_args($form_state['values']['arg'], $form_state['values']['format']);
  // Allow the authentication module to handle submitted values.
  services_auth_invoke('alter_browse_form_submit', $method, $args);
  $result = services_method_call($method['method'], $args, TRUE);
  // if the devel module is installed use kpr for fancy output with types
  if (function_exists('kpr')) {
    $_services_admin_browse_test_submit_result = kpr($result, true);
  } else {
    $_services_admin_browse_test_submit_result = '<pre>'. htmlspecialchars(print_r($result, TRUE)) .'</pre>';
  }
}

function services_admin_browse_test_unserialize_args($values, $formats) {
  $method = services_method_get(arg(4));
  $noskip = FALSE;
  // Convert args
  for ($c = count($method['args']) - 1; $c >= 0; $c--) {
    $arg = $method['args'][$c];
    $value = $values[$c];

    // Remove empty values from end of array
    // Once we find a value, we can no longer skip
    if (!is_numeric($value) && empty($value) && !$noskip) {
      continue;
    }
    $noskip = TRUE;

    switch ($formats[$c]) {
      case 'cdel';
        if (empty($value)) {
          $return[$c] = NULL;
        }
        else {
          $return[$c] = split(',', $value);
        }
        break;
      case 'json':
        if (empty($value)) {
          $return[$c] = NULL;
        }
        else {
          $return[$c] = json_decode($value, $arg['type'] === 'array');
        }
        break;
      case 'sphp':
        if (empty($value)) {
          $return[$c] = NULL;
        }
        else {
          $return[$c] = unserialize($value);
        }
        break;
      default :
        $return[$c] = $value;
    }
  }

  if ($return) ksort($return);

  return $return;
}
/**
 * Implementation of hook_theme();
 */
function theme_services_admin_browse_test($form) {
  $output = '';
  $output .= drupal_render($form['test']);

  $header = array(t('Name'), t('Required'), t('Value'), t('Format'));
  $rows = array();
  foreach (element_children($form['name']) as $key => $type) {
    $row = array();
    if (isset($form['arg'][$key]['#title'])) {
      $row[] = $form['arg'][$key]['#title'];
      unset($form['arg'][$key]['#title']);
      unset($form['name'][$key]);
    }
    else {
      $row[] = drupal_render($form['name'][$key]);
    }
    $row[] = drupal_render($form['optional'][$key]);
    $row[] = drupal_render($form['arg'][$key]);
    $row[] = drupal_render($form['format'][$key]);
    $rows[] = $row;
  }

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form['submit']);

  $output .= drupal_render($form);

  return $output;
}

/**
 * Build the admin settings form.
 *
 * @return
 *   FAPI array
 * @ingroup forms
 * @see services_admin_settings_submit() 
 */
function services_admin_settings() {
  $auth_modules = module_implements('authentication_info');

  // Add security options.
  if (!empty($auth_modules)) {
    $auth_options = array('' => t('-- Select an authentication module'));
    foreach ($auth_modules as $module) {
      $info = services_auth_info(NULL, $module);
      services_strip_hashes($info);

      $auth_options[$info['description']][$module] = $info['title'];
    }

    $form['authentication'] = array(
      '#title'        => t('Authentication'),
      '#type'         => 'fieldset',
      '#description'  => t('Changing authentication settings will require you to adjust all method calls. This will affect all applications using site services.'),
    );

    $form['authentication']['auth_module'] = array(
      '#type' => 'select',
      '#title' => t('Authentication module'),
      '#options' => $auth_options,
      '#required' => FALSE,
      '#default_value' => variable_get('services_auth_module', ''),
      '#ahah' => array(
        'path' => 'admin/services/ahah/security-options',
        'wrapper' => 'security-module-options',
        'method' => 'replace',
      ),
    );

    // Placeholder for the auth module options
    // also used as wrapper for ahah.
    $form['authentication']['options'] = array(
      '#prefix' => '<div id="security-module-options">',
      '#suffix' => '</div>',
      'settings' => array(
        '#value' => sprintf('<div class="description">%s</div>',
          t('Select an authentication module.')),
      ),
    );
    // Get the configuration form for the authentication module
    $settings = services_auth_invoke('security_settings');
    if (is_array($settings)) {
      $form['authentication']['options']['settings'] = $settings;
    }
  }
  else { // Warn if no authentication module has been installed.
    drupal_set_message(t('No authentication modules have been installed'), 'warning');
  }

  $form['services_use_content_permissions'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Apply content permissions'),
    '#default_value'  => variable_get('services_use_content_permissions', FALSE),
    '#description'    => t('CCK includes the optional Content Permissions module, which allows administrators to restrict access to content at the field level. By default, node services do NOT apply these permissions, causing all fields to be returned in all cases. Checking this box causes content permissions to be applied to node services.'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Validate callback for services_admin_settings().
 */
function services_admin_settings_validate($form, $form_state) {
  // Invoke custom validation for the auth module
  if (!empty($form_state['values']['auth_module'])) {
    services_auth_invoke_custom($form_state['values']['auth_module'],
      'security_settings_validate', $form_state);
  }
}

/**
 * Submit callback for services_admin_settings().
 */
function services_admin_settings_submit($form, $form_state) {
  // Update the services oauth module variable *if needed*.
  $old_auth = variable_get('services_auth_module', '');
  $new_auth = $form_state['values']['auth_module'];
  if ($old_auth != $new_auth) {
    variable_set('services_auth_module', $new_auth);
    // Rebuild menu so that security-related menu items can be conditionally created.
    menu_rebuild();
    drupal_set_message('Changed authentication method');
  }
  else {
    drupal_set_message('Updated authentication settings');
  }

  // Allow the authentication module to handle submitted values.
  services_auth_invoke('security_settings_submit', $form_state);
  
  // Apply the content permissions setting.
  variable_set('services_use_content_permissions', $form_state['values']['services_use_content_permissions']);

  // Clear the services cache so that methods and resources are updated according to auth settings
  cache_clear_all('services:', 'cache', TRUE);
}

/**
 * Callback for the security configuration form ahah.
 */
function _services_ahah_security_options() {
  $cached_form_state = array();
  $cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state);

  if (!empty($_POST['auth_module'])) {
    $settings = services_auth_invoke_custom($_POST['auth_module'], 'security_settings');
  }

  if (is_array($settings)) {
    $cached_form['security']['options']['settings'] = $settings;
  }
  else {
    unset($cached_form['security']['options']['settings']);
  }

  form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);

  $form_state = array('submitted' => FALSE);
  $options = $cached_form['security']['options'];
  unset($options['#prefix'], $options['#suffix']);
  $options = form_builder('_services_ahah_security_options', $options, $form_state);
  $output = drupal_render($options);

  print drupal_to_js(array(
    'status' => TRUE,
    'data' => $output,
  ));
  exit;
}