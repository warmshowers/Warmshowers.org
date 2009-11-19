<?php

class content_negotiation_filter_handler extends views_handler_filter {
  function query() {
    $table_alias = $this->query->ensure_table('node');
    $where = i18n_db_rewrite_where($table_alias, 'node');
    $this->query->add_where($this->options['group'], $where);
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['operator']['default'] = '';
    $options['value']['default'] = '';
    return $options;
  }
}
