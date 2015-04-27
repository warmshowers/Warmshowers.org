<?php

/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Render a workflow state name.
 */
class workflow_views_handler_field_state extends views_handler_field {

  function render($values) {
    return t($values->{$this->field_alias});
  }

}
