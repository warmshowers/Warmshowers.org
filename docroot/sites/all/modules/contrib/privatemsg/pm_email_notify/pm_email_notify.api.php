<?php

/**
 * @file
 * Private Messages via Email API Documentation
 */

/**
 * Alter the private message e-mail token mapping.
 *
 * @param $tokens
 *   Array of mappings from token names to values (for use with strtr()).
 * @param $recipient
 *   The recipient of the private message being sent.
 * @param $message
 *   The private message array being sent.  Must contain at
 *   least the fields 'author', 'subject', 'thread_id' and 'body'.
 * @param $language
 *   The language of the private message being sent.
 *
 * @see _pm_email_notify_token()
 */
function hook_pm_email_notify_token_alter(&$tokens, $recipient, $message, $language) {

}

/**
 * Alter the available list of private message e-mail tokens.
 *
 * @param $tokens
 *   An array of mappings from token names to descriptions.
 *   The token names should match those that are having their values
 *   specificed in hook_pm_email_notify_token_alter().
 *
 * @see _pm_email_notify_token_list()
 */
function hook_pm_email_notify_token_list_alter(&$tokens) {

}
