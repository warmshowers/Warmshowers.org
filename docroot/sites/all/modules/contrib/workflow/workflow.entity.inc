<?php

/**
 * @file
 * Integrates workflow with entity API.
 */

/**
 * Implements hook_entity_info().
 *
 * @todo: implement hook_property_info, metadata.
 */
function workflow_entity_info() {

  $entities['Workflow'] = array(
    'label' => t('Workflow'),
    'plural label' => t('Workflows'),
    'entity class' => 'Workflow',
    'controller class' => 'WorkflowController',
    'features controller class' => 'WorkflowFeaturesController',
    'module' => 'workflow',
    'base table' => 'workflows',
    'fieldable' => FALSE,
    'exportable' => TRUE,
    'entity keys' => array(
      'id' => 'wid',
      'name' => 'name',
    ),
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
    // The following is added in workflow_admin_ui.module.
/*
    // 'access callback' => 'workflow_access',
    // 'admin ui' => array(
    //   'path' => 'admin/config/workflow/workflow',
    //   'file' => 'workflow_admin_ui/workflow_admin_ui.pages.inc',
    //   'controller class' => 'EntityWorkflowUIController',
    //   'menu wildcard' => '%workflow',
    // ),
 */
  );

  $entities['WorkflowState'] = array(
    'label' => t('Workflow state'),
    'entity class' => 'WorkflowState',
    'controller class' => 'WorkflowStateController',
    // 'features controller class' => FALSE, //@todo: implement this.
    'module' => 'workflow',
    'base table' => 'workflow_states',
    'fieldable' => FALSE,
    'exportable' => FALSE,
    'entity keys' => array(
      'id' => 'sid',
    ),
    'label callback' => 'entity_class_label',
    'uri callback' => 'entity_class_uri',
  );

  $entities['WorkflowConfigTransition'] = array(
    'label' => t('Workflow config transition'),
    'entity class' => 'WorkflowConfigTransition',
    // Add controller Class. 'Workflow' class is the de-facto controller.
    'controller class' => 'WorkflowConfigTransitionController',
    'base table' => 'workflow_transitions',
    'exportable' => FALSE,
    'module' => 'workflow',
    'entity keys' => array(
      'id' => 'tid',
      'status' => 'status',
    ),
    'label callback' => 'entity_class_label',
    // 'uri callback' => 'entity_class_uri',
/*
    'view modes' => array(
      'full' => array(
        'label' => t('Full'),
        'custom settings' => TRUE,
      ),
    ),
    'views controller class' => 'EntityDefaultViewsController',
    'access callback' => 'workflow_tab_access', // @todo: use to-be workflow_access here. Access to Tab <> access to workflow.
 */
  );

  // The Controller class of Transitions and ScheduledTransition is shared.
  $entities['WorkflowTransition'] = array(
    'label' => t('Workflow executed transition'),
    'entity class' => 'WorkflowTransition',
    'controller class' => 'WorkflowTransitionController',
    'views controller class' => 'EntityDefaultViewsController',
    'rules controller class' => 'EntityDefaultRulesController',
    'features controller class' => FALSE,
    'base table' => 'workflow_node_history',
    'entity keys' => array(
      'id' => 'hid',
    ),
    'label callback' => 'entity_class_label',
    'module' => 'workflow',
  );

  $entities['WorkflowScheduledTransition'] = array(
    'label' => t('Workflow scheduled transition'),
    'entity class' => 'WorkflowScheduledTransition',
    'controller class' => 'WorkflowTransitionController',
    'views controller class' => 'EntityDefaultViewsController',
    'rules controller class' => 'EntityDefaultRulesController',
    'features controller class' => FALSE,
    'base table' => 'workflow_scheduled_transition',
    'entity keys' => array(
      // This entity does not have a ID yet. Below is to avoid the Notice: Undefined index: id in EntityDefaultMetadataController->entityPropertyInfo()
      'id' => 'do-not-use',
    ),
    'label callback' => 'entity_class_label',
    'module' => 'workflow',
  );

  return $entities;
}


/**
 * Does NOT implement hook_entity_property_info().
 *
 * By NOT implementing this hook, but using hook_entity_property_info_alter(),
 * the system itself will generate the most viable options.
 * They can then be tweaked with the _alter() function.
 *
 * @see https://www.drupal.org/node/1208874
 */
