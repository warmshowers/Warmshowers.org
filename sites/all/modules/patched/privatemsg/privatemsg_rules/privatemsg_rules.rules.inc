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
  return array(
    'privatemsg_rules_new' => array(
      'label' => t('Send a message'),
      'arguments' => array(
        'subject' => array('type' => 'string', 'label' => t('Subject')),
        'body' => array('type' => 'string', 'label' => t('Body text'), 'long' => TRUE),
        'recipient' => array('type' => 'user', 'label' => t('Recipient')),
        'author' => array('type' => 'user', 'label' => t('Author')),
      ),
      'module' => 'Privatemsg',
    ),
   'privatemsg_rules_reply' => array(
      'label' => t('Reply a message'),
      'arguments' => array(
        'thread_id' => array('type' => 'int', 'label' => t('Privatemsg thread id')),
        'body' => array('type' => 'string', 'label' => t('Body text'), 'long' => TRUE),
        'author' => array('type' => 'user', 'label' => t('Author')),
      ),
      'module' => 'Privatemsg',
    ),
   'privatemsg_rules_tag_thread' => array(
      'label' => t('Tag a privatemsg thread'),
      'arguments' => array(
        'thread_id' => array('type' => 'int', 'label' => t('Privatemsg thread id')),
        'tag_to_add'    => array('type' => 'tag', 'label' => t('Tag'), 'long' => TRUE),
        'tag_for'      => array('type' => 'user', 'label'   => t('User for which the message will be tagged')),
      ),
      'module' => 'Privatemsg',
    ),
    'privatemsg_rules_load_tag' => array(
      'label' => t('Load a tag'),
      'new variables' => array(
        'tag_loaded' => array('type' => 'tag', 'label' => t('Loaded tag')),
      ),
      'help' => t('Enter an id or name of the tag to load.'),
      'module' => 'Privatemsg',
      'eval input' => array('tagid', 'tag'),
    ),
  );
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
      'module' => 'Privatemsg',
      'arguments'   => array(
        'subject'   => array('type' => 'string', 'label' => t('Subject of the message')),
        'body'      => array('type' => 'string', 'label' => t('Body of the message')),
        'author'    => array('type' => 'user', 'label'   => t('Author of the message')),
        'recipient' => array('type' => 'user', 'label'   => t('Recipient of the message.')),
        'mid'       => array('type' => 'int', 'label'    => t('Privatemsg message id')),
        'thread_id' => array('type' => 'int', 'label'    => t('Privatemsg thread id')),
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
  return array(
    'privatemsg_rules_thread_has_tag' => array(
      'label' => t('Thread has tag'),
      'arguments' => array(
        'thread_id' => array('type' => 'int', 'label' => t('Thread to check for the tag')),
        'user' => array('type' => 'user', 'label' => t('For which user to check')),
      ),
      'module' => 'Privatemsg',
      'eval input' => array('tag'),
    ),
  );
}

/**
 * Implements hook_rules_data_type_info().
 *
 * @ingroup rules
 */
function privatemsg_rules_rules_data_type_info() {
  return array(
    'tag' => array(
      'label' => t('user'),
      'class' => 'privatemsg_rules_data_type_tag',
      'savable' => FALSE,
      'identifiable' => TRUE,
    ),
  );
}

/**
 * Loads a tag.
 */
function privatemsg_rules_load_tag($settings) {
  if (!empty($settings['tag'])) {
    $tag = db_fetch_object(db_query("SELECT * from {pm_tags} pmt WHERE pmt.tag = '%s'", $settings['tag']));
    if ($tag) {
      return array('tag_loaded' => $tag);
    }
    // If tag does not exist, create it.
    $tags = privatemsg_filter_create_tags(array($settings['tag']));
    if ($tagid = reset($tags)) {
      $settings['tagid'] = $tagid;
    }
  }
  elseif (!empty($settings['tagid'])) {
    return array('tag_loaded' => db_fetch_object(db_query('SELECT * from {pm_tags} pmt WHERE pmt.tag_id = %d', $settings['tagid'])));
  }
}

/**
 * Defines the user data type.
 */
class privatemsg_rules_data_type_tag extends rules_data_type {
  function load($tag_id) {
    return db_fetch_object(db_query('SELECT * from {pm_tags} pmt WHERE pmt.tag_id = %d', $tag_id));
  }

