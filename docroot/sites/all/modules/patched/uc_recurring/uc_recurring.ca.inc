<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to make the
 * recurring-related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implements hook_ca_entity().
 *
 * An entity is defined in order to get the recurring fee down to token in the
 * email.
 */
function uc_recurring_ca_entity() {
  // CA entity for a recurring fee object.
  $entities['uc_recurring_fee'] = array(
    '#title' => t('Recurring Fee'),
    '#type' => 'object',
    '#load' => 'uc_recurring_fee_user_load',
    '#save' => 'uc_recurring_fee_user_save',
  );

  return $entities;
}

/**
 * Implements hook_ca_predicate().
 */
function uc_recurring_ca_predicate() {
  $predicates = array();

  // Set the order status to "Completed" when checkout is complete, none
  // of the products are shippable, and the balance is less than or equal to 0.
  $predicates['uc_recurring_renewal_complete_paid'] = array(
    '#title' => t('Update order status upon renewal completion with full payment'),
    '#trigger' => 'uc_recurring_renewal_complete',
    '#class' => 'payment',
    '#status' => 1,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_payment_condition_order_balance',
          '#title' => t('If the balance is less than or equal to $0.00.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'negate' => FALSE,
            'balance_comparison' => 'less_equal',
          ),
        ),
        array(
          '#name' => 'uc_order_condition_is_shippable',
          '#title' => t('If the order is not shippable.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'negate' => TRUE,
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_order_update_status',
        '#title' => t('Update the order status to Completed.'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'order_status' => 'completed',
        ),
      ),
    ),
  );

  $predicates['uc_recurring_renewal_email_completed'] = array(
    '#title' => t('Notify customer when a renewal has occurred.'),
    '#trigger' => 'uc_recurring_renewal_complete',
    '#class' => 'notification',
    '#status' => 0,
    '#weight' => 2,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_recurring_renewal_status_condition',
          '#title' => t('If the order contains product renewal.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_renewal_email',
        '#title' => t('Send an order email regarding order renewal.'),
        '#argument_map' => array(
          'order' => 'order',
          'recurring_fee' => 'recurring_fee',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_recurring_renewal_completed_subject'),
          'message' => uc_get_message('uc_recurring_renewal_completed_message'),
          'format' => 1,
        ),
      ),
    ),
  );
  $predicates['uc_recurring_renewal_email_failed'] = array(
    '#title' => t('Notify customer when a renewal has failed.'),
    '#trigger' => 'uc_recurring_renewal_failed',
    '#class' => 'notification',
    '#status' => 0,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_recurring_renewal_status_condition',
          '#title' => t('If the order contains product renewal.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_renewal_email',
        '#title' => t('Send an order email regarding order renewal.'),
        '#argument_map' => array(
          'order' => 'order',
          'recurring_fee' => 'recurring_fee',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_recurring_renewal_failed_subject'),
          'message' => uc_get_message('uc_recurring_renewal_failed_message'),
          'format' => 1,
        ),
      ),
    ),
  );
  $predicates['uc_recurring_renewal_email_expired'] = array(
    '#title' => t('Notify customer when a renewal has expired due to failed payment.'),
    '#trigger' => 'uc_recurring_renewal_failed',
    '#class' => 'notification',
    '#status' => 0,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_recurring_renewal_status_condition',
          '#title' => t('If the order contains product renewal.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
          ),
        ),
        array(
          '#name' => 'uc_recurring_renewal_expired_condition',
          '#title' => t('If the recurring fee has expired.'),
          '#argument_map' => array(
            'recurring_fee' => 'recurring_fee',
          ),
          '#settings' => array(
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_renewal_email',
        '#title' => t('Send an order email regarding order renewal.'),
        '#argument_map' => array(
          'order' => 'order',
          'recurring_fee' => 'recurring_fee',
        ),
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_recurring_renewal_expired_subject'),
          'message' => uc_get_message('uc_recurring_renewal_expired_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  return $predicates;
}

/**
 * Implements hook_ca_condition().
 */
function uc_recurring_ca_condition() {
  $conditions['uc_recurring_renewal_status_condition'] = array(
    '#title' => t('Check if the order is a renewal'),
    '#description' => t('Returns TRUE if the current order is a renewal.'),
    '#category' => t('Order: Recurring'),
    '#callback' => 'uc_recurring_condition_order_contains_renewals',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );
  $conditions['uc_recurring_renewal_expired_condition'] = array(
    '#title' => t('Check if the recurring fee has expired'),
    '#description' => t('Returns TRUE if the current recurring fee has expired.'),
    '#category' => t('Order: Recurring'),
    '#callback' => 'uc_recurring_condition_order_has_expired',
    '#arguments' => array(
      'recurring_fee' => array(
        '#entity' => 'uc_recurring_fee',
        '#title' => t('Recurring Fee'),
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_ca_action().
 */
function uc_recurring_ca_action() {
  // Send an email to an order with a role expiration
  $actions['uc_recurring_renewal_email'] = array(
    '#title' => t('Send an order email regarding order renewal.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_recurring_action_renewal_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'recurring_fee' => array(
        '#entity' => 'uc_recurring_fee',
        '#title' => t('Recurring Fee'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_ca_trigger().
 */
function uc_recurring_ca_trigger() {
  $order = array(
    '#entity' => 'uc_order',
    '#title' => t('Order'),
  );
  $recurring_fee = array(
    '#entity' => 'uc_recurring_fee',
    '#title' => t('Recurring Fee'),
  );

  $triggers['uc_recurring_renewal_complete'] = array(
    '#title' => t('Recurring order renewal succeeded'),
    '#category' => t('Recurring Fees'),
    '#arguments' => array(
      'order' => $order,
      'recurring_fee' => $recurring_fee,
    ),
  );

  $triggers['uc_recurring_renewal_failed'] = array(
    '#title' => t('Recurring order renewal failed'),
    '#category' => t('Recurring Fees'),
    '#arguments' => array(
      'order' => $order,
      'recurring_fee' => $recurring_fee,
    ),
  );

  $triggers['uc_recurring_cancel'] = array(
    '#title' => t('Recurring order cancelled'),
    '#category' => t('Recurring Fees'),
    '#arguments' => array(
      'order' => $order,
      'recurring_fee' => $recurring_fee,
    ),
  );

  $triggers['uc_recurring_renewal_expired'] = array(
    '#title' => t('Recurring order renewal expired'),
    '#category' => t('Recurring Fees'),
    '#arguments' => array(
      'order' => $order,
      'recurring_fee' => $recurring_fee,
    ),
  );

  return $triggers;
}

/**
 * Check if the order contains a renewal product.
 *
 * @param $order
 *   The order object.
 * @param $recurring_fee
 *   The recurring fee object.
 * @param $settings
 *   The order settings.
 */
function uc_recurring_condition_order_contains_renewals($order, $settings) {
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
 * @see uc_recurring_action_renewal_email_form()
 */
function uc_recurring_action_renewal_email($order, $recurring_fee, $settings) {
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
 * Email settings form.
 *
 * @see uc_recurring_action_renewal_email()
 */
function uc_recurring_action_renewal_email_form($form_state, $settings = array()) {
  // ca_build_email_form() function seems to be incorrectly part of uc_roles so
  // we add this hack to include the uc_roles.ca.inc here.
  module_load_include('inc', 'uc_roles', 'uc_roles.ca');
  $settings += array(
    'from' => uc_store_email_from(),
    'addresses' => '[order-email]',
    'subject' => uc_get_message('uc_recurring_renewal_completed_subject'),
    'message' => uc_get_message('uc_recurring_renewal_completed_message'),
    'format' => 1,
  );
  return ca_build_email_form($form_state, $settings, array('global', 'order', 'user', 'recurring_fee'));
}


