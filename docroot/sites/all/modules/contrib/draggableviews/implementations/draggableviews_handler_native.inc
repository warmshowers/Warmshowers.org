<?php
// $Id: draggableviews_handler_native.inc,v 1.1.2.6 2009/03/08 01:16:33 sevi Exp $

/**
 * @file
 * The default implementation of draggableviews handlers.
 */

/*
 * Default Implementation: storage done by draggableviews
 */
class draggableviews_handler_native {
  var $view;

  function init($field_name, &$view) {
    $this->field_name = $field_name;
    $this->view       = $view;
    $this->pager      = $view->pager;

    $this->delta = $this->view->field[$field_name]->definition['draggableviews_delta'];

    if ($field_name == $view->style_plugin->options['tabledrag_hierarchy']['field']) {
      $this->type = 'hierarchy';
    }
    else {
      $this->type = 'order';
    }
  }

  function save($nid, $value) {
    db_query('DELETE FROM {draggableviews_structure} WHERE vid=%d AND nid=%d AND delta=%d', $this->view->vid, $nid, $this->delta);
    db_query('INSERT INTO {draggableviews_structure}(vid, nid, delta, value) VALUES(%d, %d, %d, %d)', $this->view->vid, $nid, $this->delta, $value);
  }

  function get_form_element($value, $attributes = array()) {
    if (!isset($attributes['class'])) $attributes['class'] = $field['field_name'];

    switch ($this->type) {
      default:
        $options = array();
        for ($i = $this->range_start; $i <= $this->range_end; $i++) $options[$i] = $i;

        return array(
          '#type'  => 'select',
          '#name'  => $attributes['field_name'],
          '#value' => $value,
          '#options' => $options,
          '#attributes' => array('class' => $attributes['class']),
        );
        break;

      case 'hierarchy':
        return array(
          '#type' => 'hidden',
          '#name' => $attributes['field_name'],
          '#value' => $value,
          '#attributes' => array('class' => $attributes['class']),
        );
    }
  }

  function set_range($start, $end) {
    $this->range_start = $start;
    $this->range_end   = $end;
  }

  function get_minimum_value() {
    return -1;
  }
}
