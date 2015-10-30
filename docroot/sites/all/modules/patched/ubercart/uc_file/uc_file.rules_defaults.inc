<?php

/**
 * @file
 * Default Rules configurations for uc_file.module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_file_default_rules_configuration() {
  // Renew all the files on an order when the status matches what's set
  // in the files admin settings.
  $rule = rules_reaction_rule();
  $rule->label = t('Renew purchased files');
  $rule->active = TRUE;
  $rule->event('uc_order_status_update');
  $rule->condition('data_is', array('data:select' => 'updated_order:order-status', 'value' => 'payment_received'))
    ->action('uc_file_order_renew', array(
      'order:select' => 'updated_order',
    ));
  $configs['uc_file_renewal'] = $rule;

  // Notify the user when a file is granted.
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a file is granted');
  $rule->active = TRUE;
  $rule->event('uc_file_notify_grant');
  $rule->action('uc_file_order_email', array(
    'order:select' => 'order',
    'expiration:select' => 'expiration',
    'from' => uc_store_email_from(),
    'addresses' => '[order:email]',
    'subject' => t("File Downloads for Order# [order:order-id]"),
    'message' => t("Your order (order# [order:link]) at [store:name] included file download(s). You may access them with the following link(s):\n\n[expiration:downloads]\n\nAfter downloading these files these links will have expired. If you need to download the files again, you can login at [site:login-link] and visit the \"My Account\" section of the site.\n\nThanks again, \n\n[store:name]\n[site:slogan]"),
    'format' => filter_default_format(),
  ));
  $configs['uc_file_notify_grant_trigger'] = $rule;

  return $configs;
}
