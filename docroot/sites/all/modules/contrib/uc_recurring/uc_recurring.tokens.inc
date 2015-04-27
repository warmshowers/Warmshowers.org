<?php

/**
 * @file
 * Token related functions for the Ubercart recurring payments module.
 */

/**
 * Implements hook_token_info().
 */
function uc_recurring_token_info() {
  $types = array();
  $tokens = array();

  $types['uc_recurring_fee'] = array(
    'name' => t('Recurring fees'),
    'description' => t('Tokens related to Ubercart Recurring fees.'),
    'needs-data' => 'uc_recurring_fee',
  );

  $tokens['uc_recurring_fee'] = array(
    'recurring-fee-id' => array(
      'name' => t('Recurring fee ID'),
      'description' => t('The recurring fee ID.'),
    ),
    'next-charge' => array(
      'name' => t('Next charge'),
      'description' => t('The date and time when the next charge is due to be processed.'),
      'type' => 'date',
    ),
    'fee-amount' => array(
      'name' => t('Fee amount'),
      'description' => t('The recurring fee amount due on the next charge.'),
    ),
    'fee-title' => array(
      'name' => t('Fee title'),
      'description' => t('The product title used as orders for this recurring fee.'),
    ),
    'charged-intervals' => array(
      'name' => t('Charged intervals'),
      'description' => t('The number of recurring intervals that have been charged so far.'),
    ),
    'remaining-intervals' => array(
      'name' => t('Remaining intervals'),
      'description' => t('The number of recurring intervals due left in subscription.'),
    ),
    'renewal-attempts' => array(
      'name' => t('Renewal attempts'),
      'description' => t('The number of attempts to try and renew this fee.'),
    ),
    'link' => array(
      'name' => t('Link'),
      'description' => t('A link to the recurring fee using the recurring fee ID as the text.'),
    ),
    'url' => array(
      'name' => t('URL'),
      'description' => t('The URL to the recurring fee.'),
      'type' => 'url',
    ),
  );

  return array('types' => $types, 'tokens' => $tokens);
}

/**
 * Implements hook_tokens().
 */
function uc_recurring_tokens($type, $tokens, $data = array(), $options = array()) {
  $language_code = NULL;
  if (isset($options['language'])) {
    $language_code = $options['language']->language;
  }
  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'uc_recurring_fee' && !empty($data['uc_recurring_fee'])) {
    $fee = $data['uc_recurring_fee'];
    $path = 'user/' . $fee->uid . '/recurring-fees';

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'recurring-fee-id':
          $replacements[$original] = $fee->rfid;
          break;

        case 'next-charge':
          $replacements[$original] = format_date($fee->next_charge);
          break;

        case 'fee-amount':
          $replacements[$original] = uc_currency_format($fee->fee_amount);
          break;

        case 'fee-title':
          $replacements[$original] = $sanitize ? check_plain($fee->fee_title) : $fee->fee_title;
          break;

        case 'charged-intervals':
          $replacements[$original] = $fee->charged_intervals;
          break;

        case 'remaining-intervals':
          $replacements[$original] = $fee->remaining_intervals < 0 ? t('Until cancelled') : $fee->remaining_intervals;
          break;

        case 'renewal-attempts':
          $replacements[$original] = $fee->attempts;
          break;

        case 'link':
          $replacements[$original] = l($fee->rfid, $path, array('absolute' => TRUE));
          break;

        case 'url':
          $replacements[$original] = url($path, array('absolute' => TRUE));
          break;
      }
    }

    // Handles chaining for tokens that have 'type' defined in hook_token_info()
    if ($url_tokens = token_find_with_prefix($tokens, 'url')) {
      $replacements += token_generate('url', $url_tokens, array('path' => $path), $options);
    }
    if ($next_charge_tokens = token_find_with_prefix($tokens, 'next-charge')) {
      $replacements += token_generate('date', $next_charge_tokens, array('date' => $fee->next_charge), $options);
    }
  }

  return $replacements;
}
