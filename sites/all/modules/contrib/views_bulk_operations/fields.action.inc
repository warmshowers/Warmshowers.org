<?php
// $Id: fields.action.inc,v 1.1.2.22 2010/02/03 01:58:45 kratib Exp $
/**
 * @file Drupal action to set individual field values. 
 * 
 * Heavily "inspired" by fago's work in CCK on 'Populate a field' action (cck/includes/content.rules.inc).
 */

function views_bulk_operations_fields_action_info() {
  if (!module_exists('content')) return array();
  return array('views_bulk_operations_fields_action' => array(
    'type' => 'node',
    'description' => t('Modify node fields'),
    'configurable' => TRUE,
    'behavior' => array('changes_node_property'),
  ));
}

function views_bulk_operations_fields_action_theme() {
  return array(
    'views_bulk_operations_fields_action_form' => array(
      'arguments' => array('form' => NULL),
    ),
  );
}

function views_bulk_operations_fields_action_form($context) {
  // This action form uses static-time settings. If they were not set, pull the defaults now.
  if (!isset($context['settings'])) $context['settings'] = views_bulk_operations_fields_action_settings_form_options();
  $form['#settings'] = $context['settings'];
  $form['#theme'] = 'views_bulk_operations_fields_action_form';

  module_load_include('inc', 'content', 'includes/content.node_form');
  $form_state = array();
  $node = array();
  $weight = -100;
  if (isset($context['selection'])) {
    $result = db_query("SELECT DISTINCT type FROM {node} WHERE nid IN (%s)", implode(',', array_keys($context['selection'])));
  }
  else {
    $result = db_query("SELECT type from {node_type}");
  }
  $fields = array();
  while ($type = db_result($result)) {
    $type_info = content_types($type);
    $fields += $type_info['fields'];
//    $fields += _views_bulk_operations_fields_action_non_cck($type);
  }

  if (empty($fields)) {
    form_set_error('', t('The selected nodes do not have any editable fields. Please select other nodes and try again.'));
    return array();
  }

  foreach ($fields as $field) {
    if (module_exists('content_permissions') && in_array('edit ' . $field['field_name'], module_invoke('content_permissions', 'perm')) && !user_access('edit ' . $field['field_name'])) continue;
    if (!empty($context['settings']['display_fields']) && !in_array($field['field_name'], $context['settings']['display_fields'])) continue;

    $field['required'] = FALSE;
    $node[$field['field_name']] = NULL;

    // The field info and widget.
    if (isset($field['field_form'])) { // is it our hacked definition?
      $form += (array)$field['field_form'];
    }
    else { // no, it's CCK
      $form += (array)content_field_form($form, $form_state, $field);
    }
    if (!isset($form[$field['field_name']])) continue;
    $form['#field_info'][$field['field_name']] = $field;
    
    // Adjust some settings on the field itself.
    $form[$field['field_name']]['#weight'] = $weight++;
    $form[$field['field_name']]['#prefix'] = '<div class="fields-action-togglable">' . @$form[$field['field_name']]['#prefix'];
    $form[$field['field_name']]['#suffix'] = @$form[$field['field_name']]['#suffix'] . '</div>';

    // Checkbox to enable/disable this field.
    $form[$field['field_name'] . '_check'] = array('#type' => 'checkbox', '#attributes' => array('class' => 'fields-action-toggler'));

    // Default value.
    if (@$context[$field['field_name'] . '_check']) {
      $form[$field['field_name']]['#default_value'] = $context[$field['field_name']];
      $form[$field['field_name'] . '_check']['#default_value'] = TRUE;
    }

    // PHP code to program the value.
    if (user_access('Use PHP input for field settings (dangerous - grant with care)') && $context['settings']['php_code']) {
      if (isset($field['field_form'])) {
        $sample = t('return value;');
      }
      else {
        $db_info = content_database_info($field);
        $columns = array_keys($db_info['columns']);
        foreach ($columns as $key => $column) {
          $columns[$key] = t("'@column' => value for @column", array('@column' => $column));
        }
        $sample = t("return array(\n".
                    "  0 => array(\n    @columns\n  ),\n".
                    "  // You'll usually want to stop here. Provide more values\n".
                    "  // if you want your 'default value' to be multi-valued:\n".
                    "  1 => array(\n    @columns\n  ),\n".
                    "  2 => ...\n);", 
                    array('@columns' => implode(",\n    ", $columns))
        );
      }
      $form[$field['field_name'] . '_code'] = array(
        '#type' => 'textarea',
        '#default_value' => '',
        '#rows' => 6,
        '#description' => t('Expected format: <pre>!sample</pre>', array(
          '!sample' => $sample,
        )),
        '#prefix' => '<div class="fields-action-togglable">',
        '#suffix' => '</div>',
      );
    } 
  }

  // Special case for only one field: convert the checkbox into a value that's TRUE by default.
  if (count($form['#field_info']) == 1) {
    $field_name = key($form['#field_info']);
    $form[$field_name . '_check'] = array('#type' => 'value', '#value' => TRUE);
  }

  $form['#node'] = (object)$node;
  return $form;
}

