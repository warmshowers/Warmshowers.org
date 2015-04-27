<?php
/**
 * @file
 * Provide views field handler for workflow.module.
 */

/**
 * Field handler to provide simple status name or renderer.
 */
class workflow_views_handler_field_sid extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['value'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['value'] = array(
      '#title' => t('Display value'),
      '#description' => t('Determines how the state will be displayed.'),
      '#type' => 'select',
      '#options' => array(
        '0' => t('State name'),
        '1' => t('State value'), // Keep this value for backwards compatibility.
        'count' => t('Count number of entities with this state'),
      ),
      '#default_value' => $this->options['value'],
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    $sid = $values->{$this->field_alias};
    if ($this->options['value'] == '1') {
      return (empty($sid)) ? NULL : $sid;
    }
    elseif ($this->options['value'] == 'count') {
      $state = workflow_state_load_single($sid);
      return (empty($sid)) ? 0 : $state->count();
    }
    else {
      return workflow_get_sid_label($sid);
    }
  }
}
