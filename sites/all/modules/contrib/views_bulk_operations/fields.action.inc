<?php
// $Id: fields.action.inc,v 1.1.2.10 2009/07/25 03:51:03 kratib Exp $
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

function views_bulk_operations_fields_action_form($context) {
  module_load_include('inc', 'content', 'includes/content.node_form');
  $form = array();
  $form_state = array();
  $node = array();
  $weight = -100;
  if (isset($context['selection'])) {
    $fields = array();
    $result = db_query("SELECT DISTINCT type FROM {node} WHERE nid IN (%s)", implode(',', $context['selection']));
    while ($type = db_result($result)) {
      $type_info = content_types($type);
      $fields += $type_info['fields'];
      $fields += _views_bulk_operations_fields_action_non_cck($type);
    }
  }
  else {
    $fields = content_fields();
  }
  $form['fields'] = array('#type' => 'value', '#value' => array());
  foreach ($fields as $field) {
    $field['required'] = FALSE;
    $node[$field['field_name']] = NULL;

    // The field info and widget.
    if (isset($field['field_def'])) { // is it our hacked definition?
      $form += (array)$field['field_def'];
    }
    else { // no, it's CCK
      $form += (array)content_field_form($form, $form_state, $field);
    }
    if (!isset($form[$field['field_name']])) { // some field was not rendered
      continue;
    }
    $form['#field_info'][$field['field_name']] = $field;
    $form['fields']['#value'][] = $field['field_name'];
    $form[$field['field_name']] += array('#weight' => $weight++, '#prefix' => '<div class="fields-action-togglable">', '#suffix' => '</div>');

    // Checkbox to enable/disable this field.
    $form[$field['field_name'] . '_check'] = array('#type' => 'checkbox', '#attributes' => array('class' => 'fields-action-toggler'));

    // PHP code to program the value.
    if (isset($field['field_def'])) {
      $sample = 'return value;';
    }
    else {
      $db_info = content_database_info($field);
      $columns = array_keys($db_info['columns']);
      foreach ($columns as $key => $column) {
        $columns[$key] = t("'@column' => value for @column", array('@column' => $column));
      }
      $sample = t("return array(\n  0 => array(@columns),\n  // You'll usually want to stop here. Provide more values\n  // if you want your 'default value' to be multi-valued:\n  1 => array(@columns),\n  2 => ...\n);", array('@columns' => implode(', ', $columns)));
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
  $form['#node'] = (object)$node;
  return $form;
}

function theme_views_bulk_operations_fields_action_form(&$form) {
  drupal_add_js(drupal_get_path('module', 'views_bulk_operations').'/fields.action.js');
  $header = array(theme('table_select_header_cell'), t('Field'), t('Code'));
  $rows = array();
  foreach ($form['fields']['#value'] as $field_name) {
    $rows[] = array(
      drupal_render($form[$field_name . '_check']),
      drupal_render($form[$field_name]),
      drupal_render($form[$field_name . '_code']),
    );
  }
  $output  = t('<h4>Using the Code column</h4>
                <ul>
                <li>Will override the value specified in the field widget.</li>
                <li>Should not include &lt;?php ?&gt; delimiters.</li>
                <li>If in doubt, refer to <a target="_blank" href="@link_devel">devel.module</a> \'Dev load\' tab on a content page to figure out the expected format.</li>
                </ul>', array('@link_devel' => 'http://www.drupal.org/project/devel'));
  $output .= theme('table', $header, $rows); 
  return $output;
}

function views_bulk_operations_fields_action_validate($form, $form_state) {
  $field_types = _content_field_types();
  $chosen = 0;
  foreach ($form_state['values']['fields'] as $field_name) {
    $field = content_fields($field_name);
    if ($form_state['values'][$field_name . '_check']) {
      $chosen++;
      if ($field) {
        $function = $field_types[$field['type']]['module'] .'_field';
        if (function_exists($function)) {
          $form['#node'] = (object)array('type' => '', $field_name => $form_state['values'][$field_name]);
          $items = isset($form['#node']->$field_name) ? $form['#node']->$field_name : array();
          $function('validate', $form['#node'], $field, $items, $form, NULL);
          content_field('validate', $form['#node'], $field, $items, $form, NULL);
        }
      }
    }
  }
  if (!$chosen) {
    form_set_error('', t('You must select at least one field to modify.'));
  }
}

function views_bulk_operations_fields_action_submit($form, $form_state) {
  $values = array();
  foreach ($form_state['values']['fields'] as $field_name) {
    $values[$field_name] = $form_state['values'][$field_name];
    $values[$field_name . '_check'] = $form_state['values'][$field_name . '_check'];
    $values[$field_name . '_code'] = $form_state['values'][$field_name . '_code'];
  }
  $values['fields'] = $form_state['values']['fields'];
  return $values;
}

function views_bulk_operations_fields_action(&$node, $context) {
  foreach ($context['fields'] as $field_name) {
    if ($context[$field_name . '_check']) { // use the value for this field
      if (!empty($context[$field_name . '_code'])) {
        $value = eval($context[$field_name . '_code']);
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
  $return = array();

  // Get the form definition.
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_id = $type . '_node_form';
  $form = drupal_retrieve_form($form_id, $form_state, (object)array('type' => $type, 'name' => $user ? $user->name : '', 'nid' => 0));
  drupal_prepare_form($form_id, $form, $form_state);

  // Iterate on known fields.
  foreach (array('title', 'body', 'format', 'revision', 'log', 'name', 'date', 'status', 'promote', 'sticky', 'comment') as $field_name) {
    $path = array_key_path($field_name, $form);
    if ($path === FALSE) continue;
    $field_def = array_path($form, array_merge($path, (array)$field_name));
    if (is_array($field_def) && 
        (!isset($field_def['#access']) || $field_def['#access']) &&
        (isset($field_def['#type']) && $field_def['#type'] != 'value' && $field_def['#type'] != 'hidden')
    ) {
      $field_def['#required'] = FALSE;
      $return[$field_name] = array(
        'field_name' => $field_name,
        'field_def' => array($field_name => $field_def),
      );
    }
  }

  return $return;
}

if (!function_exists('array_key_path')) {
function array_key_path($needle, $haystack, $forbidden = array(), $path = array()) {
  foreach ($haystack as $key => $val) {
    if (in_array($key, $forbidden)) {
      continue;
    }
    if (is_array($val) && is_array($sub = array_key_path($needle, $val, $forbidden, array_merge($path, (array)$key)))) {
      return $sub;
    }
    elseif ($key === $needle) {
      return $path;
    }
  }
  return FALSE;
}
}

if (!function_exists('array_path')) {
function &array_path(&$array, $path) {
  $offset =& $array;
  if ($path) foreach ($path as $index) {
    $offset =& $offset[$index];
  }
  return $offset;
}
}

