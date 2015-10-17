<?php
/**
 * @file
 * Plugin framework for Ultimate Cron.
 */

/**
 * This is the base class for all Ultimate Cron plugins.
 *
 * This class handles all the load/save settings for a plugin as well as the
 * forms, etc.
 */
class UltimateCronPlugin {
  public $name = '';
  public $title = '';
  public $description = '';
  public $plugin;
  public $settings = array();
  static public $multiple = FALSE;
  static public $instances = array();
  public $weight = 0;
  static public $globalOptions = array();

  /**
   * Constructor.
   *
   * Setup object.
   *
   * @param string $name
   *   Name of plugin.
   * @param array $plugin
   *   The plugin definition.
   */
  public function __construct($name, $plugin) {
    $this->plugin = $plugin;
    $this->title = $plugin['title'];
    $this->description = $plugin['description'];
    $this->name = $name;
    $this->type = $plugin['plugin type'];
    $this->key = 'ultimate_cron_plugin_' . $plugin['plugin type'] . '_' . $name . '_settings';
    $this->settings = variable_get($this->key, array());
  }

  /**
   * Singleton factoryLogEntry.
   */
  static public function factory($class, $name, $plugin) {
    if (empty($class::$instances[$plugin['plugin type']][$name])) {
      self::$instances[$plugin['plugin type']][$name] = new $class($name, $plugin);
    }
    return self::$instances[$plugin['plugin type']][$name];
  }

  /**
   * Get global plugin option.
   *
   * @param string $name
   *   Name of global plugin option to get.
   *
   * @return mixed
   *   Value of option if any, NULL if not found.
   */
  static public function getGlobalOption($name) {
    return isset(self::$globalOptions[$name]) ? self::$globalOptions[$name] : NULL;
  }

  /**
   * Get all global plugin options.
   *
   * @return array
   *   All options currently set, keyed by name.
   */
  static public function getGlobalOptions() {
    return self::$globalOptions;
  }

  /**
   * Set global plugin option.
   *
   * @param string $name
   *   Name of global plugin option to get.
   * @param string $value
   *   The value to give it.
   */
  static public function setGlobalOption($name, $value) {
    self::$globalOptions[$name] = $value;
  }

  /**
   * Remove a global plugin option.
   *
   * @param string $name
   *   Name of global plugin option to remove.
   */
  static public function unsetGlobalOption($name) {
    unset(self::$globalOptions[$name]);
  }

  /**
   * Remove all global plugin options.
   */
  static public function unsetGlobalOptions() {
    self::$globalOptions = array();
  }