  function get_identifier() {
    $tag = $this->get();
    return $tag->tag_id;
  }
}



/**
 * Implements hook_rules_defaults().
 *
 * @ingroup rules
 */
function privatemsg_rules_rules_defaults() {
  $config = array(
    'rules' =>
    array(
      'privatemsg_welcome_message' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_user_insert',
        '#label' => 'Send welcome message with privatemsg when a new user is created',
        '#active' => 0,
        '#weight' => '0',
        '#status' => 'custom',
        '#conditions' =>
        array(
        ),
        '#actions' =>
        array(
          1 =>
          array(
            '#weight' => -10,
            '#info' =>
            array(
              'label' => 'Load a user account',
              'new variables' =>
              array(
                'admin_user' =>
                array(
                  'label' => 'Admin User (uid 1)',
                  'label callback' => FALSE,
                  'type' => 'user',
                ),
              ),
              'module' => 'User',
            ),
            '#name' => 'rules_action_load_user',
            '#settings' =>
            array(
              'username' => '',
              'userid' => '1',
              '#argument map' =>
              array(
                'admin_user' => 'user_loaded',
              ),
            ),
            '#type' => 'action',
          ),
          0 =>
          array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' =>
            array(
              'subject' => 'Welcome!',
              'body' => 'Hey <?php $account->name; ?>, welcome on our site!',
              '#argument map' =>
              array(
                'account' => 'recipient',
                'admin_user' => 'author',
              ),
              '#eval input' =>
              array(
                'rules_input_evaluator_php' =>
                array(
                  'body' =>
                  array(
                    0 => 'account',
                  ),
                ),
              ),
            ),
            '#name' => 'privatemsg_rules_new',
            '#info' =>
            array(
              'label' => 'Send a message',
              'arguments' =>
              array(
                'subject' =>
                array(
                  'type' => 'string',
                  'label' => 'Subject',
                ),
                'body' =>
                array(
                  'type' => 'string',
                  'label' => 'Body text',
                ),
                'recipient' =>
                array(
                  'type' => 'user',
                  'label' => 'Recipient',
                ),
              ),
              'module' => 'Privatemsg',
            ),
          ),
        ),
      ),
      'privatemsg_rules_autorespond' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_privatemsg_insert',
        '#label' => 'Auto-Respond a message to UID 1',
        '#active' => 0,
        '#weight' => '0',
        '#status' => 'custom',
        '#conditions' =>
        array(
          0 =>
          array(
            '#type' => 'condition',
            '#settings' =>
            array(
              'number1' => '$mid',
              'number2' => '$thread_id',
              'operation' => 'equal',
            ),
            '#name' => 'rules_condition_number_compare',
            '#info' =>
            array(
              'label' => 'First message of thread',
              'label callback' => FALSE,
              'arguments' =>
              array(
                'number1' =>
                array(
                  'label' => 'Number 1',
                  'type' => 'number',
                ),
                'number2' =>
                array(
                  'label' => 'Number 2',
                  'type' => 'number',
                ),
              ),
              'module' => 'Rules',
            ),
            '#weight' => 0,
          ),
          1 =>
          array(
            '#info' =>
            array(
              'label' => 'Recipient is uid 1',
              'label callback' => FALSE,
              'module' => 'PHP',
              'eval input' =>
              array(
                0 => 'code',
              ),
            ),
            '#name' => 'rules_condition_custom_php',
            '#settings' =>
            array(
              'code' => 'return $recipient->uid == 1;',
              'vars' =>
              array(
                0 => 'recipient',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
          ),
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#type' => 'action',
            '#settings' =>
            array(
              'body' => 'Hey <?php echo $author->name; ?>!

  Thanks for your message, I will reply soon!

  Greetings, <?php echo $recipient->name; ?>

  (this is an automatic message)',
              '#argument map' =>
              array(
                'recipient' => 'author',
              ),
              '#eval input' =>
              array(
                'rules_input_evaluator_php' =>
                array(
                  'body' =>
                  array(
                    0 => 'author',
                    1 => 'recipient',
                  ),
                ),
              ),
            ),
            '#name' => 'privatemsg_rules_reply',
            '#info' =>
            array(
              'label' => 'Reply a message',
              'arguments' =>
              array(
                'thread_id' =>
                array(
                  'type' => 'int',
                  'label' => 'Privatemsg thread id',
                ),
                'body' =>
                array(
                  'type' => 'string',
                  'label' => 'Body text',
                  'long' => TRUE,
                ),
                'author' =>
                array(
                  'type' => 'user',
                  'label' => 'Author',
                ),
              ),
              'module' => 'Privatemsg',
            ),
            '#weight' => 0,
          ),
        ),
      ),
    ),
  );
  return $config;
}
/**
 * Helper function for sending a new message.
 */
