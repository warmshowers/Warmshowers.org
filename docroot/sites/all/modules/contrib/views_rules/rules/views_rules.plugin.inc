<?php
/**
 * @file
 * Views Rules plugin implementations.
 */

/**
 * Loop plugin for using results from a view with Rules displays.
 */
class ViewsRulesLoop extends RulesActionContainer implements ViewsRulesIterable {

  protected $itemName = 'view loop';

  protected $view;

  /**
   * @var RulesState
   */
  protected $viewLoopState;

  public function __construct($viewName = NULL, $displayName = NULL, $settings = array()) {
    $this->info += array(
      'view_name' => $viewName,
      'display_name' => $displayName,
    );
    $this->settings = (array) $settings + $this->settings;
    $this->setUp();
  }

  public function integrityCheck() {
    // Check view is configured.
    $view = $this->getView();
    if (!$view) {
      throw new RulesIntegrityException(t('%plugin: View display is not configured.', array('%plugin' => $this->getPluginName())), $this);
    }

    // Check view display is an iterator.
    $display = $view->display_handler;
    if (!isset($display)) {
      throw new RulesIntegrityException(t('%plugin: Configured view display is missing.', array('%plugin' => $this->getPluginName())), $this);
    }
    if (!$display instanceof views_rules_iterator) {
      throw new RulesIntegrityException(t('%plugin: Configured view display is not a Rules iterator.', array('%plugin' => $this->getPluginName())), $this);
    }

    // Validate view display.
    if (!$view->validate()) {
      throw new RulesIntegrityException(t('%plugin: Configured view does not validate.', array('%plugin' => $this->getPluginName())), $this);
    }

    parent::integrityCheck();

    // Check row variables.
    /** @var $display views_rules_iterator */
    foreach ($display->get_rules_variable_info() as $name => $info) {
      if (isset($this->settings[$name . ':var'])) {
        $this->checkVarName($this->settings[$name . ':var']);
      }
    }
  }

  public function evaluate(RulesState $state) {
    try {
      if ($iterator = $this->getViewIterator()) {
        // Prepare view arguments.
        $arguments = array_intersect_key($this->getExecutionArguments($state), $this->pluginParameterInfo());
        $arguments = array_values(rules_unwrap_data($arguments));
        // Execute view.
        $this->viewLoopState = $state;
        try {
          $iterator->execute_iterator($arguments, $this);
        }
        catch (views_rules_iterator_exception $e) {
          throw new RulesEvaluationException($e->getMessage(), array(), $this);
        }
        unset($this->viewLoopState);
      }
      else {
        throw new RulesEvaluationException('Unable to evaluate invalid view iterator.', array(), $this);
      }
    }
    catch (RulesEvaluationException $e) {
      rules_log($e->msg, $e->args, $e->severity);
      rules_log('Unable to evaluate %name.', array('%name' => $this->getPluginName()), RulesLog::WARN, $this);
    }
    $this->resetView();
  }

  public function evaluateRow(array $data) {
    // Clone state to evaluate children in a sandbox.
    $rowState = clone $this->viewLoopState;

    // Fill in state data.
    foreach ($this->rowVariables() as $name => $info) {
      $rowState->addVariable($name, $data[$info['source name']], $info);
    }
    parent::evaluate($rowState);

    // Update variables from parent scope.
    foreach ($this->viewLoopState->variables as $key => &$value) {
      if (array_key_exists($key, $rowState->variables)) {
        $value = $rowState->variables[$key];
      }
    }
  }

  public function pluginParameterInfo() {
    if ($iterator = $this->getViewIterator()) {
      return $iterator->get_rules_parameter_info();
    }
    return array();
  }

  protected function stateVariables($element = NULL) {
    $variables = parent::stateVariables($element);

    // Add row variables to state.
    $variables += $this->rowVariables();

    return $variables;
  }

  public function rowVariables() {
    if ($iterator = $this->getViewIterator()) {
      $variables = array();
      foreach ($iterator->get_rules_variable_info() as $name => $info) {
        if (isset($this->settings[$name . ':var'])) {
          $variables[$this->settings[$name . ':var']] = array(
            'source name' => $name,
            'type' => $info['type'],
            'label' => $this->settings[$name . ':label'],
          );
        }
      }
      return $variables;
    }
    // Return no variable.
    return array();
  }

  /**
   * @param bool $reset
   *   Whether to reset the iterator by retrieving a new one.
   * @return view
   */
  public function getView($reset = FALSE) {
    if (!isset($this->view) || $reset) {
      $view = views_get_view($this->info['view_name'], $reset);
      if ($view && $view->set_display($this->info['display_name'])) {
        $this->view = $view;
      }
    }
    return $this->view;
  }

  /**
   * @param bool $reset
   *   Whether to reset the iterator by retrieving a new one.
   * @return views_rules_iterator
   */
  public function getViewIterator($reset = FALSE) {
    if ($view = $this->getView($reset)) {
      return $view->display_handler instanceof views_rules_iterator ? $view->display_handler : NULL;
    }
    return NULL;
  }

  public function label() {
    $view = $this->getView();
    return t('Views loop: @view - @display', array(
      '@view' => ($viewLabel = $view->get_human_name()) ? $viewLabel : $view->name,
      '@display' => $view->display_handler->display->display_title,
    ));
  }

  public function dependencies() {
    // TODO Remove once http://drupal.org/node/1682524 is fixed.
    $modules = array_flip(RulesPlugin::dependencies());
    $modules += array_flip(parent::dependencies());
    return array_keys($modules);
  }

  protected function exportChildren($key = NULL) {
    return parent::exportChildren('DO');
  }

  protected function exportFlat() {
    return FALSE;
  }

  protected function exportSettings() {
    $export = array();
    $export['VIEW'] = $this->info['view_name'];
    $export['DISPLAY'] = $this->info['display_name'];
    $export += parent::exportSettings();
    $export['ROW VARIABLES'] = array();
    foreach ($this->rowVariables() as $name => $info) {
      $export['ROW VARIABLES'][$info['source name']][$name] = $info['label'];
    }
    return $export;
  }

  protected function importChildren($export, $key = NULL) {
    parent::importChildren($export, 'DO');
  }

  protected function importSettings($export) {
    // Import view.
    $this->info['view_name'] = $export['VIEW'];
    $this->info['display_name'] = $export['DISPLAY'];
    // Import row variables.
    foreach ($export['ROW VARIABLES'] as $name => $variable) {
      $this->settings[$name . ':var'] = key($variable);
      $this->settings[$name . ':label'] = current($variable);
    }
    parent::importSettings($export);
  }

  public function resetInternalCache() {
    parent::resetInternalCache();
    $this->resetView();
  }

  public function resetView() {
    $this->view = NULL;
  }
}

/**
 * Interface for an iterable Rules plugin for calling back from Views.
 */
interface ViewsRulesIterable {
  /**
   * Evaluates a view row in the loop.
   */
  public function evaluateRow(array $data);
}
