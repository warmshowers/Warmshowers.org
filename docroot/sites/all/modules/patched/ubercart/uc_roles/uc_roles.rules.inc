<?php

/**
 * @file
 * Rules hooks for uc_roles.module.
 */

/**
 * Implements hook_rules_data_info().
 *
 * An entity is defined in order to get role expiration tokens in the email.
 */
function uc_roles_rules_data_info() {
  // CA entity for a role expiration object.
  $entities['uc_roles_expiration'] = array(
    'label' => t('Ubercart role expiration'),
    'wrap' => TRUE,
    'token type' => 'uc_role',
    'property info' => array(
      'reid' => array(
        'type' => 'integer',
        'label' => t('Role expiration ID'),
        'description' => t('Primary key for role expirations.'),
      ),
      'uid' => array(
        'type' => 'integer',
        'label' => t('User ID'),
        'description' => t('The user account ID.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user account that has the role.'),
        'getter callback' => 'uc_roles_get_expiration_properties',
        'setter callback' => 'uc_roles_set_expiration_properties',
      ),
      'rid' => array(
        'type' => 'integer',
        'label' => t('Role ID'),
        'description' => t('The granted role.'),
      ),
      'expiration' => array(
        'type' => 'date',
        'label' => t('Expiration time'),
        'description' => t('The time the role will be removed from the user.'),
      ),
      'notified' => array(
        'type' => 'boolean',
        'label' => t('Notified'),
        'description' => t('Indicates the user has been warned that the role will be removed soon.'),
      ),
    ),
  );

  return $entities;
}

/**
 * Callback for getting role expiration properties.
 *
 * @see entity_metadata_node_entity_info_alter()
 */
function uc_roles_get_expiration_properties($expiration, array $options, $name, $entity_type) {
  switch ($name) {
    case 'user':
      return $expiration->uid;
  }
}

/**
 * Callback for setting role expiration properties.
 *
 * @see entity_metadata_node_entity_info_alter()
 */
function uc_roles_set_expiration_properties($expiration, $name, $value) {
  if ($name == 'user') {
    $expiration->uid = $value;
  }
}

/**
 * Implements hook_rules_action_info().
 */
function uc_roles_rules_action_info() {
  // Renew a role expiration.
  $actions['uc_roles_order_renew'] = array(
    'label' => t('Renew the roles on an order.'),
    'group' => t('Renewal'),
    'base' => 'uc_roles_action_order_renew',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'message' => array(
        'type' => 'boolean',
        'label' => t('Display messages to alert users of any new or updated roles.'),
      ),
    ),
  );

  $email_args = array(
    'expiration' => array(
      'type' => 'uc_roles_expiration',
      'label' => t('Role expiration'),
    ),
    'from' => array(
      'type' => 'text',
      'label' => t('Sender'),
    ),
    'addresses' => array(
      'type' => 'text',
      'label' => t('Recipients'),
    ),
    'subject' => array(
      'type' => 'text',
      'label' => t('Subject'),
    ),
    'message' => array(
      'type' => 'text',
      'label' => t('Message'),
    ),
    'format' => array(
      'type' => 'text',
      'label' => t('Message format'),
      'options list' => 'uc_roles_message_formats',
    ),
  );

  // Send an email to an order with a role expiration
  $actions['uc_roles_order_email'] = array(
    'label' => t('Send an order email regarding roles.'),
    'group' => t('Notification'),
    'base' => 'uc_roles_action_order_email',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ) + $email_args,
  );

  // Send an email to a user with a role expiration
  $actions['uc_roles_user_email'] = array(
    'label' => t('Send a user an email regarding roles.'),
    'group' => t('Notification'),
    'base' => 'uc_roles_action_user_email',
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
    ) + $email_args,
  );

  return $actions;
}

/**
 * Options list callback for message formats.
 */
function uc_roles_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }

  return $options;
}

/**
 * Implements hook_rules_event_info().
 */
