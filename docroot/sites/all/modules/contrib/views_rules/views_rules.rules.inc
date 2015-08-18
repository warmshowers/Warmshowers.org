<?php
/**
 * @file
 * Rules declarations.
 */

/**
 * Implements hook_rules_plugin_info().
 */
function views_rules_rules_plugin_info() {
  return array(
    'view loop' => array(
      'class' => 'ViewsRulesLoop',
      'embeddable' => 'RulesActionContainer',
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'ViewsRulesLoopUI',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function views_rules_rules_action_info() {
  return array(
    'views_rules_collect_rows' => array(
      'label' => t('Collect view result rows'),
      'group' => t('Views Rules'),
      // Declare basic 'parameter' and 'provides' to be altered at evaluation.
      'parameter' => array(
        'views_rules_display' => array(
          'label' => t('View iterator display'),
          'type' => 'text',
          'options list' => 'views_rules_list_iterators',
          'restriction' => 'input',
          'default mode' => 'input',
        ),
      ),
      'provides' => array(),
    ),
  );
}

/**
 * Implements hook_rules_file_info().
 */
function views_rules_rules_file_info() {
  return array(
    'rules/views_rules.action',
    'rules/views_rules.ui',
  );
}
