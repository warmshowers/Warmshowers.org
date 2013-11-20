<?php

/**
 * Allow taxonomy term ID(s) as argument
 */
class i18nviews_handler_argument_term_node_tid extends views_handler_argument_term_node_tid {
  function title_query() {
    $titles = array();
    $placeholders = implode(', ', array_fill(0, sizeof($this->value), '%d'));

    // ToDo: check vocabulary translation mode (if localization needed)
    $result = db_query("SELECT name, tid FROM {term_data} WHERE tid IN ($placeholders)", $this->value);
    while ($term = db_fetch_object($result)) {
      $titles[] = check_plain(tt('taxonomy:term:'. $term->tid .':name', $term->name));
    }
    return $titles;
  }
}

