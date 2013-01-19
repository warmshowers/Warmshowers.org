<?php

/**
 * @file
 * Rules specific functions that expose content_access' API.
 */


/**
 * Adds the role based settings to the form.
 */
function content_access_rules_role_based_form($settings, &$form) {
  module_load_include('inc', 'content_access', 'content_access.admin');
  $form['#includes'][] = './'. drupal_get_path('module', 'content_access') .'/content_access.admin.inc';
  $form += content_access_role_based_form($settings);
}

/**
 * Parse submitted settings for per-node form into internal format.
 */
function content_access_parse_settings(&$settings, $form, $form_state) {
  foreach (_content_access_get_operations() as $op) {
    $settings[$op] = array_keys(array_filter($form_state['values'][$op]));
  }
}

/**
 * Verifies that per content settings are activated for the given node.
 */
function _content_access_rules_check_setting($node) {
  if (!content_access_get_settings('per_node', $node->type)) {
    rules_log(t("Can't set per content permissions for content type @type. Make sure to have per content settings activated for content types you want to alter access control for.", array('@type' => node_get_types('name', $node->type))), TRUE);
    return FALSE;
  }
  return TRUE;
}


/**
 * Implementation of hook_action_info().
 */
function content_access_rules_action_info() {
  $items = array(
    'content_access_action_grant_node_permissions' => array(
      'label' => t('Grant content permissions by role'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Content access',
    ),
    'content_access_action_revoke_node_permissions' => array(
      'label' => t('Revoke content permissions by role'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Content access',
    ),
    'content_access_action_reset_node_permissions' => array(
      'label' => t('Reset content permissions'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Content')),
      ),
      'module' => 'Content access',
    ),
  );
  if (module_exists('acl')) {
    $items += array(
      'content_access_action_acl_grant_access' => array(
        'label' => t('Grant access for a user'),
        'arguments' => array(
          'node' => array('type' => 'node', 'label' => t('Content')),
          'user' => array('type' => 'user', 'label' => t('User')),
        ),
        'module' => 'Content access',
      ),
      'content_access_action_acl_revoke_access' => array(
        'label' => t('Revoke access for a user'),
        'arguments' => array(
          'node' => array('type' => 'node', 'label' => t('Content')),
          'user' => array('type' => 'user', 'label' => t('User')),
        ),
        'module' => 'Content access',
      ),
    );
  }
  return $items;
}

/**
 * Action implementation: Grant permissions for a node.
 */
function content_access_action_grant_node_permissions($node, $settings) {
  if (_content_access_rules_check_setting($node)) {
    $ca_settings = array();
    foreach (_content_access_get_operations() as $op) {
      $settings += array($op => array());
      $ca_settings[$op] = array_keys(array_flip(content_access_per_node_setting($op, $node)) + array_flip($settings[$op]));
    }
    content_access_save_per_node_settings($node, $ca_settings);
    // A following node_save() updates the grants for us.
    return array('node' => $node);
  }
}

function content_access_action_grant_node_permissions_form($settings, &$form) {
  content_access_rules_role_based_form($settings, $form);
}
function content_access_action_grant_node_permissions_submit(&$settings, $form, $form_state) {
  content_access_parse_settings($settings, $form, $form_state);
}

/**
 * Action implementation: Revoke permissions for a node.
 */
function content_access_action_revoke_node_permissions($node, $settings) {
  if (_content_access_rules_check_setting($node)) {
    $ca_settings = array();
    foreach (_content_access_get_operations() as $op) {
      $settings += array($op => array());
      $ca_settings[$op] = array_diff(content_access_per_node_setting($op, $node), $settings[$op]);
    }
    content_access_save_per_node_settings($node, $ca_settings);
    // A following node_save() updates the grants for us.
    return array('node' => $node);
  }
}

function content_access_action_revoke_node_permissions_form($settings, &$form) {
  content_access_rules_role_based_form($settings, $form);
}
function content_access_action_revoke_node_permissions_submit(&$settings, $form, $form_state) {
  content_access_parse_settings($settings, $form, $form_state);
}


/**
 * Action implementation: Grant access for a user.
 */
function content_access_action_acl_grant_access($node, $user, $settings) {
  if (_content_access_rules_check_setting($node)) {
    module_load_include('inc', 'content_access', 'content_access.admin');
    foreach ($settings['ops'] as $op) {
      $acl_id = content_access_get_acl_id($node, $op);
      acl_add_user($acl_id, $user->uid);
      acl_node_add_acl($node->nid, $acl_id, $op == 'view', $op == 'update', $op == 'delete', content_access_get_settings('priority', $node->type));
    }
    // A following node_save() updates the grants for us.
    return array('node' => $node);
  }
}

function content_access_action_acl_grant_access_form($settings, &$form) {
  $settings += array('ops' => array('view', 'update', 'delete'));
  $form['settings']['ops'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('view', 'update', 'delete')),
    '#title' => t('Operations to grant access for'),
    '#default_value' => $settings['ops'],
  );
}
function content_access_action_acl_grant_access_submit(&$settings, $form, $form_state) {
  $settings['ops'] = array_filter($settings['ops']);
}
function content_access_action_acl_grant_access_help() {
  return t("Note that this action is not going to revoke access for not chosen operations.");
}
function content_access_action_acl_grant_access_label($settings, $argument_labels) {
  return t('Grant access for @user.', $argument_labels);
}


/**
 * Action implementation: Revoke access for a user.
 */
function content_access_action_acl_revoke_access($node, $user, $settings) {
  if (_content_access_rules_check_setting($node)) {
    module_load_include('inc', 'content_access', 'content_access.admin');
    foreach ($settings['ops'] as $op) {
      $acl_id = content_access_get_acl_id($node, $op);
      acl_remove_user($acl_id, $user->uid);
    }
    // A following node_save() updates the grants for us.
    return array('node' => $node);
  }
}

function content_access_action_acl_revoke_access_form($settings, &$form) {
  $settings += array('ops' => array('view', 'update', 'delete'));
  $form['settings']['ops'] = array(
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('view', 'update', 'delete')),
    '#title' => t('Operations to revoke access for'),
    '#default_value' => $settings['ops'],
  );
}
function content_access_action_acl_revoke_access_submit(&$settings, $form, $form_state) {
  $settings['ops'] = array_filter($settings['ops']);
}
function content_access_action_acl_revoke_access_help() {
  return t("Note that this action is only able to revoke access that has been previously granted with the help of the content access module.");
}
function content_access_action_acl_revoke_access_label($settings, $argument_labels) {
  return t('Revoke access for @user.', $argument_labels);
}