// function workflow_entity_property_info() {
// $info = array();
// $return $info;
// }

/**
 * Implements hook_entity_property_info_alter().
 *
 * N.B. Keep the following functions aligned when changing properties:
 * - workflow_tokens()
 * - workflow_entity_property_info_alter()
 * - workflow_views_views_data_alter()
 */
function workflow_entity_property_info_alter(&$info) {

  // Properties for Entites. @todo wrapper: not only nodes.
//  $info['node']['properties']['workflows'] = array(
//    'type' => 'list<Workflow>',
//    'label' => t("Workflow"),
//    'description' => t("The workflow of the entity."),
//    'getter callback' => '_workflow_metadata_workflow_get_properties',
//    'entity token' => FALSE,
//  );

  // Properties for Workflow.
  $info['Workflow']['properties']['wid']['label'] = 'Workflow ID';
  $info['Workflow']['properties']['options']['label'] = t('Workflow options');
  $info['Workflow']['properties']['options']['getter callback'] = '_workflow_metadata_workflow_get_properties';
  $info['Workflow']['properties']['states'] = array(
    'type' => 'list<WorkflowState>',
    'label' => 'States',
    'description' => 'States of the Workflow.',
    'getter callback' => '_workflow_metadata_workflow_get_properties',
    // 'options list' => 'entity_metadata_user_roles',
    // 'required' => TRUE/FALSE,
    // 'entity views field' => TRUE/FALSE,
    // 'entity token' => TRUE/FALSE,
  );
  $info['Workflow']['properties']['transitions'] = array(
    'type' => 'list<WorkflowConfigTransition>',
    'label' => 'Transitions',
    'description' => 'Transitions of the Workflow.',
    'getter callback' => '_workflow_metadata_workflow_get_properties',
  );
  unset($info['Workflow']['properties']['tab_roles']['description']);
  $info['Workflow']['properties']['tab_roles'] += array(
    'type' => 'list<integer>',
    'label' => t("User roles"),
    'description' => t("The roles that can access the Workflow History tab."),
    'getter callback' => '_workflow_metadata_workflow_get_properties',
  );

  // Properties for WorkflowState.
  $info['WorkflowState']['properties']['wid']['type'] = 'Workflow';
  //
  $info['WorkflowState']['properties']['label'] = $info['WorkflowState']['properties']['state'];
  $info['WorkflowState']['properties']['label']['label'] = t("Label");
  $info['WorkflowState']['properties']['label']['description'] = t("The label of the state.");
  $info['WorkflowState']['properties']['label']['required'] = TRUE;
  unset($info['WorkflowState']['properties']['state']);

  // Properties for WorkflowConfigTransition.
  $info['WorkflowConfigTransition']['properties']['workflow'] = array(
    'type' => 'Workflow',
    'label' => t("Workflow"),
    'description' => t("The workflow of the transition."),
    'getter callback' => '_workflow_metadata_workflow_get_properties',
    'entity token' => FALSE,
  );
  // @todo: Unify ID's to old_sid, new_sid.
  $info['WorkflowConfigTransition']['properties']['sid']['description'] = 'The ID of old state.';
  $info['WorkflowConfigTransition']['properties']['target_sid']['description'] = 'The ID of new state.';

  // Unify objects to old_state, new_state.
  $info['WorkflowConfigTransition']['properties']['old_state'] = array(
    'type' => 'WorkflowState',
    'label' => t('Old state'),
    'schema field' => 'sid',
    'description' => t("The old state."),
    'getter callback' => '_workflow_metadata_workflow_get_properties',
  );
  $info['WorkflowConfigTransition']['properties']['new_state'] = array(
    'type' => 'WorkflowState',
    'label' => t('New state'),
    'schema field' => 'target_sid',
    'description' => t("The new state."),
    'getter callback' => '_workflow_metadata_workflow_get_properties',
  );
  $info['WorkflowConfigTransition']['properties']['roles']['type'] = 'list<integer>';
  $info['WorkflowConfigTransition']['properties']['roles']['description'] = t("The roles that may execute the transition.");

  // Properties for WorkflowTransition.
  $info['WorkflowTransition']['properties']['label'] = $info['WorkflowConfigTransition']['properties']['label'];
  $info['WorkflowTransition']['properties']['workflow'] = $info['WorkflowConfigTransition']['properties']['workflow'];
  $info['WorkflowTransition']['properties']['hid']['label'] = 'Transition ID';

  // @todo: Unify ID's to old_sid, new_sid.
  $info['WorkflowTransition']['properties']['old_sid']['description'] = 'The ID of old state.';
  $info['WorkflowTransition']['properties']['sid']['description'] = 'The ID of new state.';
  // Unify objects to old_state, new_state.
  $info['WorkflowTransition']['properties']['old_state'] = $info['WorkflowConfigTransition']['properties']['old_state'];
  $info['WorkflowTransition']['properties']['new_state'] = $info['WorkflowConfigTransition']['properties']['new_state'];

  $info['WorkflowTransition']['properties']['user'] = array(
    'type' => 'user',
    'label' => t('User'),
    'schema field' => 'uid',
    'description' => t('The user who triggered the transition.'),
  );

  unset($info['WorkflowTransition']['properties']['stamp']);
  $info['WorkflowTransition']['properties']['timestamp'] = array(
    'type' => 'date',
    'label' => t('Timestamp'),
    'schema field' => 'stamp',
    'description' => t('The date, time the transition was executed.'),
  );

  // Properties for WorkflowScheduledTransition.
  $info['WorkflowScheduledTransition']['properties']['scheduled']['type'] = 'date';
  $info['WorkflowScheduledTransition']['properties']['workflow'] = $info['WorkflowTransition']['properties']['workflow'];
  // @todo: Unify ID's to old_sid, new_sid.
  $info['WorkflowScheduledTransition']['properties']['old_sid']['description'] = 'The ID of old state.';
  $info['WorkflowScheduledTransition']['properties']['sid']['description'] = 'The ID of new state.';
  // Unify objects to old_state, new_state.
  $info['WorkflowScheduledTransition']['properties']['old_state'] = $info['WorkflowTransition']['properties']['old_state'];
  $info['WorkflowScheduledTransition']['properties']['old_state']['schema field'] = 'old_sid';
  $info['WorkflowScheduledTransition']['properties']['new_state'] = $info['WorkflowTransition']['properties']['new_state'];
  $info['WorkflowScheduledTransition']['properties']['new_state']['schema field'] = 'new_sid';
  $info['WorkflowScheduledTransition']['properties']['user'] = $info['WorkflowTransition']['properties']['user'];
}