function theme_views_bulk_operations_fields_action_form(&$form) {
  $output = '';
  if (user_access('Use PHP input for field settings (dangerous - grant with care)') && $form['#settings']['php_code']) {
    $output  = t('<h3>Using the Code widget</h4>
                  <ul>
                  <li>Will override the value specified in the Field widget.</li>
                  <li>Should include &lt;?php ?&gt; delimiters.</li>
                  <li>If in doubt, refer to <a target="_blank" href="@link_devel">devel.module</a> \'Dev load\' tab on a content page to figure out the expected format.</li>
                  </ul>', array('@link_devel' => 'http://www.drupal.org/project/devel')
    );
  }

  if (count($form['#field_info']) == 1) {
    $field_name = key($form['#field_info']);
    $header = NULL;
    $row[] = drupal_render($form[$field_name]);
    if (user_access('Use PHP input for field settings (dangerous - grant with care)') && $form['#settings']['php_code']) {
      $header = array(t('Field'), t('Code'));
      $row[] = drupal_render($form[$field_name . '_code']);
    }
    $output .= theme('table', $header, array(array('class' => 'fields-action-row', 'id' => 'fields-action-row' . str_replace('_', '-', $field_name), 'data' => $row)));
  }
  else {
    drupal_add_js(drupal_get_path('module', 'views_bulk_operations') . '/fields.action.js');

    if (user_access('Use PHP input for field settings (dangerous - grant with care)') && $form['#settings']['php_code']) {
      $header = array(theme('table_select_header_cell'), t('Field'), t('Code'));
      foreach ($form['#field_info'] as $field_name => $field) {
        $rows[] = array(
          'class' => 'fields-action-row',
          'id' => 'fields-action-row-' . str_replace('_', '-', $field_name), 
          'data' => array(
            drupal_render($form[$field_name . '_check']),
            drupal_render($form[$field_name]),
            drupal_render($form[$field_name . '_code']),
          ),
        );
      }
    }
    else {
      $header = array(theme('table_select_header_cell'), t('Field'));
      foreach ($form['#field_info'] as $field_name => $field) {
        $rows[] = array(
          'class' => 'fields-action-row',
          'id' => 'fields-action-row-' . str_replace('_', '-', $field_name), 
          'data' => array(
            drupal_render($form[$field_name . '_check']),
            drupal_render($form[$field_name]),
          ),
        );
      }
    }
    $output .= theme('table', $header, $rows); 
  }
  $output .= drupal_render($form);
  return $output;
}

function views_bulk_operations_fields_action_validate($form, $form_state) {
  $field_types = _content_field_types();
  $chosen = 0;
  foreach ($form['#field_info'] as $field_name => $field) {
    if ($form_state['values'][$field_name . '_check'] && isset($field['type'])) {
      $chosen++;
      $function = $field_types[$field['type']]['module'] .'_field';
      if (function_exists($function)) {
        $form['#node'] = (object)array('type' => '', $field_name => $form_state['values'][$field_name]);
        $items = isset($form['#node']->$field_name) ? $form['#node']->$field_name : array();
        $function('validate', $form['#node'], $field, $items, $form, NULL);
        content_field('validate', $form['#node'], $field, $items, $form, NULL);
      }
    }
  }
  if (!$chosen) {
    form_set_error('', t('You must select at least one field to modify.'));
  }
}

