<?php

/**
 * @file
 * Rules definitions.
 */



/**
 * Implement hook_rules_data_info().
 */
function uc_recurring_subscription_rules_data_info() {
  $types['uc_recurring_subscription'] = array(
    'label' => t('Ubercart recurring subscription'),
    'wrap' => TRUE,
    'group' => t('Ubercart'),
  );
  
  return $types;
}

/**
 * Implement hook_rules_event_info().
 */
function uc_recurring_subscription_event_info() {
  $events = array();
  return $events;
}

/**
 * Implement hook_rules_condition_info().
 */
function uc_recurring_subscription_condition_info() { 
  $conditions = array();
  return $conditions;
}

/**
 * Implement hook_rules_action_info().
 */
function uc_recurring_subscription_action_info() {
  $actions = array();
  return $actions;
}