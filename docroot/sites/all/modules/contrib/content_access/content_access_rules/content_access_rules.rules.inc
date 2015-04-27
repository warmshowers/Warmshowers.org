<?php

/**
 * @file
 * Rules specific functions that expose content_access' API.
 *
 * @todo
 * A way to enable per-node settings when a rule created, otherwise no effects
 *   will be noticed.
 * Clean-up function names.
 */

/**
 * Implements hook_rules_event_info().
 *
 * @ingroup rules
 */
function content_access_rules_rules_event_info() {
  $events['content_access_content_type'] = array('label' => t('Content type access control was changed'));
  $events['content_access_per_node'] = array('label' => t('Per node access control was changed'));

  if (module_exists('acl')) {
    $events['content_access_user_acl'] = array('label' => t('User was added to ACL'));
  }

  $items = array();
  foreach ($events as $name => $event) {
    $items[$name] = array(
      'label' => $event['label'],
      'group' => t('Content Access'),
    );
  }

  return $items;
}

/**
 * Implementation of hook_rules_action_info().
 *
 * @ingroup rules
 */
function content_access_rules_rules_action_info() {
  $role_actions = array(
    'content_access_action_grant_node_permissions' => array(
      'label' => t('Grant access by role'),
      'description' => t('Grant access to the following content'),
    ),
    'content_access_action_revoke_node_permissions' => array(
      'label' => t('Revoke access by role'),
      'description' => t('Revoke access to the following content'),
    ),
  );

  $reset_actions = array(
    'content_access_action_reset_node_permissions' => array(
      'label' => t('Reset access to content type defaults'),
      'description' => t('Reset node permissions to default permissions'),
    ),
  );

  $user_actions = array(
    'content_access_action_user_grant' => array(
      'label' => t('Grant access by user'),
      'operation' => t('Grant'),
      'description' => t('Grant access to the following content'),
    ),
    'content_access_action_user_revoke' => array(
      'label' => t('Revoke access by user'),
      'operation' => t('Revoke'),
      'description' => t('Revoke access to the following content'),
    ),
  );

  $items = array();
  foreach ($role_actions as $name => $action) {
    $items[$name] = array(
      'label' => $action['label'],
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Content'),
          'description' => $action['description'],
        ),
        'permissions' => array(
          'type' => 'list<text>',
          'label' => t('Role-based access control settings'),
          'optional' => TRUE,
          'options list' => 'content_access_action_roles_permissions_list',
          'restriction' => 'input',
        ),
      ),
      'group' => t('Content Access'),
      'callbacks' => array(
        'form_alter' => 'content_access_rules_action_form_alter',
      ),
    );
  }

  foreach ($reset_actions as $name => $action) {
    $items[$name] = array(
      'label' => $action['label'],
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Content'),
          'description' => $action['description'],
        ),
      ),
      'group' => t('Content Access'),
    );
  }

  if (module_exists('acl')) {
    foreach ($user_actions as $name => $action) {
      $items[$name] = array(
        'label' => $action['label'],
        'named parameter' => TRUE,
        'parameter' => array(
          'node' => array(
            'type' => 'node',
            'label' => t('Content'),
            'description' => $action['description'],
          ),
          'content_access_user_view' => array(
            'type' => 'user',
            'label' => t('@action view access', array('@action' => $action['operation'])),
            'optional' => TRUE,
            'description' => t('@action view access to the following user.', array('@action' => $action['operation'])),
          ),
          'content_access_user_update' => array(
            'type' => 'user',
            'label' => t('@action update access', array('@action' => $action['operation'])),
            'optional' => TRUE,
            'description' => t('@action edit access to the following user.', array('@action' => $action['operation'])),
          ),
          'content_access_user_delete' => array(
            'type' => 'user',
            'label' => t('@action delete access', array('@action' => $action['operation'])),
            'optional' => TRUE,
            'description' => t('@action delete access to the following user.', array('@action' => $action['operation'])),
          ),
        ),
        'group' => t('Content Access User'),
      );
    }
  }

  return $items;
}

