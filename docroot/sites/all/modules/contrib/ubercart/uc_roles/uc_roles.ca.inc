<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to
 * make the roles-related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implements hook_ca_entity().
 *
 * An entity is defined in order to get a role expiration down
 * to token in the email.
 */
function uc_roles_ca_entity() {
  // CA entity for a role expiration object.
  $entities['uc_roles_expiration'] = array(
    '#title' => t('Ubercart role expiration'),
    '#type' => 'object',
  );

  return $entities;
}

/**
 * Implements hook_ca_predicate().
 */
function uc_roles_ca_predicate() {
  $predicates = array();

  // Renew all the roles on an order when the status matches what's set in the roles admin settings.
  $predicates['uc_role_renewal'] = array(
    '#title' => t('Grant or renew purchased roles'),
    '#description' => t('Grant or renew purchased roles if the order status matches.'),
    '#class' => 'renewal',
    '#trigger' => 'uc_order_status_update',
    '#status' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the updated order status is payment received.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'order_status' => 'payment_received',
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_renew',
        '#title' => t('Update all role expirations for this order.'),
        '#argument_map' => array(
          'order' => 'updated_order',
        ),
        '#settings' => array(
          'message' => FALSE,
        ),
      ),
    ),
  );

  $order_args = array(
    'order' => 'order',
    'expiration' => 'expiration',
  );

  $user_args = array(
    'account' => 'account',
    'expiration' => 'expiration',
  );

  // Notify the user when a role is granted.
  $predicates['uc_role_notify_grant'] = array(
    '#title' => t('Notify customer when a role is granted'),
    '#description' => t('Notify the customer when they have had a role granted on their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_grant',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $order_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email-raw]',
          'subject' => uc_get_message('uc_roles_grant_subject'),
          'message' => uc_get_message('uc_roles_grant_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is revoked.
  $predicates['uc_role_notify_revoke'] = array(
    '#title' => t('Notify customer when a role is revoked'),
    '#description' => t('Notify the customer when they have had a role revoked from their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_revoke',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_user_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $user_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[mail]',
          'subject' => uc_get_message('uc_roles_revoke_subject'),
          'message' => uc_get_message('uc_roles_revoke_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is renewed.
  $predicates['uc_role_notify_renew'] = array(
    '#title' => t('Notify customer when a role is renewed'),
    '#description' => t('Notify the customer when they have had a role renewed on their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_renew',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $order_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email-raw]',
          'subject' => uc_get_message('uc_roles_renew_subject'),
          'message' => uc_get_message('uc_roles_renew_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is about to expire.
  $predicates['uc_role_notify_reminder'] = array(
    '#title' => t('Notify customer when a role is about to expire'),
    '#description' => t('Notify the customer when they have had a role that is about to expire.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_reminder',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_user_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $user_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[mail]',
          'subject' => uc_get_message('uc_roles_reminder_subject'),
          'message' => uc_get_message('uc_roles_reminder_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implements hook_ca_action().
 */
function uc_roles_ca_action() {
  // Renew a role expiration.
  $actions['uc_roles_order_renew'] = array(
    '#title' => t('Renew the roles on an order.'),
    '#category' => t('renewal'),
    '#callback' => 'uc_roles_action_order_renew',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  // Send an email to an order with a role expiration
  $actions['uc_roles_order_email'] = array(
    '#title' => t('Send an order email regarding roles.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_roles_action_order_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'expiration' => array(
        '#entity' => 'uc_roles_expiration',
        '#title' => t('Role expiration'),
      ),
    ),
  );

  // Send an email to a user with a role expiration
  $actions['uc_roles_user_email'] = array(
    '#title' => t('Send an order email regarding roles.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_roles_action_user_email',
    '#arguments' => array(
      'account' => array(
        '#entity' => 'user',
        '#title' => t('User'),
      ),
      'expiration' => array(
        '#entity' => 'uc_roles_expiration',
        '#title' => t('Role expiration'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_ca_trigger().
 */
function uc_roles_ca_trigger() {
  $order = array(
    '#entity' => 'uc_order',
    '#title' => t('Order'),
  );
  $account = array(
    '#entity' => 'user',
    '#title' => t('User'),
  );
  $expiration = array(
    '#entity' => 'uc_roles_expiration',
    '#title' => t('Role expiration'),
  );

  $triggers['uc_roles_notify_grant'] = array(
    '#title' => t('E-mail for granted roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_revoke'] = array(
    '#title' => t('E-mail for revoked roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'account' => $account,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_renew'] = array(
    '#title' => t('E-mail for renewed roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_reminder'] = array(
    '#title' => t('E-mail for role expiration reminders'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'account' => $account,
      'expiration' => $expiration,
    ),
  );

  return $triggers;
}

/**
 * Sends an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_roles_action_order_email_form()
 */
function uc_roles_action_order_email($order, $role_expiration, $settings) {
  $account = uc_order_user_load($order);

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
    'uc_roles' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
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
      watchdog('ca', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

/**
 * Email settings form.
 *
 * @see uc_roles_action_order_email()
 */
function uc_roles_action_order_email_form($form_state, $settings = array()) {
  return ca_build_email_form($form_state, $settings, array('global', 'uc_roles', 'user'));
}

/**
 * Sends an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_roles_action_user_email_form()
 */
function uc_roles_action_user_email($account, $role_expiration, $settings) {
  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'user' => $account,
    'uc_roles' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
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
      watchdog('ca', 'Attempt to e-mail @email concerning role notification failed.', array('@email' => $email), WATCHDOG_ERROR);
    }
  }
}

/**
 * Email settings form.
 *
 * @see uc_roles_action_user_email()
 */
function uc_roles_action_user_email_form($form_state, $settings = array()) {
  return ca_build_email_form($form_state, $settings, array('global', 'uc_roles', 'user'));
}

/**
 * Renews an orders product roles.
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
 *
 * @see uc_roles_action_order_renew_form()
 */
function uc_roles_action_order_renew($order, $settings) {
  // Load the order's user and exit if not available.
  if (!($account = user_load($order->uid))) {
    return;
  }

  // Loop through all the products on the order.
  foreach ($order->products as $product) {
    // Look for any role promotion features assigned to the product.
    $roles = db_query("SELECT * FROM {uc_roles_products} WHERE nid = %d", $product->nid);

    while ($role = db_fetch_object($roles)) {
      // Product model matches, or was 'any'.
      if (!empty($role->model) && $role->model != $product->model) {
        continue;
      }

      $existing_role = db_fetch_object(db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = %d AND rid = %d", $account->uid, $role->rid));

      // Determine the expiration timestamp for the role.
      $expiration = _uc_roles_product_get_expiration($role, $product->qty, isset($existing_role->expiration) ? $existing_role->expiration : NULL);

      // Leave an order comment.
      if (isset($existing_role->expiration)) {
        $op = 'renew';
        $comment = t('Customer user role %role renewed.', array('%role' => _uc_roles_get_name($role->rid)));

        // Renew the user's role.
        uc_roles_renew($account, $role->rid, $expiration, !$settings['message']);
      }
      else {
        $op = 'grant';
        $comment = t('Customer granted user role %role.', array('%role' => _uc_roles_get_name($role->rid)));

        // Grant the role to the user.
        uc_roles_grant($account, $role->rid, $expiration, TRUE, !$settings['message']);
      }

      // Get the new expiration (if applicable)
      $new_expiration = db_fetch_object(db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = %d AND rid = %d", $account->uid, $role->rid));
      if (!$new_expiration) {
        $new_expiration = new stdClass();
        $new_expiration->uid = $account->uid;
        $new_expiration->rid = $role->rid;
        $new_expiration->expiration = NULL;
      }

      uc_order_comment_save($order->order_id, $account->uid, $comment);

      // Trigger role email.
      ca_pull_trigger('uc_roles_notify_'. $op, $order, $new_expiration);
    }
  }
}

/**
 * @see uc_roles_action_order_renew()
 */
function uc_roles_action_order_renew_form($form_state, $settings = array()) {
  $form = array();

  $form['message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display messages to alert users of any new or updated roles.'),
    '#default_value' => isset($settings['message']) ? $settings['message'] : FALSE,
  );

  return $form;
}

/**
 * Implements hook_ca_condition().
 */
function uc_roles_ca_condition() {
  $conditions['uc_roles_condition_role'] = array(
    '#title' => t('Check the role being granted'),
    '#description' => t('Returns TRUE if the role being granted matches any of those specified below.'),
    '#category' => t('Order: Roles'),
    '#callback' => 'uc_roles_condition_role',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'expiration' => array(
        '#entity' => 'uc_roles_expiration',
        '#title' => t('Role expiration'),
      ),
    ),
  );
  return $conditions;
}

/**
 * Checks the role being granted.
 *
 * @param $order
 *   The order as passed by uc_roles_notify_*
 * @param $expiration
 *   The role and expiration data as passed by uc_roles_notify_*
 * @param $settings
 *   Settings as generated by uc_condition_role_form().
 *
 * @see uc_roles_condition_role_form()
 */
function uc_roles_condition_role($order, $expiration, $settings) {
  $roles = array_filter($settings['roles']);
  return isset($roles[$expiration->rid]);
}

/**
 * Settings form for checking the role being granted.
 *
 * @see uc_roles_condition_role()
 */
function uc_roles_condition_role_form($form_state, $settings = array()) {
  $form = ca_condition_user_roles_form($form_state, $settings);
  $form['operator']['#access'] = FALSE;
  return $form;
}
