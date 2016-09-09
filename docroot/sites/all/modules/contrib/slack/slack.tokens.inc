<?php

/**
 * Implements hook_info().
 */
function slack_token_info() {
  $tokens['slack-channel'] = array(
    'name' => t('Channel'),
    'description' => t('Name of channel.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-default-webhook-url'] = array(
    'name' => t('Webhook URL'),
    'description' => t('Webhook URL which defined on settings page.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-icon-url'] = array(
    'name' => t('Icon URL'),
    'description' => t('Absolute Url to image what you would like to use for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-message'] = array(
    'name' => t('Message'),
    'description' => t('User message'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-icon-emoji'] = array(
    'name' => t('Emoji code'),
    'description' => t('Emoji code what you would like to use for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );
  $tokens['slack-username'] = array(
    'name' => t('Username'),
    'description' => t('Default username for your Slackbot.'),
    'type' => 'text',
    'dynamic' => TRUE,
    'global_types' => TRUE,
  );

  return array('tokens' => array('slack' => $tokens));
}

/**
 * Implements hook_tokens().
 */
function slack_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  foreach ($tokens as $name => $original) {
    if ($name == 'slack-icon-url') {
      $replacements[$original] = 'image';
    }
    if ($name == 'slack-icon-emoji') {
      $replacements[$original] = 'emoji';
    }
    if ($name == 'slack-default-webhook-url') {
      $replacements[$original] = slack_get_default_webhook_url();
    }
    if ($name == 'slack-username') {
      $replacements[$original] = slack_get_default_username();
    }
  }
  return $replacements;
}
