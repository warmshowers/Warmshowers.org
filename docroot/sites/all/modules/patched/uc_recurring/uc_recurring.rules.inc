<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_recurring_rules_data_info() {
  return array(
    'uc_recurring' => array(
      'label' => t('Ubercart recurring fee object'),
      'wrap' => TRUE,
      'group' => t('Ubercart'),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 * Send an email to an order with a role expiration.
 */
function uc_recurring_rules_action_info() {
  return array(
    'uc_recurring_renewal_email' => array(
      'label' => t('Send an order email regarding order renewal.'),
      'group' => t('Recurring Fee'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
          'restriction' => 'selector',
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
          'options list' => 'uc_recurring_message_formats',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function uc_recurring_rules_event_info() {
  return array(
    'uc_recurring_renewal_complete' => array(
      'label' => t('Recurring fee renewal succeeded'),
      'group' => t('Recurring Fees'),
      'variables' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
        ),
      ),
    ),
    'uc_recurring_renewal_failed' => array(
      'label' => t('Recurring fee renewal failed'),
      'group' => t('Recurring Fees'),
      'variables' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
        ),
      ),
    ),
    'uc_recurring_cancel' => array(
      'label' => t('Recurring fee cancelled'),
      'group' => t('Recurring Fees'),
      'variables' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
        ),
      ),
    ),
    'uc_recurring_renewal_expired' => array(
      'label' => t('Recurring fee renewal expired'),
      'group' => t('Recurring Fees'),
      'variables' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_recurring_rules_condition_info() {
  return array(
    'uc_recurring_condition_order_contains_renewals' => array(
      'label' => t('Check if the order is a renewal'),
      'group' => t('Recurring'),
      'base' => 'uc_recurring_condition_order_contains_renewals',
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
          'restriction' => 'selector',
        ),
      ),
    ),
    'uc_recurring_condition_order_has_expired' => array(
      'label' => t('Check if the recurring fee has expired'),
      'group' => t('Recurring'),
      'base' => 'uc_recurring_condition_order_has_expired',
      'parameter' => array(
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
          'restriction' => 'selector',
        ),
      ),
    ),
  );
}

/**
 * Options list callback for message formats.
 */
function uc_recurring_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }

  return $options;
}

/**
 * Check if the order contains a renewal product.
 *
 * @param $order
 *   The order object.
 * @param $recurring_fee
 *   The recurring fee object.
 */
function uc_recurring_condition_order_contains_renewals($order) {
  foreach ($order->products as $products) {
    if (!empty($products->data['recurring_fee'])) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Check if the recurring fee has expired.
 */
function uc_recurring_condition_order_has_expired($recurring_fee, $settings) {
  return ($recurring_fee->remaining_intervals == 0);
}

/**
 * Send an email with order and recurring fee replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 *
 */
function uc_recurring_renewal_email($order, $recurring_fee, $from, $addresses, $subject, $message, $format) {
  $settings = array(
    'from' => $from,
    'addresses' => $addresses,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
  );

  $account = uc_order_user_load($order);

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
    'recurring_fee' => $recurring_fee,
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
      watchdog('uc_recurring', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}
