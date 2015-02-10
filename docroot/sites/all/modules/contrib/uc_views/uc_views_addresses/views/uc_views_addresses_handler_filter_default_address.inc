<?php
/**
 *
 */
class uc_views_addresses_handler_filter_default_address extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Is users default address');
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . (empty($this->value) ? '<>' : '=') . " (SELECT uad.aid FROM {uc_addresses_defaults} as uad WHERE uad.uid = $this->table_alias.uid)");
  }
}

