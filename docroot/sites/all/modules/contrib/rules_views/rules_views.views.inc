<?php


/**
 * @file
 * Views hooks and implementations for Rules Views.
 */

/**
 * Implementation of hook_views_plugins().
 */
function rules_views_views_plugins() {
  return array(
    'display' => array(
      RULES_VIEWS_EXECUTOR_DISPLAY_PLUGIN => array(
        'title' => t('Rules executor'),
        'help' => t("The view will be 'rendered into' a chosen ruleset."),
        'handler' => 'rules_views_executor_plugin_display_rules_executor',
        'theme' => 'views_view',
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Execution settings'),
        'help topic' => 'display-rules-executor',
        'parent' => 'views_periodic_execution',
        'use root account' => TRUE
      ),
    ),
    'style' => array(
      'rules_views_executor_style' => array(
        'title' => t('Rules executor'),
        'help' => t('Renders a view into a Rules ruleset.'),
        'handler' => 'rules_views_executor_plugin_style_rules_executor_style',
        'theme' => 'views_view_unformatted',
        'uses row plugin' => TRUE,
        'uses options' => FALSE,
        'type' => 'rules_executor',
        'help topic' => 'style-rss',
      ),
    ),
    'row' => array(
      'rules_views_executor_row_ruleset' => array(
        'title' => t('Rule Set'),
        'help' => t('Renders a view into a chosen rule set.'),
        'handler' => 'rules_views_executor_plugin_row_ruleset',
        'theme' => 'rules_views_executor_view_ruleset',
        'theme path' => drupal_get_path('module', 'rules_views'),
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'rules_views_executor',
        'help topic' => 'style-row-fields',
      ),
      /*'rules_executor_row_rule' => array(
        'title' => t('Rule'),
        'help' => t('Renders a view into a chosen rule.'),
        'handler' => 'rules_executor_plugin_row_rule',
        'theme' => 'views_view_fields',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'rules_executor',
        'help topic' => 'style-row-fields',
      ),*/
    ),
  );
}