  /**
   * Invoke hook_cron_alter() on plugins.
   */
  final static public function hook_cron_alter(&$jobs) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid()) {
          $plugin->cron_alter($jobs);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_pre_schedule() on plugins.
   */
  final static public function hook_cron_pre_schedule($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_pre_schedule($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_post_schedule() on plugins.
   */
  final static public function hook_cron_post_schedule($job, &$result) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_post_schedule($job, $result);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_pre_launch() on plugins.
   */
  final static public function hook_cron_pre_launch($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_pre_launch($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_post_launch() on plugins.
   */
  final static public function hook_cron_post_launch($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_post_launch($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_pre_run() on plugins.
   */
  final static public function hook_cron_pre_run($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_pre_run($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_post_run() on plugins.
   */
  final static public function hook_cron_post_run($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_post_run($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_pre_invoke() on plugins.
   */
  final static public function hook_cron_pre_invoke($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_pre_invoke($job);
        }
      }
    }
  }

  /**
   * Invoke hook_cron_post_invoke() on plugins.
   */
  final static public function hook_cron_post_invoke($job) {
    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    foreach ($plugin_types['ultimate_cron'] as $plugin_type => $info) {
      $plugins = _ultimate_cron_plugin_load_all($plugin_type);
      foreach ($plugins as $plugin) {
        if ($plugin->isValid($job)) {
          $plugin->cron_post_invoke($job);
        }
      }
    }
  }

  /**
   * A hook_cronapi() for plugins.
   */
  public function cronapi() {
    return array();
  }

  /**
   * A hook_cron_alter() for plugins.
   */
  public function cron_alter(&$jobs) {
  }

  /**
   * A hook_cron_pre_schedule() for plugins.
   */
  public function cron_pre_schedule($job) {
  }

  /**
   * A hook_cron_post_schedule() for plugins.
   */
  public function cron_post_schedule($job, &$result) {
  }

  /**
   * A hook_cron_pre_launch() for plugins.
   */
  public function cron_pre_launch($job) {
  }

  /**
   * A hook_cron_post_launch() for plugins.
   */
  public function cron_post_launch($job) {
  }

  /**
   * A hook_cron_pre_run() for plugins.
   */
  public function cron_pre_run($job) {
  }

  /**
   * A hook_cron_post_run() for plugins.
   */
  public function cron_post_run($job) {
  }

  /**
   * A hook_cron_pre_invoke() for plugins.
   */
  public function cron_pre_invoke($job) {
  }

  /**
   * A hook_cron_post_invoke() for plugins.
   */
  public function cron_post_invoke($job) {
  }

  /**
   * Signal page for plugins.
   */
  public function signal($item, $signal) {
  }

  /**
   * Allow plugins to alter the allowed operations for a job.
   */
  public function build_operations_alter($job, &$allowed_operations) {
  }

  /**
   * Get default settings.
   */
  public function getDefaultSettings($job = NULL) {
    $settings = array();
    if ($job && !empty($job->hook[$this->type][$this->name])) {
      $settings += $job->hook[$this->type][$this->name];
    }
    $settings += $this->settings + $this->defaultSettings();
    return $settings;
  }

  /**
   * Save settings to db.
   */
  public function setSettings() {
    variable_set($this->key, $this->settings);
  }

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * Get label for a specific setting.
   */
  public function settingsLabel($name, $value) {
    if (is_array($value)) {
      return implode(', ', $value);
    }
    else {
      return $value;
    }
  }

  /**
   * Format label for the plugin.
   *
   * @param UltimateCronJob $job
   *   The job for format the plugin label for.
   *
   * @return string
   *   Formatted label.
   */
  public function formatLabel($job) {
    return $job->name;
  }

  /**
   * Format verbose label for the plugin.
   *
   * @param UltimateCronJob $job
   *   The job for format the verbose plugin label for.
   *
   * @return string
   *   Verbosely formatted label.
   */
  public function formatLabelVerbose($job) {
    return $job->title;
  }

  /**
   * Default plugin valid for all jobs.
   */
  public function isValid($job = NULL) {
    return TRUE;
  }

  /**
   * Modified version drupal_array_get_nested_value().
   *
   * Removes the specified parents leaf from the array.
   *
   * @param array $array
   *   Nested associative array.
   * @param array $parents
   *   Array of key names forming a "path" where the leaf will be removed
   *   from $array.
   */
  public function drupal_array_remove_nested_value(array &$array, array $parents) {
    $ref = &$array;
    $last_parent = array_pop($parents);
    foreach ($parents as $parent) {
      if (is_array($ref) && array_key_exists($parent, $ref)) {
        $ref = &$ref[$parent];
      }
      else {
        return;
      }
    }
    unset($ref[$last_parent]);
  }

  /**
   * Clean form of empty fallback values.
   */
  public function cleanForm($elements, &$values, $parents) {
    if (empty($elements)) {
      return;
    }

    foreach (element_children($elements) as $child) {
      if (empty($child) || empty($elements[$child]) || is_numeric($child)) {
        continue;
      }
      // Process children.
      $this->cleanForm($elements[$child], $values, $parents);

      // Determine relative parents.
      $rel_parents = array_diff($elements[$child]['#parents'], $parents);
      $key_exists = NULL;
      $value = drupal_array_get_nested_value($values, $rel_parents, $key_exists);

      // Unset when applicable.
      if (!empty($elements[$child]['#markup'])) {
        self::drupal_array_remove_nested_value($values, $rel_parents);
      }
      elseif (
        $key_exists &&
        empty($value) &&
        !empty($elements[$child]['#fallback']) &&
        $value !== '0'
      ) {
        self::drupal_array_remove_nested_value($values, $rel_parents);
      }
    }
  }

  /**
   * Default settings form.
   */
  static public function defaultSettingsForm(&$form, &$form_state, $plugin_info) {
    $plugin_type = $plugin_info['type'];
    $static = $plugin_info['defaults']['static'];
    $key = 'ultimate_cron_plugin_' . $plugin_type . '_default';
    $options = array();
    foreach (_ultimate_cron_plugin_load_all($plugin_type) as $name => $plugin) {
      if ($plugin->isValid()) {
        $options[$name] = $plugin->title;
      }
    }
    $form[$key] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => variable_get($key, $static['default plugin']),
      '#title' => t('Default @plugin_type', array('@plugin_type' => $static['title singular'])),
    );
    $form = system_settings_form($form);
  }

  /**
   * Job settings form.
   */
  static public function jobSettingsForm(&$form, &$form_state, $plugin_type, $job) {
    // Check valid plugins.
    $plugins = _ultimate_cron_plugin_load_all($plugin_type);
    foreach ($plugins as $name => $plugin) {
      if (!$plugin->isValid($job)) {
        unset($plugins[$name]);
      }
    }

    // No plugins = no settings = no vertical tabs for you mister!
    if (empty($plugins)) {
      continue;
    }

    ctools_include('plugins');
    $plugin_types = ctools_plugin_get_plugin_type_info();
    $plugin_info = $plugin_types['ultimate_cron'][$plugin_type];
    $static = $plugin_info['defaults']['static'];

    // Find plugin selected on this page.
    // If "0" (meaning default) use the one defined in the hook.
    if (empty($form_state['values']['settings'][$plugin_type]['name'])) {
      $form_state['values']['settings'][$plugin_type]['name'] = 0;
      $current_plugin = $plugins[$job->hook[$plugin_type]['name']];
    }
    else {
      $current_plugin = $plugins[$form_state['values']['settings'][$plugin_type]['name']];
    }
    $form_state['previous_plugin'][$plugin_type] = $current_plugin->name;

    // Determine original plugin.
    $original_plugin = !empty($job->settings[$plugin_type]['name']) ? $job->settings[$plugin_type]['name'] : $job->hook[$plugin_type]['name'];

    // Ensure blank array.
    if (empty($form_state['values']['settings'][$plugin_type][$current_plugin->name])) {
      $form_state['values']['settings'][$plugin_type][$current_plugin->name] = array();
    }

    // Default values for current selection. If selection differs from current
    // job, then take the job into account.
    $defaults = $current_plugin->name == $original_plugin ? $job->settings : array();
    $defaults += $current_plugin->getDefaultSettings($job);

    // Plugin settings fieldset with vertical tab reference.
    $form['settings'][$plugin_type] = array(
      '#type' => 'fieldset',
      '#title' => $static['title singular proper'],
      '#group' => 'settings_tabs',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    // Ajax wrapper.
    $wrapper = 'wrapper-plugin-' . $plugin_type . '-settings';

    // Setup plugin selector.
    $options = array();
    $options[''] = t('Default (@default)', array(
      '@default' => $plugins[$job->hook[$plugin_type]['name']]->title,
    ));
    foreach ($plugins as $name => $plugin) {
      $options[$name] = $plugin->title;
    }
    $form['settings'][$plugin_type]['name'] = array(
      '#weight'        => -10,
      '#type'          => 'select',
      '#options'       => $options,
      '#default_value' => $form_state['values']['settings'][$plugin_type]['name'],
      '#title'         => $static['title singular proper'],
      '#description'   => t('Select which @plugin to use for this job.', array(
        '@plugin' => $static['title singular'],
      )),
      '#ajax' => array(
        'callback' => 'ultimate_cron_job_plugin_settings_ajax',
        'wrapper' => $wrapper,
        'method' => 'replace',
        'effect' => 'none',
      ),
    );

    $default_settings_link = l(
      t('(change default settings)'),
      'admin/config/system/cron/' . $current_plugin->type . '/' . $current_plugin->name
    );

    // Plugin specific settings wrapper for ajax replace.
    $form['settings'][$plugin_type][$current_plugin->name] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => $current_plugin->title,
      '#description' => $current_plugin->description,
      '#prefix' => '<div id="' . $wrapper . '">',
      '#suffix' => '</div>',
    );

    $form_state['default_values']['settings'][$plugin_type][$current_plugin->name] = $defaults;
    if (
      $current_plugin->name == $original_plugin &&
      isset($job->settings[$plugin_type][$current_plugin->name]) &&
      is_array($job->settings[$plugin_type][$current_plugin->name])
    ) {
      $form_state['values']['settings'][$plugin_type][$current_plugin->name] += $job->settings[$plugin_type][$current_plugin->name];
    }
    $form_state['values']['settings'][$plugin_type][$current_plugin->name] += ultimate_cron_blank_values($defaults);

    $current_plugin->settingsForm($form, $form_state, $job);
    if (empty($form['settings'][$plugin_type][$current_plugin->name]['no_settings'])) {
      $current_plugin->fallbackalize(
        $form['settings'][$plugin_type][$current_plugin->name],
        $form_state['values']['settings'][$plugin_type][$current_plugin->name],
        $form_state['default_values']['settings'][$plugin_type][$current_plugin->name],
        FALSE
      );
      $form['settings'][$plugin_type][$current_plugin->name]['#description'] .= ' ' . $default_settings_link . '.';
    }
  }

  /**
   * Job settings form validate handler.
   */
  static public function jobSettingsFormValidate($form, &$form_state, $plugin_type, $job = NULL) {
    $name = !empty($form_state['values']['settings'][$plugin_type]['name']) ? $form_state['values']['settings'][$plugin_type]['name'] : $job->hook[$plugin_type]['name'];
    $plugin = _ultimate_cron_plugin_load($plugin_type, $name);
    $plugin->settingsFormValidate($form, $form_state, $job);
  }

  /**
   * Job settings form submit handler.
   */
  static public function jobSettingsFormSubmit($form, &$form_state, $plugin_type, $job = NULL) {
    $name = !empty($form_state['values']['settings'][$plugin_type]['name']) ? $form_state['values']['settings'][$plugin_type]['name'] : $job->hook[$plugin_type]['name'];
    $plugin = _ultimate_cron_plugin_load($plugin_type, $name);
    $plugin->settingsFormSubmit($form, $form_state, $job);

    // Weed out blank values that have fallbacks.
    $elements = &$form['settings'][$plugin_type][$name];
    $values = &$form_state['values']['settings'][$plugin_type][$name];;
    $plugin->cleanForm(
      $elements,
      $values,
      array('settings', $plugin_type, $name)
    );
  }

  /**
   * Settings form.
   */
  public function settingsForm(&$form, &$form_state, $job = NULL) {
    $form['settings'][$this->type][$this->name]['no_settings'] = array(
      '#markup' => '<p>' . t('This plugin has no settings.') . '</p>',
    );
  }

  /**
   * Settings form validate handler.
   */
  public function settingsFormValidate(&$form, &$form_state, $job = NULL) {
  }

  /**
   * Settings form submit handler.
   */
  public function settingsFormSubmit(&$form, &$form_state, $job = NULL) {
  }

  /**
   * Process fallback form parameters.
   *
   * @param array $elements
   *   Elements to process.
   * @param array $defaults
   *   Default values to add to description.
   * @param bool $remove_non_fallbacks
   *   If TRUE, non fallback elements will be removed.
   */
  public function fallbackalize(&$elements, &$values, $defaults, $remove_non_fallbacks = FALSE) {
    if (empty($elements)) {
      return;
    }
    foreach (element_children($elements) as $child) {
      $element = &$elements[$child];
      if (empty($element['#tree'])) {
        $param_values = &$values;
        $param_defaults = &$defaults;
      }
      else {
        $param_values = &$values[$child];
        $param_defaults = &$defaults[$child];
      }
      $this->fallbackalize($element, $param_values, $param_defaults, $remove_non_fallbacks);

      if (empty($element['#type']) || $element['#type'] == 'fieldset') {
        continue;
      }

      if (!empty($element['#fallback'])) {
        if (!$remove_non_fallbacks) {
          if ($element['#type'] == 'radios') {
            $label = $this->settingsLabel($child, $defaults[$child]);
            $element['#options'] = array(
              '' => t('Default (@default)', array('@default' => $label)),
            ) + $element['#options'];
          }
          elseif ($element['#type'] == 'select' && empty($element['#multiple'])) {
            $label = $this->settingsLabel($child, $defaults[$child]);
            $element['#options'] = array(
              '' => t('Default (@default)', array('@default' => $label)),
            ) + $element['#options'];
          }
          elseif ($defaults[$child] !== '') {
            $element['#description'] .= ' ' . t('(Blank = @default).', array('@default' => $this->settingsLabel($child, $defaults[$child])));
          }
          unset($element['#required']);
        }
      }
      elseif (!empty($element['#type']) && $remove_non_fallbacks) {
        unset($elements[$child]);
      }
      elseif (!isset($element['#default_value']) || $element['#default_value'] === '') {
        $empty = $element['#type'] == 'checkbox' ? FALSE : '';
        $values[$child] = !empty($defaults[$child]) ? $defaults[$child] : $empty;
        $element['#default_value'] = $values[$child];
      }
    }
  }
}

class UltimateCronPluginMultiple extends UltimateCronPlugin {
  static public $multiple = TRUE;

  /**
   * Default settings form.
   */
  static public function defaultSettingsForm(&$form, &$form_state, $plugin_info) {
    $plugin_type = $plugin_info['type'];
    foreach (_ultimate_cron_plugin_load_all($plugin_type) as $name => $plugin) {
      if ($plugin->isValid()) {
        $plugins[] = l($plugin->title, "admin/config/system/cron/$plugin_type/$name");
      }
    }
    $form['available'] = array(
      '#markup' => theme('item_list', array(
        'title' => $plugin_info['defaults']['static']['title plural proper'] . ' available',
        'items' => $plugins,
      )),
    );
  }

  /**
   * Job settings form.
   */
  static public function jobSettingsForm(&$form, &$form_state, $plugin_type, $job) {
    // Check valid plugins.
    $plugins = _ultimate_cron_plugin_load_all($plugin_type);
    foreach ($plugins as $name => $plugin) {
      if (!$plugin->isValid($job)) {
        unset($plugins[$name]);
      }
    }

    // No plugins = no settings = no vertical tabs for you mister!
    if (empty($plugins)) {
      continue;
    }

    $weight = 10;
    $form_state['default_values']['settings'][$plugin_type] = array();
    $form['settings'][$plugin_type]['#tree'] = TRUE;
    foreach ($plugins as $name => $plugin) {
      $form_state['default_values']['settings'][$plugin_type][$name] = array();
      if (empty($form_state['values']['settings'][$plugin_type][$name])) {
        $form_state['values']['settings'][$plugin_type][$name] = array();
      }
      $form['settings'][$plugin_type][$name] = array(
        '#title' => $plugin->title,
        '#group' => 'settings_tabs',
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#visible' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => $weight++,
      );

      $defaults = $plugin->getDefaultSettings($job);

      $form_state['default_values']['settings'][$plugin_type][$name] += $defaults;
      $form_state['values']['settings'][$plugin_type][$name] += ultimate_cron_blank_values($defaults);

      $plugin->settingsForm($form, $form_state, $job);
      if (empty($form['settings'][$plugin_type][$name]['no_settings'])) {
        $plugin->fallbackalize(
          $form['settings'][$plugin_type][$name],
          $form_state['values']['settings'][$plugin_type][$name],
          $form_state['default_values']['settings'][$plugin_type][$name],
          FALSE
        );
      }
      else {
        unset($form['settings'][$plugin_type][$name]);
      }
    }
  }

  /**
   * Job settings form validate handler.
   */
  static public function jobSettingsFormValidate($form, &$form_state, $plugin_type, $job = NULL) {
    $plugins = _ultimate_cron_plugin_load_all($plugin_type);
    foreach ($plugins as $plugin) {
      if ($plugin->isValid($job)) {
        $plugin->settingsFormValidate($form, $form_state, $job);
      }
    }
  }

  /**
   * Job settings form submit handler.
   */
  static public function jobSettingsFormSubmit($form, &$form_state, $plugin_type, $job = NULL) {
    $plugins = _ultimate_cron_plugin_load_all($plugin_type);
    foreach ($plugins as $name => $plugin) {
      if ($plugin->isValid($job)) {
        $plugin->settingsFormSubmit($form, $form_state, $job);

        // Weed out blank values that have fallbacks.
        $elements = &$form['settings'][$plugin_type][$name];
        $values = &$form_state['values']['settings'][$plugin_type][$name];
        $plugin->cleanForm(
          $elements,
          $values,
          array('settings', $plugin_type, $name)
        );
      }
      else {
        unset($form_state['values']['settings'][$plugin_type][$name]);
      }
    }
  }
}

/**
 * Abstract class for Ultimate Cron schedulers
 *
 * A scheduler is responsible for telling Ultimate Cron whether a job should
 * run or not.
 *
 * Abstract methods:
 *   isScheduled($job)
 *     - Check if the given job is scheduled for launch at this time.
 *       TRUE if it's scheduled for launch, otherwise FALSE.
 *
 *   isBehind($job)
 *     - Check if the given job is behind its schedule.
 *       FALSE if not behind, otherwise the amount of time it's behind
 *       in seconds.
 */
abstract class UltimateCronScheduler extends UltimateCronPlugin {
  /**
   * Check job schedule.
   *
   * @param UltimateCronJob $job
   *   The job to check schedule for.
   *
   * @return bool
   *   TRUE if job is scheduled to run.
   */
  abstract public function isScheduled($job);

  /**
   * Check if job is behind schedule.
   *
   * @param UltimateCronJob $job
   *   The job to check schedule for.
   *
   * @return bool
   *   TRUE if job is behind its schedule.
   */
  abstract public function isBehind($job);
}

/**
 * Abstract class for Ultimate Cron launchers
 *
 * A launcher is responsible for locking and launching/running a job.
 *
 * Abstract methods:
 *   lock($job)
 *     - Lock a job. This method must return the lock_id on success
 *       or FALSE on failure.
 *
 *   unlock($lock_id, $manual = FALSE)
 *     - Release a specific lock id. If $manual is set, then the release
 *       was triggered manually by a user.
 *
 *   isLocked($job)
 *     - Check if a job is locked. This method must return the current
 *     - lock_id for the given job, or FALSE if it is not locked.
 *
 *   launch($job)
 *     - This method launches/runs the given job. This method must handle
 *       the locking of job before launching it. Returns TRUE on successful
 *       launch, FALSE if not.
 *
 * Important methods:
 *   isLockedMultiple($jobs)
 *     - Check locks for multiple jobs. Each launcher should implement an
 *       optimized version of this method if possible.
 *
 *   launchJobs($jobs)
 *     - Launches the jobs provided to it. A default implementation of this
 *       exists, but can be overridden. It is assumed that this function
 *       checks the jobs schedule before launching and that it also handles
 *       locking wrt concurrency for the launcher itself.
 *
 *   launchPoorman()
 *     - Launches all scheduled jobs via the proper launcher for each jobs.
 *       This method only needs to be implemented if the launcher wishes to
 *       provide a poormans cron launching mechanism. It is assumed that
 *       the poormans cron launcher handles locking wrt concurrency, etc.
 */
abstract class UltimateCronLauncher extends UltimateCronPlugin {

  /**
   * Default settings.
   */
  public function defaultSettings() {
    return array();
  }

  /**
   * Lock job.
   *
   * @param UltimateCronJob $job
   *   The job to lock.
   *
   * @return string
   *   Lock ID or FALSE.
   */
  abstract public function lock($job);

  /**
   * Unlock a lock.
   *
   * @param string $lock_id
   *   The lock id to unlock.
   * @param bool $manual
   *   Whether this is a manual unlock or not.
   *
   * @return bool
   *   TRUE on successful unlock.
   */
  abstract public function unlock($lock_id, $manual = FALSE);

  /**
   * Check if a job is locked.
   *
   * @param UltimateCronJob $job
   *   The job to check.
   *
   * @return string
   *   Lock ID of the locked job, FALSE if not locked.
   */
  abstract public function isLocked($job);

  /**
   * Launch job.
   *
   * @param UltimateCronJob $job
   *   The job to launch.
   *
   * @return bool
   *   TRUE on successful launch.
   */
  abstract public function launch($job);

  /**
   * Fallback implementation of multiple lock check.
   *
   * Each launcher should implement an optimized version of this method
   * if possible.
   *
   * @param array $jobs
   *   Array of UltimateCronJob to check.
   *
   * @return array
   *   Array of lock ids, keyed by job name.
   */
  public function isLockedMultiple($jobs) {
    $lock_ids = array();
    foreach ($jobs as $name => $job) {
      $lock_ids[$name] = $this->isLocked($job);
    }
  }

  /**
   * Run the job.
   *
   * @param UltimateCronJob $job
   *   The job to run.
   */
  public function run($job) {
    // Prevent session information from being saved while cron is running.
    $original_session_saving = drupal_save_session();
    drupal_save_session(FALSE);

    // Force the current user to anonymous to ensure consistent permissions on
    // cron runs.
    $original_user = $GLOBALS['user'];
    $GLOBALS['user'] = drupal_anonymous_user();

    $php_self = NULL;
    try {
      // Signal to whomever might be listening, that we're cron!
      // @investigate Is this safe? (He asked knowingly ...)
      $php_self = $_SERVER['PHP_SELF'] ? $_SERVER['PHP_SELF'] : '';
      $_SERVER['PHP_SELF'] = 'cron.php';

      $job->invoke();

      // Restore state.
      $_SERVER['PHP_SELF'] = $php_self;
    }
    catch (Exception $e) {
      // Restore state.
      if (isset($php_self)) {
        $_SERVER['PHP_SELF'] = $php_self;
      }

      watchdog('ultimate_cron', 'Error running @name: @error', array(
        '@name' => $job->name,
        '@error' => (string) $e,
      ), WATCHDOG_ERROR);
    }
    // Restore the user.
    $GLOBALS['user'] = $original_user;
    drupal_save_session($original_session_saving);
  }

  /**
   * Default implementation of jobs launcher.
   *
   * @param array $jobs
   *   Array of UltimateCronJob to launch.
   */
  public function launchJobs($jobs) {
    foreach ($jobs as $job) {
      if ($job->isScheduled()) {
        $job->launch();
      }
    }
  }

  /**
   * Format running state.
   */
  public function formatRunning($job) {
    $file = drupal_get_path('module', 'ultimate_cron') . '/icons/hourglass.png';
    $status = theme('image', array('path' => $file));
    $title = t('running');
    return array($status, $title);
  }

  /**
   * Format unfinished state.
   */
  public function formatUnfinished($job) {
    $file = drupal_get_path('module', 'ultimate_cron') . '/icons/lock_open.png';
    $status = theme('image', array('path' => $file));
    $title = t('unfinished but not locked?');
    return array($status, $title);
  }

  /**
   * Default implementation of formatProgress().
   *
   * @param UltimateCronJob $job
   *   Job to format progress for.
   *
   * @return string
   *   Formatted progress.
   */
  public function formatProgress($job, $progress) {
    $progress = $progress ? sprintf("(%d%%)", round($progress * 100)) : '';
    return $progress;
  }

  /**
   * Default implementation of initializeProgress().
   *
   * @param UltimateCronJob $job
   *   Job to initialize progress for.
   */
  public function initializeProgress($job) {
    $class = _ultimate_cron_get_class('progress');
    return $class::factory($job->name)->setProgress(FALSE);
  }

  /**
   * Default implementation of finishProgress().
   *
   * @param UltimateCronJob $job
   *   Job to finish progress for.
   */
  public function finishProgress($job) {
    $class = _ultimate_cron_get_class('progress');
    return $class::factory($job->name)->setProgress(FALSE);
  }

  /**
   * Default implementation of getProgress().
   *
   * @param UltimateCronJob $job
   *   Job to get progress for.
   *
   * @return float
   *   Progress for the job.
   */
  public function getProgress($job) {
    $class = _ultimate_cron_get_class('progress');
    return $class::factory($job->name)->getProgress();
  }

  /**
   * Default implementation of getProgressMultiple().
   *
   * @param UltimateCronJob $jobs
   *   Jobs to get progresses for, keyed by job name.
   *
   * @return array
   *   Progresses, keyed by job name.
   */
  public function getProgressMultiple($jobs) {
    $class = _ultimate_cron_get_class('progress');
    return $class::getProgressMultiple(array_keys($jobs));
  }

  /**
   * Default implementation of setProgress().
   *
   * @param UltimateCronJob $job
   *   Job to set progress for.
   * @param float $progress
   *   Progress (0-1).
   */
  public function setProgress($job, $progress) {
    $class = _ultimate_cron_get_class('progress');
    return $class::factory($job->name)->setProgress($progress);
  }

}

/**
 * Abstract class for Ultimate Cron loggers
 *
 * Each logger must implement its own functions for getting/setting data
 * from the its storage backend.
 *
 * Abstract methods:
 *   load($name, $lock_id = NULL)
 *     - Load a log entry. If no $lock_id is provided, this method should
 *       load the latest log entry for $name.
 *
 * "Abstract" properties:
 *   $log_entry_class
 *     - The class name of the log entry class associated with this logger.
 */
abstract class UltimateCronLogger extends UltimateCronPlugin {
  static public $log_entries = NULL;
  public $log_entry_class = 'UltimateCronLogEntry';

  /**
   * Factory method for creating a new unsaved log entry object.
   *
   * @param string $name
   *   Name of the log entry (name of the job).
   *
   * @return UltimateCronLogEntry
   *   The log entry.
   */
  public function factoryLogEntry($name) {
    return new $this->log_entry_class($name, $this);
  }

  /**
   * Create a new log entry.
   *
   * @param string $name
   *   Name of the log entry (name of the job).
   * @param string $lock_id
   *   The lock id.
   * @param string $init_message
   *   (optional) The initial message for the log entry.
   *
   * @return UltimateCronLogEntry
   *   The log entry created.
   */
  public function create($name, $lock_id, $init_message = '', $log_type = ULTIMATE_CRON_LOG_TYPE_NORMAL) {
    $log_entry = new $this->log_entry_class($name, $this, $log_type);
    $log_entry->lid = $lock_id;
    $log_entry->start_time = microtime(TRUE);
    $log_entry->init_message = $init_message;
    $log_entry->save();
    return $log_entry;
  }


  /**
   * Begin capturing messages.
   *
   * @param UltimateCronLogEntry $log_entry
   *   The log entry that should capture messages.
   */
  public function catchMessages($log_entry) {
    $class = get_class($this);
    if (!isset($class::$log_entries)) {
      $class::$log_entries = array();
      // Since we may already be inside a drupal_register_shutdown_function()
      // we cannot use that. Use PHPs register_shutdown_function() instead.
      ultimate_cron_register_shutdown_function(array($class, 'catchMessagesShutdownWrapper'), $class);
    }
    $class::$log_entries[$log_entry->lid] = $log_entry;
  }

  /**
   * End message capturing.
   *
   * Effectively disables the shutdown function for the given log entry.
   *
   * @param UltimateCronLogEntry $log_entry
   *   The log entry.
   */
  public function unCatchMessages($log_entry) {
    $class = get_class($this);
    unset($class::$log_entries[$log_entry->lid]);
  }

  /**
   * Invoke loggers watchdog hooks.
   *
   * @param array $log_entry
   *   Watchdog log entry array.
   */
  final static public function hook_watchdog(array $log_entry) {
    if (self::$log_entries) {
      foreach (self::$log_entries as $log_entry_object) {
        $log_entry_object->watchdog($log_entry);
      }
    }
  }

  /**
   * Log to ultimate cron logs only.
   *
   * @see watchdog()
   */
  final static public function log($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
    if (self::$log_entries) {
      foreach (self::$log_entries as $log_entry_object) {
        $log_entry_object->log($type, $message, $variables, $severity, $link);
      }
    }
  }

  /**
   * Shutdown handler wrapper for catching messages.
   *
   * @param string $class
   *   The class in question.
   */
  static public function catchMessagesShutdownWrapper($class) {
    if ($class::$log_entries) {
      foreach ($class::$log_entries as $log_entry) {
        $log_entry->logger->catchMessagesShutdown($log_entry);
      }
    }
  }

  /**
   * PHP shutdown function callback.
   *
   * Ensures that a log entry has been closed properly on shutdown.
   *
   * @param UltimateCronLogEntry $log_entry
   *   The log entry to close.
   */
  public function catchMessagesShutdown($log_entry) {
    $this->unCatchMessages($log_entry);

    if ($log_entry->finished) {
      return;
    }

    // Get error messages.
    $error = error_get_last();
    if ($error) {
      $message = $error['message'] . ' (line ' . $error['line'] . ' of ' . $error['file'] . ').' . "\n";
      $severity = WATCHDOG_INFO;
      if ($error['type'] && (E_NOTICE || E_USER_NOTICE || E_USER_WARNING)) {
        $severity = WATCHDOG_NOTICE;
      }
      if ($error['type'] && (E_WARNING || E_CORE_WARNING || E_USER_WARNING)) {
        $severity = WATCHDOG_WARNING;
      }
      if ($error['type'] && (E_ERROR || E_CORE_ERROR || E_USER_ERROR || E_RECOVERABLE_ERROR)) {
        $severity = WATCHDOG_ERROR;
      }

      $log_entry->log($log_entry->name, $message, array(), $severity);
    }
    $log_entry->finish();
  }

  /**
   * Load latest log entry for multiple jobs.
   *
   * This is the fallback method. Loggers should implement an optimized
   * version if possible.
   */
  public function loadLatestLogEntries($jobs, $log_types) {
    $logs = array();
    foreach ($jobs as $job) {
      $logs[$job->name] = $job->loadLatestLogEntry($log_types);
    }
    return $logs;
  }

  /**
   * Load a log.
   *
   * @param string $name
   *   Name of log.
   * @param string $lock_id
   *   Specific lock id.
   *
   * @return UltimateCronLogEntry
   *   Log entry
   */
  abstract public function load($name, $lock_id = NULL, $log_types = array(ULTIMATE_CRON_LOG_TYPE_NORMAL));

  /**
   * Get page with log entries for a job.
   *
   * @param string $name
   *   Name of job.
   * @param array $log_types
   *   Log types to get.
   * @param int $limit
   *   (optional) Number of log entries per page.
   *
   * @return array
   *   Log entries.
   */
  abstract public function getLogEntries($name, $log_types, $limit = 10);
}

/**
 * Abstract class for Ultimate Cron log entries.
 *
 * Each logger must implement its own log entry class based on this one.
 *
 * Abstract methods:
 *   save()
 *     - Save the actual log entry to whereever you please.
 *
 * Important properties:
 *   $log_entry_size
 *     - The maximum number of characters of the message in the log entry.
 */
abstract class UltimateCronLogEntry {
  public $lid = NULL;
  public $name = '';
  public $log_type = ULTIMATE_CRON_LOG_TYPE_NORMAL;
  public $uid = NULL;
  public $start_time = 0;
  public $end_time = 0;
  public $init_message = '';
  public $message = '';
  public $severity = -1;

  // Default 1MiB log entry.
  public $log_entry_size = 1048576;

  public $log_entry_fields = array(
    'lid', 'uid', 'log_type', 'start_time', 'end_time', 'init_message', 'message', 'severity',
  );

  public $logger;
  public $job;
  public $finished = FALSE;

  /**
   * Constructor.
   *
   * @param string $name
   *   Name of log.
   * @param UltimateCronLogger $logger
   *   A logger object.
   */
  public function __construct($name, $logger, $log_type = ULTIMATE_CRON_LOG_TYPE_NORMAL) {
    $this->name = $name;
    $this->logger = $logger;
    $this->log_type = $log_type;
    if (!isset($this->uid)) {
      global $user;
      $this->uid = $user->uid;
    }
  }

  /**
   * Get current log entry data as an associative array.
   *
   * @return array
   *   Log entry data.
   */
  public function getData() {
    $result = array();
    foreach ($this->log_entry_fields as $field) {
      $result[$field] = $this->$field;
    }
    return $result;
  }

  /**
   * Set current log entry data from an associative array.
   *
   * @param array $data
   *   Log entry data.
   */
  public function setData($data) {
    foreach ($this->log_entry_fields as $field) {
      if (array_key_exists($field, $data)) {
        $this->$field = $data[$field];
      }
    }
  }

  /**
   * Finish a log and save it if applicable.
   */
  public function finish() {
    if (!$this->finished) {
      $this->logger->unCatchMessages($this);
      $this->end_time = microtime(TRUE);
      $this->finished = TRUE;
      $this->save();
    }
  }

  /**
   * Implements hook_watchdog().
   *
   * Capture watchdog message and append it to the log entry.
   */
  public function watchdog(array $log_entry) {
    if (isset($log_entry['variables']) && is_array($log_entry['variables'])) {
      $this->message .= t($log_entry['message'], $log_entry['variables']) . "\n";
    }
    else {
      $this->message .= $log_entry['message'];
    }
    if ($this->severity < 0 || $this->severity > $log_entry['severity']) {
      $this->severity = $log_entry['severity'];
    }
    // Make sure that message doesn't become too big.
    if (mb_strlen($this->message) > $this->log_entry_size) {
      while (mb_strlen($this->message) > $this->log_entry_size) {
        $firstline = mb_strpos(rtrim($this->message, "\n"), "\n");
        if ($firstline === FALSE || $firstline == mb_strlen($this->message)) {
          // Only one line? That's a big line ... truncate it without mercy!
          $this->message = mb_substr($this->message, -$this->log_entry_size);
          break;
        }
        $this->message = substr($this->message, $firstline + 1);
      }
      $this->message = '.....' . $this->message;
    }
  }

  /**
   * Re-implementation of watchdog().
   *
   * @see watchdog()
   */
  public function log($type, $message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
    global $user, $base_root;

    // The user object may not exist in all conditions,
    // so 0 is substituted if needed.
    $user_uid = isset($user->uid) ? $user->uid : 0;

    // Prepare the fields to be logged.
    $log_entry = array(
      'type'        => $type,
      'message'     => $message,
      'variables'   => $variables,
      'severity'    => $severity,
      'link'        => $link,
      'user'        => $user,
      'uid'         => $user_uid,
      'request_uri' => $base_root . request_uri(),
      'referer'     => isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '',
      'ip'          => ip_address(),
      // Request time isn't accurate for long processes, use time() instead.
      'timestamp'   => time(),
    );
    $this->watchdog($log_entry);
  }

  /**
   * Start catching watchdog messages.
   */
  public function catchMessages() {
    return $this->logger->catchMessages($this);
  }

  /**
   * Stop catching watchdog messages.
   */
  public function unCatchMessages() {
    return $this->logger->unCatchMessages($this);
  }

  /**
   * Get duration.
   */
  public function getDuration() {
    $duration = 0;
    if ($this->start_time && $this->end_time) {
      $duration = (int) ($this->end_time - $this->start_time);
    }
    elseif ($this->start_time) {
      $duration = (int) (microtime(TRUE) - $this->start_time);
    }
    return $duration;
  }

  /**
   * Format duration.
   */
  public function formatDuration() {
    $duration = $this->getDuration();
    switch (TRUE) {
      case $duration >= 86400:
        $format = 'd H:i:s';
        break;

      case $duration >= 3600:
        $format = 'H:i:s';
        break;

      default:
        $format = 'i:s';
    }
    return isset($duration) ? gmdate($format, $duration) : t('N/A');
  }

  /**
   * Format start time.
   */
  public function formatStartTime() {
    return $this->start_time ? format_date((int) $this->start_time, 'custom', 'Y-m-d H:i:s') : t('Never');
  }

  /**
   * Format end time.
   */
  public function formatEndTime() {
    return $this->end_time ? t('Previous run finished @ @end_time', array(
        '@end_time' => format_date((int) $this->end_time, 'custom', 'Y-m-d H:i:s'))) : '';
  }

  /**
   * Format user.
   */
  public function formatUser() {
    $username = t('anonymous') . ' (0)';
    if ($this->uid) {
      $user = user_load($this->uid);
      $username = $user ? $user->name . " ($user->uid)" : t('N/A');
    }
    return $username;
  }

  /**
   * Format initial message.
   */
  public function formatInitMessage() {
    if ($this->start_time) {
      return $this->init_message ? $this->init_message . ' ' . t('by') . ' ' . $this->formatUser() : t('N/A');
    }
    else {
      $registered = variable_get('ultimate_cron_hooks_registered', array());
      return !empty($registered[$this->name]) ? t('Registered at @datetime', array(
        '@datetime' => format_date($registered[$this->name], 'custom', 'Y-m-d H:i:s'),
      )) : t('N/A');
    }
  }

  /**
   * Format severity.
   */
  public function formatSeverity() {
    switch ($this->severity) {
      case WATCHDOG_EMERGENCY:
      case WATCHDOG_ALERT:
      case WATCHDOG_CRITICAL:
      case WATCHDOG_ERROR:
        $file = 'misc/message-16-error.png';
        break;

      case WATCHDOG_WARNING:
        $file = 'misc/message-16-warning.png';
        break;

      case WATCHDOG_NOTICE:
        $file = 'misc/message-16-info.png';
        break;

      case WATCHDOG_INFO:
      case WATCHDOG_DEBUG:
      default:
        $file = 'misc/message-16-ok.png';
    }
    $status = theme('image', array('path' => $file));
    $severity_levels = array(
      -1 => t('no info'),
    ) + watchdog_severity_levels();
    $title = $severity_levels[$this->severity];
    return array($status, $title);
  }

  /**
   * Save log entry.
   */
  abstract public function save();
}

/**
 * Base class for settings.
 *
 * There's nothing special about this plugin.
 */
class UltimateCronSettings extends UltimateCronPluginMultiple {
}

/**
 * Base class for tagged settings.
 *
 * Settings plugins using this as a base class, will only be available
 * to jobs having the same tag as the name of the plugin.
 */
class UltimateCronTaggedSettings extends UltimateCronSettings {
  /**
   * Only valid for jobs tagged with the proper tag.
   */
  public function isValid($job = NULL) {
    return $job ? in_array($this->name, $job->hook['tags']) : parent::isValid();
  }
}
