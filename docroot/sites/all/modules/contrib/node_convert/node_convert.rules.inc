<?php
// $Id: node_convert.rules.inc,v 1.1.2.1 2011/01/09 19:06:47 placinta Exp $

/**
 * @file rules integration for the node_convert module
 */

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function node_convert_rules_event_info() {
  return array(
    'node_convert_converted_node' => array(
      'label' => t('Content has been converted'),
      'module' => 'Node',
      'arguments' => rules_events_node_arguments(t('Converted content'), t("Content's author"), TRUE),
    ),
  );
}
