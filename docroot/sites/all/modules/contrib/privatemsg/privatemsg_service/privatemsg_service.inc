<?php

/**
 * @file
 *  Link general private message module functionalities to services module.
 */

/**
 * Get all of the logged in user's private messages.
 *
 * @param $type
 * String. Which type of messages you would like to retrieve.
 * Possibilities are inbox or sent.
 *
 * @param $load_full
 * Boolean. Whether to load the full message for each message or not.
 *
 * @param $offset
 *   Integer (optional). An offset integer for paging.
 * @param $limit
 *   Integer (optional). A limit integer for paging.
 *
 * @return
 *   An array of messages.
 *
 */
function privatemsg_service_get($type = 'inbox', $load_full = FALSE, $offset = 0, $limit = 0, $uid = 0) {

  $messages = array();
  $pms = array();

  // If no type passed set default type to inbox.
  if (!$type) {
    $type = 'inbox';
  }

  // User needs to be authenticated to proceed.
  global $user;
  if (!user_is_logged_in()) {
    return services_error(t('This user is not logged in.'), 403);
  }

  // If a user id other than the current user's ID is passed,
  // validate that the authenticated user has the correct
  // permissions to read another user's messages.
  if (($uid > 0) && ($uid != $user->uid)) {
    if (user_access("read all private messages")) {
      $account = user_load($uid);
    }
    else {
      return services_error(t('This user does not have permissions to read all messages.'), 403);
    }
  }
  // Use the current user for the account.
  else {
    $account = $user;
  }

  // Construct the query and retrieve the correct set of messages.
  $query = _privatemsg_assemble_query('list', $account, $type);

  // Handle any paging if an offset or a limit was passed in.
  if (empty($offset) && !empty($limit)) {
    $result = db_query_range($query['query'], 0, $limit);
  }
  elseif (!empty($offset) && !empty($limit)) {
    $result = db_query_range($query['query'], $offset, $limit);
  }
  elseif (!empty($offset) && empty($limit)) {
    $rows = (int)db_result(db_query($query['count']));
    $result = db_query_range($query['query'], $offset, $rows);
  }
  else {
    // No paging, we are retrieving everything.
    $result = db_query($query['query']);
  }

  // Loop the result object to get the messages.
  while ($row = db_fetch_object($result)) {
    // If the full thread should be loaded, created an array
    // using the thread_id as the index.
    if ($load_full) {
      $pms[$row->thread_id] = $row;
    }
    // For message previews, just create a numbered array.
    else {
      $pms[] = $row;
    }
  }

  // Load the full thread, if necessary.
  if (!empty($pms) && $load_full) {
    foreach ($pms as $thread_id => $msg) {
      $messages[] = privatemsg_thread_load($thread_id, $account, NULL, FALSE);
    }
  }
  // Otherwise, just return message previews.
  else {
    // Replace array of participant user IDs (only)
    // with more information.
    $messages = _privatemsg_service_enhance_participants($pms);
  }

  // Return messages.
  return $messages;

}

/**
 * Get the number of unread private messages of the logged-in user.
 *
 * @return
 *   The unread count.
 */
function privatemsg_service_unread_count($uid = '') {

  // User needs to be authenticated to proceed.
  global $user;
  if (!user_is_logged_in()) {
    return services_error(t('This user is not logged in.'), 403);
  }

  // If a user id other than the current user's ID is passed,
  // validate that the authenticated user has the correct
  // permissions to read another user's messages.
  if (is_numeric($uid) && ($uid != $user->uid)) {
    if (user_access("read all private messages")) {
      $account = user_load($uid);
    }
    else {
      return services_error(t('This user does not have permissions to use this service.'), 403);
    }
  }
  // Use the current user for the account.
  else {
    $account = $user;
  }

  // Return unread count.
  return privatemsg_unread_count($account);

}

/**
 * Send a private message to one or more recipients.
 *
 * @param $recipients
 * String. A comma separated list of usernames for recipients of the message.
 *
 * @param $subject
 * String. A message subject
 *
 * @param $body
 * String. A message body
 *
 * @return
 * Boolean. Return TRUE if sending the message was successful.
 */
