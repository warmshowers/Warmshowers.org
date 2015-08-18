<?php
/**
 * @file
 * Action implementation.
 */

/**
 * Collects view results.
 */
function views_rules_collect_rows($view_display_id) {
  // Load iterator.
  if (!$view = views_rules_get_view($view_display_id)) {
    // Fail.
    return array();
  }
  /** @var $iterator views_rules_iterator */
  $iterator = $view->display_handler;

  // Collect arguments to give the view.
  $args = func_get_args();
  array_shift($args);

  // Create a collector.
  $collector = new ViewsRulesResultCollector(array_keys($iterator->get_rules_variable_info()));
  $iterator->execute_iterator($args, $collector);

  // Return data.
  $data = $collector->getData();
  return $data;
}

/**
 * Alters info for the action.
 */
function views_rules_collect_rows_info_alter(&$info, RulesAbstractPlugin $element) {
  // Add view display variables.
  if (!empty($element->settings['views_rules_display']) && $view = views_rules_get_view($element->settings['views_rules_display'])) {
    // Abort if view does not validate.
    if (!$view->validate()) {
      return;
    }
    /** @var $iterator views_rules_iterator */
    $iterator = $view->display_handler;
    if (!$iterator instanceof views_rules_iterator) {
      return;
    }
    // Add view arguments.
    foreach ($iterator->get_rules_parameter_info() as $var_name => $var_info) {
      $info['parameter'][$var_name] = $var_info;
    }
    // Add view row variable lists.
    foreach ($iterator->get_rules_variable_info() as $var_name => $var_info) {
      $type = $var_info['type'];
      $label = $var_info['label'];
      $info['provides'][$var_name] = array(
        // Change variable to list.
        'type' => "list<$type>",
        'label' => t('List of @label', array('@label' => $label)),
      ) + $var_info;
    }
  }
}

/**
 * Validates the action.
 */
function views_rules_collect_rows_validate(RulesPlugin $element) {
  $view_display_id = $element->settings['views_rules_display'];
  $view = views_rules_get_view($view_display_id);
  if (!$view || !$view->display_handler) {
    throw new RulesIntegrityException(t('The view display %display does not exist.', array('%display' => $view_display_id)), $element);
  }
  if (!$view->display_handler instanceof views_rules_iterator) {
    throw new RulesIntegrityException(t('The view display %display is not a Rules display.', array('%display' => $view_display_id)), $element);
  }
  if (!$view->validate()) {
    throw new RulesIntegrityException(t('The view display %display does not validate.', array('%display' => $view_display_id)), $element);
  }
}

/**
 * Collector for view iterator results.
 */
class ViewsRulesResultCollector implements ViewsRulesIterable {
  /**
   * Variables to collect data for.
   * @var array
   */
  protected $variables;

  /**
   * Collected data.
   * @var array
   */
  protected $data = array();

  /**
   * Creates a collector.
   *
   * @param array $variables
   *   Names of variables to collect.
   */
  public function __construct(array $variables) {
    $this->variables = $variables;
    foreach ($this->variables as $variable) {
      $this->data[$variable] = array();
    }
  }

  /**
   * Evaluates a view row in the loop.
   */
  public function evaluateRow(array $data) {
    // Collect data for each variable.
    foreach ($this->variables as $variable) {
      $this->data[$variable][] = isset($data[$variable]) ? $data[$variable] : NULL;
    }
  }

  /**
   * Returns collected data.
   */
  public function getData() {
    return $this->data;
  }
}
