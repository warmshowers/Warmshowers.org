<?php
/**
 * @file
 * Variable devel pages.
 */

require_once drupal_get_path('module', 'variable') . '/variable.variable.inc';
require_once drupal_get_path('module', 'variable_devel') . '/variable_devel.inc';

/**
 * List variables by type.
 */
function variable_devel_page_type($type = NULL) {
  if ($type && $info = variable_get_type($type)) {
    drupal_set_title(t('Variable type: !type', array('!type' => $info['title'])));
    $build['definition'] = array(
      '#title' => t('Type definition: %type', array('%type' => $type)),
      '#type' => 'item',
      '#markup' => _variable_devel_export_definition($type, $info, 'type'),
    );
    foreach (variable_get_info(NULL, array('langcode' => 'en')) as $name => $variable) {
      if ($variable['type'] == $type) {
        $variable['value'] = variable_get_value($variable);
        $list[] = array(
          l($name, 'admin/config/system/variable/definition/' . $name),
          $variable['title'],
          variable_format_value($variable),
        );
      }
    }
    if (!empty($list)) {
      $build['variables'] = array(
        '#theme' => 'table',
        '#header' => array(t('Variable'), t('Title'), t('Value')),
        '#rows' => $list,
        '#caption' => t('Variables of this type'),
      );
    }
    return $build;
  }
  else {
    $header = array(T('Name'), t('Type'), t('Parent type'), t('Module'), t('Variables'));
    $rows = array();
    $variable_types = variable_get_type();
    $modules = system_list('module_enabled');
    // Get variables by type.
    foreach (variable_get_info() as $name => $info) {
      $variable_list[$info['type']][] = l($name, 'admin/config/system/variable/definition/' . $name);
    }

    ksort($variable_types);
    foreach ($variable_types as $type => $info) {
      $info += array('type' => '');
      $rows[] = array(
        l($info['title'], 'admin/config/system/variable/type/' . $type),
        $type,
        $info['type'],
        $modules[$info['module']]->info['name'],
        isset($variable_list[$type]) ? implode(', ', $variable_list[$type]) : '',
      );
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
}

/**
 * Print variable definition.
 */
function variable_devel_page_definition($name) {
  $variable = variable_get_info($name, array('langcode' => 'en'));
  $value = variable_get($name);

  $build['definition'] = array(
    '#title' => t('Variable definition: %name', array('%name' => $name)),
    '#type' => 'item',
    '#markup' => $variable ? _variable_devel_export_definition($name, $variable) : t('Undefined'),
  );
  $build['value'] = array(
    '#title' => t('Current value'),
    '#type' => 'item',
    '#markup' => $variable ? variable_format_value($variable) : dprint_r(variable_get($name), TRUE),
  );

  return $build;
}

/**
 * List variables not declared.
 */
function variable_devel_page_undefined() {
  global $conf;

  $header = array(
    'name' => t('Name'),
    'type' => t('Type'),
    'value' => t('Value'),
  );

  foreach ($conf as $name => $value) {
    $info = variable_get_info($name);
    if (!$info) {
      $rows[] = array(
        'name' => l($name, 'admin/config/system/variable/definition/' . $name),
        'type' => gettype($value),
        'value' => is_scalar($value) ? check_plain((string)$value) : '',
      );
    }
  }
  $build['undefined'] = array(
    '#theme' => 'table',
    '#caption' => t('Variables not declared'),
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No variables.'),
  );
  return $build;
}
