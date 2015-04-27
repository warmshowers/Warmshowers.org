<?php

/**
 * @file
 * Defines a Workflow field, widget and formatter. (copied from list field).
 */

/**
 * Implements hook_field_info().
 */
function workflowfield_field_info() {
  return WorkflowItem::getInfo();
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Changes the hook_field_settings_form.
 * Fixes some Field settings and Field Instance settings, and makes sure users cannot change it.
 *
 * @todo: perhaps this is core functionality, but these values are only saved
 *        when the site builder explicitly save the instance settings. :-(
 */
function workflowfield_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  if ($form['#field']['type'] == 'workflow') {
    // The Workflow field must have a value, so set to required.
    $form['instance']['required']['#default_value'] = 1;
    $form['instance']['required']['#disabled'] = TRUE;

    // User may not set a default value (this is done by the Workflow module).
    // @see WorkflowState::getOptions()
    $form['instance']['default_value_widget']['#type'] = 'hidden';
    $form['instance']['default_value_widget']['#disabled'] = TRUE;
    unset($form['instance']['default_value_widget']);

    // Make sure only 1 value can be entered in the Workflow field.
    $form['field']['cardinality']['#default_value'] = 1;
    $form['field']['cardinality']['#disabled'] = TRUE;
  }
}

/**
 * Implements hook_field_settings_form().
 */
function workflowfield_field_settings_form($field, $instance, $has_data) {
  $form = array();
  $form_state = array();

  $workflow_field = new WorkflowItem($field, $instance);
  return $workflow_field->settingsForm($form, $form_state, $has_data);
}

/**
 * Implements property_callbacks for hook_field_info().
 */
function workflowfield_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];
  $property['getter callback'] = 'entity_metadata_field_property_get';
  $property['getter callback'] = '_workflowfield_metadata_property_get';
  // $property['setter callback'] = 'entity_metadata_field_property_set';
  // $property['setter callback'] = '_workflowfield_metadata_property_set';
  $property['options list'] = 'entity_metadata_field_options_list';
  $property['property info'] = array(
    'value' => array(
      'type' => 'integer',
      'label' => t('State ID'),
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'workflow' => array(
      'type' => 'array',
      'label' => t('Workflow details'),
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
}

/**
 * Getter callback for Workflow defined in hook_entity_property_info_alter.
 *
 * This is different from the default, because 'value' is not always set
 * and 'workflow' may be set, but is not in the field data.
 */
function _workflowfield_metadata_property_get($entity, array $options, $name, $entity_type, $info) {
  $values = array();

  // return entity_metadata_field_property_get($entity, array $options, $name, $entity_type, $info);
  $field = field_info_field($name);
  $columns = array_keys($field['columns']);
  $langcode = isset($options['language']) ? $options['language']->language : LANGUAGE_NONE;
  $langcode = entity_metadata_field_get_language($entity_type, $entity, $field, $langcode, TRUE);

  if (isset($entity->{$name}[$langcode])) {
    foreach ($entity->{$name}[$langcode] as $delta => $data) {
      // In workflowfield_property_info_callback(), we needed to set a column 'value'.
      // This is now filled from the widget data.
      // Sometimes that is not widget, or the submit function has not been processed yet.

      // On a normal widget:
      $data[$columns[0]] = $sid = isset($data['value']) ? $data['value'] : 0;
      // On a workflow form widget:
      $data[$columns[0]] = $sid = isset($data['workflow']['workflow_sid']) ? $data['workflow']['workflow_sid'] : $sid;

      $values[$delta] = $data[$columns[0]];
    }
  }
  // For an empty single-valued field, we have to return NULL.
  return $field['cardinality'] == 1 ? ($values ? reset($values) : NULL) : $values;
}

/**
 * Callback for setting field property values.
 */
function _workflowfield_metadata_property_set($entity, $name, $value, $langcode, $entity_type, $info) {
  // return entity_metadata_field_property_set($entity, $name, $value, $langcode, $entity_type, $info);
  $field = field_info_field($name);
  $columns = array_keys($field['columns']);
  $langcode = entity_metadata_field_get_language($entity_type, $entity, $field, $langcode);
  $values = $field['cardinality'] == 1 ? array($value) : (array) $value;
}


/**
 * We will be using some default formatters and widgets from the List and Options modules.
 */

/**
 * Implements hook_field_formatter_info_alter().
 *
 * The module reuses the formatters defined in list.module.
 */
function workflowfield_field_formatter_info_alter(&$info) {
  $info['list_default']['field types'][] = 'workflow';
}

/**
 * Implements hook_field_widget_info_alter().
 *
 * The module does not implement widgets of its own, but reuses the
 * widgets defined in options.module.
 *
 * @see workflowfield_options_list()
 */
function workflowfield_field_widget_info_alter(&$info) {
  $info['options_select']['field types'][] = 'workflow';
  $info['options_buttons']['field types'][] = 'workflow';
}

/**
 * Do not implement hook_field_presave(),
 * since $nid is needed, but not yet known at this moment.
 */
/*
// function workflowfield_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
// }
 */

/**
 * Implements hook_field_insert().
 */
function workflowfield_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  return workflowfield_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 *
 * It is the D7-wrapper for D8-style WorkflowDefaultWidget::submit.
 * It is called also from hook_field_insert, since we need $nid to store workflow_node_history.
 * We cannot use hook_field_presave, since $nid is not yet known at that moment.
 */
function workflowfield_field_update($entity_type, $entity, array $field, $instance, $langcode, &$items) {
  $form = array();
  $form_state = array();
  $field_name = $field['field_name'];

  if ($entity_type == 'comment') {
    // This happens when we are on an entity's comment.
    // We save the field of the node. The comment is saved automatically.
    $referenced_entity_type = 'node'; // Comments only exist on nodes.
    $referenced_entity_id = $entity->nid;
    // Load the node again, since the passed node doesn't contain proper 'type' field.
    $referenced_entity = entity_load_single($referenced_entity_type, $referenced_entity_id);

    if ($field_name) {
      // Execute the transition upon the node. Afterwards, $items is in form as expected by Field API.
      // Remember, we don't know if the transition is scheduled or not.
      $widget = new WorkflowDefaultWidget($field, $instance, $referenced_entity_type, $referenced_entity);
      $widget->submit($form, $form_state, $items); // $items is a proprietary D7 parameter.

      // Since we are saving the comment only, we must save the node separately.
      entity_save($referenced_entity_type, $referenced_entity);
    }
    else {
      // Execute the transition upon the node. Afterwards, $items is in form as expected by Field API.
      // Remember, we don't know if the transition is scheduled or not.
      $widget = new WorkflowDefaultWidget($field, $instance, $referenced_entity_type, $referenced_entity);
      $widget->submit($form, $form_state, $items); // $items is a proprietary D7 parameter.

      // Since we are saving the comment only, we must save the node separately.
      entity_save($referenced_entity_type, $referenced_entity);
    }
  }
  else {
    $widget = new WorkflowDefaultWidget($field, $instance, $entity_type, $entity);
    $widget->submit($form, $form_state, $items); // $items is a proprietary D7 parameter.
  }
}

/**
 * Implements hook_field_prepare_view().
 *
 * This hook is needed in the following case:
 * - Edit a node with Workflow Field;
 * - Change the value of the widget;
 * - Click [Preview] button;
 * See the "Notice: Undefined index: value in list_field_formatter_view() (line 467+472 of \modules\field\modules\list\list.module)."
 * This is because in the Workflow Widget, the data is stored in a 'workflow'
 * structure, not a 'value' value.
 *
 * However, it is a pity that we run this hook in every view :-(
 */
function workflowfield_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // I guess Preview mode is only with 1 entity at a time.
  if (count ( $items ) !== 1) {
    return;
  }

  foreach ($items as $id => &$item_array) {
    foreach ($item_array as $index => &$item) {
      if (!isset($item['value']) && isset($item['workflow']['workflow_sid'])) {
        $item['value'] = $item['workflow']['workflow_sid'];
      }
    }
  }
}

/**
 * Implements hook_field_delete().
 *
 * @todo: implement
 */
function workflowfield_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $workflow_field = new WorkflowItem($field, $instance, $entity_type, $entity);
  $workflow_field->delete($items);
}

