<?php
// $Id: workflow_views_handler_filter_sid.inc,v 1.1 2008/09/18 02:49:25 jvandyk Exp $

/**
 * @file
 * Provide views filter handler for workflow.module.
 */

/**
 * Filter by state.
 */
class workflow_views_handler_filter_sid extends views_handler_filter_in_operator {
  var $value_form_type = 'select';

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Workflow state');

      $workflows = workflow_get_all();
      if (count($workflows) > 1) {
        $states = array('' => t('No state'));
        foreach ($workflows as $wid => $wname) {
          $states[$wname] = workflow_get_states($wid);
        }
      }
      else {
        $states = workflow_get_states();
      }

      $this->value_options = $states;
    }
  }

  function query() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();
    $placeholder = !empty($this->definition['numeric']) ? '%d' : "'%s'";

    if (count($this->value) == 1) {
      $this->operator = ($this->operator == 'in') ? '= ' : '!= ';
      $in = !empty($this->definition['numeric']) ? '%d' : "'%s'";
    }
    else {
      $replace = array_fill(0, sizeof($this->value), $placeholder);
      $in = ' (' . implode(", ", $replace) . ')';
    }
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator . $in, $this->value);
  }
}
