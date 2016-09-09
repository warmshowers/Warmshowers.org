<?php

/**
 * @file
 * Provides functions used during Facebook login processes.
 */

/**
 * Menu callback; The main page for processing OAuth login transactions.
 *
 * @param string $action 
 *   The action being requested. Currently supports the following:
 *   - connect: Initiate Facebook connection and log a user in.
 *   - deauth: Remove the exisitng Facebook connection access.
 */
function fboauth_action_page($action) {
  global $user;

  // TODO: Support loading of more than one App ID and App Secret.
  $app_id = variable_get('fboauth_id', '');
  $app_secret = variable_get('fboauth_secret', '');

  $action_name = $action['name'];

  $error_message = t('The Facebook login could not be completed due to an error. Please create an account or contact us directly. Details about this error have already been recorded to the error log.');

  if (!($app_id && $app_secret)) {
    watchdog('fboauth', 'A Facebook login was attempted but could not be processed because the module is not yet configured. Vist the <a href="!url">Facebook OAuth configuration</a> to set up the module.', array('!url' => url('admin/config/people/fboauth')));
  }
  elseif (isset($_REQUEST['error'])) {
    $url = !$user->uid ? url('<front>', array('absolute' => TRUE)) : url("user/{$user->uid}/edit", array('absolute' => TRUE));
    $link = fboauth_action_link_properties($action_name, $url);
    watchdog('fboauth', 'A user refused to allow access to the necessary 
      Facebook information (@permissions) to login to the site.', array('@permissions' => $link['query']['scope']));
    $html_class = variable_get('fboauth_popup', 1) ? ' class="fboauth-popup"' : '';
    $error_message = t('This site requires access to information in order to 
      log you into the site. If you like, you can 
    	<a href="!facebook"' . $html_class . '>sign in with Facebook again</a> or 
      <a href="!register">through the standard username/password registration</a>.', array(
      '!facebook' => url($link['href'], array('query' => $link['query'])),
      '!register' => url('user/register')));

    drupal_set_message($error_message, 'error');

    if (variable_get('fboauth_popup', 1)) {
      // Close the popup and set the parent window to the target destination.
      print fboauth_close_popup_window($url);
      drupal_exit();
    }
    drupal_goto($url);
  }
  elseif (!isset($_REQUEST['code'])) {
    watchdog('fboauth', 'A Facebook request code was expected but no authorization was received.');
  }
  // The primary action routine after access has been approved by the user.
  elseif ($access_token = fboauth_access_token($_REQUEST['code'], $action_name, $app_id, $app_secret)) {
    $destination = fboauth_action_invoke($action_name, $app_id, $access_token);
    if (empty($destination)) {
      $destination = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : '<front>';
    }
    if (variable_get('fboauth_popup', 1)) {
      // Close the popup and set the parent window to the target destination.
      $url = !isset($user->pass) || $user->pass == '' ? url("user/{$user->uid}/edit", array('absolute' => TRUE)) : url($destination);
      print fboauth_close_popup_window($url);
      drupal_exit();
    }
    else {
      drupal_goto($destination);
    }
  }

  // In the event of an error, we stay on this page.
  return $error_message;
}

/**
 * Returns a HTML string to close the popup modal window and redirect the opener window to the relevant URL.
 *
 * @param $url
 * @return string
 */
function fboauth_close_popup_window($url) {
  return "
    <html>
    <head>
      <script type=\"text/javascript\">
      if (window.opener) {
        window.opener.location = '$url';
        setTimeout(function(){ window.close(); }, 500);
      }
      else {
        window.location = '$url';
      }
      </script>
    </head>
    </html>";
}

/**
 * Invoke an action specified through hook_fboauth_action_info().
 */
function fboauth_action_invoke($action_name, $app_id, $access_token) {
  $action = fboauth_action_load($action_name);

  // Call the specified action.
  if (isset($action['callback'])) {
    $callback = $action['callback'];

    if (function_exists($callback)) {
      return $callback($app_id, $access_token);
    }
  }
}

/**
 * Facebook OAuth callback for initiating a Facebook connection.
 */
