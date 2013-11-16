<?php

/**
 * Field handler for terms.
 */
class i18nviews_handler_field_term_node_tid extends views_handler_field_term_node_tid {
  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }

      $result = db_query("SELECT tn.vid AS node_vid, td.*, v.name as vocabulary FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid INNER JOIN {vocabulary} v ON v.vid = td.vid WHERE tn.vid IN (" . implode(', ', $vids) . ")$voc ORDER BY td.weight, td.name");

      while ($term = db_fetch_object($result)) {
      $name = tt('taxonomy:term:'. $term->tid .':name', $term->name);
        $this->items[$term->node_vid][$term->tid]['name'] = check_plain($name);
        $this->items[$term->node_vid][$term->tid]['tid'] = $term->tid;
        $this->items[$term->node_vid][$term->tid]['vid'] = $term->vid;
        $this->items[$term->node_vid][$term->tid]['vocabulary'] = check_plain($term->vocabulary);
        
        if (!empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid]['make_link'] = TRUE;
          $this->items[$term->node_vid][$term->tid]['path'] = taxonomy_term_path($term);
        }
      }
    }
  }
}

