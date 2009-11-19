<?php
// $Id: draggableviews_handler_flag_weights.inc,v 1.1.2.3 2009/05/18 10:28:46 lyricnz Exp $

/**
* @file
* Flag weights implementation for draggableviews.
*/

/*
* Implementation using the Flag Weight module.
*/
class draggableviews_handler_flag_weights {

  function init($field_name, &$view) {
    // Check that the handler is being used on the right type of field.
    if ($view->field[$field_name]->table != 'flag_content') {
      drupal_set_message("This handler cannot be applied to this field - use Weight!");
    }

    // TODO: much more checking - relationship, types, fields etc, ***CURRENT_USER***

    $this->field = $view->field[$field_name];
    $relationship = $view->relationship['flag_content_rel'];
    $this->flag_id = $relationship->definition['extra'][0]['value'];
    $this->flag = flag_get_flag($relationship->options['flag']);

    $this->view       = $view;
    $this->pager      = $view->pager;
  }

  function save($nid, $value) {
    global $user; // assume $extra['uid] = '***CURRENT_USER***'
    // For global flags, use uid 0
    $uid = $this->flag->global ? 0 : $user->uid;

error_log("flag_weights_set_weight($this->flag_id, 'node', $nid, $uid, $value)");
    flag_weights_set_weight($this->flag_id, 'node', $nid, $uid, $value);
  }

  function get_form_element($value, $attributes = array()) {
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
  }

  function set_range($start, $end) {
    $this->range_start = $start;
    $this->range_end   = $end;
  }

  function get_minimum_value() {
    return -1;
  }
}
