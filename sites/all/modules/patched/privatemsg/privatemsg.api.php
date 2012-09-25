<?php

/**
 * @file
 * Privatemsg API Documentation
 */

/**
 * @mainpage Privatemsg API Documentation
 * This is the API documentation of Privatemsg.
 *
 * - Topics:
 *  - @link api API functions @endlink
 *  - @link sql Query builder and related hooks @endlink
 *  - @link message_hooks Message hooks @endlink
 *  - @link generic_hooks Generic hooks @endlink
 *  - @link theming Theming @endlink
 *  - @link types Types of recipients @endlink
 */

/**
 * @defgroup sql Query Builder
 * Privatemsg does use its own simple query builder which allows to extend
 * SELECT-Queries in an easy way. The function _privatemsg_assemble_query
 * creates the query, based on an array $fragments with the following content.
 * Except primary_table, each key is an array itself to allow multiple values
 *
 *  - primary_table: The main table to select from
 *  - select: The fields that should be selected. This can be a simple field, a
 *    field with alias or even a subquery.
 *  - inner_join: The tables that should be joined. This is not specific to
 *    inner joins.
 *    Example: INNER JOIN pm_index pmi ON (pmi.mid = pm.mid)
 *  - where: The where conditions. The conditions are always AND, but it is
 *    possible to use OR inside a condition.
 *    Example: ⁽pmi.is_new = 1 OR pmi.deleted = 1)
 *  - order_by: Order By values, example: pm.timestamp ASC
 *  - query_args: It is possible to use the placeholders like %s in each part of
 *    the query. The values of query_args are then inserted into these.
 *    query_args consists of three arrays (join, where, having), one for each
 *    key that currently supports arguments.
 *
 * Use _privatemsg_assemble_query
 *
 * The privatemsg_assemble_query function takes a query_id as first argument
 * and optionally one or multiple arguments. query_id can either be a
 * string ('some_id') or an array('group_name', 'query_id'), if a string
 * is supplied, group_name defaults to 'privatemsg'. Returned is an array
 * with the keys 'query' (normal query) and 'count' (count query for pager).
 *
 * For the actual query data, the function group_name_sql_query_id is executed,
 * this functions does have $fragments as first parameter and then the
 * optional parameters.
 *
 * A short example:
 * @code
 * // First, create the sql function.
 * function privatemsg_sql_getsubject(&$fragments, $mid, $uid) {
 *   // Set the primary table.
 *   $fragments['primary_table'] = '{pm_message} pm';
 *
 *   // Add a field.
 *   $fragments['select'][] = 'pm.subject';
 *
 *   // Join another table.
 *   $fragment['inner_join'][] = 'JOIN {pm_index} pi ON (pi.mid = pm.mid)';
 *   $fragment['query_args']['join'][] $uid;
 *
 *   // And finally add a condition.
 *   $fragments['where'][] = 'pm.mid = %d';
 *   $fragments['query_args']['where'][] = $mid;
 * }
 *
 * // Now we can use that query everywhere.
 * $query = _privatemsg_assemble_query('getsubject', 5);
 * $result = db_query($query['query']);
 * @endcode
 * Extend existing queries
 *
 * To extend a privatemsg query, use hook_group_name_sql_query_id_alter.
 * This hook does use the same parameters as the sql function.
 *
 * Example:
 * @code
 * function mymodule_privatemsg_sql_getsubject_alter(&$fragments, $mid) {
 * // we want to load the body too..
 *  $fragments['select'][] = 'pm.body';
 * }
 * @endcode
 *
 * - List of sql query hooks.
 *  - list: List of messages, Parameters: $fragments, $accounty
 *  - list_sent: List of sent messages, Parameters: $fragments, $accounty
 *  - load: Load a single message, Parameters: $fragments, $pmid, $account
 *  - messages: Load the messages of a thread,
 *    Parameters: $fragments, $thread_id, $account
 *  - autocomplete: Searching usernames for the autocomplete feature,
 *    Parameters: $fragments, $search, $names
 *  - participants: Loads all participants of a thread,
 *    Parameters: $fragments, $thread_id
 *  - unread_count: Number of unread messages for a user,
 *    Parameters: $fragments, $account
 *
 * - The following query_id's are used in pm_block_user
 *  - threadautors: Return all authors of one or multiple threads,
 *    Parameters: $fragments, $threads
 */

