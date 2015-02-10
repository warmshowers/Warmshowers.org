<?php

/**
 * @file
 * CC data views field handler
 */

class uc_views_handler_field_order_cc_data extends views_handler_field {
  function render($values) {
    // Initialize the encryption key and class.
    $key = uc_credit_encryption_key();
    $crypt = new uc_encryption_class;
    $data = unserialize($values->{$this->field_alias});
    $cc_data = $data['cc_data'];

    // In recent ubercart versions, they base64_encode() the payment details
    // before encrypting. We can detect encoded data by the lack of colons,
    // due to base64's limited character set.
    // see uc_credit_cache()
    $decrypted_data = $crypt->decrypt($key, $cc_data);
    if (strpos($decrypted_data, ':') === FALSE) {
      $decrypted_data = base64_decode($decrypted_data);
    }
    $cc_data = unserialize($decrypted_data);

    return $cc_data[$this->definition['cc field']];
  }
}