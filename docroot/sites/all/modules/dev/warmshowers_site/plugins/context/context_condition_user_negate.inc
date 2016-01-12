<?php

/**
 * Expose current user role as a context condition with reverse options.
 */
class context_condition_user_negate extends context_condition_user {

  function options_form($context){
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'negate_role' => array(
        '#title' => t('Make role a negative condition'),
        '#type' => 'checkbox',
        '#description' => t("Checking this box will make this condition fire if the user's role is NOT one of the role's checked"),
        '#default_value' => isset($defaults['negate_role']) ? $defaults['negate_role'] : 0,
      ),
    );
  }

  function execute($account) {
    foreach ($this->get_contexts() as $context) {
      $options = $this->fetch_from_context($context, 'options');
      $intersect = array_intersect($account->roles, $this->fetch_from_context($context, 'values'));
      $negate_role = isset($options['negate_role']) && $options['negate_role'];

      if ($negate_role && !$intersect) {
        $this->condition_met($context);
      } elseif (!$negate_role && $intersect) {
        $this->condition_met($context);
      }
    }
  }

}