/**
 * @addtogroup sql
 * @{
 */

/**
 * Query to search for autocomplete usernames.
 *
 * @param $fragments
 *   Query fragments
 * @param $search
 *   Search for that username
 * @param $names
 *   Names that are already in the list and are excluded
 */
function hook_privatemsg_sql_autocomplete_alter(&$fragments, $search, $names) {
  global $user;
  // Extend the query that searches for usernames

  // $fragments is explained in the api documentation in detail

  // The query is already set up, it's searching for usernames which start with
  // $search and are not $names (may be empty)
  // the main table is {user} a

  // for example, add a join on a table where the user connections are stored
  // and specify that only users connected with the current user should be
  // loaded.
  $fragments['inner_join'] = 'INNER JOIN {my_table} m ON (m.user1 = u.uid AND m.user2 = %d)';
  $fragments['query_args'][] = $user->uid;
}

/**
 * Display a list of threads.
 *
 * @param $fragments
 *   Query fragments
 * @param $account
 *   User object
 */
function hook_privatemsg_sql_list_alter(&$fragment, $account) {

}

/**
 * Query definition to load a message.
 *
 * @param $fragments
 *   Query fragments array.
 * @param $pmid
 *   the id of the message.
  * @param $account
 *   User object of account for which to load the message.
 */
function hook_privatemsg_sql_load_alter(&$fragments, $pmid, $account = NULL) {

}

/**
 * Query definition to load messages of one or multiple threads.
 *
 * @param $fragments
 *   Query fragments array.
 * @param $threads
 *   Array with one or multiple thread id's.
 * @param $account
 *   User object for which the messages are being loaded.
 * @param $load_all
 *   Deleted messages are only loaded if this is set to TRUE.
 */
function hook_privatemsg_sql_messages_alter(&$fragments, $threads, $account = NULL, $load_all = FALSE) {

}

/**
 * Alter the query that loads the participants of a thread.
 *
 * @param $fragments
 *   Query fragments
 * @param $thread_id
 *   Thread id, pmi.thread_id is the same as the mid of the first
 *   message of that thread
 */
function hook_privatemsg_sql_participants_alter(&$fragment, $thread_id) {

}

/**
 * Loads all unread messages of a user (only the count query is used).
 *
 * @param $fragments
 *   Query fragments
 * @param $account
 *   User object
 */
function hook_privatemsg_sql_unread_count_alter(&$fragment, $account) {

}

/**
 * @}
 */

/**
 * @defgroup api API functions
 *
 * There are two different functions to send messages.
 * Either by starting a @link privatemsg_new_thread new thread @endlink
 * or @link privatemsg_reply reply @endlink to an existing thread.
 *
 * There is also a function which returns a link to the privatemsg new message
 * form with the recipient pre-filled if the user is allowed to.
 * privatemsg_get_link().
 */

