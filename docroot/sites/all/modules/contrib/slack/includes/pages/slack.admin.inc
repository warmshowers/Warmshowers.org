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
  $form['slack_icon_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of image'),
    '#options' => array (
      'emoji' => t('Emoji'),
      'image' => t('Image'),
      'none' => t('None (Use default integration settings)')),
    '#default_value' => slack_get_default_icon_type(),
  );
  $form['slack_icon_emoji'] = array(
    '#type' => 'textfield',
    '#title' => t('Emoji code'),
    '#default_value' => variable_get('slack_icon_emoji'),
    '#description' => t('What emoji would you use for your SlackBot?'),
    '#states' => array(
      'visible' => array(
        ':input[name="slack_icon_type"]' => array(
          'value' => 'emoji',
        ),
      ),
    ),
  );
  $form['slack_icon_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Image URL '),
    '#default_value' => variable_get('slack_icon_url'),
    '#description' => t('What icon would you use for your SlackBot?'),
    '#states' => array(
      'visible' => array(
        ':input[name="slack_icon_type"]' => array(
          'value' => 'image',
        ),
      ),
    ),
  );
  $form['slack_attachment_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send message with attachment styling'),
    '#description' => t('Attachments allow further styling of your messages.'),
    '#default_value' => variable_get('slack_attachment_enabled'),
  );

  $attachment_state_toggle = array(
    'visible' => array(
      ':input[name="slack_attachment_enabled"]' => array(
        'checked' => TRUE,
      )
    )
  );
  $form['slack_attachment_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message color'),
    '#description' => t('What color do you want to use for your slack messages by default?'),
    '#default_value' => variable_get('slack_attachment_color'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_pretext'] = array(
    '#type' => 'textfield',
    '#title' => t('Default pretext'),
    '#description' => t('Set a pretext of your message to provide meta information..'),
    '#default_value' => variable_get('slack_attachment_pretext'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message title'),
    '#description' => t('What title do you want to use for your slack messages by default?'),
    '#default_value' => variable_get('slack_attachment_title'),
    '#states' => $attachment_state_toggle,
  );

  $form['slack_attachment_title_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Default link for title'),
    '#description' => t('Add a link to your message title.'),
    '#default_value' => variable_get('slack_attachment_title_link'),
    '#states' => $attachment_state_toggle,
  );
  return system_settings_form($form);
}
