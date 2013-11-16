<?php
// $Id: draggableviews_handler_cck.inc,v 1.1.2.6 2009/03/09 21:26:41 sevi Exp $

/**
 * @file
 * The default implementation for draggableviews.
 */

/*
 * Default Implementation with CCK-fields
 */
class draggableviews_handler_cck {
  function init($field_name, &$view) {
    $this->view            = $view;
    $this->pager           = $view->pager;

    $this->field_name      = $field_name;

    $this->options         = $view->display['default']->display_options['style_options'];
    $this->fields          = $view->field;
    $this->field           = $this->fields[$field_name];

    $this->field_alias     = $this->fields[$field_name]->field_alias;

    $this->real_field_name = $this->fields[$field_name]->content_field['field_name'];
    $this->field_type      = $this->fields[$field_name]->content_field['type'];

    $this->allowed_values  = content_allowed_values($this->field->content_field);
  }

  function save($nid, $value) {

    $node = node_load(array('nid' => $nid));
    // get field
    if (isset($node->{$this->real_field_name})) {
      $field = &$node->{$this->real_field_name};
    }
    else {
      // return if field does not exist
      return;
    }

    if (!isset($value)) {
      // sometimes there is no value available (e.g. a root without a parent)

      // set empty array
      $value = 0;
    }

    //differ between certain field types
    switch ($this->field_type) {
      case 'nodereference':
        $field[0]['nid'] = $value;
        break;

      default:
      case 'number_integer':
        $field[0]['value'] = $value;
        break;
    }

    // If we didn't set $node->date then node_submit() would set it to the current timestamp.
    $node->date = format_date($node->created, 'custom', 'Y-m-d H:i:s O');

    // finally save the node
    if ($node = node_submit($node)) {
      node_save($node);
    }
  }

  function get_form_element($value, $attributes = array()) {
    if (!isset($attributes['class'])) $attributes['class'] = $field['field_name'];

    switch ($this->field->content_field['widget']['type']) {
      case 'optionwidgets_select':
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

      case 'hidden':
      default:
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
