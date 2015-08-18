<?php
/**
 * @file
 * Configures views for use within Rules.
 */

/**
 * Rules display plugin.
 */
class views_rules_plugin_display_rules extends views_plugin_display implements views_rules_iterator {
  /**
   * Defines options for configuring the display with Rules.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['rules_parameter'] = array('default' => array());
    $options['rules_variables'] = array('default' => array());

    return $options;
  }

  /**
   * Displays Rules configuration summary.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    if ($this->uses_fields() || $entity_info = entity_get_info($this->view->base_table)) {
      // Add Rules category.
      $categories['rules'] = array(
        'title' => t('Rules settings'),
        'column' => 'second',
        'build' => array(
          '#weight' => -10,
        ),
      );

      // Add 'parameter' and 'provides' options.
      if ($this->get_handlers('argument')) {
        $options['rules_parameter'] = array(
          'category' => 'rules',
          'title' => t('Parameters'),
          'value' => t('edit contextual filter info'),
        );
      }
      $options['rules_variables'] = array(
        'category' => 'rules',
        'title' => t('Row variables'),
        'value' => isset($entity_info) ? $entity_info['label'] : t('edit field info'),
      );
    }
  }

  /**
   * Builds display options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'rules_parameter':
        $form['#tree'] = TRUE;
        $this->parameter_option_form($form, $form_state);
        break;

      case 'rules_variables':
        $form['#tree'] = TRUE;
        $this->variables_option_form($form, $form_state);
        break;
    }
  }

  /**
   * Validates submitted option values.
   */
  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    $errors = array();
    if (isset($form_state['values']['options'])) {
      switch ($form_state['section']) {
        case 'rules_parameter':
          $errors = $this->parameter_options_validate($form_state['values']['options']);
          break;

        case 'rules_variables':
          $errors = $this->variables_options_validate($form_state['values']['options']);
          break;
      }
    }

