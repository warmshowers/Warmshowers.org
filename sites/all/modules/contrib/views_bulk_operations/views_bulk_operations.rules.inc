<?php
// $Id: views_bulk_operations.rules.inc,v 1.1.2.1 2009/11/24 00:58:24 kratib Exp $

function views_bulk_operations_rules_action_info_alter(&$actions) {
  $actions['rules_core_views_bulk_operations_action'] = array(
    'label' => t('Execute a VBO programmatically'),
    // Make sure there is something passed for $object to the action.
    'arguments' => array('object' => array('type' => 'value', 'default value' => NULL)),
    'module' => 'Views Bulk Operations',
    'eval input' => array(),
    // Let the rules system for executing core style actions execute it.
    'base' => 'rules_core_action_execute',
    'action_name' => 'views_bulk_operations_action',
    'configurable' => TRUE,
  );
}