function views_bulk_operations_fields_action_submit($form, $form_state) {
  $values = array();
  foreach ($form['#field_info'] as $field_name => $field) {
    $values[$field_name] = $form_state['values'][$field_name];
    $values[$field_name . '_check'] = $form_state['values'][$field_name . '_check'];
    if (isset($form_state['values'][$field_name . '_code'])) {
      $values[$field_name . '_code'] = $form_state['values'][$field_name . '_code'];
    }
  }
  $values['#field_info'] = $form['#field_info'];
  return $values;
}

function views_bulk_operations_fields_action(&$node, $context) {
  foreach ($context['#field_info'] as $field_name => $field) {
    if ($context[$field_name . '_check']) { // use the value for this field
      if (!empty($context[$field_name . '_code'])) {
        $value = eval('?>' . $context[$field_name . '_code']);
      }
      else {
        $value = $context[$field_name];
      }
      $node->$field_name = $value;
      if ($field_name == 'name') { // special case: fix uid when given author name
        if ($account = user_load(array('name' => $node->name))) {
          $node->uid = $account->uid;
        }
        else {
          $node->uid = 0;
        }
      }
    }
  }
}

function _views_bulk_operations_fields_action_non_cck($type) {
  module_load_include('inc', 'node', 'node.pages');
  global $user;
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_id = $type . '_node_form';
  $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');
  $form = drupal_retrieve_form($form_id, $form_state, $node);
  drupal_prepare_form($form_id, $form, $form_state);
  return _views_bulk_operations_fields_action_non_cck_traverse($form, array('title', 'language', 'body', 'format', 'revision', 'log', 'name', 'date', 'status', 'promote', 'sticky', 'comment'));
}

function _views_bulk_operations_fields_action_non_cck_traverse($element, $fields) {
  $return = array();
  foreach (element_children($element) as $field_name) {
    if (empty($element[$field_name]['#type']) || 
        in_array($element[$field_name]['#type'], array('value', 'hidden', 'fieldset')) ||
        empty($element[$field_name]['#title']) ||
        (isset($element[$field_name]['#access']) && !$element[$field_name]['#access'])
    ) {
      $return += _views_bulk_operations_fields_action_non_cck_traverse($element[$field_name], $fields);
    }
    else if (in_array($field_name, $fields, TRUE)) {
      $element[$field_name]['#required'] = FALSE;
      $return[$field_name] = array(
        'field_name' => $field_name,
        'field_form' => array($field_name => $element[$field_name]),
        'widget' => array('label' => $element[$field_name]['#title']),
      );
    }
  }
  return $return;
}

function views_bulk_operations_fields_action_settings_form_options() {
  $options['php_code'] = FALSE;
  $options['display_fields'] = NULL;
  return $options;
}

function views_bulk_operations_fields_action_settings_form($options) {
  $form['php_code'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show PHP code area'),
    '#description' => t('Check this ON if you want to show a textarea for each field to allow the user to write a PHP script that will populate the value of this field.'),
    '#default_value' => $options['php_code'],
  );
  $fields = array();
  foreach (content_fields() as $field) {
    $fields[$field['field_name']] = $field['widget']['label'] .' ('. $field['field_name'] .')';
  }
  /*
  foreach (node_get_types() as $type => $info) {
    foreach (_views_bulk_operations_fields_action_non_cck($type) as $field) {
      if (!isset($fields[$field['field_name']])) {
        $fields[$field['field_name']] = $field['widget']['label'] .' ('. $field['field_name'] .')';
      }
    }
  }
  */
  if (is_null($options['display_fields'])) {
    $options['display_fields'] = array_keys($fields);
  }
  $form['display_fields'] = array(
    '#type' => 'select',
    '#title' => t('Display fields'),
    '#options' => $fields,
    '#multiple' => TRUE,
    '#description' => t('Select which field(s) the action form should present to the user.'),
    '#default_value' => $options['display_fields'],
  );
  return $form;
}

function views_bulk_operations_fields_action_settings_form_validate($form, $form_state) {
  if (empty($form_state['values']['display_fields'])) {
    form_set_error($form_state['values']['_error_element_base'] . 'display_fields', t('You must select at least one field to be shown to the user.'));
  }
}

