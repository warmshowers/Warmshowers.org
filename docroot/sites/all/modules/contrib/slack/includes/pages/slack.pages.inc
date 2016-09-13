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
  $channel = $form_state['values']['slack_test_channel'];
  $message = $form_state['values']['slack_test_message'];
  $result = slack_send_message($message, $channel);
  if (!$result) {
    drupal_set_message(t("Message wasn't sent. Please, check slack module configuration."));
  }
  elseif (!isset($result->error) && $result->code == SLACK_CODE_OK) {
    drupal_set_message(t('Message was successfully sent.'));
  }
  else {
    drupal_set_message(t("Message wasn't sent."), 'error');
  }
}
