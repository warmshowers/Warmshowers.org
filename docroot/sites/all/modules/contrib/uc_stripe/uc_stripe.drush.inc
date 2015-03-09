<?php

/**
 * @file
 *   Delete legacy uc_stripe subscriptions that were created by v1.
 */

/**
 * Implements hook_drush_command().
 *
 *
 * @return
 *   An associative array describing your command(s).
 */
function uc_stripe_drush_command() {
  $items = array();

  $items['subscription-cancel'] = array(
    'description' => "Delete subscriptions created by uc_stripe v1 and existing in uc_recurring_stripe table.",
    'arguments' => array(
      'order_id' => 'Order ID from uc_stripe_recurring table',
    ),
    'options' => array(
      'dry-run' => 'Dry run - does not actually execute, but show what would be done',
    ),
    'examples' => array(
      'drush subscription-cancel',
      'drush subscription-cancel --dry-run',
      'drush subscription-cancel <order_id>',
    ),
    'aliases' => array('subcancel'),
  );

  return $items;
}


/**
 * Command callback
 *
 * Delete active subscriptions found in uc_recurring_stripe table.
 */
function drush_uc_stripe_subscription_cancel($order_id = 'all') {

  $dry_run = drush_get_option('dry-run');

  if (!db_table_exists('uc_recurring_stripe')) {
    drush_print('No uc_recurring_stripe table exists, exiting.');
    return;
  }

  if ($dry_run) {
    drush_print('dry-run is set so not deleting any subscriptions');
  }
  _uc_stripe_prepare_api();
  $query = 'SELECT * from {uc_recurring_stripe} ucs ';
  if ($order_id != 'all') {
    $query .= ' WHERE order_id = %d';
  }
  $result = db_query($query, array($order_id));

  while ($item = db_fetch_array($result)) {
    $customer = NULL;
    $account = user_load($item['uid']);

    try {
      $customer = Stripe_Customer::retrieve($item['customer_id']);
      $subscriptions = $customer->subscriptions->all();
    } catch (Exception $e) {
      drush_print("Failed to retrieve customer subscriptions for {$item['customer_id']} (user:$account->name}. Message: {$e->getMessage()}");
      continue;
    }

    $count = count($subscriptions['data']);

    $msg = "Retrieved Stripe Customer with $count subscriptions,  uid:{$item['uid']} username:{$account->name} order:{$item['order_id']} stripe customer id:{$item['customer_id']}";
    drush_print($msg);
    foreach ($subscriptions['data'] as $sub) {
      drush_print("   Subscription: {$sub->id} {$sub->status} {$sub->plan->name} {$sub->plan->id}");
      if (empty($dry_run) && drush_confirm('Are you sure you want to cancel this subscription?')) {
        try {
          $sub->cancel();
          drush_print("    Cancelled subscription {$sub->id}");
        } catch (Exception $e) {
          drush_print("    FAILED to cancel subscription {$sub->id} Message: {$e->getMessage()}");
        }
      }
    }
  }
}
