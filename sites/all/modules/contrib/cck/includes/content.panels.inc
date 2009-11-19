<?php
// $Id: content.panels.inc 505 2009-05-24 18:55:09Z rfay $

/**
 * @file
 * Panels 2 interface for CCK fieldgroups and fields.
 *
 * TODO:
 *
 * - Adjust the fields and groups that are displayed for context, when that
 *   capability is added to Panels.
 *
 */

/**
 * Implementation of hook_panels_content_types()
 */
function content_panels_content_types() {
  $items = array();
  $items['content_field'] = array(
    'title' => t('Content field'),
    'single' => TRUE,
    'content_types' => 'content_panels_field_content_types',
    'add callback' => 'content_panels_edit_field',
    'edit callback' => 'content_panels_edit_field',
    'render callback' => 'content_panels_render_field',
    'title callback' => 'content_panels_title_content',
  );
  return $items;
}

function content_panels_field_content_types() {
  return array(
    'description' => array(
      'title' => t('Content field'),
      'icon' => 'icon_node.png',
      'path' => panels_get_path('content_types/node'),
      'description' => t('A content field from the referenced node.'),
      'required context' => new panels_required_context(t('Node'), 'node'),
      'category' => array(t('Node context'), -9),
    ),
  );
}

function content_panels_edit_field($id, $parents, $conf = array()) {
  $form = array();
  $form['label'] = array(
    '#type' => 'select',
    '#title' => t('Label'),
    '#default_value' => isset($conf['label']) ? $conf['label'] : '',
    '#options' => array(
      'normal' => t('Block title'),
      'above' => t('Above'),
      'inline' => t('Inline'),
      'hidden' => t('Hidden'),
    ),
    '#description' => t('Configure how the label is going to be displayed.'),
  );
  $options = array('' => '');
  $fields = content_fields();
  $field_types = _content_field_types();
  foreach ($fields as $field_name => $field) {
    $type_info = $field_types[$field['type']];
    foreach ($type_info['formatters'] as $formatter_name => $formatter) {
      $label  = $type_info['label'] .':'. $field['widget']['label'] ;
      $label .= $field['widget']['label'] != $field_name ? ' ('. $field_name .')' : '';
      $options[$label][$field_name .':'. $formatter_name] = $formatter['label'];
    }
  }
  ksort($options);
  $form['field_formatter'] = array(
    '#type' => 'select',
    '#title' => t('Field / Formatter'),
    '#default_value' => isset($conf['field_formatter']) ? $conf['field_formatter'] : '',
    '#options' => $options,
    '#description' => t('Select a field and formatter.'),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * 'Title' callback for the 'field' content type.
 */
function content_panels_title_content($subtype, $conf, $context) {
  $data = explode(':', $conf['field_formatter']);
  $field_name = $data[0];
  $formatter = $data[1];
  $fields = content_fields();
  $field = $fields[$field_name];
  $field_types = _content_field_types();
  return t('"@s" field @name', array('@s' => $context->identifier, '@name' => $field_types[$field['type']]['label'] .': '. $field['widget']['label'] .' ('. $field['field_name'])) .')';
}

function content_panels_render_field($subtype, $conf, $panel_args, $context) {
  if (is_array($context)) {
    $context = array_pop($context);
  }
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $data = explode(':', $conf['field_formatter']);
  $field_name = $data[0];
  $formatter = $data[1];

  $field = content_fields($field_name);

  // Force panel settings into the field's display settings.
  $field['display_settings']['label']['format'] = $conf['label'] == 'normal' ? 'hidden' : $conf['label'];
  $field['display_settings']['full']['format'] = $formatter;
  $node->build_mode = NODE_BUILD_NORMAL;
  // TODO : allow panel-specific template suggestions for content-field.tpl.php ?

  $output = content_view_field($field, $node);

  $block->module = 'content';
  $block->delta = $field_name;
  if ($conf['label'] == 'normal') {
    $block->title = $field['widget']['label'];
  }
  $block->content = $output;

  return $block;
}