/**
 * @defgroup message_hooks Message hooks
 * All message-level hooks look like hook_privatemsg_message_op,
 * where op is one of the following:
 * - @link hook_privatemsg_message_load load @endlink: Called when a full
 *   message is loaded similiar to nodeapi_load, new values can be returned and
 *   will be added to $message, parameter: $message
 * - @link hook_privatemsg_message_validate validate @endlink: Validation,
 *   before the message is sent/saved. Return validation errors as array,
 *   parameter: $message, $form = FALSE
 * - @link hook_privatemsg_message_presave_alter presave_alter @endlink: Last
 *   changes to $message before the message is saved, parameter: $message
 * - @link hook_privatemsg_message_insert insert @endlink: message has been
 *   saved, $message has been updated with the mid and thread_id,
 *   parameter: $message
 * - @link hook_privatemsg_message_delete delete @endlink: the message is
 *   going to be deleted, parameter: $message
 * - @link hook_privatemsg_message_view_alter view_alter @endlink: the message
 *   is going to be displayed, parameter: $vars
 * - @link hook_privatemsg_message_recipient_change recipient changed @endlink:
 *   a recipient is added or removed from/to a message.
 *
 * In hooks with _alter suffix, $message is by reference.
 *
 * $message is an array, with all the relevant information about the message.
 * The information in there can be changed/extended by modules, but looks
 * typically like this:
 * @code
 * array (
 *   'mid' => 3517, // message id, identifies a message
 *   'author' => 27, // author id
 *   'subject' => 'raclebugav', // Message subject
 *   'body' => 'bla bla', // Body of the message
 *   'timestamp' => 351287003, // unix timestamp, creation time
 *   'is_new' => 0, // If the message has been read by the user
 *   'thread_id' => 3341, // thread id, this is actually the mid from the first
 *                           message of the thread
 * )
 * @endcode
 */

/**
 * @addtogroup message_hooks
 * @{
 */

/**
 * Is called after the message has been loaded.
 *
 * Return data will be merged with the $message array.
 *
 * @param $message
 *    Message array
 */
function hook_privatemsg_message_load($message) {
  return array('my_key' => 'my_value');
}

/**
 * Is called when a message is flushed.
 *
 * The message will be deleted from the database, remove any related data here.
 *
 * @param $message
 *   Message array
 */
function hook_privatemsg_message_flush($message) {

}

/**
 * Validate a message before it is sent/saved in the database.
 *
 * Validation errors can be returned, either as a string or as array when there
 * are multiple errors. If the $form flag is set, errors should be reported
 * with form_set_error instead.
 *
 * @todo adapt api return value changes
 *
 * @param $message
 *   Message array
 */
function hook_privatemsg_message_validate($message, $form = FALSE) {
  global $_privatemsg_invalid_recipients;
  $_privatemsg_invalid_recipients = array();

  $errors = array();

  foreach ($message['recipients'] as $recipient) {
    if ($recipient->name == 'blocked user') {
      $_privatemsg_invalid_recipients[] = $recipient->uid;
      $errors[] = t('%name has chosen to not recieve any more messages from you.', array('%name' => privatemsg_recipient_format($recipient, array('plain' => TRUE))));
    }
  }
}

/**
 * Change the message before it is stored.
 *
 * Alter the message, for example remove recipients that have been detected as
 * invalid or forbidden in the validate hook.
 *
 * @param $message
 *   Message array
 */
function hook_privatemsg_message_presave_alter(&$message) {
  // delete recipients which have been marked as invalid
  global $_privatemsg_invalid_recipients;
  foreach ($_privatemsg_invalid_recipients as $invalid) {
    unset($message['recipients'][$invalid]);
  }
}
/**
 * Act on the $vars before a message is displayed.
 *
 * This is called in the preprocess hook of the privatemsg-view template.
 * The $message data is aviable in $vars['message'].
 *
 * @param $var
 *   Template variables
 */
function hook_privatemsg_message_view_alter(&$var) {
  // add a link to each message
  $vars['message_links'][] = array('title' => t('My link'), 'href' => '/path/to/my/action/'. $vars['message']['mid']);
}

/**
 * This hook is executed after the message has been saved.
 *
 * $message is updated with mid and thread id. Use this hook to store data,
 * that needs to point to the saved message for example attachments.
 *
 * @param $message
 *   Message array
 */
function hook_privatemsg_message_insert($message) {
  _mymodule_save_data($message['mid']);
}

/**
 * This hook is invoked when a recipient is added to a message.
 *
 * Since the hook might be invoked hundreds of times during batch or cron, only
 * ids are passed and not complete user/message objects.
 *
 * @param $mid
 *   Id of the message.
 * @param $thread_id
 *   Id of the thread the message belongs to.
 * @param $recipient_id
 *   Recipient id, a user id if type is user or hidden.
 * @param $type
 *   Type of the recipient.
 * @param $added
 *   TRUE if the recipient is added, FALSE if he is removed.
 */