function fboauth_action_connect($app_id, $access_token) {
  global $user;

  // Save access_token in session for future use.
  $_SESSION['fboauth']['access_token'] = $access_token;

  $id = 'me'; // Versions prior to v2.4 and v2.5 are working with just this.
  if (variable_get('fboauth_apiversion', 'v2.3') == 'v2.5'
   || variable_get('fboauth_apiversion', 'v2.3') == 'v2.4') {
    $id .= '?fields=email,name,first_name,last_name,age_range,link,gender,locale,timezone,updated_time,verified,birthday,location';
  }

  $fbuser = fboauth_graph_query($id, $access_token);

  if (variable_get('fboauth_debug', '') == 1) {
    watchdog('debug', '<pre>fbuser=' . print_r($fbuser, TRUE) . '</pre>');
  }

  // Use fake email if user email not available.
  if (empty($fbuser->email)) {
    $fbuser->email = $fbuser->id . '@facebook.com';
  }

  $uid = fboauth_uid_load($fbuser->id);
  // If the user isn't logged in.
  if (!$user->uid) {
    // See if they are connected to FB & is an association between FB & Drupal.
    if ($uid && ($account = user_load($uid))) {
      fboauth_login_user($account);
    }
    // No association between FB & this Drupal site, yet.
    // So lets check & see if the FB address matches a Drupal email address.
    else {
      if (!empty($fbuser->email)) {
        $account = NULL;

        // Check and see if multiple_email module is in use.
        if (module_exists('multiple_email')) {
          if ($multiple_email_object = multiple_email_find_address($fbuser->email)) {
            $account = user_load($multiple_email_object->uid);
            if ($multiple_email_object->confirmed) {
              // we're good  
            }
            else {
              // note:  drupal security team doesn't consider it a vulnerabilty that UID is publicly available
              // https://www.drupal.org/node/1004778
              drupal_set_message(t("We found your e-mail @email in the @sitename system, but it hasn't been confirmed. " .
                      'Please <a href="!login">login manually</a> and then <a href="!edit">' .
                      'resend your confirmation code</a> to confirm that you are the owner of this email address. This is required before you can connect to the site from Facebook with it.', array(
                '@email' => $fbuser->email,
                '@sitename' => variable_get('site_name', ''),
                '!login' => url('user/login'),
                '!edit' => url('user/' . $account->uid . '/edit/email-addresses'))));
              return;
            }
          }
          else {
            // Email address not found in System   
          }
        }
        else {
          // Just use the e-mail from the users table.
          $account = user_load_by_mail($fbuser->email);
        }
        // If the Facebook e-mail address matches an existing account, bind them
        // together and log in as that account.
        if ($account) {
          // Connect the account only if we allow anonymous users to connect accounts that have
          // never been connected before.
          if (variable_get('fboauth_anon_connect', TRUE)) {
            // Logins will be denied if the user's account is blocked.
            if (fboauth_login_user($account)) {
              fboauth_save($account->uid, $fbuser->id);
              drupal_set_message(t("You've connected your account with Facebook."));
            }
          }
          else {
            drupal_set_message(t('We found your e-mail @email in the @sitename system, but the account has never been connected to Facebook before. ' .
                    'Please <a href="!login">login manually</a> and <a href="!edit">connect to Facebook</a> while logged in. ' .
                    'Once you have completed this step, you may login through Facebook whenever you like.', array(
              '@email' => $fbuser->email,
              '@sitename' => variable_get('site_name', ''),
              '!login' => url('user/login'),
              '!edit' => url('user/' . $account->uid . '/edit'))));
          }
        }
        // Register a new user only if allowed.
        elseif (variable_get('user_register', 1)) {
          $account = fboauth_create_user($fbuser);

          if (!isset($account) || empty($account)) {
            drupal_set_message(t('Unable to create a new account using your Facebook profile.'), 'warning');
            return;
          }

          // Load the account fresh just to have a fully-loaded object.
          $account = user_load($account->uid);

          // If the account requires administrator approval the new account will
          // have a status of '0' and not be activated yet.
          if ($account->status == 0) {
            _user_mail_notify('register_pending_approval', $account);
            drupal_set_message(
                t('An account has been created for you on @sitename but an ' .
                    'administrator needs to approve your account. In the meantime, ' .
                    'a welcome message with further instructions has been sent ' .
                    'to your e-mail address.', array(
              '@sitename' => variable_get('site_name', ''))));
          }
          // Log in the user if no approval is required.
          elseif (fboauth_login_user($account)) {
            drupal_set_message(t('Welcome to @sitename. ' .
                    'Basic information has been imported from Facebook into your account. ' .
                    'You may want to <a href="!edit">edit your account</a> to confirm the ' .
                    'details and set a password.', array(
              '@sitename' => variable_get('site_name', ''),
              '!edit' => url('user/' . $account->uid . '/edit'))));
          }
          // If the login fails, fboauth_login_user() throws an error message.
        }
        // Since user's can't create new accounts on their own, show an error.
        else {
          drupal_set_message(t('Your Facebook e-mail address does not match 
            any existing accounts. If you have an account, you must first 
            log in before you can connect your account to Facebook. 
            Creation of new accounts on this site is disabled.'));
        }
      }
      // Email not provided by Facebook.
      else {
        drupal_set_message(t("Facebook didn't provide an e-mail address " .
                "to be associated with your account, so we can't compare it " .
                "with the e-mail addresses in this system."));
        return;
      }
      // Done if no e-mail address provided by facebook.
    }
  }
  else {
    // The user is already logged in to Drupal.
    // So just associate the two accounts.
    fboauth_save($user->uid, $fbuser->id);
    drupal_set_message(t("You've connected your account with Facebook."));
  }
}

/**
 * Facebook OAuth callback for deauthorizing the site from Facebook.
 */
function fboauth_action_deauth($app_id, $access_token) {
  global $user;

  // Deauthorize our application from Facebook.
  $result = fboauth_graph_query('me/permissions', $access_token, array(), 'DELETE');

  // If successful, also remove the uid-fbid pairing.
  if (isset($result->success) && $result->success) {
    $fbid = fboauth_fbid_load($user->uid);
    fboauth_save($user->uid, NULL);

    // Allow other modules to hook into a deauth event.
    module_invoke_all('fboauth_deauthorize', $user->uid, $fbid);

    drupal_set_message(t('Your account has been disconnected from Facebook.'));
  }
  else {
    drupal_set_message(t('There was an error disconnecting from Facebook. The server returned %message.', array('%message' => $result->error_msg)), 'error');
    watchdog('There was an error disconnecting the user %username from Facebook. The server returned %message.', array('%message' => $result->error_msg, '%username' => $user->name));
  }
}

/**
 * Given a Facebook user object, associate or save a Drupal user account.
 */
function fboauth_create_user($fbuser, $options = array()) {
  // Set default options.
  $defaults = array(
    'username' => variable_get('fboauth_user_username', 'name'),
    'picture' => (variable_get('user_pictures', 0) && variable_get('fboauth_user_picture', 'picture')) ? 'picture' : '',
    'status' => variable_get('user_register', 1) == 1 ? 1 : 0,
  );
  $options += $defaults;

  // Depending on settings use profile id or real name for drupal username.
  switch ($options['username']) {
    case 'name':
      $username = $fbuser->name;
      break;

    case 'id':
      $username = $fbuser->id;
      break;
  }

  // If an account already exists with that name, increment until the namespace
  // is available.
  $query = "SELECT uid FROM {users} WHERE name = :name";
  $uid = db_query($query, array(':name' => $username))->fetchField();
  $i = 0;
  while ($uid) {
    $i++;
    $uid = db_query($query, array(':name' => ($username . $i)))->fetchField();
  }
  if ($i > 0) {
    $username = $username . $i;
  }

  // Initialize basic properties that are unlikely to need changing.
  $edit = array(
    'name' => $username,
    'mail' => $fbuser->email,
    'init' => $fbuser->email,
    // If user_register is "1", then no approval required.
    'status' => $options['status'],
    'timezone' => variable_get('date_default_timezone'),
    // TODO: is this appropriate in sites with no default?
    'fboauth' => TRUE,
    // Signify this is being imported by Facebook OAuth.
    // So that other modules can load the account.
    'fboauth_fbid' => $fbuser->id,
  );

  // Profile module support.
  if (module_exists('profile')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.profile');
    fboauth_profile_create_user($edit, $fbuser);
  }

  // Field module support.
  module_load_include('inc', 'fboauth', 'includes/fboauth.field');
  fboauth_field_create_user($edit, $fbuser);

  // Allow other modules to manipulate the user information before save.
  foreach (module_implements('fboauth_user_presave') as $module) {
    $function = $module . '_fboauth_user_presave';
    $function($edit, $fbuser);
  }

  // check for a valid email address and username and if they don't exist return NULL
  if (!isset($edit) || empty($edit)) {
    watchdog('fboauth', 'Account information not provided.');
    return NULL;
  }
  if (!isset($edit['mail']) || empty($edit['mail'])) {
    watchdog('fboauth', 'Email address not provided by Facebook.');
    return NULL;
  }
  if (!isset($edit['name']) || empty($edit['name'])) {
    watchdog('fboauth', 'Username not able to be retrieved from Facebook or be generated.');
    return NULL;
  }

  $account = user_save(NULL, $edit);

  // Retrieve the user's picture from Facebook and save it locally.
  if ($account->uid && $options['picture'] === 'picture') {
    $picture_directory = file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
    if (file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY)) {
      $picture_result = drupal_http_request('https://graph.facebook.com/' . variable_get('fboauth_apiversion', 'v2.3') . '/' . $fbuser->id . '/picture?type=large');
      $picture_path = file_stream_wrapper_uri_normalize($picture_directory . '/picture-' . $account->uid . '-' . REQUEST_TIME . '.jpg');
      $picture_file = file_save_data($picture_result->data, $picture_path, FILE_EXISTS_REPLACE);

      // Check to make sure the picture isn't too large for the site settings.
      $max_dimensions = variable_get('user_picture_dimensions', '85x85');
      file_validate_image_resolution($picture_file, $max_dimensions);

      // Update the user record.
      $picture_file->uid = $account->uid;
      $picture_file = file_save($picture_file);
      file_usage_add($picture_file, 'user', 'user', $account->uid);
      db_update('users')
          ->fields(array(
            'picture' => $picture_file->fid,
          ))
          ->condition('uid', $account->uid)
          ->execute();
    }
  }

  // Allow other modules to manipulate the user information after save.
  foreach (module_implements('fboauth_user_save') as $module) {
    $function = $module . '_fboauth_user_save';
    $function($account, $fbuser);
  }

  return $account;
}

/**
 * Given a Drupal user object, log the user in.
 *
 * This acts as a wrapper around user_external_login() in Drupal 6 and as a full
 * replacement function in Drupal 7, since no direct equivalent exists.
 *
 * @param object $account
 *   A Drupal user account or UID.
 */
function fboauth_login_user($account) {
  global $user;
  if (module_exists('boost')) {
    boost_set_cookie($account->uid);
  }

  if ($account->status) {
    $form_state['uid'] = $account->uid;
    user_login_submit(array(), $form_state);
  }
  else {
    if ($account->access) {
      drupal_set_message(t('The account with username %name and email %mail ' .
              'is blocked.', array('%name' => $account->name,
        '%mail' => $account->mail,)), 'error');
    }
    else {
      drupal_set_message(t('The account with username %name and email %mail ' .
              'is not yet activated.', array('%name' => $account->name,
        '%mail' => $account->mail,)), 'error');
    }
  }

  return !empty($user->uid);
}

/**
 * Given an approval code from Facebook, return an access token.
 *
 * The approval code is generated by Facebook when a user grants access to our
 * site application to use their data. We use this approval code to get an
 * access token from Facebook. The access token usually is valid for about
 * 15 minutes, allowing us to pull as much information as we want about the
 * user.
 *
 * @param string $code
 *   An approval code from Facebook. Usually pulled from the ?code GET parameter
 *   after a user has approved our application's access to their information.
 *
 * @param string $action_name
 *   The action is the directory name underneath the "fboauth" path. This value
 *   must be the same between the page originally provided to Facebook as the
 *   "redirect" URL and when requesting an access token.
 *
 * @return string
 *   An access token that can be used in REST queries against Facebook's Graph
 *   API, which will provide us with info about the Facebook user.
 */
function fboauth_access_token($code, $action_name, $app_id = NULL, $app_secret = NULL) {
  // Use the default App ID and App Secret if not specified.
  $app_id = isset($app_id) ? $app_id : variable_get('fboauth_id', '');
  $app_secret = isset($app_secret) ? $app_secret : variable_get('fboauth_secret', '');

  // Note that the "code" provided by Facebook is a hash based on the client_id,
  // client_secret, and redirect_url. All of these things must be IDENTICAL to
  // the same values that were passed to Facebook in the approval request. See
  // the fboauth_link_properties function.
  $query = array(
    'client_id' => $app_id,
    'client_secret' => $app_secret,
    'redirect_uri' => fboauth_action_url('fboauth/' . $action_name, array('absolute' => TRUE, 'query' => !empty($_GET['destination']) ? array('destination' => $_GET['destination']) : array())),
    'code' => $code,
  );

  $token_url = url('https://graph.facebook.com/' . variable_get('fboauth_apiversion', 'v2.3') . '/oauth/access_token', array('absolute' => TRUE, 'query' => $query));

  $attempts = 5;
  do {
    $authentication_result = drupal_http_request($token_url);
    if ($authentication_result->code == 200) {
      break;
    }
    // Facebook access code generation seems to take a lot of time. That's why we have to wait
    // some seconds before the code can be acquired.
    sleep(1);
  } while (--$attempts > 0);

  if ($authentication_result->code != 200) {
    $error = !empty($authentication_result->error) ? $authentication_result->error : t('(no error returned)');
    $data = !empty($authentication_result->data) ? print_r($authentication_result->data, TRUE) : t('(no data returned)');
    watchdog('fboauth', 'Facebook OAuth could not acquire an access token from Facebook. 
      We queried the following URL: <code><pre>@url</pre></code>.' .
        " Facebook's servers returned an error " .
        '@error: <code><pre>@return</pre></code>', array('@url' => $token_url, '@error' => $error, '@return' => $data));
  }
  else {
    $authentication_values = drupal_json_decode($authentication_result->data);
  }

  return isset($authentication_values['access_token']) ? $authentication_values['access_token'] : NULL;
}

/**
 * Return a list of permissions based on a list of properties or connections.
 *
 * @param array $access_requested
 *   Optional. A list of Facebook user properties or connections. If not
 *   specified, a list of all known permissions will be returned.
 *
 * @return array
 *   A list of Facebook permission names necessary to access those properties or
 *   connections.
 *
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_permissions($access_requested = NULL) {
  $permissions = array();
  $permission_names = array(
    'user_about_me' => t('About yourself description'),
    'user_birthday' => t('Your birthday'),
    'user_education_history' => t('Your education history'),
    'user_events' => t("Events you're attending"),
    'user_friends' => t('Access your friends'),
    'user_groups' => t('Your groups'),
    'user_hometown' => t('Your hometown'),
    'user_likes' => t('Your likes'),
    'user_location' => t('Your location'),
    'user_photo_video_tags' => t("Photos and videos you've been tagged in"),
    'user_photos' => t("Photos you've uploaded"),
    'user_relationships' => t('Access to your family and personal relationships and relationship status'),
    'user_relationship_details' => t('Your relationship preferences'),
    'user_religion_politics' => t('Your religious and political affiliations'),
    'user_status' => t('Your most recent status message'),
    'user_videos' => t("Videos you've uploaded"),
    'user_website' => t('Your website URL'),
    'user_work_history' => t('Your work history'),
    'email' => t('Your e-mail'),
    'read_custom_friendlists' => t('Access your lists of friends'),
    'read_insights' => t('Access your Facebook insights data'),
    'read_mailbox' => t('Access your Facebook inbox'),
    'read_stream' => t('Access your new feed'),
    'ads_management' => t('Manage your ads'),
  );

  $properties = fboauth_user_properties();
  foreach ($properties as $property => $property_info) {
    if (isset($property_info['permission']) && (!isset($access_requested) || in_array($property, $access_requested))) {
      $permissions[$property_info['permission']] = isset($permission_names[$property_info['permission']]) ? $permission_names[$property_info['permission']] : $property_info['permission'];
    }
  }

  $connections = fboauth_user_connections();
  foreach ($connections as $connection => $connection_info) {
    if (isset($connection_info['permission']) && (!isset($access_requested) || in_array($connection, $access_requested))) {
      $permissions[$connection_info['permission']] = isset($permission_names[$connection_info['permission']]) ? $permission_names[$connection_info['permission']] : $connection_info['permission'];
    }
  }

  return $permissions;
}

/**
 * Return a list of Facebook user properties.
 *
 * This function provides a list of properties that may be attached directly to
 * a Facebook user account. This information is immediately available when a
 * user logs in with Facebook connect and may be stored locally.
 *
 * Each property requires extended permission granted by the end-user. The
 * returned array of properties provides the name of each required permission
 * and a human-readable name for the property.
 *
 * Note that access to a user's id, name, first_name, last_name, gender, locale,
 * link, timezone, updated_time, and verified properties are always available
 * if the user grants generic access to your application.
 *
 * @param bool $include_common
 *   Optionally include all common properties in this list.
 *
 * @see fboauth_user_connections()
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_properties($include_common = FALSE) {
  $properties = array(
    'about' => array(
      'permission' => 'user_about_me',
      'label' => t('About me (a short bio)'),
      'field_types' => array('text', 'text_long'),
    ),
    'bio' => array(
      'permission' => 'user_about_me',
      'label' => t('Biography'),
      'field_types' => array('text_long'),
    ),
    'birthday' => array(
      'permission' => 'user_birthday',
      'label' => t('Birthday'),
      'field_types' => array('text', 'date', 'datetime', 'datestamp'),
    ),
    'education' => array(
      'permission' => 'user_education_history',
      'label' => t('Education history'),
    ),
    'email' => array(
      'permission' => 'email',
      'label' => t('E-mail'),
    ),
    'hometown' => array(
      'permission' => 'user_hometown',
      'label' => t('Hometown'),
      'field_types' => array('text'),
    ),
    'location' => array(
      'permission' => 'user_location',
      'label' => t('Location'),
      'field_types' => array('text', 'location'),
    ),
    'relationship_status' => array(
      'permission' => 'user_relationships',
      'label' => t("Relationship status (Single, Married, It's complicated, etc.)"),
      'field_types' => array('text', 'list_text'),
    ),
    'interested_in' => array(
      'permission' => 'user_relationship_details',
      'label' => t('Interested in (Men, Women)'),
    ),
    'significant_other' => array(
      'permission' => 'user_relationship_details',
      'label' => t('Signficant other'),
      'field_types' => array('text'),
    ),
    'political' => array(
      'permission' => 'user_religion_politics',
      'label' => t('Political view'),
      'field_types' => array('text'),
    ),
    'quotes' => array(
      'permission' => 'user_about_me',
      'label' => t('Favorite quotes'),
    ),
    'religion' => array(
      'permission' => 'user_religion_politics',
      'label' => t('Religion'),
      'field_types' => array('text'),
    ),
    'website' => array(
      'permission' => 'user_website',
      'label' => t('Website'),
      'field_types' => array('text', 'link_field'),
    ),
    'work' => array(
      'permission' => 'user_work_history',
      'label' => t('Work history'),
    ),
    'picture' => array(
      'label' => t('Profile picture'),
      'field_types' => array('image'),
    ),
  );

  // Common properties. Always defined so that modules may alter if needed.
  $common = array(
    'id' => array(
      'label' => t('Facebook ID'),
    ),
    'name' => array(
      'label' => t('Full name'),
      'field_types' => array('text'),
    ),
    'first_name' => array(
      'label' => t('First name'),
      'field_types' => array('text'),
    ),
    'last_name' => array(
      'label' => t('Last name'),
      'field_types' => array('text'),
    ),
    'gender' => array(
      'label' => t('Gender'),
      'field_types' => array('text', 'list_text'),
    ),
    'locale' => array(
      'label' => t('Locale'),
    ),
    'link' => array(
      'label' => t('Facebook profile link'),
      'field_types' => array('text', 'link_field'),
    ),
    'timezone' => array(
      'label' => t('Timezone'),
    ),
    'updated_time' => array(
      'label' => t('Updated time'),
    ),
    'verified' => array(
      'label' => t('Verified'),
    ),
  );
  $properties += $common;

  drupal_alter('fboauth_user_properties', $properties);
  ksort($properties);

  // Filter out unneeded properties.
  if (!$include_common) {
    $properties = array_diff_key($properties, $common);
  }

  return $properties;
}

/**
 * Return a list of Facebook connection points.
 *
 * This function provides a list of all of the Facebook GraphAPI connection
 * points that can be access to learn extended information about a user. Usually
 * each of these connection points will allow querying against content the user
 * has created as opposed to information directly about the user.
 *
 * Each connection requires extended permission granted by the end-user. The
 * returned array of connections provides the name of each required permission
 * and a human-readable name for the connection.
 *
 * Note that access to the "picture" connection are included by default with
 * the public_profile permission, which is always required.
 *
 * @see fboauth_user_properties()
 * @see http://developers.facebook.com/docs/reference/api/user/
 * @see http://developers.facebook.com/docs/authentication/permissions/
 */
function fboauth_user_connections() {
  return array(
    'accounts' => array(
      'permission' => 'manage_pages',
      'label' => t('Account pages'),
    ),
    'apprequests' => array(
      'label' => t('App requests'),
    // No permission.
    ),
    'albums' => array(
      'permission' => 'user_photos',
      'label' => t('Photo albums'),
    ),
    'books' => array(
      'permission' => 'user_likes',
      'label' => t('Books liked'),
    ),
    'events' => array(
      'permission' => 'user_events',
      'label' => t('Events'),
    ),
    'feed' => array(
      'permission' => 'read_stream',
      'label' => t("User's wall"),
    ),
    'friendlists' => array(
      'permission' => 'read_custom_friendlists',
      'label' => t('Lists of friends'),
    ),
    'friends' => array(
      'permission' => 'user_friends',
      'label' => t('Friends'),
    ),
    'home' => array(
      'permission' => 'read_stream',
      'label' => t("User's news feed"),
    ),
    'inbox' => array(
      'permission' => 'read_mailbox',
      'label' => t('Inbox threads'),
    ),
    'likes' => array(
      'permission' => 'user_likes',
      'label' => t('Pages liked'),
    ),
    'links' => array(
      'permission' => 'read_stream',
      'label' => t('Posted links'),
    ),
    'movies' => array(
      'permission' => 'user_likes',
      'label' => t('Movies liked'),
    ),
    'music' => array(
      'permission' => 'user_likes',
      'label' => t('Music liked'),
    ),
    'outbox' => array(
      'permission' => 'read_mailbox',
      'label' => t('Outbox'),
    ),
    'photos' => array(
      'permission' => 'user_photos',
      'label' => t('Photos'),
    ),
    'posts' => array(
      'permission' => 'user_posts',
      'label' => t('Posts'),
    ),
    'statuses' => array(
      'permission' => 'read_stream',
      'label' => t('Status updates'),
    ),
    'tagged' => array(
      'permission' => 'read_stream',
      'label' => t('Tagged in photos, videos, and posts'),
    ),
    'tagged_places' => array(
      'permission' => 'user_tagged_places',
      'label' => t('List of tagged places for this person'),
    ),
    'television' => array(
      'permission' => 'user_likes',
      'label' => t('Television liked'),
    ),
    'updates' => array(
      'permission' => 'read_mailbox',
      'label' => t('Inbox updates'),
    ),
    'videos' => array(
      'permission' => 'user_videos',
      'label' => t('Videos'),
    ),
  );
}

/**
 * Utility function to retrieve all permissions required for Facebook connect.
 */
function fboauth_user_connect_permissions() {
  $connect_permissions = array();
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_email', TRUE) ? array('email') : array());
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_properties', array()));
  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_connections', array()));

  if (module_exists('profile')) {
    $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_profile', array()));
  }

  $connect_permissions += fboauth_user_permissions(variable_get('fboauth_user_fields', array()));

  return $connect_permissions;
}

/**
 * Execute a Graph API query through Facebook.
 *
 * @see http://developers.facebook.com/docs/reference/api/
 */
function fboauth_graph_query($id, $access_token = NULL, $parameters = array(), $method = 'GET') {
  if (isset($access_token)) {
    $parameters['access_token'] = $access_token;
  }

  if ($method == 'GET' || $method == 'DELETE') {
    $graph_url = url('https://graph.facebook.com/' . variable_get('fboauth_apiversion', 'v2.3') . '/' . $id, array(
      'absolute' => TRUE,
      'query' => $parameters));
    $graph_result = drupal_http_request(
        $graph_url, array(
      'headers' => array(),
      'method' => $method));
  }
  elseif ($method == 'POST') {
    $graph_url = 'https://graph.facebook.com/' . variable_get('fboauth_apiversion', 'v2.3') . '/' . $id;
    $post_data = http_build_query($parameters, '', '&');
    $graph_result = drupal_http_request(
        $graph_url, array(
      'headers' => array(),
      'method' => $method,
      'data' => $post_data));
  }
  else {
    drupal_set_message(t('Invalid request type "@type" for Facebook graphy query. Must be either @get, @post, or @delete.', array(
      '@type' => $method,
      '@get' => 'GET',
      '@post' => 'POST',
      '@delete' => 'DELETE')), 'error');
  }

  // If the response contains a redirect (such as to an image), return the
  // redirect as the data. i.e. https://graph.facebook.com/v2.3/19292868552/picture.
  if (isset($graph_result->redirect_url)) {
    $data = array(
      'data' => $graph_result->data,
      'redirect_code' => $graph_result->redirect_code,
      'redirect_url' => $graph_result->redirect_url,
    );
  }
  else {
    $data = json_decode($graph_result->data);
  }

  return $data;
}

/**
 * Process a deauthorization request from Facebook.
 *
 * @see https://developers.facebook.com/docs/authentication/signed_request/
 */
function fboauth_deauthorize() {
  // A signed_request key is used when a deauth url is provided by the app.
  if (isset($_POST['signed_request'])) {
    $app_secret = variable_get('fboauth_secret', '');
    $signed_request = fboauth_parse_signed_request(check_plain($_POST['signed_request']), $app_secret);

    if ($signed_request && $deauth_uid = fboauth_uid_load($signed_request['user_id'])) {
      fboauth_save($deauth_uid, NULL);
      watchdog('fboauth', 'The account for UID @uid has been disconnected from Facebook by the user via Facebook.', array('@uid' => $deauth_uid));

      // Allow other modules to hook into a deauth event.
      module_invoke_all('fboauth_deauthorize', $deauth_uid, $signed_request['user_id']);
    }
  }
}

/**
 * Parse a signed_request from Facebook.
 *
 * @see http://developers.facebook.com/docs/authentication/signed_request/
 */
function fboauth_parse_signed_request($signed_request, $secret) {
  list($encoded_signature, $payload) = explode('.', $signed_request, 2);

  // Decode the data.
  $signature = fboauth_base64_url_decode($encoded_signature);
  $data = json_decode(fboauth_base64_url_decode($payload), TRUE);

  if (strtoupper($data['algorithm']) !== 'HMAC-SHA256') {
    watchdog('fboauth', 'A Facebook deauthorization request failed: Unknown signed request algorithm. Expected HMAC-SHA256.');
    return NULL;
  }

  // Check the signature.
  $expected_signature = hash_hmac('sha256', $payload, $secret, $raw = TRUE);
  if ($signature !== $expected_signature) {
    watchdog('fboauth', 'A Facebook deauthorization request failed: Bad Signed JSON signature!');
    return NULL;
  }

  return $data;
}

/**
 * Helper function for signed_requests from Facebook.
 *
 * @param string $input
 *   A string of text passed back from a Facebook signed request.
 *
 * @return string
 *   The decoded string.
 *
 * @see fboauth_parse_signed_request()
 */
function fboauth_base64_url_decode($input) {
  return base64_decode(strtr($input, '-_', '+/'));
}
