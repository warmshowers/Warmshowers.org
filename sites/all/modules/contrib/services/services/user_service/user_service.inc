<?php
// $Id: user_service.inc,v 1.1.2.8.2.11 2010/02/27 05:57:03 heyrocker Exp $

/**
 * @file
 *  Link general user functionalities to services module.
 */

/**
 * Delete a user.
 *
 * @param $uid
 *   UID of the user to be deleted.
 *
 * @see user_delete()
 */
function user_service_delete($uid) {
  $account = user_load($uid);
  if (empty($account)) {
    return services_error(t('There is no user with such ID.'), 404);
  }
  user_delete($account, $uid);

  // Everything went right.
  return TRUE;
}

/**
 * Check to see if the services account is allowed to delete a user.
 *
 * @param $uid
 *   UID of the user to be deleted.
 */
function user_service_delete_access($uid) {
  global $user;
  return (($user->uid == $uid && user_access('delete own user')) ||
    ($user->uid != $uid && user_access('delete any user')));
}

/**
 * Get user details.
 *
 * @param $uid
 *   UID of the user to be loaded.
 *
 * @return
 *   A user object.
 *
 * @see user_load()
 */
function user_service_get($uid) {
  $account = user_load($uid);
  if (empty($account)) {
    return services_error(t('There is no user with such ID.'), 404);
  }

  // Everything went right.
  return $account;
}

/**
 * Check if the user is allowed to get the user data.
 *
 * @param $uid
 *   Number. The user ID.
 */
function user_service_get_access($uid) {
  global $user;
  return (($user->uid == $uid && user_access('get own user data')) || ($user->uid != $uid && user_access('get any user data')));
}

/**
 * Login a user using the specified credentials.
 *
 * Note this will transfer a plaintext password.
 *
 * @param $username
 *   Username to be logged in.
 * @param $password
 *   Password, must be plain text and not hashed.
 *
 * @return
 *   A valid session object.
 */
function user_service_login($username, $password) {
  global $user;

  if ($user->uid) {
    // user is already logged in
    return services_error(t('Already logged in as !user.', array('!user' => $user->name)), 406);
  }

  $user = user_authenticate(array('name' => $username, 'pass' => $password));

  if ($user->uid) {
    // Regenerate the session ID to prevent against session fixation attacks.
    sess_regenerate();
    module_invoke_all('user', 'login', NULL, $user);

    $return = new stdClass();
    $return->sessid = session_id();
    $return->user = $user;

    return $return;
  }
  session_destroy();
  return services_error(t('Wrong username or password.'), 401);
}

/**
 * Logout the current user.
 */
function user_service_logout() {
  global $user;

  if (!$user->uid) {
    // User is not logged in
    return services_error(t('User is not logged in.'), 406);
  }

  watchdog('user', 'Session closed for %name.', array('%name' => theme('placeholder', $user->name)));

  // Destroy the current session:
  session_destroy();
  module_invoke_all('user', 'logout', NULL, $user);

  // Load the anonymous user
  $user = drupal_anonymous_user();

  return TRUE;
}

/**
 * Save user data.
 *
 * This can create a new user or modify an existing user, depending on the
 * information passed to the function. This function uses drupal_execute()
 * and as such exepects all input to match the submitting form in question.
 *
 * @param $account
 *   An array of account information.
 *
 * @return
 *   The UID of the user whose information was saved.
 */
function user_service_save($account) {
  // Load the required includes for saving profile information
  // with drupal_execute().
  module_load_include('inc', 'user', 'user.pages');
  
  // if uid is present then update, otherwise insert
  $update = user_load($account['uid']);	
  $form_state = array();

  // Any logged in user is by default authenticated,
  // and leaving this role set in the user's roles array
  // causes big problems because of a FAPI hack that controls
  // this checkbox on the user create and edit form (and thus
  // causes problems with drupal_execute()). Therefore we just
  // force it to 0 here.
  if (isset($account['roles'][2])) {
    $account['roles'][2] = 0;
  }

  if (!isset($update->uid)) {
    // register a new user
    $form_state['values'] = $account;
    $form_state['values']['pass'] = array(
      'pass1' => $account['pass'],
      'pass2' => $account['pass'],
    );
    $form_state['values']['op'] = t('Create new account');
    $ret = drupal_execute('user_register', $form_state);
  }
  else {
    // If a profile category was passed in, use it. Otherwise default
    // to 'account' (for saving core user data.)
    $category = 'account';
    if (isset($account['category'])) {
      $category = $account['category'];
      unset($account['category']);
    }
    
    // Drop any passed in values into the $account var. Anything
    // unused by the form just gets ignored.
    foreach ($account as $key => $value) {
      $form_state['values'][$key] = $value;
    }

    $form_state['values']['op'] = 'Save';
    $form_state['values']['_category'] = $category;
    $form_state['values']['_account'] = $account;
    $ret = drupal_execute('user_profile_form', $form_state, (object) $account, $category);
  }

  // Error if needed.
  if ($errors = form_get_errors()) {
    return services_error(implode("\n", $errors), 401);
  }
  else {
    return $form_state['user']->uid;
  }
}

/**
 * Check if the services account is allowed to create or edit user data.
 *
 * @param $account
 *   An array of user account information (note that UID is the only
 *   property used.)
 */
function user_service_save_access($account) {
  global $user;
  return ((empty($account['uid']) && user_access('create new users')) ||
    ($user->uid == $account['uid'] && user_access('update own user data')) ||
    ($user->uid !=  $account['uid'] && user_access('update any user data')));
}
