<?php
/**
 * Filter by attributes - by justindodge (original)
 * Modified to support product (not order) attributes plus inventory by jerry
 */
class uc_views_attribute_handler_filter_product_attr extends views_handler_filter_in_operator {

  function has_extra_options() { return TRUE; }

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'select');

    return $options;
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $aid = explode('_', $this->field);
      $aid = $aid[1];

      $this->value_title = t('Options');
      $result = db_query("SELECT name, oid FROM {uc_attribute_options} WHERE aid = %d ORDER BY ordering", $aid);

      while ($row = db_fetch_object($result)) {
        $options[$row->oid] = $row->name;
      }
      if (count($options) == 0) {
        $options[] = t('No options found.');
      }
      $this->value_options = $options;
    }
  }

  function extra_options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array('select' => t('Checkboxes/Dropdown'), 'textfield' => t('Textfield')),
      '#default_value' => $this->options['type'],
    );
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    if ($this->options['type'] == 'textfield') {
      $form['value'] = array (
        '#type' => 'textfield',
        '#title' => 'Text search',
      );
    }
  }

  function query() {
    $aid = explode('_', $this->field);
    $aid = $aid[1];

    $this->ensure_my_table();
    $this->real_field = 'combination';
    $field = "{$this->table_alias}.{$this->real_field}";
    if ($this->operator == 'not in') {
      $not = 'NOT';
    }
    if(!is_array($this->value)) {
      $this->value = array($this->value);
    }
    $add = FALSE;
    $optval = array();
    foreach ($this->value as &$value) {
    	if($this->options['type'] == 'textfield') {
      	 $optval[] = db_result(db_query("SELECT oid FROM {uc_attribute_options} WHERE UPPER(name) = UPPER('%s')", $value));
    	}
    	else {
    	   $optval[] = $value;
    	}
    	if (!empty($optval)) {
    	  $add = TRUE;
    	}
    	else {
    	  //If we let the query get added, only products with the attribute enabled & with the value of that attribute empty
    	  //will return.  By default, I think it makes more sense to allow any product to return despite whether or not it
    	  //has the attribute if the user put nothing in to search for.
    	  //Adding a new type of filter that merely captures whether or not the attribute exists at all for the product may
    	  //be the best way to capture the alternative.  Or, perhaps a config option could be added to this filter :
    	  //"Show products only if they have this attribute enabled".  If this option is unchecked, a blank value returns all
    	  //products. If checked, a blank value at least filters out products that dont have this attribute available.
    	  continue;
    	}
    }
    if ($add) {
      $match = '';
      foreach ($optval as &$oneopt) {
        $var = array($aid => $oneopt);
        $servar = serialize($var);
        $l = strpos($servar, '{') + 1;
        $r = strpos($servar, '}') - $l;
        $value = substr($servar, $l, $r);
        if (!empty($match)) {
          if ($this->operator == 'not in') {
            $match .= ' AND ';
          }
          else {
            $match .= ' OR ';
          }
        }
        $match .= "($field $not LIKE '%{$value}%')";
      }
    	$this->query->add_where($this->options['group'], $match);
      if (is_null($this->query->get_table_info('uc_product_stock'))) {
        $stock_alias = $this->query->ensure_table('uc_product_stock');
        $this->query->add_where($this->options['group'], "{$stock_alias}.sku = {$this->table_alias}.model");
        $this->query->add_where($this->options['group'], "({$stock_alias}.active = 0) OR ({$stock_alias}.stock > 0)");
      }
    }
  }
}