/**
 * Implements hook_field_is_empty().
 *
 * The Workflow field is never empty.
 */
function workflowfield_field_is_empty($item, $field) {
  // $workflow_field = new WorkflowItem($field, $instance, $entity_type, $entity);
  // $workflow_field->isEmpty($item);
  return FALSE;
}

/**
 * Implements hook_field_delete_field().
 *
 * @todo: implement functionality from workflow_node_delete().
 */
/*
// function workflowfield_field_delete_field($entity_type, $entity, $field, $instance, $langcode, &$items) {
// }
 */

/**
 * Callback function for list.module formatter.
 *
 * Returns array of allowed values when using a 'core' list formatter,
 * instead of the 'workflow' formatter.
 *
 * @see list_allowed_values()
 */
function workflowfield_allowed_values($field, $instance, $entity_type, $entity) {
  $workflow_field = new WorkflowItem($field, $instance, $entity_type, $entity);
  return $workflow_field->getAllowedValues();
}

/**
 * Implements hook_options_list().
 *
 * Callback function for the default Options widgets.
 *
 * @todo: move to a class.
 */
function workflowfield_options_list($field, $instance, $entity_type, $entity) {
  global $user; // @todo #2287057: OK?
  // @todo: Perhaps global user is not always the correct user.
  // E.g., on ScheduledTransition->execute()? But this function is mostly used in UI.

  $options = array();

  if ($entity) {
    // Get the allowed new states for the entity's current state.
    $field_name = $field['field_name'];
    $sid = workflow_node_current_state($entity, $entity_type, $field_name);
    $state = workflow_state_load_single($sid);
    $options = $state->getOptions($entity_type, $entity, $field_name, $user, FALSE);
  }
  else {
    // $field['settings']['wid'] can be numeric or named.
    if ($workflow = workflow_load_single($field['settings']['wid'])) {
      // There is no entity, E.g., on the Rules action "Set a data value".
      $state = new WorkflowState(array('wid' => $workflow->wid));
      $options = $state->getOptions('', NULL, '', $user, FALSE);
    }
  }
  return $options;
}
