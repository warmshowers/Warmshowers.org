<?php

/**
 * @file
 * Views style plugin for the views_watchdog module.
 */


/**
 * Provides style options for watchdog tables.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_watchdog_table extends views_plugin_style_table {

  var $base_table = 'watchdog';
  var $base_field = 'wid';


  function option_definition() {

    $options = parent::option_definition();

    $options['watchdog_table_type'] = array('default' => TRUE);
    $options['watchdog_table_type_class'] = array('default' => 'dblog-[type]');
    $options['watchdog_table_severity'] = array('default' => TRUE);
    $options['watchdog_table_severity_class'] = array('default' => 'dblog-[severity]');

    return $options;
  }


  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    $form['watchdog_table_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add CSS classes to each table row based on type.'),
      '#default_value' => !empty($this->options['watchdog_table_type']),
    );
    $form['watchdog_table_type_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Type class'),
      '#description' => t('The CSS class to apply to the row. Replacement patterns: [type]'),
      '#default_value' => !empty($this->options['watchdog_table_type_class']) ? $this->options['watchdog_table_type_class'] : 'dblog-[type]',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-watchdog-table-type' => array(1),
      ),
    );
    $form['watchdog_table_severity'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add CSS classes to each table row based on severity level.'),
      '#default_value' => !empty($this->options['watchdog_table_severity']),
    );
    $form['watchdog_table_severity_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Severity class'),
      '#description' => t('The CSS class to apply to the row. Replacement patterns: [severity]'),
      '#default_value' => !empty($this->options['watchdog_table_severity_class']) ? $this->options['watchdog_table_severity_class'] : 'dblog-[severity]',
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-watchdog-table-severity' => array(1),
      ),
    );
  }


  function options_validate(&$form, &$form_state) {

    parent::options_validate($form, $form_state);

    $class = str_replace('[type]', '', trim($form_state['values']['style_options']['watchdog_table_type_class']));
    if (preg_match('/[^a-zA-Z0-9-]/', $class)) {
      $field = $form['watchdog_table_type_class'];
      form_error($field, t('CSS class name must be alphanumeric or dashes only.'));
    }
    $class = str_replace('[severity]', '', trim($form_state['values']['style_options']['watchdog_table_severity_class']));
    if (preg_match('/[^a-zA-Z0-9-]/', $class)) {
      $field = $form['watchdog_table_severity_class'];
      form_error($field, t('CSS class name must be alphanumeric or dashes only.'));
    }
  }


  function query() {

    parent::query();

    // Add base field
    if (isset($this->options['relationship']) && isset($this->view->relationship[$this->options['relationship']])) {
      $relationship = $this->view->relationship[$this->options['relationship']];
      $this->field_alias = $this->view->query->add_field($relationship->alias, $this->base_field);
    }
    else {
      $this->field_alias = $this->view->query->add_field($this->base_table, $this->base_field);
    }

    // Add base field as second sort parameter
    $this->view->query->add_orderby($this->base_table, $this->base_field, $this->order);
  }
}
