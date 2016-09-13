<?php

/**
 * @file
 * Slack integration module rules functions.
 */

/**
 * Implements hook_rules_action_info().
 */
function slack_rules_action_info() {
  $actions = array();
  $actions['slack_send_message'] = array(
    'base' => 'slack_rules_send_message_action',
    'label' => t('Slack send message'),
    'group' => t('Slack'),
    'parameter' => array(
      'message' => array(
        'type' => 'text',
        'label' => t('Sending message'),
      ),
      'channel' => array(
        'type' => 'text',
        'label' => t('Channel'),
        'description' => t("It will be using a channel from slack module settings, if you don't enter channel here."),
        'optional' => TRUE,
      ),
      'username' => array(
        'type' => 'text',
        'label' => t('Username'),
        'description' => t("It will be using a username from slack module settings, if you don't enter username here."),
        'optional' => TRUE,
      ),
    ),
  );
  return $actions;
}

/**
 * Rules action for sending a message to the Slack.
 */
function slack_rules_send_message_action($message, $channel, $username) {
  if (!$channel) {
    $channel = slack_get_default_channel();
  }
  if (!$username) {
    $username = slack_get_default_username();
  }
  slack_send_message($message, $channel, $username);
}
