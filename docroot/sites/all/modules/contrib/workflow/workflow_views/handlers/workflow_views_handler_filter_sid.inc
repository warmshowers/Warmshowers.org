<?php
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
    if (isset($this->value_options)) {
      return;
    }

    if (!isset($this->value_options)) {
      // Show the possible State options.
      $this->value_options = array();
      $this->value_title = t('Workflow state');

      $all = (bool) $this->options['expose']['workflow_include_all'];
      $wid = isset($this->options['expose']['workflow_reduce_wid']) ? $this->options['expose']['workflow_reduce_wid'] : 0;

      $states = array();
      // Count the workflows to determine grouping.
      // Even if $wid is not set, we may only have 1 workflow.
      $grouped = FALSE;
      $workflows = workflow_load_multiple($wid ? array($wid) : FALSE);
      $count = count($workflows);
      if ($count > 1) {
        $states += array('' => t('No state'));
        $states += array('ANY' => t('A state'));
        $grouped = TRUE;
      }

      $states += workflow_get_workflow_state_names($wid, $grouped, $all);
      $this->value_options = $states;
    }
    return $this->value_options;
  }

  function expose_options() {
    parent::expose_options();
    $this->options['expose']['workflow_reduce_wid'] = '';
    $this->options['expose']['workflow_include_all'] = FALSE;
  }

  function expose_form(&$form, &$form_state) {

    $workflows[''] = t('- Select a value -');
    foreach (workflow_load_multiple() as $workflow) {
      $workflows[$workflow->wid] = $workflow->label();
    }

    $form['expose']['workflow_reduce_wid'] = array(
      '#type' => 'select',
      '#title' => t('Workflows'),
      '#options' => $workflows,
      '#description' => t('Select which workflow the states are given from.'),
      '#default_value' => $this->options['expose']['workflow_reduce_wid'],
    );
    $form['expose']['workflow_include_all'] = array(
      '#type' => 'checkbox',
      '#title' => "Include '(creation)' and inactive states?",
      '#default_value' => $this->options['expose']['workflow_include_all'],
    );
    parent::expose_form($form, $form_state);
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['expose']['contains']['workflow_reduce_wid'] = array('default' => '');
    $options['expose']['contains']['workflow_include_all'] = array('default' => FALSE, 'bool' => TRUE);

    return $options;
  }

  function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
    $info = $this->operators();
    $this->get_value_options();
    if (!is_array($this->value)) {
      return;
    }
    $operator = check_plain($info[$this->operator]['short']);
    $values = '';
    if (in_array($this->operator, $this->operator_values(1))) {
      // !!! here unlike views_handler_filter_in_operator class.
      $options_sids = array();
      foreach ($this->value_options as $key => $value) {
        if (is_array($value)) {
          foreach ($value as $k => $v) {
            $options_sids[$k] = $v;
          }
        }
        else {
          $options_sids[$key] = $value;
        }
      }

      // Remove every element which is not known.
      foreach ($this->value as $value) {
        if (!isset($options_sids[$value])) {// !!! Unlike views_handler_filter_in_operator class.
          unset($this->value[$value]);
        }
      }
      // Choose different kind of ouput for 0, a single and multiple values.
      if (count($this->value) == 0) {
        $values = t('Unknown');
      }
      elseif (count($this->value) == 1) {
        // If any, use the 'single' short name of the operator instead.
        if (isset($info[$this->operator]['short_single'])) {
          $operator = check_plain($info[$this->operator]['short_single']);
        }

        $keys = $this->value;
        $value = array_shift($keys);
        if (isset($options_sids[$value])) {// !!! Unlike views_handler_filter_in_operator class.
          $values = check_plain($options_sids[$value]);
        }
        else {
          $values = '';
        }
      }
      else {
        foreach ($this->value as $value) {
          if ($values !== '') {
            $values .= ', ';
          }
          if (drupal_strlen($values) > 8) {
            $values .= '...';
            break;
          }
          if (isset($options_sids[$value])) {// !!! Unlike views_handler_filter_in_operator class.
            $values .= check_plain($options_sids[$value]);
          }
        }
      }
    }

    return $operator . (($values !== '') ? ' ' . $values : '');
  }

  function query() {
    $value = $this->is_a_group() && !$this->options['expose']['multiple'] ? drupal_array_merge_deep_array($this->value) : $this->value;
    if (empty($value)) {
      return;
    }

    $this->ensure_my_table();
    if (count($value) == 1) {
      if (current($value) == '') {
        $value = NULL;
        $this->operator = ($this->operator == 'in') ? 'IS NULL' : 'IS NOT NULL';
      }
      elseif (current($value) == 'ANY') {
        $value = NULL;
        $this->operator = ($this->operator == 'in') ? 'IS NOT NULL' : 'IS NULL';
      }
      else {
        $this->operator = ($this->operator == 'in') ? '= ' : '!= ';
      }
    }
    if ($this->operator == 'empty') {
      $value = NULL;
      $this->operator = 'IS NULL';
    }
    elseif ($this->operator == 'not empty') {
      $value = NULL;
      $this->operator = 'IS NOT NULL';
    }

    $this->query->add_where($this->options['group'], $this->table_alias . '.' . $this->real_field, $value, $this->operator);
  }
}
