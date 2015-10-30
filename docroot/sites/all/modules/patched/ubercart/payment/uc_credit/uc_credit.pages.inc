<?php

/**
 * @file
 * Credit menu items.
 */

/**
 * Prints the contents of the CVV information popup window.
 */
function uc_credit_cvv_info() {
  $output = "<!DOCTYPE html>\n";
  $output .= '<html><head><meta charset="utf-8" /></head><body>';

  $output .= '<b>' . t('What is the CVV?') . '</b><p>' . t('CVV stands for Card Verification Value. This number is used as a security feature to protect you from credit card fraud.  Finding the number on your card is a very simple process.  Just follow the directions below.') . '</p>';
  $cc_types = array(
    'visa' => t('Visa'),
    'mastercard' => t('MasterCard'),
    'discover' => t('Discover')
  );
  foreach ($cc_types as $id => $type) {
    if (variable_get('uc_credit_' . $id, TRUE)) {
      $valid_types[] = $type;
    }
  }
  if (count($valid_types) > 0) {
    $output .= '<br /><b>' . implode(', ', $valid_types) . ':</b><p>';
    $output .= theme('image', array(
      'path' => drupal_get_path('module', 'uc_credit') . '/images/visa_cvv.jpg',
      'attributes' => array('align' => 'left'),
    ));
    $output .= t('The CVV for these cards is found on the back side of the card.  It is only the last three digits on the far right of the signature panel box.');
    $output .= '</p>';
  }

  if (variable_get('uc_credit_amex', TRUE)) {
    $output .= '<br /><p><b>' . t('American Express') . ':</b><p>';
    $output .= theme('image', array(
      'path' => drupal_get_path('module', 'uc_credit') . '/images/amex_cvv.jpg',
      'attributes' => array('align' => 'left'),
    ));
    $output .= t('The CVV on American Express cards is found on the front of the card.  It is a four digit number printed in smaller text on the right side above the credit card number.');
    $output .= '</p>';
  }

  $output .= '<p align="right"><input type="button" onclick="self.close();" value="' . t('Close this window') . '" /></p>';

  $output .= '</body></html>';

  print $output;
  exit();
}