/**
 * Action implementation: Reset permissions for a node.
 */
function content_access_action_reset_node_permissions($node) {
  if (_content_access_rules_check_setting($node)) {
    content_access_delete_per_node_settings($node);
    // A following node_save() updates the grants for us.
    return array('node' => $node);
  }
}

/**
 * Implementation of hook_condition_info().
 */
function content_access_rules_condition_info() {
  return array(
    'content_access_condition_check_permissions' => array(
    'label' => t('Check role based settings'),
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Content')),
    ),
    'help' => t('Evaluates to TRUE, if content access allows all selected operations for the given roles.'),
    'module' => 'Content access',
    ),
  );
}

/**
 * Condition implementation: Check if node has permissions.
 *
 * @note
 *   This will only check for the existence of permissions, not the
 *   absence of. I.e. a rule that checks just for authenticated write
 *   will return TRUE for a node that allows authenticated and anonymous
 *   write.
 */
function content_access_condition_check_permissions($node, $settings) {
  if (_content_access_rules_check_setting($node)) {
    // Compare our settings with node's settings.
    foreach (_content_access_get_operations() as $op) {
      $settings += array($op => array());
      $expect_roles = $settings[$op];
      $current_roles = content_access_per_node_setting($op, $node);
      if (count(array_diff($expect_roles, $current_roles)) != 0) {
        return FALSE;
      }
    }
    return TRUE;
  }
  return FALSE;
}

function content_access_condition_check_permissions_form($settings, &$form) {
  content_access_rules_role_based_form($settings, $form);
}
function content_access_condition_check_permissions_submit(&$settings, $form, $form_state) {
  content_access_parse_settings($settings, $form, $form_state);
}

/**
 * Automatically upgrade the old 'content_access_action_set_node_permissions' to an
 * 'content_access_action_grant_node_permissions' action.
 */
function content_access_action_set_node_permissions_upgrade(&$element) {
  $element['#name'] = 'content_access_action_grant_node_permissions';
}
