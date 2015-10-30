<?php

/**
 * @file
 * Rules hooks and functions for uc_file.module.
 */

/**
 * Implements hook_rules_data_info().
 *
 * An entity is defined in order to get file expiration(s) down to token in the
 * email.
 */
function uc_file_rules_data_info() {
  $data['uc_file_expiration'] = array(
    'label' => t('Ubercart file expiration(s)'),
    'wrap' => TRUE,
    'property info' => array(
      'fuid' => array(
        'type' => 'integer',
        'label' => t('File-user ID'),
        'description' => t('Primary key for user permission to download a file.'),
      ),
      'fid' => array(
        'type' => 'integer',
        'label' => t('File ID'),
        'description' => t('The file that may be downloaded.'),
      ),
      'file' => array(
        'type' => 'file',
        'label' => t('File'),
        'description' => t('The file that may be downloaded.'),
        'getter callback' => 'uc_file_get_expiration_properties',
        'setter callback' => 'uc_file_set_expiration_properties',
      ),
      'uid' => array(
        'type' => 'integer',
        'label' => t('User ID'),
        'description' => t('The user account ID.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'description' => t('The user account that may download the file.'),
        'getter callback' => 'uc_file_get_expiration_properties',
        'setter callback' => 'uc_file_set_expiration_properties',
      ),
      'pfid' => array(
        'type' => 'integer',
        'label' => t('Product feature ID'),
        'description' => t('The product feature that granted permission to download the file.'),
      ),
      'file-key' => array(
        'type' => 'text',
        'label' => t('File key'),
        'description' => t('A hash of the permission and expiraiton data.'),
      ),
      'granted' => array(
        'type' => 'date',
        'label' => t('Granted time'),
        'description' => t('The time the permission to download the file was granted.'),
      ),
      'expiration' => array(
        'type' => 'date',
        'label' => t('Expiration time'),
        'description' => t('The time when the permission to download the file will expire.'),
      ),
      'accessed' => array(
        'type' => 'integer',
        'label' => t('Accesses'),
        'description' => t('The number of times the file has been accessed.'),
      ),
      'addresses' => array(
        'type' => 'list<text>',
        'label' => t('IP addresses'),
        'description' => t('List of IP addresses to which the file has been downloaded.'),
      ),
      'download-limit' => array(
        'type' => 'integer',
        'label' => t('Download limit'),
        'description' => t('The numer of times the user may download the file.'),
      ),
      'address-limit' => array(
        'type' => 'integer',
        'label' => t('Address limit'),
        'description' => t('The number of different IP addresses that may download the file.'),
      ),
    ),
    'group' => t('File download'),
    'token type' => 'uc_file',
  );

  return $data;
}

/**
 * Callback for getting download expiration properties.
 *
 * @see entity_metadata_node_entity_info_alter()
 */
function uc_file_get_expiration_properties($expiration, array $options, $name, $entity_type) {
  switch ($name) {
    case 'user':
      return $expiration->uid;
    case 'file':
      return $expiration->fid;
  }
}

/**
 * Callback for setting download expiration properties.
 *
 * @see entity_metadata_node_entity_info_alter()
 */
function uc_file_set_expiration_properties($expiration, $name, $value) {
  if ($name == 'user') {
    $expiration->uid = $value;
  }
  elseif ($name == 'file') {
    $expiration->fid = $value;
  }
}

/**
 * Implements hook_rules_action_info().
 */
function uc_file_rules_action_info() {
  // Renew file expirations.
  $actions['uc_file_order_renew'] = array(
    'label' => t('Renew the files on an order.'),
    'group' => t('renewal'),
    'base' => 'uc_file_action_order_renew',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
  );

  // Send an email to an order with a file expiration
  $actions['uc_file_order_email'] = array(
    'label' => t('Send an order email regarding files.'),
    'group' => t('Notification'),
    'base' => 'uc_file_action_order_email',
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'expiration' => array(
        'type' => 'uc_file_expiration',
        'label' => t('File expiration'),
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
        'options list' => 'uc_file_message_formats',
      ),
    ),
  );

  return $actions;
}

/**
 * Options list callback for message formats.
 */
function uc_file_message_formats() {
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
function uc_file_rules_event_info() {
  $events['uc_file_notify_grant'] = array(
    'label' => t('E-mail for granted files'),
    'group' => t('Notification'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'expiration' => array(
        'type' => 'uc_file_expiration',
        'label' => t('File expiration'),
      ),
    ),
  );

  return $events;
}

/**
 * Send an email with order and file replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 */
function uc_file_action_order_email($order, $file_expiration, $from, $addresses, $subject, $message, $format) {
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
    'uc_file' => $file_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('uc_file', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
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
    $files = db_query("SELECT * FROM {uc_file_products} fp " .
      "INNER JOIN {uc_product_features} pf ON pf.pfid = fp.pfid " .
      "INNER JOIN {uc_files} f ON f.fid = fp.fid " .
      "WHERE nid = :nid", array(':nid' => $product->nid));

    foreach ($files as $file) {

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
        'expiration' => _uc_file_expiration_date(uc_file_get_time_limit($file), ($file_user ? max($file_user->expiration, REQUEST_TIME) : NULL)),
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
    rules_invoke_event('uc_file_notify_grant', $order, $user_downloads);
  }
}
