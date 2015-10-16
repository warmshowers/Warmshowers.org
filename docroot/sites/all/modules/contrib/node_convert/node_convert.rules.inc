<?php

/**
 * @file
 * rules integration for the node_convert module
 */

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */

function node_convert_rules_event_info() {
  return array(
    'node_convert_converted_node' => array(
      'label' => t('Content has been converted'),
      'module' => 'node',
      'group' => t('Node'),
      'variables' => rules_events_node_variables(t('Converted node')),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */

function node_convert_rules_action_info() {
  $actions = array();
  $actions['node_convert_convert_nodes_using_template_rules_callback'] = array(
    'label' => t('Convert a node'),
    'group' => t('Node'),
    'parameter' => array(
      'template' => array(
        'type' => 'text',
        'label' => t('Select the Convert Template you wish to use.'),
        'options list' => 'node_convert_rules_template_list',
      ),
      'node' => array(
        'type' => 'node',
        'label' => t('Node to convert'),
      ),
    ),
  );
  return $actions;
}

/**
 * Takes rules action parameters and passes them to the convert function.
 *
 * @param $template
 *   The template id to use for conversion.
 * @param $node
 *   Full node object
 * @return bool FALSE if conversion fails, this is return of convert function.
 */
function node_convert_convert_nodes_using_template_rules_callback($template, $node){
  return node_convert_convert_nodes_using_template(array($node->nid), $template);
}

/**
 * Helper - Gets a list of templates and id's.
 *
 * @return array A associated array of template id's and names.
 */
function node_convert_rules_template_list() {
  $template_objects = node_convert_load_all_templates();
  $templates = array();
  foreach ($template_objects as $template) {
    $templates[$template->machine_name] = $template->name;
  }
  return $templates;
}