<?php

/**
 * Implementation of hook_ca_entity().
 */
function uc_recurring_subscription_ca_entity() {
  $entities = array();

  $entities['uc_recurring_subscription'] = array(
    '#title' => t('Recurring subscription'),
    '#type' => 'object',
  );

  return $entities;
}

/**
 * Implementation of hook_ca_condition().
 */
function uc_recurring_subscription_ca_condition() {
  $condition = array();
  return $conditions;
}

/**
 * Implementation of hook_ca_action().
 */
function uc_recurring_subscription_ca_action() {
  // actions to grant/revoke role on user account.
  $actions['uc_recurring_subscription_grant_role'] = array(
    '#title' => t('Grant a role on an order.'),
    '#category' => t('Role'),
    '#callback' => 'uc_recurring_subscription_action_grant_role',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );
  $actions['uc_recurring_subscription_revoke_role'] = array(
    '#title' => t('Revoke a role on an order.'),
    '#category' => t('Role'),
    '#callback' => 'uc_recurring_subscription_action_revoke_role',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  if (module_exists('og_actions')) {
    // actions to subscribe/unsubscribe user from organic group.
    $actions['uc_recurring_subscription_grant_og'] = array(
      '#title' => t('Grant organic group membership on order.'),
      '#category' => t('Organic Group'),
      '#callback' => 'uc_recurring_subscription_action_grant_og',
      '#arguments' => array(
        'order' => array(
          '#entity' => 'uc_order',
          '#title' => t('Order'),
        ),
      ),
    );
    $actions['uc_recurring_subscription_revoke_og'] = array(
      '#title' => t('Revoke organic group membership on order.'),
      '#category' => t('Organic Group'),
      '#callback' => 'uc_recurring_subscription_action_revoke_og',
      '#arguments' => array(
        'order' => array(
          '#entity' => 'uc_order',
          '#title' => t('Order'),
        ),
      ),
    );
  }

  return $actions;
}

/**
 * Implementation of hook_ca_trigger().
 */
function uc_recurring_subscription_ca_trigger() {
  // use the existing uc_recurring triggers
  $triggers = array();
  return $triggers;
}

/**
 * Implementation of hook_ca_predicate().
 */
function uc_recurring_subscription_ca_predicate() {
  $predicates = array();

  // Grant user roles to user when checkout is complete.
  $predicates['uc_recurring_subscription_grant_roles'] = array(
    '#title' => t('Grant user roles on subscription created.'),
    '#trigger' => 'uc_order_status_update',
    '#class' => 'roles',
    '#status' => 1,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If order status is completes.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'negate' => FALSE,
            'order_status' => 'completed',
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_subscription_grant_role',
        '#title' => t('Grant user role(s).'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'role_option' => 'subscribe_grant',
          'role' => array(),
        ),
      ),
    ),
  );

  // Grant user roles to user when subscription expires.
  $predicates['uc_recurring_subscription_expired_grant_roles'] = array(
    '#title' => t('Grant user roles on subscription expiration.'),
    '#trigger' => 'uc_recurring_renewal_expired',
    '#class' => 'roles',
    '#status' => 1,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_subscription_grant_role',
        '#title' => t('Grant user role(s).'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'role_option' => 'expire_grant',
          'role' => array(),
        ),
      ),
    ),
  );

  // Revoke user roles to user when subscription expires.
  $predicates['uc_recurring_subscription_expired_revoke_roles'] = array(
    '#title' => t('Revoke user roles on subscription expiration.'),
    '#trigger' => 'uc_recurring_renewal_expired',
    '#class' => 'roles',
    '#status' => 1,
    '#weight' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_recurring_subscription_revoke_role',
        '#title' => t('Revoke user role(s).'),
        '#argument_map' => array(
          'order' => 'order',
        ),
        '#settings' => array(
          'role_option' => 'expire_revoke',
          'role' => array(),
        ),
      ),
    ),
  );

  // Grant organic groups to user when checkout is complete.
  if (module_exists('og_actions')) {
    $predicates['uc_recurring_subscription_grant_og'] = array(
      '#title' => t('Grant organic groups on subscription created.'),
      '#trigger' => 'uc_order_status_update',
      '#class' => 'og',
      '#status' => 1,
      '#weight' => 1,
      '#conditions' => array(
        '#operator' => 'AND',
        '#conditions' => array(
          array(
            '#name' => 'uc_order_status_condition',
            '#title' => t('If order status is completes.'),
            '#argument_map' => array(
              'order' => 'updated_order',
            ),
            '#settings' => array(
              'negate' => FALSE,
              'order_status' => 'completed',
            ),
          ),
        ),
      ),
      '#actions' => array(
        array(
          '#name' => 'uc_recurring_subscription_grant_og',
          '#title' => t('Grant groups on subscribe.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'group_option' => 'subscribe_grant_og',
            'groups' => array(),
          ),
        )
      ),
    );

    // Grant organic groups to user when subscription expires.
    $predicates['uc_recurring_subscription_expired_grant_og'] = array(
      '#title' => t('Grant organic groups on subscription expiration.'),
      '#trigger' => 'uc_recurring_renewal_expired',
      '#class' => 'og',
      '#status' => 1,
      '#weight' => 1,
      '#conditions' => array(
        '#operator' => 'AND',
        '#conditions' => array(
        ),
      ),
      '#actions' => array(
        array(
          '#name' => 'uc_recurring_subscription_grant_og',
          '#title' => t('Grant groups on expire.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'group_option' => '',
            'groups' => array(),
          ),
        )
      ),
    );

    // Revoke user roles to user when subscription expires.
    $predicates['uc_recurring_subscription_expired_revoke_og'] = array(
      '#title' => t('Revoke organic groups on subscription expiration.'),
      '#trigger' => 'uc_recurring_renewal_expired',
      '#class' => 'og',
      '#status' => 1,
      '#weight' => 1,
      '#conditions' => array(
        '#operator' => 'AND',
        '#conditions' => array(
        ),
      ),
      '#actions' => array(
        array(
          '#name' => 'uc_recurring_subscription_revoke_og',
          '#title' => t('Revoke groups on expire.'),
          '#argument_map' => array(
            'order' => 'order',
          ),
          '#settings' => array(
            'group_option' => 'expire_revoke_og',
            'groups' => array(),
          ),
        )
      ),
    );
  };

  return $predicates;
}

