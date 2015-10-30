<?php

/**
 * @file
 * Credit card data views field handler.
 */

/**
 * Field handler: displays credit card data.
 */
class uc_order_handler_field_order_cc_data extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    // Initialize the encryption key and class.
    $key = uc_credit_encryption_key();
    $crypt = new UbercartEncryption();
    $data = unserialize($values->{$this->field_alias});
    if (isset($data['cc_data'])) {
      $cc_data = $crypt->decrypt($key, $data['cc_data']);
      if (strpos($cc_data, ':') === FALSE) {
        $cc_data = base64_decode($cc_data);
      }
      $cc_data = unserialize($cc_data);

      if (isset($cc_data[$this->definition['cc field']])) {
        return $cc_data[$this->definition['cc field']];
      }
    }
  }

}