function hook_privatemsg_message_recipient_changed($mid, $thread_id, $recipient_id, $type, $added) {
  if ($added && ($type == 'user' || $type == 'hidden')) {
    privatemsg_filter_add_tags(array($thread_id), variable_get('privatemsg_filter_inbox_tag', ''), (object)array('uid' => $recipient));
  }
}

/**
 * @}
 */

/**
 * @defgroup generic_hooks Generic Hooks
 * @{
 *
 * Some generic hooks that can't be categorized.
 */

/**
 * Check if the author can send a message to the recipients.
 *
 * This can be used to limit who can write whom based on other modules and/or
 * settings.
 *
 * @param $author
 *   Author of the message to be sent
 * @param $recipients
 *   Recipient of the message
 * @param $context
 *   Additional information. Can contain the thread_id to indicate that this is
 *   a reply on an existing thread.
 * @return
 *   An indexed array of arrays with the keys recipient ({type}_{key}) and
 *   message (The reason why the recipient has been blocked).
 */
function hook_privatemsg_block_message($author, $recipients, $context = array()) {
  $blocked = array();
  foreach($recipients as $recipient_id => $recipient) {
    // Deny/block if the recipient type is role and the account does not have
    // the necessary permission.
    if ($recipient->type == 'role' && $recipient->recipient == 2) {
      $blocked[] = array(
        'recipient' => $recipient_id,
        'message' => t('Not allowed to write private messages to the role authenticated user'),
      );
    }
  }
  return $blocked;
}
/**
 * Add content to the view thread page.
 *
 * @param $content
 *   Render-able array of the content of the page.
 * @param $thread
 *   Thread object which contains the messages and other information about
 *   the thread.
 */
function hook_privatemsg_view_messages_alter($content, $thread) {
  if (privatemsg_user_access('tag private messages')) {
    $content['tags'] = privatemsg_filter_show_tags($thread['thread_id'], !empty($_GET['show_tags_form']));
  }
}

/**
 * List of possible templates.
 */
function hook_privatemsg_view_template() {

}

/**
 * Expose operations/actions which can be executed on threads.
 *
 * Return an array of operations to privatemsg, the key of each operation is the
 * operation key or name.
 *
 * @see _privatemsg_action_form()
 * @see privatemsg_list_submit()
 */
function hook_privatemsg_thread_operations() {
  return array(
    'operation key' => array(
      'label' => 'Label of the operation. Only use this if the operation
                  should be displayed automatically in the action form',
      'callback' => 'privatemsg_thread_change_status', // Function callback that will be executed.
      'callback arguments' => array('status' => PRIVATEMSG_READ), // Additional arguments to above function
      'undo callback' => 'privatemsg_thread_change_status',  // Provide a function which can "undo" the operation. Optional.
      'undo callback arguments' => array('status' => PRIVATEMSG_UNREAD), // Additional arguments to above function.
    ),
  );
}

/**
 * Allows to alter the found autocompletions.
 *
 * @param $matches
 *   Array of matching recipient objects.
 * @param $names
 *   Array of names that are already in the list.
 * @param $fragment
 *   Fragment that is currently searched for.
 */
function hook_privatemsg_autocomplete_alter(&$matches, $names, $fragment) {
  // Remove all types other than user if accessed through
  // messages/user/autocomplete.
  if (arg(1) == 'user') {
    foreach ($matches as $id => $match) {
      if ($match->type != 'user') {
        unset($matches[$id]);
      }
    }
  }
}

/**
 * Allows to alter found recipient types for a given string.
 *
 * @param $matches
 *   Array of matching recipient objects.
 * @param $string
 *   String representation of the recipient.
 */
function hook_privatemsg_name_lookup_matches(&$matches, $string) {

}

