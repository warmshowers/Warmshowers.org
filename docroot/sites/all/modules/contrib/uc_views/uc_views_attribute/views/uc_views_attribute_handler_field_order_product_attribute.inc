<?php

/**
 * Field handler to provide a human-readable version of the selected combination of attributes
 */
class uc_views_attribute_handler_field_order_product_attribute extends views_handler_field {

  /**
   * Defines a few default options for the combination field
   */
  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function render($values) {
    $data = unserialize($values->{$this->field_alias});
    $result = "";
    if (is_array($data['attributes'])) {
      $result = '';
      $rows = array();
      foreach ($data['attributes'] as $attribute => $option) {
        $rows[] = t('@attribute: @option', array('@attribute' => $attribute, '@option' => implode(', ', (array)$option)));
        if (count($rows)) {
          $result = theme('item_list', $rows, NULL, 'ul', array('class' => 'product-description'));
        }
      }
    }
    return $result;
  }
}
