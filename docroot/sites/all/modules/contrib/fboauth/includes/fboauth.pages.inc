<?php

/**
 * @file
 * Administrative pages and functions for Facebook OAuth module.
 */

/**
 * Menu callback; Display the settings form for Facebook OAuth.
 */
function fboauth_settings_form($form, &$form_state) {
  module_load_include('inc', 'fboauth', 'includes/fboauth.fboauth');

  $form['fboauth_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#size' => 20,
    '#maxlengh' => 50,
    '#description' => t('To use Facebook connect, a Facebook Application must be created. Set up your app in <a href="http://www.facebook.com/developers/apps.php">my apps</a> on Facebook.') . ' ' . t('Enter your App ID here.'),
    '#default_value' => variable_get('fboauth_id', ''),
  );
  $form['fboauth_apiversion'] = array(
    '#type' => 'select',
    '#title' => t('API Version'),
    '#options' => drupal_map_assoc(array('v2.3', 'v2.4', 'v2.5')),
    '#description' => t('To use Facebook connect, a Facebook Application must be created. Set up your app in <a href="http://www.facebook.com/developers/apps.php">my apps</a> on Facebook.') . ' ' . t('Enter your App API version here. If your version is prior to 2.3 select v2.3 here.'),
    '#default_value' => variable_get('fboauth_apiversion', 'v2.3'),
  );
  $form['fboauth_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#size' => 40,
    '#maxlengh' => 50,
    '#description' => t('To use Facebook connect, a Facebook Application must be created. Set up your app in <a href="http://www.facebook.com/developers/apps.php">my apps</a> on Facebook.') . ' ' . t('Enter your App Secret here.'),
    '#default_value' => variable_get('fboauth_secret', ''),
  );
  $form['fboauth_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Capture Debug Information'),
    '#default_value' => variable_get('fboauth_debug', ''),
    '#description' => t('When checked, if Database Logging is enabled, it will store the raw user object returned by Facebook. You can see it under <em>admin/reports/dblog</em>. Uncheck for production sites.'),
  );

  $form['fboauth_popup'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a popup window for Facebook authentication dialog'),
    '#description' => t('This may provide a better user experience when logging into Facebook, but may also be blocked by browser popup blocker plugins.'),
    '#default_value' => variable_get('fboauth_popup', 1),
  );

  $form['fboauth_always_load_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always load the popup window javascript file'),
    '#description' => t('This will load the popup window javascript file on every page load rather than only when the block is loaded. This is mainly for when you want to implement the Facebook button code manually.'),
    '#default_value' => variable_get('fboauth_always_load_js', 0),
    '#states' => array(
      'invisible' => array(
        ':input[name="fboauth_popup"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['fboauth_anon_connect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow anonymous users to connect an existing unconnected drupal account to their Facebook account?'),
    '#description' => t('Should anonymous users be able to connect an existing unconnected drupal account to a Facebook account based solely on an email address match?'),
    '#default_value' => variable_get('fboauth_anon_connect', TRUE),
  );

  $form['fboauth_basic_mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic mapping'),
  );

  $form['fboauth_basic_mapping']['fboauth_user_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Facebook e-mail address'),
    '#description' => t('Importing Facebook e-mail addresses requires additional confirmation from the end-user, but it is <strong>strongly recommended</strong> to ensure proper user functionality (password reset, newsletter functionality, etc). If not used, providing a user with some kind of proxy e-mail address is recommended.'),
    '#default_value' => variable_get('fboauth_user_email', TRUE),
  );

  $form['fboauth_basic_mapping']['fboauth_user_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Facebook user picture'),
    '#description' => t('Import the Facebook picture to the built-in user picture.'),
    '#default_value' => variable_get('fboauth_user_picture', 'picture') === 'picture',
    '#return_value' => 'picture',
    '#access' => variable_get('user_pictures', 0),
  );

  $form['fboauth_basic_mapping']['fboauth_user_username'] = array(
    '#type' => 'radios',
    '#title' => t('User name import'),
    '#options' => array(
      'name' => t('Real name (i.e. John Smith)'),
      'id' => t('Profile ID (i.e. 10152608721639101)'),
    ),
    '#description' => t('Select the Facebook value used to set the user\'s name when connecting with Facebook. <strong>Warning:</strong> choosing "Profile ID" will result in all the usernames created with Facebook being numeric ones. You may want to implement !hook_username_alter to hide this fact.', array(
      '!hook_username_alter' => l('hook_username_alter', 'https://api.drupal.org/api/function/hook_username_alter'),
    )),
    '#default_value' => variable_get('fboauth_user_username', 'name'),
  );

  // If Profile module exists, allow mapping of individual properties.
  if (module_exists('profile')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.profile');
    fboauth_profile_form_alter($form, $form_state);
  }

  // Add in Field module mapping options, which are always available.
  module_load_include('inc', 'fboauth', 'includes/fboauth.field');
  fboauth_field_form_alter($form, $form_state);

  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced permission settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('If using profile mapping, the necessary permissions will automatically requested and these settings do not need adjustment. If handling mapping manually or integrating with a custom module, you may request additional permissions for your application here. Requesting more information from users may make them less likely to trust your website. Adding additional permissions after a user has connected will require additional confirmation of the new permissions upon next login. For more information about properties these, read the <a href="http://developers.facebook.com/docs/reference/api/user/">Facebook User API</a>. Note that enabling more permissions here <strong>does not import more data</strong>. It just makes this information available for you to access in a custom module.'),
    '#tree' => FALSE,
    '#weight' => 20,
  );

  $properties = fboauth_user_properties();
  $property_options = array();
  foreach ($properties as $property => $property_info) {
    $property_options[$property] = '[' . $property . '] ' . $property_info['label'];
  }
  $property_options['email'] .= ' (' . t('Strongly recommended to ensure proper account functionality.') . ')';

  $form['permissions']['fboauth_user_properties'] = array(
    '#title' => t('Request additional permission to the following'),
    '#type' => 'checkboxes',
    '#options' => $property_options,
    '#default_value' => variable_get('fboauth_user_properties', array()),
    '#description' => t('The following properties are always available: id, name, first_name, last_name, gender, locale, link, timezone, updated_time, and verified.'),
  );

  $connections = fboauth_user_connections();
  $connection_options = array();
  foreach ($connections as $connection => $connection_info) {
    $connection_options[$connection] = '[' . $connection . '] ' . $connection_info['label'];
  }

  $form['permissions']['fboauth_user_connections'] = array(
    '#title' => t('Connections and extended permissions'),
    '#type' => 'checkboxes',
    '#options' => $connection_options,
    '#default_value' => variable_get('fboauth_user_connections', array()),
    '#description' => t('Requesting this any of this information is rarely useful during the login process. It is often better to set up dedicated permission requests after the user has created an account for extended access.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Form validation function for fboauth_settings_form().
 */
function fboauth_settings_form_validate($form, &$form_state) {
  // Remove trailing spaces from keys.
  $form_state['values']['fboauth_id'] = trim($form_state['values']['fboauth_id']);
  $form_state['values']['fboauth_secret'] = trim($form_state['values']['fboauth_secret']);
  variable_set('fboauth_popup', $form_state['values']['fboauth_popup']);

  // Set fboauth_always_load_js to be FALSE if fboauth_always_load_js has been
  // turned off as well.
  $load_js = $form_state['values']['fboauth_always_load_js'];
  if (!$form_state['values']['fboauth_popup']) {
    $load_js = 0;
  }
  variable_set('fboauth_always_load_js', $load_js);

  // Do some basic data input validation.
  if (!is_numeric($form_state['values']['fboauth_id']) || strlen($form_state['values']['fboauth_id']) > 20) {
    form_error($form['fboauth_id'], t('The App ID must be an integer (usually 11 characters).'));
  }
  if (strlen($form_state['values']['fboauth_secret']) != 32) {
    form_error($form['fboauth_secret'], t('The App Secret does not appear to be valid. It is usually a 32 character hash.'));
  }
}

/**
 * Form submission function for fboauth_settings_form().
 */
function fboauth_settings_form_submit($form, &$form_state) {
  variable_set('fboauth_id', $form_state['values']['fboauth_id']);
  variable_set('fboauth_apiversion', $form_state['values']['fboauth_apiversion']);
  variable_set('fboauth_secret', $form_state['values']['fboauth_secret']);
  variable_set('fboauth_debug', $form_state['values']['fboauth_debug']);

  variable_set('fboauth_anon_connect', $form_state['values']['fboauth_anon_connect']);

  variable_set('fboauth_user_email', $form_state['values']['fboauth_user_email']);
  variable_set('fboauth_user_username', $form_state['values']['fboauth_user_username']);
  variable_set('fboauth_user_picture', $form_state['values']['fboauth_user_picture']);

  // Save profile module values.
  if (module_exists('profile')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.profile');
    fboauth_profile_form_submit($form, $form_state);
  }

  // Save field module values.
  module_load_include('inc', 'fboauth', 'includes/fboauth.field');
  fboauth_field_form_submit($form, $form_state);

  // Clean up saved checkbox values.
  variable_set('fboauth_user_properties', array_values(array_filter($form_state['values']['fboauth_user_properties'])));
  variable_set('fboauth_user_connections', array_values(array_filter($form_state['values']['fboauth_user_connections'])));

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * User settings page for Facebook OAuth.
 *
 * Note that currently this is not a form, it's just a normal page.
 */
function fboauth_user_form($account) {
  module_load_include('inc', 'fboauth', 'includes/fboauth.fboauth');
  return theme('fboauth_user_info', array('account' => $account));
}

/**
 * Provided themed information about the user's current Facebook connection.
 */
function theme_fboauth_user_info($variables) {
  $account = $variables['account'];

  $fbid = fboauth_fbid_load($account->uid);
  $output = '';
  if ($fbid !== FALSE) {
    $output .= '<p>' . t('Your account is connected with Facebook. At present, this site has access to the following information from Facebook.') . '</p>';
    $output .= theme('item_list', array('items' => array('basic' => t('Your basic information (Name, Gender, Picture, etc.)'))) + fboauth_user_connect_permissions());
    $output .= '<p>' . t('You may disconnect your account from Facebook at any time by using the Deauthorize option below. If deauthorized, you will no longer be able to use Facebook to log into this account and must use a normal password. <strong>If you have not yet set a password, you will not be able to log onto this site</strong>.') . '</p>';
    $output .= fboauth_action_display('deauth');
  }
  else {
    $output .= '<p>' . t('Your account is not currently connected with Facebook. To connect this account to Facebook, click on the link below.') . '</p>';
    $output .= fboauth_action_display('connect');
  }

  return $output;
}
