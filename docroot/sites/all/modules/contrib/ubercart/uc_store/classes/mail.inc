<?php

/**
 * @file
 * Utility class definition.
 */

/**
 * Modifies the Drupal mail system to send HTML emails.
 */
class UbercartMailSystem extends DefaultMailSystem {

  /**
   * Concatenates and wraps the e-mail body for plain-text mails.
   *
   * @param $message
   *   A message array, as described in hook_mail_alter().
   *
   * @return
   *   The formatted $message.
   */
  public function format(array $message) {
    $message['body'] = implode("\n\n", $message['body']);
    return $message;
  }

}
