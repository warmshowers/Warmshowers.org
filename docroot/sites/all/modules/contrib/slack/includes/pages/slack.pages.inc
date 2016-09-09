<?php

/**
 * @file
 * Slack module page functions.
 */

/**
 * Slack test message form.
 */
function slack_send_test_message_form($form, &$form_state) {
  $form['slack_test_channel'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel'),
    '#default_value' => variable_get('slack_channel'),
  );

  $form['slack_attachments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachments options'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('slack_attachment_enabled'),
  );
  $form['slack_attachments']['slack_attachment_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable attachments'),
    '#description' => t('Attachments allow further styling of your messages.'),
    '#default_value' => variable_get('slack_attachment_enabled'),
  );
  $form['slack_attachments']['slack_test_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => variable_get('slack_attachment_color'),
  );
  $form['slack_attachments']['slack_test_pretext'] = array(
    '#type' => 'textfield',
    '#title' => t('Message pretext'),
    '#default_value' => variable_get('slack_attachment_pretext'),
  );
  $form['slack_attachments']['slack_test_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Message title'),
    '#default_value' => variable_get('slack_attachment_title'),
  );
  $form['slack_attachments']['slack_test_title_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Message title link'),
    '#default_value' => variable_get('slack_attachment_title_link'),
  );
  $form['slack_test_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send message'),
  );
  return $form;
}

/**
 * Submit handler for slack test message form.
 */
function slack_send_test_message_form_submit($form, &$form_state) {
  $webhook_url = slack_get_default_webhook_url();
  $channel = $form_state['values']['slack_test_channel'];
  $message = $form_state['values']['slack_test_message'];

  $attachment_options = array();
  $use_attachments = $form_state['values']['slack_attachment_enabled'];
  if($use_attachments) {
    $attachment_options['pretext'] = $form_state['values']['slack_test_pretext'];
    $attachment_options['title'] = $form_state['values']['slack_test_title'];
    $attachment_options['title_link'] = $form_state['values']['slack_test_title_link'];
    $attachment_options['color'] = $form_state['values']['slack_test_color'];
  }
  $icon_type = variable_get('slack_icon_type');
  $icon = variable_get('slack_icon_url');
  $emoji = variable_get('slack_icon_emoji');

  $icon_options = array('icon'=>$icon, 'emoji'=>$emoji, 'type'=>$icon_type);

  $result = slack_send_message($webhook_url, $message, $channel, '', $icon_options, $attachment_options);
  if (!$result) {
    drupal_set_message(t("Message wasn't sent. Please, check slack module configuration."));
  }
  elseif (!isset($result->error) && $result->code == SLACK_CODE_OK) {
    drupal_set_message(t('Message was successfully sent.'));
  }
  else {
    drupal_set_message(t("Message wasn't sent."), 'error');
    slack_watchdog_sending_error($result);
  }
}

/**
 * Create a log in watchdog containing Slack's response information when message
 * sending was failed.
 */
function slack_watchdog_sending_error($result) {
  $message = '';
  $status_message = $result->status_message;
  if (isset($status_message)) {
    $message .= "<strong>Error message:</strong> $status_message";
  }

  $code = $result->code;
  if (isset($code)) {
    $message .= "<br /><strong>Code:</strong> $code";
  }

  $request = $result->request;
  if (isset($request)) {
    $message .= "<br /><strong>Request:</strong> $request";
  }

  $headers = $result->headers;
  if (isset($headers)) {
    $message .= "<br /><br /><strong>Headers:</strong><br><ul>";
    foreach ($headers as $key => $header) {
      $message .= "<li>$key: $header</li>";
    }
    $message .= '</ul>';
  }

  watchdog('slack', $message, array(), WATCHDOG_ERROR);
}