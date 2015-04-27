<?php

/**
 * Validate whether an argument is the current user or has a permission.
 *
 * This supports either numeric arguments (UID) or strings (username) and
 * converts either one into the user's UID.  This validator also sets the
 * argument's title to the username.
 */
class uc_order_plugin_argument_validate_user_perm extends views_plugin_argument_validate_user {
  function option_definition() {
    $options = parent::option_definition();

    $options['perm'] = array('default' => 'view all orders');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['restrict_roles']['#access'] = FALSE;
    $form['roles']['#access'] = FALSE;

    $perms = array();
    $module_info = system_get_info('module');

    // Get list of permissions
    foreach (module_implements('permission') as $module) {
      $permissions = module_invoke($module, 'permission');
      foreach ($permissions as $name => $perm) {
        $perms[$module_info[$module]['name']][$name] = strip_tags($perm['title']);
      }
    }

    asort($perms);

    $form['perm'] = array(
      '#type' => 'select',
      '#options' => $perms,
      '#title' => t('Permission'),
      '#default_value' => $this->options['perm'],
      '#description' => t('Users with the selected permission flag will be able to bypass validation.'),
    );
  }

  function validate_argument($argument) {
    $type = $this->options['type'];
    // is_numeric() can return false positives, so we ensure it's an integer.
    // However, is_integer() will always fail, since $argument is a string.
    if (is_numeric($argument) && $argument == (int)$argument) {
      if ($type == 'uid' || $type == 'either') {
        if ($argument == $GLOBALS['user']->uid) {
          // If you assign an object to a variable in PHP, the variable
          // automatically acts as a reference, not a copy, so we use
          // clone to ensure that we don't actually mess with the
          // real global $user object.
          $account = clone $GLOBALS['user'];
        }
        $where = 'uid = :argument';
      }
    }
    else {
      if ($type == 'name' || $type == 'either') {
        $name = !empty($GLOBALS['user']->name) ? $GLOBALS['user']->name : variable_get('anonymous', t('Anonymous'));
        if ($argument == $name) {
          $account = clone $GLOBALS['user'];
        }
        $where = "name = :argument";
      }
    }

    // If we don't have a WHERE clause, the argument is invalid.
    if (empty($where)) {
      return FALSE;
    }

    if (!isset($account)) {
      $query = "SELECT uid, name FROM {users} WHERE $where";
      $account = db_query($query, array(':argument' => $argument))->fetchObject();
    }
    if (empty($account)) {
      // User not found.
      return FALSE;
    }

    // If the current user is not the account specified by the argument
    // and doesn't have the correct permission, validation fails.
    if ($GLOBALS['user']->uid != $account->uid && !user_access($this->options['perm'])) {
      return FALSE;
    }

    $this->argument->argument = $account->uid;
    $this->argument->validated_title = isset($account->name) ? check_plain($account->name) : check_plain(variable_get('anonymous', t('Anonymous')));
    return TRUE;
  }
}
