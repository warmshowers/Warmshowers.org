<?php

/**
 * @file
 * Hooks and callback functions for rules.module integration.
 */

/**
 * Implements hook_rules_action_info().
 *
 * @ingroup rules
 */
function privatemsg_rules_rules_action_info() {

  // Re-use existing information and also get information about fields added
  // to private messages.
  $fields = entity_get_all_property_info('privatemsg_message');

  // Remove properties that can't be used here.
  unset($fields['mid']);
  unset($fields['thread_id']);
  unset($fields['timestamp']);

  // Remove fields, they are not supported yet.
  foreach (array_keys($fields) as $key) {
    if (strpos($key, 'field_') === 0) {
      unset($fields[$key]);
    }
  }

  // Update some descriptions and other definitions.
  $fields['subject']['description'] = t('Subject of the message. To use the tokens provided by Privatemsg, use {token}, for example: {privatemsg_message:recipient:name}.');
  $fields['body']['description'] = t('Body of the message. To use the tokens provided by Privatemsg, use {token}, for example: {privatemsg_message:recipient:name}.');
  $fields['body']['long'] = TRUE;
  // Make subject and body translatable.
  $fields['subject']['translatable'] = TRUE;
  $fields['body']['translatable'] = TRUE;

  // Reply doesn't have subject either.
  $reply_fields = $fields;
  unset($reply_fields['subject']);

  $actions = array(
    'privatemsg_rules_new' => array(
      'label' => t('Send a message'),
      'named parameter' => TRUE,
      'parameter' => array(
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient'),
          'description' => t('Recipient of the message.'),
        ),
      ) + $fields,
      'new variables' => array(
        'thread_id' => array(
          'type' => 'integer',
          'label' => t('ID of new thread'),
        ),
      ),
      'group' => t('Private messages'),
    ),
   'privatemsg_rules_reply' => array(
      'label' => t('Reply to a message'),
      'named parameter' => TRUE,
      'parameter' => array(
        'thread_id' => array(
          'type' => 'integer',
          'label' => t('Privatemsg thread id'),
          'description' => t('Thread ID of the thread that should be responded to.')
        ),
      ) + $reply_fields,
      'group' => t('Private messages'),
    ),
    'privatemsg_rules_unread_count' => array(
      'label' => t('Load number of unread messages'),
      'parameter' => array(
        'account' => array(
          'type' => 'user',
          'label' => t('User account'),
          'description' => t('Specify the user for which the number of unread messages should be loaded.'),
        ),
      ),
      'new variables' => array(
        'unread_count' => array(
          'type' => 'integer',
          'label' => t('Number of unread messages'),
        ),
      ),
      'group' => t('Private messages'),
    ),
  );
  if (module_exists('privatemsg_filter')) {
    $actions['privatemsg_rules_tag_thread'] = array(
      'label' => t('Tag a privatemsg thread'),
      'parameter' => array(
        'thread_id' => array(
          'type' => 'integer',
          'label' => t('Specify the thread ID that should be tagged.'),
        ),
        'account' => array(
          'type' => 'user',
          'label' => t('Specify for which user the message should tagged.'),
        ),
        'privatemsg_tag' => array(
          'type' => 'text',
          'label' => t('Name of the tag that should be added.'),
          'description' => t('Multiple tags can be specified, separated by a comma.'),
        ),
      ),
      'group' => t('Private messages'),
    );
  }
  if (module_exists('privatemsg_roles')) {
    $actions['privatemsg_rules_new_role'] = array(
      'label' => t('Send a message to a role'),
      'named parameter' => TRUE,
      'parameter' => array(
        'roles' => array(
          'type' => 'list<integer>',
          'label' => t('Roles'),
          'options list' => 'entity_metadata_user_roles',
          'description' => t('Select the roles whose users should receive the message.'),
        ),
      ) + $fields,
      'new variables' => array(
        'thread_id' => array(
          'type' => 'integer',
          'label' => t('ID of new thread'),
        ),
      ),
      'group' => t('Private messages'),
    );
  }
  return $actions;
}


/**
 * Implements hook_rules_event_info().
 *
 * @ingroup rules
 */
