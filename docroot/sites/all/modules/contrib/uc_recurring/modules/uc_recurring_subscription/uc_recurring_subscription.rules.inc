<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implements hook_rules_data_info().
 */
function uc_recurring_subscription_rules_data_info() {
  return array(
    'uc_recurring_subscription' => array(
      'label' => t('Ubercart recurring subscription'),
      'wrap' => TRUE,
      'group' => t('Ubercart'),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function uc_recurring_subscription_rules_event_info() {
  return array();
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_recurring_subscription_rules_condition_info() {
  return array();
}

/**
 * Implements hook_rules_action_info().
 */
function uc_recurring_subscription_rules_action_info() {
  $actions = array();
  $actions['uc_recurring_subscription_grant_role'] = array(
    'label' => t('Grant a role on an order.'),
    'group' => t('Role'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'role_option' => array(
        'type' => 'text',
        'label' => t('Where are the role(s) defined?'),
        'options list' => '_uc_recurring_subscription_role_options',
      ),
      'roles' => array(
        'type' => 'list<integer>',
        'label' => t('Role'),
        'options list' => '_uc_recurring_subscription_user_roles',
        'optional' => TRUE,
      ),
    ),
  );
  $actions['uc_recurring_subscription_revoke_role'] = array(
    'label' => t('Revoke a role on an order.'),
    'group' => t('Role'),
    'parameter' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
      'role_option' => array(
        'type' => 'text',
        'label' => t('Where are the role(s) defined?'),
        'options list' => '_uc_recurring_subscription_role_options',
      ),
      'role_option' => array(
        'type' => 'text',
        'label' => t('Where are the role(s) defined?'),
        'options list' => '_uc_recurring_subscription_role_options',
      ),
      'roles' => array(
        'type' => 'list<integer>',
        'label' => t('Role'),
        'options list' => '_uc_recurring_subscription_user_roles',
        'optional' => TRUE,
      ),
    ),
  );

  if (module_exists('og')) {
    // actions to subscribe/unsubscribe user from organic group.
    $actions['uc_recurring_subscription_grant_og'] = array(
      'label' => t('Grant organic group membership on order.'),
      'group' => t('Organic Group'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'group_option' => array(
          'type' => 'text',
          'label' => t('Where are the role(s) defined?'),
          'options list' => '_uc_recurring_subscription_role_options',
        ),
        'group' => array(
          'type' => 'group',
          'label' => t('Group'),
        ),
      ),
    );
    $actions['uc_recurring_subscription_revoke_og'] = array(
      'label' => t('Revoke organic group membership on order.'),
      'group' => t('Organic Group'),
      'parameter' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'group_option' => array(
          'type' => 'text',
          'label' => t('Where are the role(s) defined?'),
          'options list' => '_uc_recurring_subscription_role_options',
        ),
        'group' => array(
          'type' => 'group',
          'label' => t('Group'),
        ),
      ),
    );
  }
  return $actions;
}

function _uc_recurring_subscription_role_options() {
  return array(
    'subscribe_grant' => t('Apply the roles set by the Subscription Manager on creation.'),
    'expire_grant' => t('Apply the roles set by the Subscription Manager on expiration.'),
    'expire_revoke' => t('Revoke the roles set by the Subscription Manager on expiration.'),
    'custom' => t('Custom selected role (select from below)'),
  );
}

function _uc_recurring_subscription_group_options() {
  return array(
    'subscribe_grant_og' => t('Apply the groups set by the Subscription Manager on creation.'),
    'expire_grant_og' => t('Apply the groups set by the Subscription Manager on expiration.'),
    'expire_revoke_og' => t('Revoke the groups set by the Subscription Manager on expiration.'),
    'custom' => t('Custom selected groups (select from below)'),
  );
}

function _uc_recurring_subscription_user_roles() {
  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  return $roles;
}

/**
 * Grant user roles.
 */
function uc_recurring_subscription_grant_role($order, $role_option, $roles) {
  $account = user_load($order->uid);

  $role_names = user_roles(TRUE);
  if ($role_option == 'custom') {
    foreach ($roles as $rid => $role) {
      $account->roles[$rid] = $role_names[$rid];
      watchdog('uc_recurring', 'Granted !role role to !user', array('!role' => $role_names[$rid], '!user' => $account->name));
    }
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      if (!empty($subscription->access[$role_option])) {
        $account->roles += $subscription->access[$role_option];
      }
    }
  }
  $account = user_save($account, array('roles' => $account->roles));
}

/**
 * Revoke user roles.
 */
function uc_recurring_subscription_revoke_role($order, $role_option, $roles) {
  $account = user_load($order->uid);

  $role_names = user_roles(TRUE);
  if ($role_option == 'custom') {
    foreach ($roles as $rid => $role) {
      unset($account->roles[$rid]);
      watchdog('uc_recurring', 'Revoked !role role from !user', array('!role' => $role_names[$rid], '!user' => $account->name));
    }
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      if (!empty($subscription->access[$role_option])) {
        foreach ($subscription->access[$role_option] as $rid => $role) {
          // @todo: we need to check other subscriptions to ensure that the role
          // is no longer required by another subscription.
          unset($account->roles[$rid]);
        }
      }
    }
  }
  $account = user_save($account, array('roles' => $account->roles));
}

/**
 * Grant user organic groups.
 */
function uc_recurring_subscription_grant_og($order, $group_option, $group) {
  if (!$order->uid || !($account = user_load($order->uid))) {
    return;
  }
  if ($group_option == 'custom' && is_array($group) && !empty($group)) {
    og_subscribe_user_action($account, $group);
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      $context['groups'] = $subscription->access[$group_option];
      if (is_array($context['groups']) && !empty($context['groups'])) {
        og_subscribe_user_action($account, $context);
        $comment = t('User access granted to group(s): %group.', array('%group' => implode(',', $context['groups'])));
        uc_order_comment_save($order->order_id, $account->uid, $comment);
      }
    }
  }
}

/**
 * Revoke user organic groups.
 */
function uc_recurring_subscription_revoke_og($order, $group_option, $group) {
  // Load user.
  if (!$order->uid || !($account = user_load($order->uid))) {
    return;
  }
  if ($group_option == 'custom' && is_array($group) && !empty($group)) {
    og_unsubscribe_user_action($account, $group);
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      $context['groups'] = $subscription->access[$group_option];
      if (is_array($context['groups']) && !empty($context['groups'])) {
        og_unsubscribe_user_action($account, $context);
        $comment = t('User access revoked for group(s): %group.', array('%group' => implode(',', $context['groups'])));
        uc_order_comment_save($order->order_id, $account->uid, $comment);
      }
    }
  }
}
