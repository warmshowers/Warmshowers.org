<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * * Implements hook_default_rules_configuration().
 */
function uc_recurring_subscription_default_rules_configuration() {
  $configs = array();

  // Grant user roles to user when checkout is complete.
  $rule = rules_reaction_rule();
  $rule->label = t('Grant user roles on subscription created.');
  $rule->active = TRUE;
  $rule->weight = 1;
  $rule->event('uc_order_status_update')
    // This is the entity way of doing things
    //->condition('data_is', array('data:select' => 'order:order-status', 'value' => 'completed'))
    // This is the traditional way of doing things
    ->condition('uc_order_condition_order_state', array('order:select' => 'order', 'order_state' => 'completed'))
    ->action('uc_recurring_subscription_grant_role', array(
      'order:select' => 'order',
      'role_option' => 'subscribe_grant',
      'roles' => array(),
    ));
  $configs['uc_recurring_subscription_grant_roles'] = $rule;

  // Grant user roles to user when subscription expires.
  $rule = rules_reaction_rule();
  $rule->label = t('Grant user roles on subscription created.');
  $rule->active = TRUE;
  $rule->weight = 1;
  $rule->event('uc_recurring_renewal_expired')
    ->action('uc_recurring_subscription_grant_role', array(
      'order:select' => 'order',
      'role_option' => 'expire_grant',
      'roles' => array(),
    ));
  $configs['uc_recurring_subscription_expired_grant_roles'] = $rule;

  // Revoke user roles to user when subscription expires.
  $rule = rules_reaction_rule();
  $rule->label = t('Revoke user roles on subscription expired.');
  $rule->active = TRUE;
  $rule->weight = 1;
  $rule->event('uc_recurring_renewal_expired')
    ->action('uc_recurring_subscription_revoke_role', array(
      'order:select' => 'order',
      'role_option' => 'expire_revoke',
      'roles' => array(),
    ));
  $configs['uc_recurring_subscription_expired_revoke_roles'] = $rule;
  
  // Revoke user roles to user when subscription is cancelled
  $rule = rules_reaction_rule();
  $rule->label = t('Revoke user roles on subscription cancelled. --xomby');
  $rule->active = TRUE;
  $rule->weight = 1;
  $rule->event('uc_recurring_cancel')
    // This is the entity way of doing things
    //->condition('data_is', array('data:select' => 'order:order-status', 'value' => 'completed'))
    // This is the traditional way of doing things
    //->condition('uc_order_condition_order_state', array('order:select' => 'order', 'order_state' => 'cancelled'))
    ->action('uc_recurring_subscription_revoke_role', array(
      'order:select' => 'order',
      'role_option' => 'expire_revoke',
      'roles' => array(),
    ));
  $configs['uc_recurring_subscription_cancelled_revoke_roles'] = $rule;
  if (module_exists('og')) {
    // Grant organic groups to user when checkout is complete.
    $rule = rules_reaction_rule();
    $rule->label = t('Grant organic groups on subscription created.');
    $rule->active = TRUE;
    $rule->weight = 1;
    $rule->event('uc_order_status_update')
      // This is the entity way of doing things
      //->condition('data_is', array('data:select' => 'updated-order:order-status', 'value' => 'completed'))
      // This is the traditional way of doing things
      ->condition('uc_order_condition_order_state', array('order:select' => 'updated-order', 'order_state' => 'completed'))
      ->action('uc_recurring_subscription_grant_og', array(
        'order:select' => 'order',
        'group_option' => 'subscribe_grant_og',
        'group' => NULL,
      ));
    $configs['uc_recurring_subscription_grant_og'] = $rule;

    // Grant organic groups to user when subscription expires.
    $rule = rules_reaction_rule();
    $rule->label = t('Grant organic groups on subscription created.');
    $rule->active = TRUE;
    $rule->weight = 1;
    $rule->event('uc_recurring_renewal_expired')
      ->action('uc_recurring_subscription_grant_og', array(
        'order:select' => 'order',
        'group_option' => 'expire_grant_og', // @@@ Was '' in CA -- this rule was broken in 6?
        'group' => NULL,
      ));
    $configs['uc_recurring_subscription_expired_grant_og'] = $rule;

    // Revoke organic groups to user when subscription expires.
    $rule = rules_reaction_rule();
    $rule->label = t('Revoke organic groups on subscription expiration.');
    $rule->active = TRUE;
    $rule->weight = 1;
    $rule->event('uc_recurring_renewal_expired')
      ->action('uc_recurring_subscription_revoke_og', array(
        'order:select' => 'order',
        'role_option' => 'expire_revoke_og',
        'role' => NULL,
      ));
    $configs['uc_recurring_subscription_expired_revoke_og'] = $rule;
  }
  return $configs;
}
