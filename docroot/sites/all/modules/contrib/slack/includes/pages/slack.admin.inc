<?php

/**
 * @file
 * Slack integration module admin functions.
 */

/**
 * Slack module admin form.
 */
function slack_configure_form($form, &$form_state) {
  $form['slack_webhook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Webhook URL'),
    '#description' => t('Enter your Webhook URL from an Incoming WebHooks integration. It looks like https://hooks.slack.com/services/XXXXXXXXX/YYYYYYYYY/ZZZZZZZZZZZZZZZZZZZZZZZZ'),
    '#default_value' => slack_get_default_webhook_url(),
    '#required' => TRUE,
  );
  $form['slack_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Default channel'),
    '#description' => t('Enter your channel name with # symbol, for example #general (or @username for a private message or a private group name).'),
    '#default_value' => variable_get('slack_channel'),
  );
  $form['slack_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Default username'),
    '#description' => t('What would you like to name your Slack bot?'),
    '#default_value' => variable_get('slack_username'),
  );
  return system_settings_form($form);
}
