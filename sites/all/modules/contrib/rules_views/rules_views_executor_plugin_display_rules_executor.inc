<?php


/**
 * @file
 * The Rules Views Executor Views display plugin.
 *
 * This is the main Rules Views Executor display plugin that allows our style and
 * row style plugins to hook into the right displays. We also provide a way to
 * specify the execution interval for this display.
 */
class rules_views_executor_plugin_display_rules_executor extends views_periodic_execution_plugin_display {
  function get_style_type() { return 'rules_views_executor'; }

  function defaultable_sections($section = NULL) {
    if (in_array($section, array('style_options', 'style_plugin', 'row_options', 'row_plugin', ))) {
      return FALSE;
    }

    $sections = parent::defaultable_sections($section);

    return $sections;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['displays'] = array('default' => array());

    // Overrides for standard stuff:
    $options['style_plugin']['default'] = 'rules_views_executor_style';
    $options['row_plugin']['default'] = 'rules_views_executor_row_ruleset';
    $options['defaults']['default']['style_plugin'] = FALSE;
    $options['defaults']['default']['style_options'] = FALSE;
    $options['defaults']['default']['row_plugin'] = FALSE;
    $options['defaults']['default']['row_options'] = FALSE;
    $options['username']['default'] = 'Rules executor';
    $options['permissions']['default'] = array(
      'access comments',
      'administer comments',
      'access content',
      'administer nodes',
      'administer users',
    );

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['rules_executor'] = array(
      'title' => t('Rules Executor'),
    );

    $options['username'] = array(
      'category' => 'rules_executor',
      'title' => t('Username'),
      'value' => $this->get_option('username'),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'username':
        $form['#title'] = t('Username');
        $form['#help_topic'] = 'interval';
        $form['#help_module'] = 'rules_executor';
        $form['username'] = array(
          '#type' => 'textfield',
          '#description' => t('Choose a username to execute the ruleset as. If you choose a username that doesn\'t exist, a user with that name will be created for you (at runtime).'),
          '#default_value' => $this->get_option('username'),
          '#autocomplete_path' => 'user/autocomplete',
        );
        $all_permissions = array();
        foreach (module_list(FALSE, FALSE, TRUE) as $module) {
          if ($perms = module_invoke($module, 'perm')) {
            sort($perms);
            $all_permissions = array_merge($all_permissions, $perms);
          }
        }
        $form['permissions'] = array(
          '#type' => 'checkboxes',
          '#description' => t('IF you have chosen to create a new user above, then choose the permissions this user will be created with, they must have the permissions to perform whatever actions are necessary for the correct execution of your chosen rule/rule set.'),
          '#options' => drupal_map_assoc($all_permissions),
          '#default_value' => $this->get_option('permissions'),
        );

        break;
    }
  }

  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'username':
        $this->set_option('username', $form_state['values']['username']);
        $this->set_option('permissions', array_filter($form_state['values']['permissions']));
        break;
    }
  }

  function get_username() {
    return $this->get_option('username');
  }

  function get_permissions() {
    return $this->get_option('permissions');
  }
}
