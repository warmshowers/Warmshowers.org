<?php

/**
 * @file
 * Rules configurations for uc_usps module.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function uc_usps_default_rules_configuration_alter(&$configs) {
  if (!isset($configs['get_quote_from_usps'])) {
    return;
  }

  // Domestic areas include U.S., American Samoa, Guam, Puerto Rico, and the Virgin Islands
  $countries = array(
    16 => t('American Samoa'),
    316 => t('Guam'),
    630 => t('Puerto Rico'),
    840 => t('United States'),
    850 => t('Virgin Islands (US)'),
  );

  $domestic = rules_or();
  $domestic->label = t('Order delivers to one of');
  $domestic_env = clone $domestic;

  $foreign = rules_or();
  $foreign->negate();
  $foreign->label = t('NOT Order delivers to one of');
  $foreign_env = clone $foreign;

  foreach ($countries as $country => $name) {
    $condition = rules_condition('data_is', array('data:select' => 'order:delivery-address:country', 'value' => $country));
    $condition->label = $name;

    $domestic->condition(clone $condition);
    $domestic_env->condition(clone $condition);
    $foreign->condition(clone $condition);
    $foreign_env->condition($condition);
  }


  $configs['get_quote_from_usps']->condition($domestic);
  $configs['get_quote_from_usps_env']->condition($domestic_env);
  $configs['get_quote_from_usps_intl']->condition($foreign);
  $configs['get_quote_from_usps_intl_env']->condition($foreign_env);
}
