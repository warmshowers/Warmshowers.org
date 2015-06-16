<?php

/**
 * @file
 * Admin menu callbacks for privatemsg_limits module.
 */

/**
 * Menu callback for the admin configuration.
 */
function privatemsg_limits_admin($form, &$form_state) {
  $default = array(
    '#type'  => 'textfield',
    '#size' => '10',
  );

  $form['conversation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Conversation settings'),
  );
  $form['conversation']['privatemsg_limits_messages_per_thread'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of messages in a conversation'),
    '#description' => t('The maximum number of messages that can be included in a conversation. If this maximum has already been reached and a user attempts to add an additional message, then the action specified below shall be taken. Leave this set to 0 if you want unlimited messages per conversation.'),
    '#default_value' => variable_get('privatemsg_limits_messages_per_thread', 0),
    '#size' => '3',
  );
  $form['conversation']['privatemsg_limits_messages_per_thread_action'] = array(
    '#type' => 'radios',
    '#title' => t('Maximum messages action'),
    '#description' => t('This action shall be taken once a thread has already reached its maximum number of messages.'),
    '#default_value' => variable_get('privatemsg_limits_messages_per_thread_action', 'create-new'),
    '#options' => array(
      'create-new' => t('Create new conversation'),
      'block-message' => t('Block message from being sent')
    ),
  );

  $form['receive'] = array(
    '#type' => 'fieldset',
    '#title' => t('Receive settings'),
  );
  $form['receive']['privatemsg_limits_receive_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of messages/conversations that a user may have.'),
    '#default_value' => variable_get('privatemsg_limits_receive_enabled', FALSE),
  );
  $form['receive']['privatemsg_limits_receive_object'] = array(
    '#type'  => 'select',
    '#title' => t('What should be limited'),
    '#description' => t('Do you want to limit the number of individual messages a user can send, or limit the number of message conversations a user can initiate? An exceeded conversation limit will allow users to still write individual messages in existing conversations, but not start new ones.'),
    '#default_value' => variable_get('privatemsg_limits_receive_object', 'message'),
    '#options' => array('message' => t('Messages'), 'thread' => t('Conversations')),
  );
  $form['receive']['privatemsg_limits_receive_amount'] = array(
    '#type'  => 'textfield',
    '#title' => t('Maximum number of messages/conversations per user'),
    '#description' => t('The total number of messages/conversations that a user may have before he has to delete old messages/conversations. When this limit is reached, users are not allowed to receive or send new messages.'),
    '#default_value' => variable_get('privatemsg_limits_receive_amount', 0),
    '#size' => '10',
  );
  $form['receive'] += _privatemsg_limits_role_override_form('receive_amount', $default);

  $form['send'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send settings'),
  );
  $form['send']['privatemsg_limits_send_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of messages/conversations that a user can send in a given time period'),
    '#default_value' => variable_get('privatemsg_limits_send_enabled', FALSE),
  );
  $form['send']['privatemsg_limits_send_object'] = array(
    '#type'  => 'select',
    '#title' => t('What should be limited'),
    '#description' => t('Do you want to limit the number of individual messages a user can send, or limit the number of message conversations a user can initiate? An exceeded conversation limit will allow users to still write individual messages in existing conversations, but not start new ones.'),
    '#default_value' => variable_get('privatemsg_limits_send_object', 'message'),
    '#options' => array('message' => t('Messages'), 'thread' => t('Conversations')),
  );
  $form['send']['privatemsg_limits_send_amount'] = array(
    '#type'  => 'textfield',
    '#title' => t('Maximum number of messages/conversations per time period'),
    '#description' => t('The total number of messages/conversations that a user may send in a given time period.'),
    '#default_value' => variable_get('privatemsg_limits_send_amount', 0),
    '#size' => '10',
  );
  $form['send']['privatemsg_limits_send_timeframe'] = array(
    '#type'  => 'select',
    '#title' => t('Set the time period to be used'),
    '#description' => t("Specify the time period to be used when limiting a user's sent messages/threads."),
    '#default_value' => variable_get('privatemsg_limits_send_timeframe', 3600),
    '#options' => drupal_map_assoc(array(60, 300, 1800, 3600, 86400, 604800, 604800 * 4), 'format_interval'),
  );
  $form['send'] += _privatemsg_limits_role_override_form('send_amount', $default);

  $form['recipients'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recipient settings'),
  );
  $form['recipients']['privatemsg_limits_recipients_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Limit the total number of recipients that a user can include in a single message'),
    '#default_value' => variable_get('privatemsg_limits_recipients_enabled', FALSE),
  );
  $form['recipients']['privatemsg_limits_recipients_amount'] = array(
    '#type'  => 'textfield',
    '#title' => t('Maximum number of recipients per message'),
    '#description' => t('Defines the total number of recipients a user may include in a single message. This setting only affects new messages (and not replies on existing message threads). A role (just like any other recipient type) is considered to be a single recipient regardless of the number of users with that role.'),
    '#default_value' => variable_get('privatemsg_limits_recipients_amount', 0),
    '#size' => '10',
  );
  $form['recipients'] += _privatemsg_limits_role_override_form('recipients_amount', $default);

  return system_settings_form($form);
}

/**
 * Provides a generic rules form to override the default values.
 *
 * @param string $name    Unique name for the value
 * @param array  $default default form configuration
 *
 * @return array Form fieldset
 */
function _privatemsg_limits_role_override_form($name, $default) {
  $form['roles'] = array(
    '#type'   => 'fieldset',
    '#description' => t('You may override the maximum limit specified above on a per-role basis. If a user has multiple roles, then the highest maximum value shall be used. Enter "unlimited" if a role may have unlimited messages/conversations. Enter "0" if you do not want to override the maximum limit setting.'),
    '#title'  => t('Override maximum limit by role'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach (user_roles(TRUE) as $id => $role) {
    $form['roles']["privatemsg_limits_{$name}_role_{$id}"] = $default + array(
      '#title' => $role,
      '#default_value' => variable_get("privatemsg_limits_{$name}_role_" . $id, 0),
    );
  }
  return $form;
}