/**
 * Returns an options list array for the content access permission parameter.
 */
function content_access_action_roles_permissions_list() {
  $options = array();
  $roles = array_map('filter_xss_admin', user_roles());

  foreach (_content_access_get_operations() as $op => $label) {
    foreach ($roles as $rid => $role) {
      $options[$op][$op . ':' . $rid] = $op . ' ' . $role;
    }
  }
  return $options;
}

/**
 * Alter the settings form to render the text<list> as checkboxes.
 */
function content_access_rules_action_form_alter(&$form, &$form_state) {
  // Per node access control should be enabled for this type of action to be
  // effective.
  drupal_set_message(t('Access control settings will not be executed for the affected node unless you enabled \'%per_node\' from the Access Control tab on content type page of the node', array('%per_node' => 'Per content node access control settings')), 'warning');

  // Alter the text<list> to make it into checkboxes groups
  $elements =& $form['parameter']['permissions']['settings']['permissions'];
  $elements = content_access_rules_checkboxes_form((array) $elements['#default_value']);

  // Add our own after build callback for fixing the form value afterwards.
  $elements['#after_build'][] = 'content_access_rules_action_form_after_build';

  // Make sure our include file is loaded when FAPI processes the form.
  form_load_include($form_state, 'inc', 'content_access_rules', 'content_access_rules.rules');
}

/**
 * Form after build callback.
 *
 * Get the form settings as checkboxes, convert them back to list<text>.
 */
function content_access_rules_action_form_after_build($element, &$form_state) {
  if ($form_state['process_input']) {
    $form_value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $value = content_access_rules_transform_to_rule_value($form_value);
    form_set_value($element, $value, $form_state);
  }
  return $element;
}

/**
 * Returns the form elements for configuring content access per-role permissions.
 */
function content_access_rules_checkboxes_form($value) {
  $form = array();
  $roles = array_map('filter_xss_admin', user_roles());
  $defaults = content_access_rules_transform_rules_value($value);

  foreach (_content_access_get_operations() as $op => $label) {
    $form[$op] = array(
      '#type' => 'checkboxes',
      '#prefix' => '<div class="content_access-div">',
      '#suffix' => '</div>',
      '#options' => $roles,
      '#title' => $label,
      '#default_value' => isset($defaults[$op]) ? $defaults[$op] : array(),
      '#process' => array('form_process_checkboxes', 'content_access_disable_checkboxes'),
    );
  }
  $form['clearer'] = array(
    '#value' => '<br clear="all" />',
  );
  drupal_add_css(drupal_get_path('module', 'content_access') . '/content_access.css');

  return $form;
}

/**
 * Transforms the array of text values used by Rules to an array keyed by $op and $rid.
 *
 * @see content_access_rules_transform_to_rule_value()
 */
function content_access_rules_transform_rules_value($value) {
  $array = array();
  foreach ($value as $op_role) {
    $parts = explode(':', $op_role);
    // The first item is $op and the second $rid.
    $array[$parts[0]][] = $parts[1];
  }
  return $array;
}

/**
 * Transform the form values array keyed by $op and $rid to an array of text values as used by Rules.
 *
 * @see content_access_rules_transform_rules_value()
 */
function content_access_rules_transform_to_rule_value($form_values) {
  $value = array();
  foreach ($form_values as $op => $array) {
    foreach (array_filter($array) as $rid => $data) {
      $value[] = $op . ':' . $rid;
    }
  }
  return $value;
}

/**
 * Action implementation: Grant permissions for a node.
 */
