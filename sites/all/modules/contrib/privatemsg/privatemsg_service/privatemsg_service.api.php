<?php

/**
 * @file
 * Privatemsg Service API Documentation
 */

/**
 * Add additional information to messages loaded by privatemsg.getThread.
 *
 * @param $message
 *   Privatemsg message.
 * @return
 *   Associative array. The key will be used as the array key in the message
 *   array, the value will be used as the value for that message enhancement.
 */
function hook_privatemsg_service_enhance_message($message) {

  return array('process_time' => time());

}