/**
 * Grant a role.
 */
function uc_recurring_subscription_action_grant_role($order, $settings) {
  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  $account = user_load($order->uid);

  $roles = user_roles(TRUE);
  if ($settings['role_option'] == 'custom') {
    $account->roles += $settings['role'];
    watchdog('uc_recurring', 'Granted !role role to !user', array('!role' => $roles[$rid], '!user' => $account->name));
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      if (!empty($subscription->access[$settings['role_option']])) {
        $account->roles += $subscription->access[$settings['role_option']];
      }
    }
  }
  user_save($account, array('roles' => $account->roles));
}

/**
 * Grant role CA form.
 */
function uc_recurring_subscription_action_grant_role_form($form_state, $settings = array()) {
  drupal_add_css(drupal_get_path('module', 'uc_recurring_subscription') . '/uc_recurring_subscription.css');
  $form['role_option'] = array(
    '#type' => 'radios',
    '#title' => t('Where are the role(s) defined?'),
    '#attributes' => array('class' => 'ca-role-select-option'),
    '#options' => array(
      'subscribe_grant' => t('Apply the roles set by the Subscription Manager on creation.'),
      'expire_grant' => t('Apply the roles set by the Subscription Manager on expiration.'),
      'expire_revoke' => t('Revoke the roles set by the Subscription Manager on expiration.'),
      'custom' => t('Custom selected role (select from below)'),
    ),
    '#default_value' => $settings['role_option'],
  );

  $roles = user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  if (empty($roles)) {
    $form['role_error'] = array(
      '#value' => t('There are no roles available to select. <a href="!url">Add a role</a>.', array('!url' => url('admin/user/roles'))),
    );
  }
  else {
    $form['role'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Custom roles'),
      '#attributes' => array('class' => 'ca-role-option'),
      '#options' => $roles,
      '#default_value' => $settings['role'],
    );
  }
  return $form;
}

