<?php

/**
 * Expose current user role as a context condition.
 * Taken from http://cgit.drupalcode.org/sandbox-petermallett-1962466/tree/plugins/context_condition_not_roles.inc
 */
class context_condition_negate_roles extends context_condition {
  function condition_values() {
    $values = array();
    foreach (user_roles() as $rid => $role_name) {
      if ($rid == DRUPAL_ANONYMOUS_RID) {
        $values['anonymous user'] = check_plain($role_name);
      }
      elseif ($rid == DRUPAL_AUTHENTICATED_RID) {
        $values['authenticated user'] = check_plain($role_name);
      }
      else {
        $values[$role_name] = check_plain($role_name);
      }
    }
    return $values;
  }

  function execute($account) {
    $roles = $account->roles;
    foreach ($roles as $rid => $role) {
      foreach ($this->get_contexts($role) as $context) {
        // If the user is in any of the roles, the condition is not satisfied.
        return NULL;
      }
    }

    // If the user roles didn't cause the condition to return, it is met.
    foreach ($this->get_contexts() as $context) {
      $this->condition_met($context);
    }
  }
}
