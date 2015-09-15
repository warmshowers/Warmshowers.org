<?php

/**
 * @file
 * Rules definitions.
 */

/**
 * Implements hook_rules_event_info().
 * Invoked when a new recurring product fee is created.
 */
function uc_recurring_product_rules_event_info() {
  return array(
    'uc_recurring_product_renewal_created' => array(
      'label' => t('Recurring product fee created'),
      'group' => t('Recurring Fees'),
      'variables' => array(
        'order' => array(
          'type' => 'uc_order',
          'label' => t('Order'),
        ),
        'recurring_fee' => array(
          'type' => 'uc_recurring_fee',
          'label' => t('Recurring Fee'),
        ),
      ),
    ),
  );
}