/**
 * Revoke user role(s).
 */
function uc_recurring_subscription_action_revoke_role($order, $settings) {
  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  $account = user_load($order->uid);

  $roles = user_roles(TRUE);
  if ($settings['role_option'] == 'custom') {
    foreach ($settings['role'] as $rid => $role) {
      unset($account->roles[$rid]);
      watchdog('uc_recurring', 'Revoked !role role from !user', array('!role' => $roles[$rid], '!user' => $account->name));
    }
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      if (!empty($subscription->access[$settings['role_option']])) {
        foreach ($subscription->access[$settings['role_option']] as $rid => $role) {
          // @todo: we need to check other subscriptions to ensure that the role
          // is no longer required by another subscription.
          unset($account->roles[$rid]);
        }
      }
    }
  }
  user_save($account, array('roles' => $account->roles));

  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  // TODO Convert "user_load" to "user_load_multiple" if "$order->uid" is other than a uid.
  // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
  // Example: array_shift(user_load_multiple(array(), $order->uid))
  $account = user_load($order->uid);
}

/**
 * The revoke form is the same as the grant role form.
 */
function uc_recurring_subscription_action_revoke_role_form($form_state, $settings = array()) {
  return uc_recurring_subscription_action_grant_role_form($form_state, $settings);
}

/**
 * Grant Organic Group CA form.
 */
function uc_recurring_subscription_action_grant_og_form($form_state, $settings = array()) {
  drupal_add_css(drupal_get_path('module', 'uc_recurring_subscription') .'/uc_recurring_subscription.css');
  $form['group_option'] = array(
    '#type' => 'radios',
    '#title' => t('Where are the groups(s) defined?'),
    '#attributes' => array('class' => 'ca-role-select-option'),
    '#options' => array(
      'subscribe_grant_og' => t('Apply the groups set by the Subscription Manager on creation.'),
      'expire_grant_og' => t('Apply the groups set by the Subscription Manager on expiration.'),
      'expire_revoke_og' => t('Revoke the groups set by the Subscription Manager on expiration.'),
      'custom' => t('Custom selected groups (select from below)'),
    ),
    '#default_value' => $settings['group_option'],
  );
  $form += og_subscribe_user_action_form($settings);
  $form['groups']['#required'] = FALSE;
  $form['groups']['#description'] = t('Hold CTRL key and left mouse click to select multiple groups.');
  return $form;
}

/**
 * Revoke Organic Group CA form - same as the grant role form
 */
function uc_recurring_subscription_action_revoke_og_form($form_state, $settings = array()) {
  return uc_recurring_subscription_action_grant_og_form($form_state, $settings);
}

/**
 * Grant user organic groups.
 */
function uc_recurring_subscription_action_grant_og($order, $settings) {
  // Load user.
  if (!$order->uid || !($account = user_load($order->uid))) {
    return;
  }
  if ($settings['group_option'] == 'custom' && is_array($settings['groups']) && !empty($settings['groups'])) {
    og_subscribe_user_action($account, $settings['groups']);
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      $context['groups'] = $subscription->access[$settings['group_option']];
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
function uc_recurring_subscription_action_revoke_og($order, $settings) {
  // Load user.
  if (!$order->uid || !($account = user_load($order->uid))) {
    return;
  }
  if ($settings['group_option'] == 'custom' && is_array($settings['groups']) && !empty($settings['groups'])) {
    og_unsubscribe_user_action($account, $settings['groups']);
  }
  else {
    foreach ($order->products as $pid => $product) {
      $subscription = uc_recurring_subscription_load($product->nid);
      $context['groups'] = $subscription->access[$settings['group_option']];
      if (is_array($context['groups']) && !empty($context['groups'])) {
        og_unsubscribe_user_action($account, $context);
        $comment = t('User access revoked for group(s): %group.', array('%group' => implode(',', $context['groups'])));
        uc_order_comment_save($order->order_id, $account->uid, $comment);
      }
    }
  }
}