/**
 * Getter callback for Workflow defined in hook_entity_property_info_alter.
 */
function _workflow_metadata_workflow_get_properties($entity, array $options, $name, $entity_type, $property) {
  switch ($name) {
//    // The workflows of a normal entity.
//    case 'workflows':
//      $workflow = _workflow_get_workflow_creation_sid($entity_type, $entity, $field_name);
//      return $workflow;
//      return $entity->getWorkflow();

    // The workflows of a Workflow entity.
    case 'workflow':
      return $entity->getWorkflow();

    case 'states':
      return $entity->getStates();

    case 'transitions':
      // @todo: for some reason, getTransitions() gives no result.
      return $entity->getTransitions();

    case 'old_state':
    case 'old-state':
      return $entity->getOldState();

    case 'new_state':
    case 'new-state':
      return $entity->getNewState();

    case 'tab_roles':
      // @todo: for some reason, Tab_roles gives no result.
      // Code copied from 'user' entity.
      return isset($entity->tab_roles) ? array_keys($entity->tab_roles) : array();

    case 'langcode':
      // Gets the language code of a Workflow Field, hence its State, Transitions.
      // '$property' is $field_name.
      $langcode = LANGUAGE_NONE;

      if (!$property) {
        // Workflow_node. Translations are not supported.
      }
      else {
        // Get language code for a field or property.
        // getPropertyLanguage() may return NULL if no language is set,
        // or may not exist on properties.
        $wrapper = entity_metadata_wrapper($entity_type, $entity);
        // Get languagecode. May be empty if 'und'.
        if (method_exists($wrapper->{$property}, 'getPropertyLanguage')) {
          if (!$langcode = $wrapper->{$property}->getPropertyLanguage()) {
            $langcode = LANGUAGE_NONE;
          }
        }
      }
      return $langcode;

    // The following properties need more love. Also test their tokens!
    case 'options':
      return 'n/a';
  }
}

