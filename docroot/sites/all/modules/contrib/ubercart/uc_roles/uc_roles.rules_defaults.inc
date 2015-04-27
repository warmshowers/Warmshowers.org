<?php

/**
 * @file
 * Default Rules configurations for uc_roles.module.
 */

/**
 * Implements hook_defualt_rules_configuration().
 */
function uc_roles_default_rules_configuration() {
  $configs = array();

  // Renew all the roles on an order when the status matches what's set
  // in the roles admin settings.
  $rule = rules_reaction_rule();
  $rule->label = t('Grant or renew purchased roles');
  $rule->active = TRUE;
  $rule->event('uc_order_status_update');
  $rule->condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'payment_received'))
    ->action('uc_roles_order_renew', array('order:select' => 'updated_order', 'message' => FALSE));
  $configs['uc_role_renewal'] = $rule;

  $order_args = array(
    'order:select' => 'order',
    'expiration:select' => 'expiration',
  );

  $user_args = array(
    'account:select' => 'account',
    'expiration:select' => 'expiration',
  );

  // Notify the user when a role is granted.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a role is granted');
  $rule->active = TRUE;
  $rule->event('uc_roles_notify_grant');
  $rule->action('uc_roles_order_email', array(
    'order:select' => 'order',
    'expiration:select' => 'expiration',
    'from' => uc_store_email_from(),
    'addresses' => '[order:email]',
    'subject' => uc_get_message('uc_roles_grant_subject'),
    'message' => uc_get_message('uc_roles_grant_message'),
    'format' => filter_default_format(),
  ));
  $configs['uc_role_notify_grant'] = $rule;

  // Notify the user when a role is revoked.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a role is revoked');
  $rule->active = TRUE;
  $rule->event('uc_roles_notify_revoke');
  $rule->action('uc_roles_user_email', array(
    'account:select' => 'account',
    'expiration:select' => 'expiration',
    'from' => uc_store_email_from(),
    'addresses' => '[account:mail]',
    'subject' => uc_get_message('uc_roles_revoke_subject'),
    'message' => uc_get_message('uc_roles_revoke_message'),
    'format' => filter_default_format(),
  ));
  $configs['uc_role_notify_revoke'] = $rule;

  // Notify the user when a role is renewed.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a role is renewed');
  $rule->active = TRUE;
  $rule->event('uc_roles_notify_renew');
  $rule->action('uc_roles_order_email', array(
    'order:select' => 'order',
    'expiration:select' => 'expiration',
    'from' => uc_store_email_from(),
    'addresses' => '[order:email]',
    'subject' => uc_get_message('uc_roles_renew_subject'),
    'message' => uc_get_message('uc_roles_renew_message'),
    'format' => filter_default_format(),
  ));
  $configs['uc_role_notify_renew'] = $rule;

  // Notify the user when a role is about to expire.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a role is about to expire');
  $rule->active = TRUE;
  $rule->event('uc_roles_notify_reminder');
  $rule->action('uc_roles_user_email', array(
    'account:select' => 'account',
    'expiration:select' => 'expiration',
    'from' => uc_store_email_from(),
    'addresses' => '[account:mail]',
    'subject' => uc_get_message('uc_roles_reminder_subject'),
    'message' => uc_get_message('uc_roles_reminder_message'),
    'format' => filter_default_format(),
  ));
  $configs['uc_role_notify_reminder'] = $rule;

  return $configs;
}
