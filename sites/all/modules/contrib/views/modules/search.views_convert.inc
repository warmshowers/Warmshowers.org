<?php
// $Id: search.views_convert.inc,v 1.1 2009/04/07 23:24:55 merlinofchaos Exp $

/**
 * @file
 * Field conversion for fields handled by this module.
 */

function search_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($type) {
    case 'filter':
      switch ($field['tablename']) {
        case 'temp_search_results':
          switch ($field['field']) {
            case 'word':
              $item = $view->get_item($display, 'filter', $id);
              $item['table'] = 'search_index';
              $item['field'] = 'keys';
              $item['value'] = $field['value'];
              $view->set_item($display, 'filter', $id, $item);
              break;
          }
          break;
      }
      break;
  }
}