function privatemsg_rules_new($subject, $body, $recipient, $author) {
  rules_log(t('Writing new message with subject %subject to %user from %author', array('%subject' => $subject, '%user' => $recipient->name, '%author' => $author->name)));
  // Enforce type user on recipient, see http://drupal.org/node/1079644.
  $recipient->type = 'user';
  $recipient->recipient = $recipient->uid;
  $result = privatemsg_new_thread(array($recipient), $subject, $body, array('author' => $author));
  if ($result['success']) {
    rules_log(t('New message sucessfully sent, !link', array('!link' => l($subject, 'messages/view/' . $result['message']['thread_id']))));
  }
  else {
    foreach ($result['messages']['error'] as $message) {
      rules_log(t('Message not sent, reported error: !error', array('!error' => $message)));
    }
  }
  foreach ($result['messages']['warning'] as $message) {
    rules_log(t('Warning message reported when trying to send message: !warning', array('!warning' => $message)), TRUE);
  }
}

/**
 * Helper function for replying to a thread.
 */
function privatemsg_rules_reply($thread_id, $body, $author) {
  rules_log(t('Writing reply to thread %thread_id from %author', array('%thread_id' => $thread_id, '%user' => $recipient->name, '%author' => $author->name)));
  $result = privatemsg_reply($thread_id, $body, array('author' => $author));
  if ($result['success']) {
    rules_log(t('New message sucessfully sent, !link', array('!link' => l($subject, 'messages/view/' . $result['message']['thread_id']))));
  }
  else {
    foreach ($result['messages']['error'] as $message) {
      rules_log(t('Message not sent, reported error: !error', array('!error' => $message)), TRUE);
    }
  }
  foreach ($result['messages']['warning'] as $message) {
    rules_log(t('Warning message reported when trying to send message: !warning', array('!warning' => $message)), TRUE);
  }
}

/**
 * Helper function to tag a thread.
 */
function privatemsg_rules_tag_thread($thread_id, $tag, $account) {
  rules_log(t('Tagging %thread_id with %tag for %name', array('%thread_id' => $thread_id, '%tag' => $tag->tag, '%name' => $account->name)));
  privatemsg_filter_add_tags(array($thread_id), $tag->tag_id, $account);
}

/**
 * Define form to load a tag.
 */
function privatemsg_rules_load_tag_form($settings, &$form) {
  $form['settings']['tag'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag name'),
    '#maxlength' => 60,
    '#default_value' => isset($settings['tag']) ? $settings['tag'] : '',
    '#weight' => -1,
    '#description' => t('Name of the tag to be loaded. The tag will be created if he does not exist.'),
  );
  $form['settings']['tagid'] = array(
    '#type' => 'textfield',
    '#title' => t('Tag id'),
    '#default_value' => isset($settings['tagid']) ? $settings['tagid'] : '',
    '#description' => t('Id of the tag to be loaded.'),
  );
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
function privatemsg_rules_thread_has_tag($thread_id, $account, $tag_settings) {
  $tag = privatemsg_rules_load_tag($tag_settings);
  $tag = $tag['tag_loaded'];

  if (empty($tag)) {
    rules_log(t('No tag to compare found'), TRUE);
    return FALSE;
  }

  $query = _privatemsg_assemble_query(array('tags', 'privatemsg_filter'), $account, array($thread_id));
  $result = db_query($query['query']);
  while ($row = db_fetch_object($result)) {
    if ($row->tag_id == $tag->tag_id) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Form definition for thread has tag settings form
 */
function privatemsg_rules_thread_has_tag_form($settings, &$form) {
  // Reuse load tag form.
  return privatemsg_rules_load_tag_form($settings, $form);
}
