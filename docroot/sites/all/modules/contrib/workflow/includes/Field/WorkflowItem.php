<?php

/**
 * @file
 * Contains workflow\includes\Field\WorkflowItem.
 * @see https://drupal.org/node/2064123 for 'Field Type Plugin' change record D7->D8.
 */

/**
 * Plugin implementation of the 'workflow' field type.
 *
 * @FieldType(
 *   id = "workflow",
 *   label = @Translation("Workflow"),
 *   description = @Translation("This field stores Workflow values for a certain Workflow type from a list of allowed 'value => label' pairs, i.e. 'Publishing': 1 => unpublished, 2 => draft, 3 => published."),
 *   default_widget = "options_select",
 *   default_formatter = "list_formatter",
 *   property_type' = WORKFLOWFIELD_PROPERTY_TYPE,
 * )
 */
class WorkflowItem extends WorkflowD7Base {// D8: extends ConfigFieldItemBase implements PrepareCacheInterface {
  /**
   * Function, that gets replaced by the 'annotations' in D8. (@see comments above this class)
   */
  public static function getInfo() {
    return array(
      'workflow' => array(
        'label' => t('Workflow'),
        'description' => t("This field stores Workflow values for a certain Workflow type from a list of allowed 'value => label' pairs, i.e. 'Publishing': 1 => unpublished, 2 => draft, 3 => published."),
        'settings' => array(
          'allowed_values_function' => 'workflowfield_allowed_values', // For the list.module formatter
          // 'allowed_values_function' => 'WorkflowItem::getAllowedValues', // For the list.module formatter.
          'wid' => '',
          // 'history' => 1,
          // 'schedule' => 0,
          // 'comment' => 0,
          'widget' => array(
            'options' => 'select',
            'name_as_title' => 1,
            'hide' => 0,
            'schedule' => 1,
            'schedule_timezone' => 1,
            'comment' => 1,
          ),
          'watchdog_log' => 1,
          'history' => array(
            'history_tab_show' => 1,
            'roles' => array(),
          ),
        ),
        'instance_settings' => array(),
        'default_widget' => 'workflow',
        'default_formatter' => 'list_default',
        // Properties are introduced in Entity API and used for Rules integration.
        'property_type' => WORKFLOWFIELD_PROPERTY_TYPE,
        'property_callbacks' => array('workflowfield_property_info_callback'),
      ),
    );
  }

