<?php
// $Id: workflow_views_handler_field_sid.inc,v 1.1 2008/09/18 02:49:25 jvandyk Exp $

/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 */
class workflow_views_handler_field_sid extends views_handler_field {
  function render($values) {
    if (empty($values->{$this->field_alias})) {
      return t('No state');
    }

    static $states;
    if (!isset($states)) {
      $states = workflow_get_states();
    }
    $output = $states[$values->{$this->field_alias}];
    if (empty($output)) {
      $output = t('Unknown state');
    }
    return check_plain($output);
  }
}