    foreach ($errors as $error) {
      if (isset($error['id'])) {
        $element = &$form[$error['id']];
        if (isset($error['element'])) {
          $element = &$element[$error['element']];
        }
        form_error($element, $error['message']);
      }
      else {
        form_error($form, $error['message']);
      }
    }
  }

  /**
   * Consolidates submitted option values.
   */
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    switch ($form_state['section']) {
      case 'rules_parameter':
      case 'rules_variables':
        $options = isset($form_state['values']['options']) ? $form_state['values']['options'] : array();
        $this->set_option($form_state['section'], $options);
        break;
    }
  }

  /**
   * Validates display options.
   */
  function validate() {
    $errors = parent::validate();

    $options = $this->get_option('rules_parameter');
    if ($this->parameter_options_validate($options)) {
      $errors[] = t('Parameters in display "@display" are not correctly configured.', array('@display' => $this->display->display_title));
    }

    $options = $this->get_option('rules_variables');
    if ($this->variables_options_validate($options)) {
      $errors[] = t('Row variables in display "@display" are not correctly configured.', array('@display' => $this->display->display_title));
    }

    return $errors;
  }

  /**
   * Gets a list of argument labels.
   */
  function get_argument_labels() {
    $options = array();
    foreach ($this->get_handlers('relationship') as $relationship => $handler) {
      $relationships[$relationship] = $handler->label();
    }

    foreach ($this->get_handlers('argument') as $id => $handler) {
      $options[$id] = $handler->ui_name();
      if (!empty($handler->options['relationship']) && !empty($relationships[$handler->options['relationship']])) {
        $options[$id] = '(' . $relationships[$handler->options['relationship']] . ') ' . $options[$id];
      }
    }
    return $options;
  }

  /**
   * Builds parameter option form.
   */
  function parameter_option_form(&$form, &$form_state) {
    $form['#title'] = t('Rules: parameters');
    $form['help'] = array(
      '#prefix' => '<p>',
      '#markup' => t('Configure the variable info for each contextual filter as they would be used as parameters in Rules.'),
      '#suffix' => '</p>',
    );

    // Build variable forms.
    $option_parameter = (array) $this->get_option('rules_parameter');
    foreach ($this->get_argument_labels() as $variable => $label) {
      $option_parameter += array($variable => array());
      $form[$variable] = $this->get_parameter_form($variable, $label, $option_parameter[$variable]);
    }
  }

  /**
   * Builds row variables option form.
   */
  function variables_option_form(&$form, &$form_state) {
    $form['#title'] = t('Rules: row variables');

    // Add configuration for fields.
    if ($this->uses_fields()) {
      $form['help'] = array(
        '#prefix' => '<p>',
        '#markup' => t('Configure the variable info for each field as they would be used in Rules.'),
        '#suffix' => '</p>',
      );

      // Build variable forms.
      $option_variables = (array) $this->get_option('rules_variables');
      foreach ($this->get_field_labels() as $variable => $label) {
        $option_variables += array($variable => array());
        $form[$variable] = $this->get_variable_form($variable, $label, $option_variables[$variable], TRUE);
      }
    }
    // Show notice for default entity type.
    elseif ($entity_info = entity_get_info($entity_type = $this->view->base_table)) {
      $form['notice'] = array(
        '#markup' => '<p>' . t('The row style does not use fields. The loop item variable will be the entity variable.') . '</p>',
      );
      $form['variable'] = array(
        '#prefix' => '<p>' . t('Variable details:') . '</p>',
        '#markup' => '<dl>' .
          '<dt>' . t('Type') . '</dt>' .
          '<dd>' . $entity_info['label'] . '</dd>' .
          '<dt>' . t('Label') . '</dt>' .
          '<dd>' . $entity_info['label'] . '</dd>' .
          '<dt>' . t('Name') . '</dt>' .
          '<dd>' . $entity_type . '</dd>' .
          '</dl>',
      );
    }
  }

  /**
   * Builds the form element for a single parameter.
   */
  function get_parameter_form($name, $label, $info, $optional = FALSE) {
    $form_options = array(
      'optional' => $optional,
    );
    return $this->build_variable_form($name, $label, $info, $form_options);
  }

  /**
   * Builds the form element for a single variable.
   */
  function get_variable_form($name, $label, $info, $optional = FALSE) {
    $items = views_rules_data_types(array('entity' => TRUE));
    $type_options = views_rules_data_type_options($items);
    $form_options = array(
      'optional' => $optional,
      'type_options' => $type_options,
      'rendered' => TRUE,
    );
    return $this->build_variable_form($name, $label, $info, $form_options);
  }

  /**
   * Builds the form element for a single variable.
   */
  function build_variable_form($name, $label, $info, $options = array()) {
    $options += array(
      'optional' => FALSE,
      'type_options' => NULL,
      'rendered' => FALSE,
    );

    $info += array(
      'type' => NULL,
      'label' => $label,
      'name' => $name,
      'enabled' => 1,
      'rendered' => 0,
    );

    $form = array(
      '#type' => 'fieldset',
      '#title' => check_plain($label),
      '#tree' => TRUE,
    );
    if (!$options['optional']) {
      $form['enabled'] = array(
        '#type' => 'value',
        '#value' => 1,
      );
    }
    else {
      $enabled_css_id = drupal_html_id('views-rules-variable-enabled');
      $form['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enabled'),
        '#default_value' => $info['enabled'],
        '#description' => t('Uncheck this box to make this variable unavailable for use in Rules.'),
        '#attributes' => array('id' => $enabled_css_id),
      );
    }
    $states = !isset($enabled_css_id) ? NULL : array(
      'visible' => array(
        '#' . $enabled_css_id => array('checked' => TRUE),
      ),
    );
    if ($options['rendered']) {
      $form['rendered'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use rendered result'),
        '#default_value' => $info['rendered'],
        '#description' => t('Check to use rendered value (e.g. rewritten) instead of the raw value. Note that a rendered field may contain markup but is not affected by the field\'s "Style settings".'),
        '#states' => $states,
      );
    }
    $form['_pre_wrap'] = array(
      '#markup' => '<div class="clearfix">',
    );
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Data type'),
      '#options' => is_array($options['type_options']) ? $options['type_options'] : views_rules_data_type_options(),
      '#empty_value' => '',
      '#default_value' => $info['type'],
      '#required' => empty($options['optional']),
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#states' => $states,
    );
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#default_value' => $info['label'],
      '#size' => 25,
      '#required' => empty($options['optional']),
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#states' => $states,
    );
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#default_value' => $info['name'],
      '#size' => 25,
      '#required' => empty($options['optional']),
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#states' => $states,
    );
    $form['_post_wrap'] = array(
      '#markup' => '</div>',
    );

    return $form;
  }

  /**
   * Extracts options configured as enabled.
   */
  function extract_enabled_options($options) {
    $enabled_options = array();
    foreach ($options as $id => $option) {
      if (!empty($option['enabled'])) {
        $enabled_options[$id] = $option;
      }
    }
    return $enabled_options;
  }

  /**
   * Validates parameter options.
   */
  function parameter_options_validate($options) {
    $errors = array();

    if ($missing = array_diff_key($this->get_argument_labels(), $options)) {
      foreach ($missing as $id => $label) {
        $errors[] = array(
          'id' => $id,
          'message' => t('The %var contextual filter is not configured.', array('%var' => $label)),
        );
      }
    }
    $errors = array_merge($errors, $this->validate_machine_name($options));
    $errors = array_merge($errors, $this->validate_unique_names($options));

    return $errors;
  }

  /**
   * Validates row variable options.
   */
  function variables_options_validate($options) {
    $errors = array();

    if ($this->uses_fields()) {
      if ($missing = array_diff_key($this->get_field_labels(), $options)) {
        foreach ($missing as $id => $label) {
          $errors[] = array(
            'id' => $id,
            'message' => t('The %var field is not configured.', array('%var' => $label)),
          );
        }
      }
      $options = $this->extract_enabled_options($options);
      $errors = array_merge($errors, $this->validate_machine_name($options));
      // Send parameter names to unique names validation to make sure variable
      // names do not conflict with parameter names.
      $parameter_names = array();
      foreach (((array) $this->get_option('rules_parameter')) as $parameter) {
        $parameter_names[] = $parameter['name'];
      }
      $errors = array_merge($errors, $this->validate_unique_names($options, $parameter_names));
      // Ensure enabled options are fully specified.
      $errors = array_merge($errors, $this->validate_complete_variables($options));
    }

    return $errors;
  }

  /**
   * Validates variable uniqueness.
   */
  function validate_machine_name($options) {
    $errors = array();
    foreach ($options as $id => $info) {
      $name = $info['name'];
      if (!preg_match('!^[a-z0-9_]+$!', $name)) {
        $errors[] = array(
          'id' => $id,
          'element' => 'name',
          'message' => t('The machine-readable name %name contains invalid characters (valid characters are lowercase letters, numbers, and underscores).'),
        );
      }
    }
    return $errors;
  }

  /**
   * Validates variable uniqueness.
   */
  function validate_unique_names($options, $additional = array()) {
    $errors = array();
    $names = drupal_map_assoc($additional);
    foreach ($options as $id => $info) {
      $name = $info['name'];
      if (isset($names[$name])) {
        $errors[] = array(
          'id' => $id,
          'element' => 'name',
          'message' => t('The machine-readable name %name is already taken.', array('%name' => $name)),
        );
      }
      elseif ($name == 'views_rules_display') {
        $errors[] = array(
          'id' => $id,
          'element' => 'name',
          'message' => t('The machine-readable name %name is reserved for internal use.', array('%name' => $name)),
        );
      }
      else {
        $names[$name] = $name;
      }
    }
    return $errors;
  }

  /**
   * Validates variable info completeness.
   */
  function validate_complete_variables($options, $check_enabled = FALSE) {
    $errors = array();
    $labels = $this->get_field_labels();
    foreach ($options as $id => $info) {
      if (!$check_enabled || !empty($info['enabled'])) {
        foreach (array('type', 'label', 'name') as $element) {
          if (!isset($info[$element]) || $info[$element] === '') {
            $errors[] = array(
              'id' => $id,
              'element' => $element,
              'message' => t('The variable @element for %var is missing.', array(
                '@element' => $element,
                '%var' => $labels[$id],
              )),
            );
          }
        }
      }
    }
    return $errors;
  }

  /**
   * Gets parameter info for Rules.
   */
  function get_rules_parameter_info() {
    return $this->get_processed_rules_parameter_info();
  }

  function get_processed_rules_parameter_info($view_key = FALSE) {
    $option_parameter = (array) $this->get_option('rules_parameter');
    $option_parameter = $this->extract_enabled_options($option_parameter);
    $option_parameter = $this->variable_array_map_keys($option_parameter, array_keys($this->get_argument_labels()));
    return $this->get_rules_info_from_option($option_parameter, $view_key);
  }

  /**
   * Gets row variable info for Rules.
   */
  function get_rules_variable_info() {
    return $this->get_processed_rules_variable_info();
  }

  function get_processed_rules_variable_info($view_key = FALSE) {
    // Return configured field variables.
    if ($this->uses_fields()) {
      $option_variables = (array) $this->get_option('rules_variables');
      $option_variables = $this->extract_enabled_options($option_variables);
      $option_variables = $this->variable_array_map_keys($option_variables, array_keys($this->get_field_labels()));
      return $this->get_rules_info_from_option($option_variables, $view_key);
    }
    // Return row variable.
    elseif ($entity_info = entity_get_info($entity_type = $this->view->base_table)) {
      $info = array(
        $entity_type => array(
          'type' => $entity_type,
          'label' => $entity_info['label'],
        ),
      );
      return $info;
    }

    // Return no variable otherwise.
    return array();
  }

  /**
   * Extracts values from an array using an ordered list of keys.
   */
  function variable_array_map_keys($array, $keys) {
    $return = array();
    foreach ($keys as $key) {
      if (array_key_exists($key, $array)) {
        $return[$key] = $array[$key];
      }
    }
    return $return;
  }

  /**
   * Gets processed Rules info from an option value.
   */
  function get_rules_info_from_option($option, $view_key = FALSE) {
    $info = array();
    foreach ($option as $var => $var_info) {
      $var_info += array(
        'type' => NULL,
        'name' => NULL,
        'label' => NULL,
      );
      $info[$var_info['name']] = array(
        'type' => $var_info['type'],
        'label' => $var_info['label'],
      );
      if ($view_key) {
        $info[$var_info['name']]['view key'] = $var;
      }
    }
    return $info;
  }

  /**
   * Executes the iterator display.
   *
   * @param array $arguments
   * @param ViewsRulesIterable $iterable
   * @throws views_rules_iterator_exception
   *   If an error occurred while executing the view.
   */
  function execute_iterator($arguments, $iterable) {
    // Prepares the view.
    $this->view->set_display($this->display->id);
    $this->view->pre_execute($arguments);

    // Iterate view result rows.
    $this->iterate_rows($iterable);

    $this->view->post_execute();
  }

  /**
   * @param ViewsRulesIterable $iterable
   * @throws views_rules_iterator_exception
   *   If an error occurred while executing the view.
   */
  function iterate_rows($iterable) {
    // Execute view.
    $this->view->execute($this->view->current_display);
    // Check display has not failed.
    if (!empty($this->build_info['fail'])) {
      throw new views_rules_iterator_exception('Failed to build view display.');
    }
    if (!empty($this->view->build_info['denied'])) {
      throw new views_rules_iterator_exception('Access to view display is denied.');
    }
    // Iterate through results.
    if ($variable_info = $this->get_processed_rules_variable_info(TRUE)) {
      $view_variable_info = (array) $this->get_option('rules_variables');
      foreach ($this->view->result as $row_index => $row) {
        // Build row data.
        $data = array();
        if ($this->uses_fields()) {
          foreach ($variable_info as $var_name => $info) {
            $option_info = $view_variable_info[$info['view key']];
            if (empty($option_info['rendered'])) {
              $data[$var_name] = $this->view->style_plugin->get_field_value($row_index, $info['view key']);
            }
            else {
              $data[$var_name] = $this->view->style_plugin->get_field($row_index, $info['view key']);
            }
          }
        }
        else {
          $data[key($variable_info)] = $row->{$this->view->base_field};
        }
        // Evaluate row.
        $iterable->evaluateRow($data);
      }
    }
  }
}

/**
 * Interface for a Views iterator.
 */
interface views_rules_iterator {
  /**
   * Executes the iterator display.
   *
   * @param array $arguments
   * @param ViewsRulesIterable $iterable
   * @throws views_rules_iterator_exception
   *   If an error occurred while executing the view.
   */
  function execute_iterator($arguments, $iterable);

  /**
   * Returns parameter info for use with Rules.
   */
  function get_rules_parameter_info();

  /**
   * Returns row variable info for use with Rules.
   */
  function get_rules_variable_info();
}

/**
 * Exception thrown during view iterator execution.
 */
class views_rules_iterator_exception extends Exception {}
