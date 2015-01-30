<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to
 * make the file-related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implements hook_ca_entity().
 *
 * An entity is defined in order to get file expiration(s) down
 * to token in the email.
 */
function uc_file_ca_entity() {

  // CA entity for a file expiration object.
  $entities['uc_file_expiration'] = array(
    '#title' => t('Ubercart file expiration(s)'),
    '#type' => 'array',
  );

  return $entities;
}

/**
 * Implements hook_ca_predicate().
 */
function uc_file_ca_predicate() {

  // Renew all the files on an order when the status matches what's set in the files admin settings.
  $configurations['uc_file_renewal'] = array(
    '#title' => t('Renew purchased files'),
    '#description' => t('Renew purchased files if the order status matches.'),
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
        '#name' => 'uc_file_order_renew',
        '#title' => t('Update all file expirations for this order.'),
        '#argument_map' => array(
          'order' => 'updated_order',
        ),
      ),
    ),
  );

  $order_args = array(
    'order' => 'order',
    'expiration' => 'expiration',
  );

  // Notify the user when a file is granted.
  $configurations['uc_file_notify_grant_trigger'] = array(
    '#title' => t('Notify customer when a file is granted'),
    '#description' => t('Notify the customer when they have had a file granted on their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_file_notify_grant',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_file_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $order_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email-raw]',
          'subject' => t("File Downloads for Order# [order-id]"),
          'message' => t("Your order (order# [order-link]) at [store-name] included file download(s). You may access them with the following link(s):\n\n[file-downloads]\n\nAfter downloading these files these links will have expired. If you need to download the files again, you can login at [site-login] and visit the \"My Account\" section of the site.\n\nThanks again, \n\n[store-name]\n[site-slogan]"),
          'format' => 1,
        ),
      ),
    ),
  );

  return $configurations;
}

/**
 * Implements hook_ca_action().
 */
function uc_file_ca_action() {

  $actions['uc_file_order_renew'] = array(
    '#title' => t('Renew the files on an order.'),
    '#category' => t('renewal'),
    '#callback' => 'uc_file_action_order_renew',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  // Send an email to an order with a file expiration
  $actions['uc_file_order_email'] = array(
    '#title' => t('Send an order email regarding files.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_file_action_order_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'expiration' => array(
        '#entity' => 'uc_file_expiration',
        '#title' => t('File expiration'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_ca_trigger().
 */
function uc_file_ca_trigger() {

  $args = array(
    'order' => array(
      '#entity' => 'uc_order',
      '#title' => t('Order'),
    ),
    'expiration' => array(
      '#entity' => 'uc_file_expiration',
      '#title' => t('File expiration'),
    ),
  );

  $triggers['uc_file_notify_grant'] = array(
    '#title' => t('E-mail for granted files'),
    '#category' => t('Notification'),
    '#arguments' => $args,
  );

  return $triggers;
}

/**
 * Sends an email with order and file replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 * @see uc_file_action_order_email_form()
 */
function uc_file_action_order_email($order, $file_expiration, $settings) {
  $account = uc_order_user_load($order);

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
    'uc_file' => $file_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
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
 * @see uc_file_action_order_email()
 */
function uc_file_action_order_email_form($form_state, $settings = array()) {
  return ca_build_email_form($form_state, $settings, array('global', 'uc_file'));
}

/**
 * Renews an orders product files.
 *
 * This function updates access limits on all files found on all products
 * on a given order. First, the order user is loaded, then the order's products
 * are scanned for file product features. An order comment is saved, and the
 * user is notified in Drupal, as well as through the email address associated
 * with the order.
 *
 * @param $order
 *   An Ubercart order object.
 */
function uc_file_action_order_renew($order) {

  $user_downloads = array();

  // Load user.
  if (!$order->uid || !($order_user = user_load($order->uid))) {
    return;
  }

  // Scan products for models matching downloads.
  foreach ($order->products as $product) {
    $files = db_query("SELECT * FROM {uc_file_products} AS fp ".
      "INNER JOIN {uc_product_features} AS pf ON pf.pfid = fp.pfid ".
      "INNER JOIN {uc_files} as f ON f.fid = fp.fid ".
      "WHERE nid = %d", $product->nid);

    while ($file = db_fetch_object($files)) {

      // Either they match, or the file was set to any SKU.
      if (!empty($file->model) && $file->model != $product->model) {
        continue;
      }

      // Grab any existing privilege so we can calculate the new expiration time
      // as an offset of the previous.
      $file_user = _uc_file_user_get($order_user, $file->fid);

      // Get the limit info from the product feature
      $file_modification = array(
        'download_limit' => uc_file_get_download_limit($file),
        'address_limit' => uc_file_get_address_limit($file),
        'expiration' => _uc_file_expiration_date(uc_file_get_time_limit($file), max($file_user->expiration, time())),
      );

      // Add file_user(s) for this file/directory. (No overwrite)
      $new_files = uc_file_user_renew($file->fid, $order_user, $file->pfid, $file_modification, FALSE);

      // Save for notification.
      $user_downloads = array_merge($user_downloads, $new_files);

      // Note on the order where the user has gained download permission.
      if (is_dir(uc_file_qualify_file($file->filename))) {
        $comment = t('User can now download files in the directory %dir.', array('%dir' => $file->filename));
      }
      else {
        $comment = t('User can now download the file %file.', array('%file' => basename($file->filename)));
      }
      uc_order_comment_save($order->order_id, $order_user->uid, $comment);
    }
  }

  // Notify the user of their download(s).
  if ($user_downloads) {
    ca_pull_trigger('uc_file_notify_grant', $order, $user_downloads);
  }
}
