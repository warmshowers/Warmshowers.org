<?php

/**
 * @file
 * Provide drupal_mail_wrapper() function for the smtp_library variable.
 */

/**
 * Sends a prepared email message.
 *
 * @param array $message
 *   The email message to be sent.
 *
 * @return
 *   TRUE if the message was successfully sent; otherwise FALSE.
 */
function drupal_mail_wrapper(array $message) {
  $mailsystem = drupal_mail_system($message['module'], $message['key']);
  return $mailsystem->mail($message);
}
