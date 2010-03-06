<?php
// $Id: views_bulk_operations_plugin_style.inc,v 1.1.2.48 2010/02/04 00:43:02 kratib Exp $

class views_bulk_operations_plugin_style extends views_plugin_style_table {
  function options(&$options) {
    parent::options($options);
    $options['selected_operations'] = array();
    $options['execution_type'] = VBO_EXECUTION_DIRECT;
    $options['display_type'] = 0;
    $options['skip_confirmation'] = FALSE;
    $options['display_result'] = TRUE;
    $options['merge_single_action'] = TRUE;
    $options['hide_select_all'] = FALSE;
    $this->populate_operations($options);

    // Per-action settings.
    foreach ($this->options['all_operations'] as $key => $operation) {
      $form_function = $operation['callback'] . '_settings_form_options';
      if (function_exists($form_function)) {
        $options[$key] = call_user_func($form_function);
      }
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $execution = array(
      VBO_EXECUTION_DIRECT => t('Invoke them directly'),
      VBO_EXECUTION_BATCH => t('Use Batch API'),
    );
    if (module_exists('job_queue')) {
      $execution[VBO_EXECUTION_QUEUE] = t('Use <a href="@jobqueue">Job Queue</a>', array('@jobqueue' => url('http://drupal.org/project/job_queue'))); 
    }
    $form['execution_type'] = array(
      '#type' => 'radios',
      '#title' => t('To execute operations'),
      '#default_value' => $this->options['execution_type'],
      '#options' => $execution,
    );
    $form['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['hide_select_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide select all checkbox'),
      '#description' => t('Check this box to hide the "select all" checkbox and associated "select across all pages" button.'),
      '#default_value' => $this->options['hide_select_all'],
    );
    $form['skip_confirmation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip confirmation step'),
      '#description' => t('Check this box to skip the confirmation page on this view and directly execute the operation.'),
      '#default_value' => $this->options['skip_confirmation'],
    );
    $form['display_result'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display processing result'),
      '#description' => t('Check this box to let Drupal display a message with the result of processing the selected objects.'),
      '#default_value' => $this->options['display_result'],
    );
    $form['merge_single_action'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge single action\'s form with node selection view'),
      '#description' => t('In case only one action is selected *and* this action is configurable, display its action form along with the node selection view.'),
      '#default_value' => $this->options['merge_single_action'],
    );
    $form['selected_operations'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selected operations'),
      '#options' => $this->get_operations_options(), 
      '#default_value' => $this->options['selected_operations'],
    );

    // Per-action settings form.
    $object_info = _views_bulk_operations_object_info_for_view($this->view);
    foreach ($this->options['all_operations'] as $key => $operation) {
      if ($operation['type'] != 'system' && $operation['type'] != $object_info['type']) continue;
      $form_function = $operation['callback'] . '_settings_form';
      if (function_exists($form_function)) {
        $form[$key] = array(
          '#tree' => TRUE,
          '#type' => 'fieldset',
          '#title' => $operation['label'],
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        $form[$key] += call_user_func($form_function, $this->options[$key]);
      }
    }
  }

