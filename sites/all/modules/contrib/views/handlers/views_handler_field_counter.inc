<?php
// $Id: views_handler_field_counter.inc,v 1.1 2009/06/02 20:18:26 merlinofchaos Exp $

class views_handler_field_counter extends views_handler_field {
  function query() {
    // do nothing -- to override the parent query.
  }

  function render($values) {
    $pager = $this->view->pager;
    // Get the base count of the pager.
    if ($pager['use_pager']) {
      $count = ($pager['items_per_page'] * $pager['current_page']);
      $count += $pager['offset'];
    }
    // Add the counter for the current site.
    $count += array_search($values, $this->view->result) + 1;

     return $count;
  }
}
