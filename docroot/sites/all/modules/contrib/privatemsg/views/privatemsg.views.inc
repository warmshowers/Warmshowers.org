<?php
/**
 * @file
 * Provide views data and handlers for privatemsg.module
 */

/**
 * Implements hook_views_data().
 */
function privatemsg_views_data() {
  $data['users']['privatemsg_link'] = array(
    'group' => t('Privatemsg'),
    'real field' => 'uid',
    'title' => t('Send message'),
    'field' => array(
      'title' => t('Send Privatemsg link'),
      'help' => t('Displays a link to send a private message to a user.'),
      'handler' => 'views_handler_field_privatemsg_link',
    ),
  );
  return $data;
}

