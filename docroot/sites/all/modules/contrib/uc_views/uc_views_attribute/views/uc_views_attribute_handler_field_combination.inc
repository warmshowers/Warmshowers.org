<?php

/**
 * @file
 * thank you hanoii for providing this patch
 */

/**
 * Field handler to provide a human-readable version of the stored serialized combination of attributes
 */
class uc_views_attribute_handler_field_combination extends views_handler_field {
 /**
   *
   * @param integer $nid
   *   Node ID
   * @return array
   *   An array where the key is the serialized combination that's not on the
   *   uc_product_adjustments db and the value is an array of
   *   attr_name => attr_option according to the combination
   *
   *   @see uc_product_adjustments_form()
   */
  function _uc_stock_extra_combinations($nid) {
    $combinations = array();
    $query_select = "SELECT DISTINCT";
    $query_from = " FROM";
    $query_where = " WHERE";
    $query_order = " ORDER BY";
    $result = db_query("SELECT pa.nid, pa.aid, pa.ordering, a.name, a.ordering, ao.aid, COUNT(po.oid) FROM {uc_product_attributes} AS pa LEFT JOIN {uc_attributes} AS a ON pa.aid = a.aid LEFT JOIN {uc_attribute_options} AS ao ON a.aid = ao.aid LEFT JOIN {uc_product_options} AS po ON ao.oid = po.oid AND po.nid = %d WHERE pa.nid = %d GROUP BY ao.aid, pa.aid, a.name, pa.ordering, a.ordering, pa.nid HAVING count(po.oid) > 0 ORDER BY pa.ordering, a.ordering", $nid, $nid);
    $i = 1;
    $values = array();

    while ($prod_attr = db_fetch_object($result)) {
      $query_select .= " ao$i.aid AS aid$i, ao$i.name AS name$i, ao$i.oid AS oid$i, po$i.ordering,";
      $query_from .= " ({uc_product_options} AS po$i LEFT JOIN {uc_attribute_options} AS ao$i ON po$i.oid = ao$i.oid AND po$i.nid = %d),";
      $values[] = $nid;
      $query_where .= " ao$i.aid = ". $prod_attr->aid ." AND";
      $query_order .= " po$i.ordering, ao$i.name,";
      ++$i;
      $attribute_ids[] = $prod_attr->aid;
    }
    $num_prod_attr = count($attribute_ids);

    // Remove last connecting parts (commas, "AND")
    $query_select = rtrim($query_select, ',');
    $query_from = rtrim($query_from, ',');
    $query_where = substr($query_where, 0, strlen($query_where) - 4);
    $query_order = rtrim($query_order, ',');

    if ($num_prod_attr) {
      $result = db_query($query_select . $query_from . $query_where . $query_order, $values);
      while ($row = db_fetch_object($result)) {
        $attrs_info = array();
        $attrs = array();
        for ($i = 1 ; $i <= $num_prod_attr ; $i++) {
          $aid_key = "aid$i";
          $oid_key = "oid$i";
          $name_key = "name$i";
          $aid = $row->$aid_key;
          $attr = db_result(db_query("SELECT name FROM {uc_attributes} WHERE aid = %d", $aid));
          $attrs_info[$attr] = $row->$name_key;
          $attrs[$aid] = $row->$oid_key;
        }
        asort($attrs);
        $combinations[serialize($attrs)] = $attrs_info;
      }
    }

    // Exclude configured combinations
    $result = db_query("SELECT * FROM {uc_product_adjustments} WHERE nid = %d", $nid);
    while ($row = db_fetch_object($result)) {
      // TODO: Remove unserialize/asort/serialize because rc6+ already sorts it
      $c = unserialize($row->combination);
      asort($c);
      $c = serialize($c);
      unset($combinations[$c]);
    }

    return $combinations;
  }

  /**
   * Defines a few default options for the combination field
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['fetch_if_null'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Adds form elements for a few options
   */
  function options_form(&$form, &$form_state) {
    // TODO: Add an option for grouping fields
    parent::options_form($form, $form_state);
    $form['fetch_if_null'] = array(
      '#type' => 'checkbox',
      '#title' => t('Fetch attributes if there is no combination'),
      '#description' => t('If checked, and if the combination is not found for a specific SKU/model, get the attributes and options from other tables.'),
      '#default_value' => $this->options['fetch_if_null'],
    );

  }

  function query() {
    // TODO: Don't add field to the query if we want to group them
    return parent::query();
  }

  function pre_render($values) {
    // TODO: Get the grouped values (by nid or by stock depending if used as a relationship or not)
    // If there are no values to render (displaying a summary, or query returned no results),
    // or if this is not a grouped field, do nothing specific.
    if (isset($this->view->build_info['summary']) || empty($values) || !$this->options['group']) {
      return parent::pre_render($values);
    }
  }

  function render($values) {
    $items = array();
    $combination = $values->{$this->field_alias};
    if (is_string($combination)) {
      $combination = unserialize($combination);
      $attributes = array();
      foreach($combination as $aid => $oid) {
        $attribute = uc_attribute_load($aid);
        $option = uc_attribute_option_load($oid);
        $attributes[] = "$attribute->name: $option->name";
      }
      $items[] = implode($attributes, ', ');
      return theme('item_list', $items);
    }
    // combination information is not found, render other node's attribute and options
    else if ($this->options['fetch_if_null']) {
      $nid = $values->nid;
      $combinations = $this->_uc_stock_extra_combinations($nid);
      if (!empty($combinations)) {
        foreach ($combinations as $combination => $attr_info) {
          $attributes = array();
          foreach ($attr_info as $attr => $option) {
            $attributes[] = "$attr: $option";
          }
          $items[] = implode($attributes, ', ');
        }
        return theme('item_list', $items);
      }
    }
    return parent::render($values);
  }
}
