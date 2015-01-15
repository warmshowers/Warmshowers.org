<?php

/**
 * @file
 * Credit menu items.
 *
 */

// Prints the contents of the CVV information popup window.
function uc_credit_cvv_info() {
  $output = '<b>'. t('What is the CVV?') .'</b><p>'. t('CVV stands for Card Verification Value. This number is used as a security feature to protect you from credit card fraud.  Finding the number on your card is a very simple process.  Just follow the directions below.') .'</p>';
  $cc_types = array('visa', 'mastercard', 'discover');
  foreach ($cc_types as $type) {
    if (variable_get('uc_credit_'. $type, TRUE)) {
      $valid_types[] = ucfirst($type);
    }
  }
  if (count($valid_types) > 0) {
    $output .= '<br /><b>'. implode(', ', $valid_types) .':</b><p><img src="'
             . base_path() . drupal_get_path('module', 'uc_credit')
              .'/images/visa_cvv.jpg" align="left" />'
            . t('The CVV for these cards is found on the back side of the card.  It is only the last three digits on the far right of the signature panel box.') .'</p>';
  }

  if (variable_get('uc_credit_amex', TRUE)) {
    $output .= '<br /><p><b>'. t('American Express') .':</b><p><img src="'
             . base_path() . drupal_get_path('module', 'uc_credit')
              .'/images/amex_cvv.jpg" align="left" />'
             . t('The CVV on American Express cards is found on the front of the card.  It is a four digit number printed in smaller text on the right side above the credit card number.') .'</p>';
  }

  $output .= '<p><div align="right"><input type="button" onclick="self.close();" '
            .'value="'. t('Close this window') .'" /></div></p>';

  print $output;
  exit();
}
