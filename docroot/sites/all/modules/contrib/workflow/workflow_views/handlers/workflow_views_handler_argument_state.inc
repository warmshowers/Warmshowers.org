<?php

/**
 * @file
 * Provide views argument handler for workflow.module.
 */

/**
 * Argument handler to accept a node type.
 */
class views_handler_argument_workflow_state extends views_handler_argument {
  function construct() {
    parent::construct('type');
  }

  /**
   * Overrides the behavior of summary_name().
   *
   * Gets the user-friendly version of the workflow state.
   */
  function summary_name($data) {
    return workflow_get_sid_label($data->{$this->name_alias});
  }

  /**
   * Overrides the behavior of title().
   * Get the user-friendly version of the workflow state.
   */
  function title() {
    return workflow_get_sid_label($this->argument);
  }
}