function privatemsg_service_send($recipients, $subject, $body = '') {

  // Make sure the message author is logged in.
  if (!user_is_logged_in()) {
    return services_error(t('Author is not logged in.'), 403);
  }

  // Validate at least 1 recipient has been passed in.
  if ($recipients == '') {
    return services_error(t('There are no recipients, please enter a recipient for the message.'), 400);
  }

  // Validate that this messages has a subject.
  if ($subject == '') {
    return services_error(t('Please specify a subject line.'), 400);
  }

  // Load the full user object.
  global $user;
  $account = user_load($user->uid);

  // Convert the recipients string to an array of user objects.
  list($recipients, $invalid, $duplicates, $denieds) = _privatemsg_parse_userstring($recipients);

  // Problem: At least one of the recipients could not be found.
  if (!empty($invalid)) {
    $invalid_usernames = array('@names' => implode(', ', $invalid));
    return services_error(t('One or more recipients are invalid: @names', $invalid_usernames), 400);
  }

  // Problem: At least one of the recipients could not be found.
  if (!empty($duplicates)) {
    $duplicate_usernames = array('@names' => implode(', ', $duplicates));
    return services_error(t('One or more recipients are not unique: @names', $duplicate_usernames), 400);
  }

  // Problem: At least one of the recipients could not be found.
  if (!empty($denieds)) {
    $denied_usernames = array('@names' => implode(', ', $denied_usernames));
    return services_error(t('Not allowed to write to the following recipients: @names', $denied_usernames), 400);
  }

  // Attempt to send a new message.
  $result = privatemsg_new_thread($recipients, $subject, $body, array('author' => $account));

  // Return success status.
  if ($result['success']) {
    return TRUE;
  }
  else {
    return services_error(implode("\n", $result['messages']['error']), 400);
  }
}

/**
 * Reply to an existing thread.
 *
 * @param $body
 * String. A message body
 *
 * @param $thread_id
 * Integer. A thread ID.
 *
 * @return
 * Boolean. Return TRUE if replying to a thread was successful.
 */
function privatemsg_service_reply($body, $thread_id) {

  // Make sure the message author is logged in.
  if (!user_is_logged_in()) {
    return services_error(t('Author is not logged in.'), 403);
  }

  // Make sure the body is not empty.
  if ($body == '') {
    return services_error(t('Please specify a body for this reply.'), 400);
  }

  // Verify that a thread id has been specified.
  if ($thread_id == '') {
    return services_error(t('Please specify a thread ID for this reply.'), 400);
  }

  // Load the full user object.
  global $user;
  $account = user_load($user->uid);

  // Attempt to send a reply.
  $result = privatemsg_reply($thread_id, $body, array('author' => $account));

  // Return success status.
  if ($result['success']) {
    return TRUE;
  }
  // If $result[0] this means the thread could not be loaded.
  elseif (!empty($result[0])) {
    return services_error($result[0], 404);
  }
  // Else there was some other problem.
  else {
    return services_error(implode("\n", $result['messages']['error']), 400);
  }

}


/**
 * Get all messages in a thread.
 *
 * @param <int> @thread_id
 *   ID of the thread to be loaded.
 * @param <int> $offset
 *   Optional: Message offset from the start of the thread.
 * @return
 *   An array of messages in a thread.
 */
function privatemsg_service_get_thread($thread_id, $offset = 0) {

  // Return if wrong paramters are passed.
  if (!$thread_id || !is_numeric($thread_id)) {
    return services_error(t('Invalid parameters passed'), 400);
  }

  // Make sure the user is logged in.
  global $user;
  $account = user_load($user->uid);
  if (!user_is_logged_in()) {
    return services_error(t('The user is not logged in.'), 403);
  }

  // Return the full thread.
  $thread = privatemsg_thread_load($thread_id, $account, $offset);

  // Simplify thread object and enhance messages.
  $thread = _privatemsg_service_process_thread($thread);

  // Resort the array keys for messages starting at 0.
  $thread['messages'] = array_values($thread['messages']);

  return $thread;

}
