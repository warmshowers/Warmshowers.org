<?php
/**
 * @file
 * Some utility functions for Variable devel.
 */

include_once DRUPAL_ROOT . '/includes/utility.inc';

/**
 * Implements hook_variable_settings_form_alter()
 *
 * Compile and display undefined variables.
 */
function _variable_devel_settings_form_alter(&$form, &$form_state, $form_id) {
  list($variables, $groups) = _variable_devel_scan_form($form);

  if ($variables) {
    $form['variable_devel'] = array(
      '#title' => t('Variable definitions (Variable devel)'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE, '#collapsed' => TRUE,
    );
    foreach ($variables as $name => $definition) {
      $form['variable_devel'][]['#markup'] =  _variable_devel_export_definition($name, $definition);
    }
    foreach ($groups as $name => $definition) {
      $form['variable_devel'][]['#markup'] =  _variable_devel_export_definition($name, $definition, 'group');
    }
  }
}

/**
 * Scan form for variables.
 */
function _variable_devel_scan_form($form, $parent = array()) {
  $variables = $groups = array();
  foreach (element_children($form) as $name) {
    $element = $form[$name];
    $definition = array();
    if (isset($element['#type'])) {
      switch ($element['#type']) {
        case 'fieldset':
          if (!empty($element['#title'])) {
            $groups[$name] = array('title' => $element['#title']);
          }
          list($group_variables, $group_groups) = _variable_devel_scan_form($element, array('group' => $name));
          $variables += $group_variables;
          $groups += $group_groups;
          break;
        case 'textfield':
          $definition['type'] = 'string';
          break;
        case 'textarea':
          $definition['type'] = 'text';
          break;
        case 'text_format':
          $definition['type'] = 'text_format';
          break;
        case 'radio':
        case 'checkbox':
          $definition['type'] = 'boolean';
          break;
        case 'radios':
        case 'checkboxes':
        case 'select':
          $definition['type'] = 'select';
          $definition['options'] = $element['#options'];
          // Handle default for multiple options.
          if (is_array($element['#default_value'])) {
            $default = array_filter($element['#default_value']);
            if (!empty($element['#multiple'])) {
              $definition['default'] = $default;
              $definition['type'] = 'options';
            }
            else {
              $definition['default'] = reset($default);
            }
          }
          break;
      }
      if ($definition) {
        if (isset($element['#title'])) {
          $definition['title'] =  $element['#title'] ;
        }
        // Do not add description, it may have formatted strings.
        if (!isset($definition['default']) && isset($element['#default_value'])) {
          $definition['default'] = $element['#default_value'];
        }
        // Add properties from parent.
        $definition += $parent;
        $variables[$name] = $definition;
      }
    }
  }
  return array($variables, $groups);
}

/**
 * Export variable definition.
 */
function _variable_devel_export_definition($name, $definition, $type = 'variable') {
  $output = '<pre>';
  $output .= '$' . $type . "['$name'] = array(\n";
  foreach ($definition as $property => $value) {
    $output .= "  '" . check_plain($property) . "' => ";
    if ($property == 'title' || $property == 'description') {
      $output .= "t('" . check_plain($value) . "', array(), \$options)";
    }
    elseif (is_array($value)) {
      $output .= drupal_var_export($value, '  ');
    }
    else {
      $output .= is_int($value) ? $value : "'" . check_plain($value) . "'";
    }
    $output .= ",\n";
  }
  $output .= ');</pre>';
  return $output;
}

/**
 * Format definition array.
 */
function _variable_devel_format_array($array) {
  $rows = array();
  foreach ($array as $name => $value) {
    $rows[$name] = array(
      array('data' => check_plain($name)),
      dprint_r($value, TRUE),
    );
  }
  return theme('table', array('rows' => $rows));
}
