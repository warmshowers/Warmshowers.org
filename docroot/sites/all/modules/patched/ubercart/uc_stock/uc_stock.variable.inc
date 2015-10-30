<?php

/**
 * @file
 * Variable module hook implementations.
 */


/**
 * Implements hook_variable_group_info().
 */
function uc_stock_variable_group_info() {
  $groups['uc_stock'] = array(
    'title' => t('Ubercart stock settings'),
    'access' => 'administer store',
    'path' => array('admin/store/settings/stock'),
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function uc_stock_variable_info($options) {
  $variables['uc_stock_threshold_notification_message'] = array(
    'type' => 'text',
    'title' => t('Message text', array(), $options),
    'description' => t('The message the user receives when the stock level reaches its threshold value.', array(), $options),
    'group' => 'uc_stock',
    'default' => t('[store:name]: Stock threshold limit reached', array(), $options),
  );
  $variables['uc_stock_threshold_notification_subject'] = array(
    'type' => 'text',
    'title' => t('Message subject', array(), $options),
    'description' => t('The subject line of the message the user receives when the stock level reaches its threshold value.', array(), $options),
    'group' => 'uc_stock',
    'default' => t('This message has been sent to let you know that the stock level for "[node:title]" with SKU [uc_stock:model] has reached [uc_stock:level]. There may not be enough units in stock to fulfill order #[uc_order:link].', array(), $options),
  );
  return $variables;
}
