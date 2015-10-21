<?php

/**
 * @file
 * Hooks provided by Mandrill Incoming for interacting with incoming events.
 */

/**
 * @addtogroup hooks
 */

/**
 * Example implementation of hook_mandrill_incoming_event().
 *
 * @param object $event
 *   The event object format is described at http://help.mandrill.com/entries/22092308-What-is-the-format-of-inbound-email-webhooks-.
 *
 * @return int or arrray
 *   MANDRILL_INCOMING_HANDLED
 *   MANDRILL_INCOMING_UNHANDLED
 *   array(MANDRILL_INCOMING_ERROR => string message)
 */
function hook_mandrill_incoming_event($event) {
  $msg = $event->msg;
  watchdog('mandrill_incoming', 'Received "inbound" from=%from to=%to subject=%subject',
    array(
      '%from' => print_r($msg->from_email, TRUE),
      '%to' => print_r($msg->to, TRUE),
      '%subject' => $msg->subject,
    )
  );
  return MANDRILL_INCOMING_UNHANDLED;
  // return MANDRILL_INCOMING_HANDLED;
  // return array(MANDRILL_INCOMING_ERROR => t('Some error message');
}