/**
 * Allows response to a successful operation.
 *
 * @param $operation
 *   The operation that was executed.
 * @param $threads
 *   An array which contains the thread ids on which the operation
 *   has been executed.
 * @param $account
 *   An user account object if an other user than the currrently logged in is
 *   affected.
 *
 * @see hook_privatemsg_thread_operations()
 * @see privatemsg_operation_execute()
 */
function hook_privatemsg_operation_executed($operation, $threads, $account = NULL) {

}

/**
 * Allows response to a status change.
 *
 * @param $pmid
 *   Message id.
 * @param $status
 *   Either PRIVATEMSG_READ or PRIVATEMSG_UNREAD.
 * @param $account
 *   User object, defaults to the current user.
 *
 * @see privatemsg_message_change_status()
 */
function hook_privatemsg_message_status_changed($pmid, $status, $account) {

}

/**
 * Allows response to a deleted change.
 *
 * Modules implementing this hook should be aware that messages are only
 * marked as deleted and not removed from the database. They will only
 * eventually be deleted by the flushing.
 *
 * Therefore, modules should not delete data in this hook but in
 * hook_privatemsg_message_flush().
 *
 * @param $mid
 *   Message id.
 * @param $deleted
 *   TRUE when the message was marked as deleted, FALSE when marked as not
 *   deleted.
 * @param $account
 *   User object, if NULL then the message was marked as deleted for all users.
 *
 * @see privatemsg_message_change_delete()
 * @see privatemsg_thread_change_delete()
 *
 */
function hook_privatemsg_message_status_delete($mid, $deleted, $account) {

}

/**
 * @}
 */

/**
 * @defgroup types Types of recipients
 *
 * It is possible to define other types of recipients than the usual single
 * user. These types are defined through a hook and a few callbacks and are
 * stored in the {pm_index} table for each recipient entry.
 *
 * The combination of a recipient type and recipient ID defines a unique
 * recipient. Note: The recipient ID must be an integer; in older versions,
 * this was simply a user ID.
 *
 * This feature is usually used to define groups of recipients. Privatemsg
 * comes with the privatemsg_roles sub-module, which allows to send messages to
 * all members of a specific group.
 *
 * When sending a new message with a recipient type other than user, Privatemsg
 * only inserts a single entry for that recipient type. However, when looking
 * for messages for a user, Privatemsg only looks for recipient types user and
 * hidden. To fill the gap, Privatemsg defines three ways how a non-user
 * type is converted to hidden recipient entries.
 *
 * - For small recipient types (by default <100 recipients, configurable), the
 *   entries are added directly after saving the original private message.
 * - When sending messages through the UI, bigger recipient types are handled
 *   with batch API.
 * - For messages sent by the API, the hidden recipients are generated during
 *   cron runs.
 *
 * Once all hidden recipients are added, the original recipient type is marked
 * as read so Privatemsg knows that he has been processed.
 *
 * Privatemsg defines the following types:
 *
 * - user: This is the default recipient type which is used for a single user.
 * - hidden: Used to add internal recipient entries for other recipient types.
 * - role: The sub-module privatemsg_roles defines an additional type called
 *   role. This allows to send messages to all members of a role.
 *
 * To implement a new type, the following hooks need to be implemented. Note
 * that most of these hooks can also be used alone for other functionality than
 * defining recipient types.
 *
 * - hook_privatemsg_recipient_type_info() - Tell Privatemsg about your
 *   recipient type(s).
 * - hook_privatemsg_name_lookup() - Convert a string to an
 *   recipient object
 *
 * Additionaly, there is also a hook_privatemsg_recipient_type_info_alter() that
 * allows to alter recipient type definitions.
 */

/**
 * @addtogroup types
 * @{
 */