function content_access_action_grant_node_permissions($node, $permissions) {
  if (!empty($node->nid) && _content_access_rules_check_setting($node)) {
    // Transform the value to the content-access format.
    $settings = content_access_rules_transform_rules_value($permissions);
    $ca_settings = array();
    foreach (_content_access_get_operations() as $op => $label) {
      // Merge in the array of role-ids for each operation.
      $settings += array($op => array());
      $ca_settings[$op] = array_keys(array_flip(content_access_per_node_setting($op, $node)) + array_flip($settings[$op]));
    }
    content_access_save_per_node_settings($node, $ca_settings);
    content_access_action_aquire_grants($node);
  }
}

/**
 * Action implementation: Revoke permissions for a node.
 */
function content_access_action_revoke_node_permissions($node, $permissions) {
  if (!empty($node->nid) && _content_access_rules_check_setting($node)) {
    // Transform the value to the content-access format.
    $settings = content_access_rules_transform_rules_value($permissions);

    $ca_settings = array();
    foreach (_content_access_get_operations() as $op => $label) {
      $settings += array($op => array());
      $ca_settings[$op] = array_diff(content_access_per_node_setting($op, $node), $settings[$op]);
    }
    content_access_save_per_node_settings($node, $ca_settings);
    content_access_action_aquire_grants($node);
  }
}

/**
 * Action implementation: Reset permissions for a node.
 */
function content_access_action_reset_node_permissions($node) {
  content_access_delete_per_node_settings($node);
  content_access_action_aquire_grants($node);
}

/**
 * Verifies that per content settings are activated for the given node.
 */
function _content_access_rules_check_setting($node) {
  $type = $node->type;
  $settings = variable_get('content_access_' . $type, array());

  if (isset($settings['per_node']) && $settings['per_node']) {
    return TRUE;
  }

  // If we didn't find any settings in content access for this type return
  // false as we don't want to process it.
  rules_log("Can't set per content permissions for content type @type. Make sure to have per content settings activated for content types you want to alter access control for.", array('@type' => $node->type), RulesLog::WARN);
  return FALSE;
}

/**
 * Split the settings string into array.
 */
function content_access_action_settings($action_settings = array()) {
  $roles_ids = array_flip(user_roles());

  foreach (_content_access_get_operations() as $op => $label) {
    $settings[$op] = array();
  }

  foreach ($action_settings as $op_role => $role) {
    $op = substr($op_role, 0, strpos($op_role, ':'));
    $rid = $roles_ids[$role];
    $settings[$op][] = $rid;
  }

  return $settings;
}

/**
 * Action implementation: Grant user access.
 */
function content_access_action_user_grant($params) {
  content_access_action_user($params, 'grant');
}

/**
 * Action implementation: Revoke user access.
 */
function content_access_action_user_revoke($params) {
  content_access_action_user($params, 'revoke');
}

/**
 * Process Rule's param, and grant by the passed operation.
 */
function content_access_action_user($params, $type) {
  $ops = array('view', 'update', 'delete');
  $settings = array();
  $node = $params['node'];

  foreach ($ops as $op) {
    if ($params['content_access_user_' . $op]) {
      $settings[$op] = $params['content_access_user_' . $op]->uid;
    }
  }

  foreach ($settings as $op => $uid) {
    $acl_id = content_access_get_acl_id($node, $op);
    acl_node_add_acl($node->nid, $acl_id, (int) ($op == 'view'), (int) ($op == 'update'), (int) ($op == 'delete'), content_access_get_settings('priority', $node->type));

    db_delete('acl_user')
      ->condition('acl_id', $acl_id)
      ->condition('uid', $uid)
      ->execute();

    if ($type == 'grant') {
      db_insert('acl_user')
        ->fields(array(
          'acl_id' => $acl_id,
          'uid' => $uid,
        ))
        ->execute();
    }
  }

  content_access_action_aquire_grants($node);
}

/**
 * Apply the new grants to the affected node.
 */
function content_access_action_aquire_grants($node) {
  // node_save() does implement node_access_acquire_grants() so we don't want
  // to execute it again or we'll get a duplicated key exception
  if (!isset($node->op) ||
      (isset($node->op) && $node->op != 'Save')) {
    node_access_acquire_grants($node);
  }
}