/**
 * Entity loader for Workflow.
 *
 * Also used as Menu wild card loader {wildcard_name}_load for '%workflow'.
 *
 * @see http://www.phpgainers.com/content/creating-menu-wildcard-loader-function-drupal-7
 * @todo D8: deprecated in favour of workflow_load_single(), not needed for menu.
 *
 * $id can be numeric ID ('wid') or machine name ('name').
 * Caveat: this only works for entities with EntityAPIControllerExportable. #1741956
 */
function workflow_load($id) {
  // Some Admin UI menu page loaders pass the $wid as string, not int.
  // @see workflow_admin_ui_edit_form_validate().
  $workflow = entity_load_single('Workflow', $id);
  return $workflow;
}

function workflow_load_by_name($name) {
  $workflows = entity_load_multiple_by_name('Workflow', array($name));
  return reset($workflows);
}

function workflow_load_multiple_by_name($names = FALSE) {
  $workflows = entity_load_multiple_by_name('Workflow', $names);
  return $workflows;
}

function workflow_load_single($id) {
  return entity_load_single('Workflow', $id);
}

function workflow_load_multiple($ids = FALSE, $reset = FALSE) {
  return entity_load('Workflow', $ids, array(), $reset);
}

function workflow_create($name) {
  // @todo: avoid double names in db-table, to get rid of this line of code.
  $workflow = workflow_load_by_name($name);
  if (!$workflow) {
    $workflow = entity_create('Workflow', array('name' => $name));
  }
  return $workflow;
}


/**
 * Helper function, to get the label of a given workflow.
 *
 * @see workflow_get_sid_label($sid)
 * @see workflow_get_wid_label($wid)
 */
function workflow_label($workflow) {
  if ($workflow === FALSE) {
    $output = t('No workflow');
  }
  elseif ($workflow) {
    $output = check_plain(t($workflow->label()));
  }
  else {
    // E.g., NULL.
    $output = t('Unknown workflow');
  }
  return $output;
}

/**
 * Reset the Workflow when States, Transitions have been changed.
 */
function workflow_reset_cache($wid) {
  $ids = array($wid);
  entity_get_controller('Workflow')->resetCache($ids);
}

/**
 * CRUD for WorkflowState.
 */
function workflow_state_load($sid) {
  return WorkflowState::load($sid);
}

function workflow_state_load_single($sid) {
  return WorkflowState::load($sid);
}

function workflow_state_load_multiple($wid = 0, $reset = FALSE) {
  return WorkflowState::getStates($wid, $reset);
}

function workflow_state_load_by_name($name, $wid) {
  return WorkflowState::loadByName($name, $wid);
}

/**
 * Load WorkflowTransitions, most recent first.
 *
 * @param string $field_name
 *   Optional. Can be NULL, if you want to load any field.
 *
 * @deprecated: workflow_get_workflow_node_history_by_nid() --> workflow_transition_load_single()
 * @deprecated: workflow_get_recent_node_history() --> workflow_transition_load_single()
 */
function workflow_transition_load_multiple($entity_type, array $entity_ids, $field_name = '', $limit = NULL, $langcode = '') {
  return WorkflowTransition::loadMultiple($entity_type, $entity_ids, $field_name, $limit, $langcode);
}

/**
 * Load WorkflowTransitions, most recent first.
 *
 * @return WorkflowTransition
 *   object representing one row from the {workflow_node_history} table.
 */
function workflow_transition_load_single($entity_type, $entity_id, $field_name = '', $langcode = '') {
  $limit = 1;
  if ($transitions = workflow_transition_load_multiple($entity_type, array($entity_id), $field_name, $limit, $langcode)) {
    return reset($transitions);
  }
  return NULL;
}

/**
 * Load function belonging to the menu option 'workflow-comment/%'.
 *
 * Maps to this function just like 'node/%node' maps to node_load().
 *
 * @param int $hid
 *   The ID of the workflow state transition record to load.
 *
 * @return WorkflowTransition
 *   object representing one row from the {workflow_node_history} table.
 */
function workflow_transition_load($hid) {
  return entity_load_single('WorkflowTransition', $hid);
}
