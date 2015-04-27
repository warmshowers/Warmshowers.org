<?php


/**
 * Provides a basic display plugin that executes periodically.
 */
class views_periodic_execution_plugin_display extends views_plugin_display {

  var $_old_account;

  function option_definition() {
    $options = parent::option_definition();
    $options['interval'] = array('default' => 60*60);

    return $options;
  }

  function _option_intervals() {
    static $computed_options;

    if (!is_null($computed_options)) {
      return $computed_options;
    }

    $options = array(
      60 * 30, // 30 Minutes
      60 * 60, // 1 Hour
      60 * 60 * 6, // 6 Hours
      60 * 60 * 12, // 12 Hours
      60 * 60 * 24, // 24 Hours
      60 * 60 * 24 * 7, // 7 Days
      60 * 60 * 24 * 28, // 28 Days
      60 * 60 * 24 * 365, // 365 Days
    );

    $computed_options = drupal_map_assoc($options, 'format_interval');

    // Add an option to trigger the action every time the cron is launched, at
    // the beginning of the list and an option to never execute at the end:
    $computed_options = array(0 => t('As often as possible (All cron calls)'))
      + $computed_options
      + array(-1 => t('Never execute automatically (No cron calls)'));


    return $computed_options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['periodic_execution'] = array(
      'title' => t('Periodic Execution'),
    );
    $intervals = $this->_option_intervals();
    $interval = $intervals[$this->get_option('interval')];

    $options['interval'] = array(
      'category' => 'periodic_execution',
      'title' => t('Interval'),
      'value' => $interval,
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'interval':
        $form['#title'] .= t('Execution interval');
        //$form['#help_topic'] = 'interval';
        //$form['#help_module'] = 'rules_views_executor';
        $form['interval'] = array(
          '#type' => 'select',
          '#description' => t('Choose how often this view should be automatically executed.'),
          '#default_value' => $this->get_option('interval'),
          '#options' => $this->_option_intervals(),
        );
        break;
    }
  }

  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'interval':
        $this->set_option('interval', $form_state['values']['interval']);
        break;
    }
  }

  /**
   * Get the execution interval for this display.
   */
  function get_interval() {
    return $this->get_option('interval');
  }

  /**
   * Get the execution interval for this display.
   */
  function get_last_execution_time() {
    // Just pass through to the parent module:
    return views_periodic_execution_view_last_executed_time_get($this->view->name, $this->display->id);
  }

  /**
   * Set the execution interval for this display.
   */
  function set_last_execution_time($time = NULL) {
    if (is_null($time)) {
      $time = time();
    }
    // Pass through to the parent module:
    views_periodic_execution_view_last_executed_time_set($this->view->name, $this->display->id, $time);
  }

  /**
   * Does the display use the root account?
   */
  function use_root_account() {
    if (!empty($this->definition['use root account'])) {
      return $this->definition['use root account'];
    }
    return FALSE;
  }

  /**
   * Sets up the execution of this display.
   *
   * This should be called before the execute() method. It will optionally set
   * the Drupal user to the root user.
   */
   function set_up() {
     if ($this->use_root_account()) {
       // We need to switch the current user over to the root Drupal user:
       global $user;
       $this->_old_account = $user;

       $account = user_load(1);
       if ($account) {
         // This next line is important:
         drupal_save_session(FALSE);
         $user = $account;
       }

     }

   }

   /**
    * Cleans up after execution of this display.
    */
    function tear_down() {
      if ($this->use_root_account() && !empty($this->_old_account)) {
        // We need to switch the current user back to what it was:
        global $user;
        $user = $this->_old_account;

        // Now we can start saving the session again:
        drupal_save_session(TRUE);


      }

    }

  /**
   * Render this display.
   *
   * Child classes that override this method should really, really call
   * the tear_down() method when they are done.
   */
  function render() {
    $output = theme($this->theme_functions(), array('view' => $this->view));

    // Record that we were executed:
    if (empty($this->view->live_preview)) {
      $this->set_last_execution_time();
    }

    $this->tear_down();

    return $output;
  }

  function execute() {
    return $this->view->render($this->display->id);
  }

  /**
   * Set up any variables on the view prior to execution. These are separated
   * from execute because they are extremely common and unlikely to be
   * overridden on an individual display.
   */
  function pre_execute() {
    parent::pre_execute();

    $this->set_up();
  }


}