function uc_roles_rules_event_info() {
  $order = array(
    'type' => 'uc_order',
    'label' => t('Order'),
  );
  $account = array(
    'type' => 'user',
    'label' => t('User'),
  );
  $expiration = array(
    'type' => 'uc_roles_expiration',
    'label' => t('Role expiration'),
  );

  $events['uc_roles_notify_grant'] = array(
    'label' => t('E-mail for granted roles'),
    'group' => t('Notification'),
    'variables' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $events['uc_roles_notify_revoke'] = array(
    'label' => t('E-mail for revoked roles'),
    'group' => t('Notification'),
    'variables' => array(
      'account' => $account,
      'expiration' => $expiration,
    ),
  );

  $events['uc_roles_notify_renew'] = array(
    'label' => t('E-mail for renewed roles'),
    'group' => t('Notification'),
    'variables' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $events['uc_roles_notify_reminder'] = array(
    'label' => t('E-mail for role expiration reminders'),
    'group' => t('Notification'),
    'variables' => array(
      'account' => $account,
      'expiration' => $expiration,
    ),
  );

  return $events;
}

/**
 * Send an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_roles_action_order_email_form()
 */
function uc_roles_action_order_email($order, $role_expiration, $from, $addresses, $subject, $message, $format) {
  $settings = array(
    'from' => $from,
    'addresses' => $addresses,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
  );
  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'uc_order' => $order,
    'uc_role' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $address = trim($address);
    // Remove blank lines
    if (!empty($address)) {
      $recipients[] = $address;
    }
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('uc_roles', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Send an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_roles_action_user_email_form()
 */
function uc_roles_action_user_email($account, $role_expiration, $from, $addresses, $subject, $message, $format) {
  $settings = array(
    'from' => $from,
    'addresses' => $addresses,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
  );
  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'user' => $account,
    'uc_role' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $address = trim($address);
    // Remove blank lines
    if (!empty($address)) {
      $recipients[] = $address;
    }
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('uc_roles', 'Attempt to e-mail @email concerning role notification failed.', array('@email' => $email), WATCHDOG_ERROR);
    }
  }
}

/**
 * Renews an order's product roles.
 *
 * This function updates expiration time on all roles found on all products
 * on a given order. First, the order user is loaded, then the order's products
 * are scanned for role product features. If any are found, the expiration time
 * of the role is set using the feature settings to determine the new length of
 * time the new expiration will last. An order comment is saved, and the user
 * is notified in Drupal, as well as through the email address associated with
 * the order.
 *
 * @param $order
 *   An Ubercart order object.
 * @param $message
 *   If TRUE, messages will be displayed to the user about the renewal.
 */
function uc_roles_action_order_renew($order, $message) {
  // Load the order's user and exit if not available.
  if (!($account = user_load($order->uid))) {
    return;
  }

  // Loop through all the products on the order.
  foreach ($order->products as $product) {
    // Look for any role promotion features assigned to the product.
    $roles = db_query("SELECT * FROM {uc_roles_products} WHERE nid = :nid", array(':nid' => $product->nid));

    foreach ($roles as $role) {
      // Product model matches, or was 'any'.
      if (!empty($role->model) && $role->model != $product->model) {
        continue;
      }

      $existing_role = db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = :uid AND rid = :rid", array(':uid' => $account->uid, ':rid' => $role->rid))->fetchObject();

      // Determine the expiration timestamp for the role.
      $expiration = _uc_roles_product_get_expiration($role, $product->qty, isset($existing_role->expiration) ? $existing_role->expiration : NULL);

      // Leave an order comment.
      if (isset($existing_role->expiration)) {
        $op = 'renew';
        $comment = t('Customer user role %role renewed.', array('%role' => _uc_roles_get_name($role->rid)));

        // Renew the user's role.
        uc_roles_renew($account, $role->rid, $expiration, !$message);
      }
      else {
        $op = 'grant';
        $comment = t('Customer granted user role %role.', array('%role' => _uc_roles_get_name($role->rid)));

        // Grant the role to the user.
        uc_roles_grant($account, $role->rid, $expiration, TRUE, !$message);
      }

      // Get the new expiration (if applicable)
      $new_expiration = db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = :uid AND rid = :rid", array(':uid' => $account->uid, ':rid' => $role->rid))->fetchObject();
      if (!$new_expiration) {
        $new_expiration = new stdClass();
        $new_expiration->uid = $account->uid;
        $new_expiration->rid = $role->rid;
        $new_expiration->expiration = NULL;
      }

      uc_order_comment_save($order->order_id, $account->uid, $comment);

      // Trigger role email.
      rules_invoke_event('uc_roles_notify_' . $op, $order, $new_expiration);
    }
  }
}
