<?php

/**
 * @file
 * Ubercart product list position handler.
 */

class draggableviews_handler_ubercart extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);
  }

  function save($nid, $value) {
    $node = node_load($nid);

    switch ($this->type) {
      case 'order':
        db_query("UPDATE {uc_products} SET ordering = %d WHERE nid = %d AND vid = %d", $value, $node->nid, $node->vid);
        break;
    }
  }
}