  function options_validate(&$form, &$form_state) {
    foreach (array_filter($form_state['values']['style_options']['selected_operations']) as $selected_operation) {
      if (!isset($form_state['values']['style_options'][$selected_operation])) continue;
      $operation = $this->options['all_operations'][$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_validate';
      if (function_exists($form_function)) {
        $form_state['values']['style_options'][$selected_operation]['_error_element_base'] = 'style_options][' . $selected_operation . '][';
        call_user_func($form_function, $form, array('values' => $form_state['values']['style_options'][$selected_operation]));
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    foreach (array_filter($form_state['values']['style_options']['selected_operations']) as $selected_operation) {
      if (!isset($form_state['values']['style_options'][$selected_operation])) continue;
      $operation = $this->options['all_operations'][$selected_operation];
      $form_function = $operation['callback'] . '_settings_form_submit';
      if (function_exists($form_function)) {
        call_user_func($form_function, $form, array('values' => $form_state['values']['style_options'][$selected_operation]));
      }
    }
  }

  function render() {
    // We build the groups here to pass them to the node_selector function through the form.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    $this->sets = $sets;

    // Append suffix to avoid clashing between multiple VBOs on same page.
    static $form_suffix;
    if (isset($form_suffix)) {
      $form_suffix++;
    }
    else {
      $form_suffix = 1;
    }
    return drupal_get_form('views_bulk_operations_form__' . $form_suffix, $this);
  }

  function get_selected_operations() {
    $selected = array();
    foreach (array_filter($this->options['selected_operations']) as $key) {
      if (module_exists('actions_permissions')) {
        $perm = actions_permissions_get_perm($this->options['all_operations'][$key]['label'], $this->options['all_operations'][$key]['callback']);
        if (!user_access($perm)) continue;
      }
      if (!empty($this->options['all_operations'][$key]['permissions'])) foreach ($this->options['all_operations'][$key]['permissions'] as $perm) {
        if (!user_access($perm)) continue 2;
      }
      if (isset($this->options['all_operations'][$key])) {
        $selected[$key] = $this->options['all_operations'][$key]['label'];
      }
    }
    return $selected;
  }

  function get_operation_info($key) {
    $operation = $this->options['all_operations'][$key];
    if ($operation['type'] == 'system') { // System actions: morph them into current view type
      $object_info = _views_bulk_operations_object_info_for_view($this->view);
      $operation['type'] = $object_info['type'];
    }
    return $operation;
  }

  private function get_operations_options() {
    $options = array();
    $object_info = _views_bulk_operations_object_info_for_view($this->view);
    if (!$object_info) return NULL;
    foreach ($this->options['all_operations'] as $key => $operation) {
      if ($operation['type'] == 'system') { // System actions: morph them into current view type
        $operation['type'] = $object_info['type'];
      }
      if ($operation['type'] == $object_info['type']) {
        $options[$key] = $operation['label'] .' ('. $key .')'; 
      }
    }
    return $options;
  }

  private function populate_operations(&$options) {
    $operations = array();

    module_load_include('inc', 'node', 'node.admin');
    foreach (_views_bulk_operations_get_object_info() as $object_type => $object_info) {
      $hook_name = $object_type .'_operations';
      foreach (module_invoke_all($hook_name) as $operation) {
        if (empty($operation['callback'])) continue;
        $key = $operation['callback'] . (empty($operation['callback arguments']) ? '' : ':'. md5(serialize($operation['callback arguments'])));
        if (!isset($operation['behavior'])) { // assume operations modify nodes by default
          $operation['behavior'] = array('changes_node_property');
        }
        $operations[$key] = array(
          'key'                 => $key,
          'label'               => $operation['label'],
          'callback'            => $operation['callback'],
          'callback arguments'  => isset($operation['callback arguments']) ? $operation['callback arguments'] : array(),
          'configurable'        => isset($operation['configurable']) ? $operation['configurable'] : FALSE,
          'source'              => 'operation',
          'type'                => $object_type,
          'aggregate'           => isset($operation['aggregate']) ? (int)$operation['aggregate'] : VBO_AGGREGATE_OPTIONAL,
          'access op'           => $this->get_access_op($operation),
          'permissions'         => isset($operation['permissions']) ? $operation['permissions'] : NULL,
        );
      }
    }

    $action_operations = actions_list() + $this->get_custom_actions();
    foreach ($action_operations as $callback => $operation) {
      $key = isset($operation['key']) ? $operation['key'] : $callback;
      $operations[$key] = array(
        'key'                   => $key,
        'label'                 => $operation['description'],
        'callback'              => $callback,
        'callback arguments'    => isset($operation['parameters']) ? $operation['parameters'] : array(),
        'configurable'          => isset($operation['configurable']) ? $operation['configurable'] : FALSE,
        'source'                => 'action',
        'type'                  => $operation['type'],
        'aggregate'             => VBO_AGGREGATE_FORBIDDEN,
        'access op'             => $this->get_access_op($operation),
        'permissions'           => isset($operation['permissions']) ? $operation['permissions'] : NULL,
      );
    }

    uasort($operations, create_function('$a, $b', 'return strcasecmp($a["label"], $b["label"]);'));
    $options['all_operations'] = $operations;
  }

  private function get_access_op($operation) {
    $access_op = 0;
    if (isset($operation['behavior'])) {
      if (in_array('views_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_VIEW;
      }
      if (in_array('changes_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_UPDATE;
      }
      if (in_array('creates_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_CREATE;
      }
      if (in_array('deletes_node_property', $operation['behavior'])) {
        $access_op |= VBO_ACCESS_OP_DELETE;
      }
    }
    return $access_op;
  }

  private function get_custom_actions() {
    $actions = array();
    $static_actions = actions_list();
    $result = db_query("SELECT * FROM {actions} WHERE parameters > ''");
    while ($action = db_fetch_object($result)) {
      $parameters = unserialize($action->parameters);
      $actions[$action->aid] = array(
        'description' => $action->description,
        'type' => $action->type,
        'configurable' => FALSE,
        'parameters' => $parameters,
        'key' => $action->callback . (empty($parameters) ? '' : ':'. md5($action->parameters)),
      );
      foreach (array('behavior', 'permissions') as $attribute) {
        if (isset($static_actions[$action->callback][$attribute])) $actions[$action->aid][$attribute] = $static_actions[$action->callback][$attribute];
      }
    }
    return $actions;
  }

  function get_operation_settings($operation) {
    return (isset($this->options[$operation['key']]) ? $this->options[$operation['key']] : NULL);
  }

  function strip_view() {
    $this->view->pager_original = $this->view->pager;
    $this->view->set_use_pager(FALSE);
    $this->view->exposed_widgets = NULL;
    $this->view->display_handler->set_option('header', '');
    $this->view->display_handler->set_option('footer', '');
    $this->view->display_handler->set_option('use_pager', FALSE);
    $this->view->attachment_before = '';
    $this->view->attachment_after = '';
    $this->view->feed_icon = NULL;
  }
}

