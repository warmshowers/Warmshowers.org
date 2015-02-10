<?php

/**
 * @file
 * thank you hanoii for providing this patch
 */

/**
 * Field handler to calculate the price out of the combination of attributes
 */
class uc_views_attribute_handler_field_combination_price extends uc_views_handler_field_money_amount {

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
    // Is this the proper way to access vid?
    $this->aliases['vid'] = $this->query->add_field('node', 'vid');
  }

  function render($values) {
    switch ($this->definition['price']) {
      case 'sell_price':
        $uc_products_field = 'sell_price';
        $uc_product_options_field = 'price';
        $field_alias = 'combination_sell_price';
        break;

      case 'cost':
        $uc_products_field = 'cost';
        $uc_product_options_field = 'cost';
        $field_alias = 'combination_cost_price';
        break;
    }
    // I wonder if we can have the price as additional field
    // Because this can be used in very complex queries, I rather take that
    // price out from the db directly here.
    // I did try to add it through
    // additional_fields => array('product_price' => array('table' => 'uc_products', 'field' => 'sell_price'))
    // but it didn't work.
    $price = db_result(db_query("SELECT $uc_products_field FROM {uc_products} WHERE nid = %d AND vid = %d", $values->nid, $values->{$this->aliases['vid']}));
    $offset_total = 0;

    $combination =$values->{$this->aliases['combination']};
    if ($combination) {
      $combination = unserialize($combination);
      foreach ($combination as $aid => $oid) {
        $offset = db_result(db_query("SELECT $uc_product_options_field FROM {uc_product_options} WHERE nid = %d AND oid = %d", $values->nid, $oid));
        $offset_total += $offset;
      }
    }

    // Setting this internal variables so that the parent render works.
    $this->field_alias = $field_alias;
    $values->{$this->field_alias} = $price + $offset_total;
    return parent::render($values);
  }
}