/**
 * This hook is used to tell privatemsg about the recipient types defined by a
 * module. Each type consists of an array keyed by the internal recipient type
 * name and the following keys must be defined.
 *
 * * name: Translated name of the recipient type.
 * * description: A short description of how to send messages to to that
 *   recipient type. This is displayed below the To: field when sending a
 *   message.
 * * load: A callback function that can load recipients based on their id,
 *   example: privatemsg_roles_load_multiple().
 * * format: Theme function to format the recipient before displaying. Must be
 *   defined with hook_theme(), example: theme_privatemsg_roles_format().
 * * autocomplete: Function callback to return possible autocomplete matches,
 *   example: privatemsg_roles_autocomplete().
 * * generate recipients: Function callback to return user ids which belong to a
 *   recipient type, example: privatemsg_roles_load_recipients().
 * * max: Function callback to return the highest user id of a recipient type,
 *   example: privatemsg_roles_count_recipients().
 * * write access: Optionally define a permission which controls write access
 *   to that recipient type.
 * * write callback: Optionally define a callback function that returns an
 *   access decision (allow = TRUE, deny = FALSE) for whether the current user
 *   can write to recipients of the given recipient type.
 * * view access: Optionally define a permission which controls if the user is
 *   able to see the recipient when he is looking at a thread.
 * * view callback: Optionally define a callback function that returns an
 *   access decision (allow = TRUE, deny = FALSE) for whether the current user
 *   can see recipients of the given recipient type.
 */
function hook_privatemsg_recipient_type_info() {
  return array(
    'role' => array(
      'name' => t('Role'),
      'description' => t('Enter the name of a role to write a message to all users which have that role. Example: authenticated user.'),
      'load' => 'privatemsg_roles_load_multiple',
      'format' => 'privatemsg_roles_format',
      'autocomplete' => 'privatemsg_roles_autocomplete',
      'generate recipients' => 'privatemsg_roles_load_recipients',
      'count' => 'privatemsg_roles_count_recipients',
      'write callback' => 'privatemsg_roles_write_access',
      'view access' => 'view roles recipients',
    ),
  );
}

/**
 * Hook which allows to look up a user object.
 *
 * You can try to look up a user object based on the information passed to the
 * hook. The first hook that successfully looks up a specific string wins.
 *
 * Therefore, it is important to only return something if you can actually look
 * up the string.
 */
function hook_privatemsg_name_lookup($string) {
  $result = db_query("SELECT *, rid AS recipient FROM {role} WHERE name = '%s'", trim($role));
  if ($role = db_fetch_object($result)) {
    $role->type = 'role';
    return $role;
  }
}

/**
 * Allows to alter the defined recipient types.
 *
 * @param $types
 *   Array with the recipient types.
 *
 * @see hook_privatemsg_recipient_type_info()
 */
function hook_privatemsg_recipient_type_info_alter(&$types) {

}

/**
 * @}
 */

/**
 * Declare headers for message listings.
 *
 * @return
 *   An array keyed by an identifier. All header definition keys for theme_table
 *   and tablesortort_sql() and the following additional keys:
 *     - #enabled: TRUE if the header should be enabled by default. FALSE by
 *                 default.
 *     - #locked: TRUE if it the header should be locked and can not be
 *                enabled or disabled in the user interface.
 *     - #weight: The default weight which can be changed in the user interface.
 *     - #title: A title used in the administrative user interface. Defaults to
 *               data.
 *     - #access: Control if the header is accessible, TRUE or FALSE.
 *     - #theme: Optionally define a theme function for the field. Defaults to
 *               'privatemsg_list_field_$key'.
 *
 * @see theme_table
 * @ingroup theming
 */
function hook_privatemsg_header_info() {
  return array(
    'subject' => array(
      'data'    => t('Subject'),
      'field'   => 'subject',
      'class'   => 'privatemsg-header-subject',
      '#enabled' => TRUE,
      '#locked'  => TRUE,
      '#weight'  => -20,
    ),
  );
}

/**
 * Alter the defined header structure.
 *
 * @param $headers
 *   All headers returned by hook_privatemsg_header_info().
 */
function hook_privatemsg_header_info_alter(&$headers) {

}