function privatemsg_rules_rules_event_info() {
  return array(
    'privatemsg_insert' => array(
      'label'  => t('A message is sent'),
      'group' => t('Private messages'),
      'variables'   => array(
        'privatemsg_message' => array(
          'type' => 'privatemsg_message',
          'bundle' => 'privatemsg_message',
          'label' => t('Private message'),
        ),
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient of the message.'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 *
 * @ingroup rules
 */
function privatemsg_rules_rules_condition_info() {
  if (module_exists('privatemsg_filter')) {
    return array(
      'privatemsg_rules_thread_has_tag' => array(
        'label' => t('Thread has tag'),
        'parameter' => array(
          'thread_id' => array(
            'type' => 'integer',
            'label' => t('Specify the thread ID that should be checked.'),
          ),
          'account' => array(
            'type' => 'user',
            'label' => t('Specify the user that should be checked.'),
          ),
          'privatemsg_tag' => array(
            'type' => 'text',
            'label' => t('Name of the tag for which should be checked.'),
          ),
          'check_all' => array(
            'type' => 'boolean',
            'label' => t('Only return TRUE if all tags are found.'),
          ),
        ),
        'group' => t('Private messages'),
      ),
    );
  }
}

/**
 * Converts temporary token format ({token}) to real tokens.
 */
function privatemsg_rules_replace_tokens($text) {
  return preg_replace('/{([a-z0-9_:]+)}/', '[$1]', $text);
}

/**
 * Helper function for sending a new message.
 */
function privatemsg_rules_new($args, $element) {
  // Recipient could be a wrapped entity. They do not play nice
  // together with the Privatemsg API because that one assumes recipient
  // objects which have additional properties like type.
  $recipient = $args['recipient'];
  if ($recipient instanceof EntityDrupalWrapper) {
    $recipient = user_load($recipient->getIdentifier());
  }

  // Enforce recipient type values.
  $recipient->recipient = $recipient->uid;
  $recipient->type = 'user';

  // Replace token placeholders.
  $args['subject'] = privatemsg_rules_replace_tokens($args['subject']);
  $args['body'] = privatemsg_rules_replace_tokens($args['body']);
  rules_log('Writing new message with subject %subject to %user from %author', array('%subject' => $args['subject'], '%user' => $recipient->name, '%author' => $args['author']->name));
  $result = privatemsg_new_thread(array($recipient), $args['subject'], $args['body'], $args);
  if ($result['success']) {
    rules_log('New message sucessfully sent, !link', array('!link' => l($args['subject'], 'messages/view/' . $result['message']->thread_id)));
  }
  else {
    foreach ($result['messages']['error'] as $args) {
      rules_log('Message not sent, reported error: !error', array('!error' => $args), RulesLog::ERROR);
    }
  }
  foreach ($result['messages']['warning'] as $args) {
    rules_log('Warning message reported when trying to send message: !warning', array('!warning' => $args), RulesLog::WARN);
  }

  // Return thread ID of new thread (or NULL if failed).
  if ($result['success']) {
    return array(
      'thread_id' => $result['message']->thread_id,
    );
  }
  else {
    return array(
      'thread_id' => NULL,
    );
  }
}

/**
 * Implements ACTIONBASE_form_alter().
 */
function privatemsg_rules_new_form_alter(&$form, &$form_state) {
  $form['parameter']['subject']['settings']['subject']['#type'] = 'textfield';
}

/**
 * Helper function for sending a new message to a role.
 */
function privatemsg_rules_new_role($args, $element) {
  $args['subject'] = privatemsg_rules_replace_tokens($args['subject']);
  $args['body'] = privatemsg_rules_replace_tokens($args['body']);

  $recipient_array = array();
  $recipient_names = array();
  foreach ($args['roles'] as $rid) {
    $recipient_array[$rid] = user_role_load($rid);
    $recipient_array[$rid]->type = 'role';
    $recipient_array[$rid]->recipient = $recipient_array[$rid]->rid;
    $recipient_names[] = $recipient_array[$rid]->name;
  }
  $recipient_names = implode(', ', $recipient_names);

  rules_log('Writing new message with subject %subject to roles %roles from %author.', array('%subject' => $args['subject'], '%author' => $args['author']->name, '%roles' => $recipient_names));
  $result = privatemsg_new_thread($recipient_array, $args['subject'], $args['body'], $args);
  if ($result['success']) {
    rules_log('New message sucessfully sent, !link.', array('!link' => l($args['subject'], 'messages/view/' . $result['message']->thread_id)));
  }
  else {
    foreach ($result['messages']['error'] as $message) {
      rules_log('Message not sent, reported error: !error.', array('!error' => $message), RulesLog::ERROR);
    }
  }
  foreach ($result['messages']['warning'] as $message) {
    rules_log('Warning message reported when trying to send message: !warning.', array('!warning' => $message), RulesLog::WARN);
  }

  // Return thread ID of new thread (or NULL if failed).
  if ($result['success']) {
    return array(
      'thread_id' => $result['message']->thread_id,
    );
  }
  else {
    return array(
      'thread_id' => NULL,
    );
  }
}

/**
 * Implements ACTIONBASE_form_alter().
 */
function privatemsg_rules_new_role_form_alter(&$form, &$form_state) {
  $form['parameter']['subject']['settings']['subject']['#type'] = 'textfield';
}

/**
 * Helper function for replying to a thread.
 */
function privatemsg_rules_reply($args, $element) {
  $args['body'] = privatemsg_rules_replace_tokens($args['body']);

  rules_log('Writing reply to thread %thread_id from %author', array('%thread_id' => $args['thread_id'], '%author' => $args['author']->name));
  $result = privatemsg_reply($args['thread_id'], $args['body'], $args);
  if ($result['success']) {
    rules_log('New message sucessfully sent, !link', array('!link' => l(drupal_substr($args['body'], 0, 15), 'messages/view/' . $result['message']->thread_id)));
  }
  else {
    foreach ($result['messages']['error'] as $message) {
      rules_log('Message not sent, reported error: !error', array('!error' => $message), RulesLog::ERROR);
    }
  }
  foreach ($result['messages']['warning'] as $message) {
    rules_log('Warning message reported when trying to send message: !warning', array('!warning' => $message), RulesLog::WARN);
  }
}

/**
 * Implements ACTIONBASE_form_alter().
 */
function privatemsg_rules_thread_has_tag_form_alter(&$form, &$form_state) {
  $form['parameter']['privatemsg_tag']['settings']['privatemsg_tag']['#type'] = 'textfield';
  $form['parameter']['privatemsg_tag']['settings']['privatemsg_tag']['#autocomplete_path'] = 'messages/filter/tag-autocomplete';
}

/**
 * Check if a thread has a specific tag.
 *
 * @param $thread_id
 *   Which thread to check.
 * @param $account
 *   Which user should be checked.
 * @param $tag_settings
 *   Rules settings, includes the tag that should be checked.
 *
 * @return
 *   TRUE if the thread has such a tag, FALSe otherwise.
 */
function privatemsg_rules_thread_has_tag($thread_id, $account, $tag_string, $check_all = FALSE) {
  $tags = explode(',', $tag_string);
  $tag_ids = privatemsg_filter_create_tags($tags);

  if (empty($tag_ids)) {
    rules_log('No valid tag could be loaded or created.', array(), RulesLog::ERROR);
    return;
  }

  $query = _privatemsg_assemble_query(array('tags', 'privatemsg_filter'), $account, array($thread_id));
  $tag_ids_on_thread = $query->execute()->fetchCol();

  if ($check_all) {
    // If check_all is set, only return TRUE if all tags exist.
    if (count(array_diff($tag_ids, $tag_ids_on_thread)) == 0) {
      return TRUE;
    }
  }
  else {
    // If not, then it is enough it at least one of the submitted tags is used
    // on the thread.
    if (count(array_diff($tag_ids, $tag_ids_on_thread)) < count($tag_ids)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Implements ACTIONBASE_form_alter().
 */
function privatemsg_rules_tag_thread_form_alter(&$form, &$form_state) {
  $form['parameter']['privatemsg_tag']['settings']['privatemsg_tag']['#type'] = 'textfield';
  $form['parameter']['privatemsg_tag']['settings']['privatemsg_tag']['#autocomplete_path'] = 'messages/filter/tag-autocomplete';
}

function privatemsg_rules_tag_thread($thread_id, $account, $tag_string) {
  $tags = explode(',', $tag_string);
  $tag_ids = privatemsg_filter_create_tags($tags);

  if (empty($tag_ids)) {
    rules_log('No valid tag could be loaded or created.', array(), RulesLog::ERROR);
    return;
  }

  privatemsg_filter_add_tags(array($thread_id), $tag_ids, $account);
}

function privatemsg_rules_unread_count($account) {
  return array(
    'unread_count' => privatemsg_unread_count($account),
  );
}

