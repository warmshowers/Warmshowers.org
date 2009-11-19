<?php
// $Id: ruleset.action.inc,v 1.1.2.2 2009/05/10 02:48:35 kratib Exp $

function views_bulk_operations_ruleset_action_info() {
  if (!module_exists('rules')) return array();
  $actions = array();
  foreach (rules_get_configured_items('rule_sets') as $ruleset_key => $ruleset) {
    if (count($ruleset['arguments']) == 1) { // For now, we only accept rulesets with one parameter (taken to be the 'type')
      $arg = key($ruleset['arguments']);
      $actions["views_bulk_operations_ruleset_action_{$ruleset_key}"] = array(
        'type' => $ruleset['arguments'][$arg]['type'],
        'parameters' => array('ruleset' => $ruleset_key),
        'description' => $ruleset['label'],
        'configurable' => FALSE,
        'rules_ignore' => TRUE,
      );
      eval(<<<EOS
if (!function_exists('views_bulk_operations_ruleset_action_{$ruleset_key}')) {
  function views_bulk_operations_ruleset_action_{$ruleset_key}(&\$object, \$context) {
    rules_invoke_rule_set(\$context['ruleset'], \$object);
  }
}
EOS
      );
    }
  }
  return $actions;
}