  /**
   * Implements hook_field_settings_form() -> ConfigFieldItemInterface::settingsForm().
   */
  public function settingsForm(array $form, array &$form_state, $has_data) {
    $field_info = self::getInfo();
    $settings = $this->field['settings'];
    $settings += $field_info['workflow']['settings'];
    $settings['widget'] += $field_info['workflow']['settings']['widget'];

    // Create list of all Workflow types. Include an initial empty value.
    // Validate each workflow, and generate a message if not complete.
    $workflows = array();
    $workflows[''] = t('- Select a value -');
    foreach (workflow_load_multiple() as $wid => $workflow) {
      if ($workflow->isValid()) {
        $workflows[$wid] = check_plain($workflow->label()); // No t() on settings page.
      }
    }

    // Set message, if no 'validated' workflows exist.
    if (count($workflows) == 1) {
      drupal_set_message(
        t('You must create at least one workflow before content can be
          assigned to a workflow.')
      );
    }

    // The allowed_values_functions is used in the formatter from list.module.
    $element['allowed_values_function'] = array(
      '#type' => 'value',
      '#value' => $settings['allowed_values_function'], // = 'workflowfield_allowed_values',
    );

    // $field['settings']['wid'] can be numeric or named, or empty.
    $wid = isset($settings['wid']) ? $settings['wid'] : '';
    // Let the user choose between the available workflow types.
    $element['wid'] = array(
      '#type' => 'select',
      '#title' => t('Workflow type'),
      '#options' => $workflows,
      '#default_value' => $wid,
      '#required' => TRUE,
      '#disabled' => $has_data,
      '#description' => t('Choose the Workflow type. Maintain workflows !url.', array('!url' => l(t('here'), 'admin/config/workflow/workflow'))),
    );

    // Inform the user of possible states.
    // If no Workflow type is selected yet, do not show anything.
    if ($wid) {
      // Get a string representation to show all options.
      $allowed_values_string = $this->_allowed_values_string($wid);

      $element['allowed_values_string'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values for the selected Workflow type'),
        '#default_value' => $allowed_values_string,
        '#rows' => 10,
        '#access' => TRUE, // User can see the data,
        '#disabled' => TRUE, // .. but cannot change them.
      );
    }

    $element['widget'] = array(
      '#type' => 'fieldset',
      '#title' => t('Workflow widget'),
      '#description' => t('Set some global properties of the widgets for this
        workflow. Some can be altered per widget instance.'
      ),
    );
    $element['widget']['options'] = array(
      '#type' => 'select',
      '#title' => t('How to show the available states'),
      '#required' => FALSE,
      '#default_value' => $settings['widget']['options'],
      // '#multiple' => TRUE / FALSE,
      '#options' => array(
        // These options are taken from options.module
        'select' => 'Select list',
        'radios' => 'Radio buttons',
        // This option does not work properly on Comment Add form.
        'buttons' => 'Action buttons',
      ),
      '#description' => t("The Widget shows all available states. Decide which
        is the best way to show them. ('Action buttons' do not work on Comment form.)"
      ),
    );
    $element['widget']['hide'] = array(
      '#type' => 'checkbox',
      '#attributes' => array('class' => array('container-inline')),
      '#title' => t('Hide the widget on Entity form.'),
      '#default_value' => $settings['widget']['hide'],
      '#description' => t(
        'Using Workflow Field, the widget is always shown when editing an
        Entity. Set this checkbox in case you only want to change the status
        on the Workflow History tab or on the Node View. (This checkbox is
        only needed because Drupal core does not have a <hidden> widget.)'
      ),
    );
    $element['widget']['name_as_title'] = array(
      '#type' => 'checkbox',
      '#attributes' => array('class' => array('container-inline')),
      '#title' => t('Use the workflow name as the title of the workflow form'),
      '#default_value' => $settings['widget']['name_as_title'],
      '#description' => t(
        'The workflow section of the editing form is in its own fieldset.
         Checking the box will add the workflow name as the title of workflow
         section of the editing form.'
      ),
    );
    $element['widget']['schedule'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow scheduling of workflow transitions.'),
      '#required' => FALSE,
      '#default_value' => $settings['widget']['schedule'],
      '#description' => t(
        'Workflow transitions may be scheduled to a moment in the future.
         Soon after the desired moment, the transition is executed by Cron.
         This may be hidden by settings in widgets, formatters or permissions.'
      ),
    );
    $element['widget']['schedule_timezone'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show a timezone when scheduling a transition.'),
      '#required' => FALSE,
      '#default_value' => $settings['widget']['schedule_timezone'],
    );
    $element['widget']['comment'] = array(
      '#type' => 'select',
      '#title' => t('Allow adding a comment to workflow transitions'),
      '#required' => FALSE,
      '#options' => array(
        // Use 0/1/2 to stay compatible with previous checkbox.
        0 => t('hidden'),
        1 => t('optional'),
        2 => t('required'),
      ),
      '#default_value' => $settings['widget']['comment'],
      '#description' => t('On the Workflow form, a Comment form can be included
        so that the person making the state change can record reasons for doing
        so. The comment is then included in the node\'s workflow history. This
        may be altered by settings in widgets, formatters or permissions.'
      ),
    );

    $element['watchdog_log'] = array(
      '#type' => 'checkbox',
      '#attributes' => array('class' => array('container-inline')),
      '#title' => t('Log informational watchdog messages when a transition is
        executed (a state value is changed)'),
      '#default_value' => $settings['watchdog_log'],
      '#description' => t('Optionally log transition state changes to watchdog.'),
    );

    $element['history'] = array(
      '#type' => 'fieldset',
      '#title' => t('Workflow history'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $element['history']['history_tab_show'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the workflow history, and show it on a separate tab.'),
      '#required' => FALSE,
      '#default_value' => $settings['history']['history_tab_show'],
      '#description' => t("Every state change is recorded in table
        {workflow_node_history}. If checked and user has proper permission, a
        tab 'Workflow' is shown on the entity view page, which gives access to
        the History of the workflow. If you have multiple workflows per bundle,
        better disable this feature, and use, clone & adapt the Views display
        'Workflow history per Entity'."),
    );
    $element['history']['roles'] = array(
      '#type' => 'checkboxes',
      '#options' => workflow_get_roles(),
      '#title' => t('Workflow history permissions'),
      '#default_value' => $settings['history']['roles'],
      '#description' => t('Select any roles that should have access to the workflow tab on nodes that have a workflow.'),
    );

    return $element;
  }

  /*
   * Currently, there are no instance Settings.
   * hook_field_instance_settings_form() -> ConfigFieldItemInterface::instanceSettingsForm()
   */
  // public function instanceSettingsForm(array $form, array &$form_state, $has_data) {
  // }

  /**
   * Does NOT not implement hook_field_presave().
   *
   * Since $nid is needed, but not yet known at this moment.
   * hook_field_presave() -> FieldItemInterface::preSave()
   */
  // function workflowfield_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // }

  /**
   * Implements hook_field_insert() -> FieldItemInterface::insert().
   */
  public function insert() {
    return $this->update();
  }

  /**
   * Implements hook_field_update() -> FieldItemInterface::update().
   *
   * @todo: ATM, it is not used anymore. But it should replace DefaultWidget::submit().
   * 
   * It is called also from hook_field_insert(), since we need $nid to store {workflow_node_history}.
   * We cannot use hook_field_presave(), since $nid is not yet known at that moment.
   *
   * "Contrary to the old D7 hooks, the methods do not receive the parent entity
   * "or the langcode of the field values as parameters. If needed, those can be accessed
   * "by the getEntity() and getLangcode() methods on the Field and FieldItem classes.
   */
  public function update(&$items) {// ($entity_type, $entity, $field, $instance, $langcode, &$items) {

    $field_name = $this->field['field_name'];
    // $field['settings']['wid'] can be numeric or named.
    $workflow = workflow_load_single($this->field['settings']['wid']);
    $wid = $workflow->wid;

    // @todo D8: remove below lines.
    $entity = $this->entity;
    $entity_type = $this->entity_type;
    $entity_id = entity_id($entity_type, $entity);

    if (!$entity) {
      // No entity available, we are on the field Settings page - 'default value' field.
      // This is hidden from the admin, because the default value can be different for every user.
    }
    elseif (!$entity_id && $entity_type == 'comment') {
      // Not possible: a comment on a non-existent node.
    }
    elseif ($entity_id && $this->entity_type == 'comment') {
      // This happens when we are on an entity's comment.
      $referenced_entity_type = 'node'; // Comments only exist on nodes.
      $referenced_entity = entity_load_single($referenced_entity_type, $entity_id); // Comments only exist on nodes.

      // Submit the data. $items is reset by reference to normal value, and is magically saved by the field itself.
      $form = array();
      $form_state = array();
      $widget = new WorkflowDefaultWidget($this->field, $this->instance, $referenced_entity_type, $referenced_entity);
      $widget->submit($form, $form_state, $items); // $items is a proprietary D7 parameter.

      // Remember: we are on a comment form, so the comment is saved automatically, but the referenced entity is not.
      // @todo: probably we'd like to do this form within the Widget, but
      // $todo: that does not know wether we are on a comment or a node form.
      //
      // Widget::submit() returns the new value in a 'sane' state.
      // Save the referenced entity, but only is transition succeeded, and is not scheduled.
      $old_sid = _workflow_get_sid_by_items($referenced_entity->{$field_name}[LANGUAGE_NONE]);
      $new_sid = _workflow_get_sid_by_items($items);
      if ($old_sid != $new_sid) {
        $referenced_entity->{$field_name}[LANGUAGE_NONE] = $items;
        entity_save($referenced_entity_type, $referenced_entity);
      }
    }
    elseif ($this->entity_type != 'comment') {
      if (isset($items[0]['value'])) {
        // A 'normal' options.module-widget is used, and $items[0]['value'] is already properly set.
      }
      elseif (isset($items[0]['workflow'])) {
        // The WorkflowDefaultWidget is used.
        // Submit the data. $items is reset by reference to normal value, and is magically saved by the field itself.
        $form = array();
        $form_state = array();
        $widget = new WorkflowDefaultWidget($this->field, $this->instance, $entity_type, $entity);
        $widget->submit($form, $form_state, $items); // $items is a proprietary D7 parameter.
      }
      else {
        drupal_set_message(t('error in WorkfowItem->update()'), 'error');
      }
    }
    // A 'normal' node add page.
    // We should not be here, since we only do inserts after $entity_id is known.
    // $current_sid = workflow_load_single($wid)->getCreationSid();
  }

  /**
   * Implements hook_field_delete() -> FieldItemInterface::delete().
   */
  public function delete($items) {
    global $user;

    $entity_type = $this->entity_type;
    $entity = $this->entity;
    $entity_id = entity_id($entity_type, $entity);
    $field_name = $this->field['field_name'];

    // Delete the record in {workflow_node} - not for Workflow Field.
    // Use a one-liner for better code analysis when grepping on old code.
    (!$field_name) ? workflow_delete_workflow_node_by_nid($entity_id) : NULL;

    // Add a history record in {workflow_node_history}.
    // @see drupal.org/node/2165349, comment by Bastlynn:
    // The reason for this history log upon delete is because Workflow module
    // has historically been used to track node states and accountability in
    // business environments where accountability for changes over time is
    // *absolutely* required. Think banking and/or particularly strict
    // retention policies for legal reasons.
    //
    // However, a deleted nid may be re-used under certain circumstances:
    // e.g., working with InnoDB or after restart the DB server.
    // This may cause that old history is associated with a new node.
    $old_sid = _workflow_get_sid_by_items($items);
    $new_sid = (int) WORKFLOW_DELETION;
    $comment = t('Entity deleted.');
    $transition = new WorkflowTransition();
    $transition->setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $user->uid, REQUEST_TIME, $comment);
    $transition->save();

    // Delete all records for this node in {workflow_scheduled_transition}.
    foreach (WorkflowScheduledTransition::load($entity_type, $entity_id, $field_name) as $scheduled_transition) {
      $scheduled_transition->delete();
    }
  }

  /**
   * Helper functions for the Field Settings page.
   *
   * Generates a string representation of an array of 'allowed values'.
   * This is a copy from list.module's list_allowed_values_string().
   * The string format is suitable for edition in a textarea.
   *
   * @param int $wid
   *   The Workflow Id.
   *
   * @return
   *   The string representation of the $values array:
   *    - Values are separated by a carriage return.
   *    - Each value is in the format "value|label" or "value".
   */
  protected function _allowed_values_string($wid = 0) {
    $lines = array();
    $states = workflow_state_load_multiple($wid);
    $previous_wid = -1;

    foreach ($states as $state) {
      // Only show enabled states.
      if ($state->isActive()) {
        // Show a Workflow name between Workflows, if more then 1 in the list.
        if (($wid == 0) && ($previous_wid <> $state->wid)) {
          $previous_wid = $state->wid;
          $lines[] = $state->name . "'s states: ";
        }
        $label = check_plain(t($state->label()));
        $states[$state->sid] = $label;
        $lines[] = $state->sid . ' | ' . $label;
      }
    }
    return implode("\n", $lines);
  }

  /**
   * Helper function for list.module formatter.
   *
   * Callback function for the list module formatter.
   *
   * @see list_allowed_values
   *   "The strings are not safe for output. Keys and values of the array should
   *   "be sanitized through field_filter_xss() before being displayed.
   *
   * @return array
   *   The array of allowed values. Keys of the array are the raw stored values
   *   (number or text), values of the array are the display labels.
   *   It contains all possible values, beause the result is cached,
   *   and used for all nodes on a page.
   */
  public function getAllowedValues() {
    // Get all state names, including inactive states.
    $options = workflow_get_workflow_state_names(0, $grouped = FALSE, $all = TRUE);
    return $options;
  }

}
