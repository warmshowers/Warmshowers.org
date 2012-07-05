<?php


/**
 * @file
 * The Rules Views Executor rule set views row plugin.
 *
 * This plugin allows you to pick a rule set to render the row into, and then
 * match up the arguments to the rule set and the views fields.
 *
 */

class rules_views_executor_plugin_row_ruleset extends views_plugin_row {

  /**
   * Render a row object. This usually passes through to a theme template
   * of some form, but not always.
   */
  function render($row) {
    // We need to render the row:
    $output = parent::render($row);

    // Load the ruleset:
    $ruleset = rules_get_rule_sets($this->options['ruleset']);
    $arguments = isset($ruleset['arguments']) ? $ruleset['arguments'] : array();
    $arguments_values = array();

    // Load the arguments to the rule set:
    foreach ($arguments as $arg_id => $argument) {
      $field_id = $this->options['arguments'][$arg_id];
      $arguments_values[$arg_id] = $output[$field_id]->content;
    }

    // Are we being previewed?
    if (isset($this->view->live_preview) && $this->view->live_preview) {
      // Show something sensible when live previewing:
      $output = t('I would have rendered the following row: <pre>!row</pre> into the rule set: !rule', array(
        '!row' => print_r($arguments_values, TRUE),
        '!rule' => print_r($ruleset['label'], TRUE),
      ));
    }
    // Schedule or execute the rule set.
    else {
      $username = $this->display->handler->get_username();
      $permissions = $this->display->handler->get_permissions();

      // Do we have jobqueue around for scheduling:
      if (function_exists('job_queue_add')) {
        job_queue_add('rules_views_executor_execute_ruleset', t('Scheduled execution of ruleset: @ruleset', array('@ruleset' => $ruleset['label'])), array($this->options['ruleset'], $arguments_values, $username), '', TRUE);


      }
      // Otherwise execute the rule:
      else {
        rules_views_executor_execute_ruleset($this->options['ruleset'], $arguments_values, $username);
      }

      $output = '';
    }

    return $output;
  }


  function option_definition() {
    $options = parent::option_definition();

    $options['ruleset'] = array('default' => '');
    $options['arguments'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $sets = rules_get_configured_items('rule_sets');
    $options = array();
    $all_rules_arguments = array();

    foreach ($sets as $set_id => $set) {
      $options[$set_id] = $set['label'];
      if (isset($set['arguments'])) {
        foreach ($set['arguments'] as $arg_id => $argument) {
          $all_rules_arguments[$set_id . ':' . $arg_id] = $argument;
        }
      }

    }

    $form['ruleset'] = array(
      '#type' => 'select',
      '#default_value' => $this->options['ruleset'],
      '#title' => t('Rule Set'),
      '#description' => t('Rule Set to render the view into.'),
      '#options' => $options,
    );

    // Get the arguments of the current rule set:
    $ruleset = rules_get_rule_sets($this->options['ruleset']);
    //$arguments = isset($ruleset['arguments']) ? $ruleset['arguments'] : array();

    $fields = array();
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      $fields[$field] = $handler->ui_name();
    }

    $form['arguments'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rule set arguments'),
      '#description' => t('Choose which fields should be rendered into the rule set arguments'),
    );

    foreach ($all_rules_arguments as $rules_id_arg_id => $argument) {
      list($rules_id, $arg_id) = explode(':', $rules_id_arg_id, 2);

        $form['arguments'][$rules_id][$arg_id] = array(
          '#type' => 'select',
          '#title' => t('@label (type: %type)', array('@label' => $argument['label'], '%type' => $argument['type'])),
          '#options' => $fields,
          '#default_value' => isset($this->options['arguments'][$arg_id]) ? $this->options['arguments'][$arg_id] : '',
          '#dependency' => array('edit-row-options-ruleset' => array($rules_id)),
          '#process' => array('views_process_dependency'),
        );

    }

  }

  function options_submit($form, &$form_state) {
    // Remove the array structure from the arguments collection:
    $arguments = isset($form_state['values']['row_options']['arguments'][$form_state['values']['row_options']['ruleset']]) ? $form_state['values']['row_options']['arguments'][$form_state['values']['row_options']['ruleset']] : array();

    $form_state['values']['row_options']['arguments'] = $arguments;

    parent::options_submit($form, $form_state);
  }

  function _is_rules_argument_identifiable($type) {
    static $types =  array();

    if (!isset($types[$type])) {
      $rules_data_type = rules_get_data_types($type);
      $rules_data_type_class = new $rules_data_type['class'];
      $rules_data_type_class->construct($type);

      $types[$type] = $rules_data_type_class->is_identifiable();
    }

    return $types[$type];
  }

  function _is_rules_argument_formable($type) {
    static $types =  array();

    if (!isset($types[$type])) {
      $rules_data_type = rules_get_data_types($type);
      $rules_data_type_class = new $rules_data_type['class'];
      $rules_data_type_class->construct($type);
      $types[$type] = $rules_data_type_class->uses_input_form();
    }

    return $types[$type];
  }
}
