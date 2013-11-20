<?php
// $Id: draggableviews_handler_flag_weights.inc,v 1.1.2.7 2010/05/11 03:31:09 lyricnz Exp $

/**
* @file
* Flag weights implementation for draggableviews.
*/

/*
* Implementation using the Flag Weight module.
*/
class draggableviews_handler_flag_weights extends draggableviews_handler {
  function init($field_name, &$view) {
    parent::init($field_name, $view);

    // Check that the handler is being used on the right type of field.
    if ($view->field[$field_name]->table != 'flag_content') {
      drupal_set_message("This handler cannot be applied to this field - use Weight!");
    }

    // TODO: much more checking - relationship, types, fields etc, ***CURRENT_USER***

    $this->field = $view->field[$field_name];
    $relationship = $view->relationship['flag_content_rel'];
    $this->flag_id = $relationship->definition['extra'][0]['value'];
    $this->flag = flag_get_flag($relationship->options['flag']);
  }

  function save($nid, $value) {
    global $user; // assume $extra['uid] = '***CURRENT_USER***'
    // For global flags, use uid 0
    $uid = $this->flag->global ? 0 : $user->uid;

    flag_weights_set_weight($this->flag_id, $this->flag->content_type, $nid, $uid, $value);
  }
}
