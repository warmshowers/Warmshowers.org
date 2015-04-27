<?php

/**
 * @file
 * Provides Features integration for Workflow using the CRUD API.
 *
 * As you will notice this file will only handle the <export> of Worflows,
 * including states and transitions. The <import> is handeled magically,
 * and all modifications are done in function Workflow::save().
 *
 * @see http://foxinbox.org/content/features-magic-how-features-adds-additional-items-export for an introduction.
 */

define('WORKFLOW_FEATURES_AUTHOR_NAME', 'workflow_features_author_name');

// Even if workflow Node is not enabled, Features may use Node API's type_maps.
module_load_include('inc', 'workflow', 'workflow.node.type_map');

/**
 * Default controller handling features integration.
 */
class WorkflowFeaturesController extends EntityDefaultFeaturesController {

  /**
   * Generates the result for hook_features_export().
   */
  public function export($data, &$export, $module_name = '') {
    $pipe = parent::export($data, $export, $module_name);

    foreach ($data as $workflow_name) {
      if ($workflow = workflow_load_by_name($workflow_name)) {
        // Add dependency on workflow_node.
        if (count($workflow->getTypeMap())) {
          $export['dependencies']['workflownode'] = 'workflownode';
        }
        // Add dependency on workflow_field.
        $export['features']['Workflow'][$workflow_name] = $workflow_name;
      }
    }
    return $pipe;
  }

  /**
   * Generates the result for hook_features_export_render().
   *
   * This is a copy of the parent, adding 'system_roles'.
   * The workflow is imported in the target system with Workflow::save().
   */
  public function export_render($module, $data, $export = NULL) {
    $translatables = $code = array();
    $code[] = '  $workflows = array();';
    $code[] = '';
    foreach ($data as $identifier) {
      // Clone workflow to make sure changes are not propagated to original.
      if ($workflow = entity_load_single($this->type, $identifier)) {
        // Make sure data is not copied to the database.
        $workflow = clone $workflow;
        // Modification for the Workflow object:
        // For mapping workflow_field, add original wid in case target system
        // already contains workflows.
        $workflow->wid_original = $workflow->wid;
        // Add roles to translate role IDs on target system.
        $permission = NULL;
        $workflow->system_roles = workflow_get_roles($permission);
        // >> Now resume with normal flow.

        // Make sure to escape the characters \ and '.
        // The following method has the advantage, that you can export with
        // features,
        // and later import without enabling Features in the target system.
        $workflow_export = addcslashes(entity_export($this->type, $workflow, '  '), '\\\'');
        $workflow_identifier = features_var_export($identifier);
        $code[] = "  // Exported workflow: {$workflow_identifier}";
        $code[] = "  \$workflows[{$workflow_identifier}] = entity_import('{$this->type}', '" . $workflow_export . "');";
        $code[] = "";
      }
    }
    $code[] = '  return $workflows;';
    $code = implode("\n", $code);

    $hook = isset($this->info['export']['default hook']) ? $this->info['export']['default hook'] : 'default_' . $this->type;

    return array($hook => $code);
  }

  /**
   * Overridden to not delete upon revert.
   */
/*
  function revert($module = NULL) {
    // Loads defaults from feature code.
    $defaults = features_get_default($entity_type, $module);
    if ($defaults = features_get_default($this->type, $module)) {
      foreach ($defaults as $name => $entity) {
        entity_delete($this->type, $name);
      }
    }
  }
 */
}

/**
 * Implements hook_features_COMPONENT_alter().
 *
 * Adds the corresponding Workflow to the WorkflowField.
 */
function workflow_features_pipe_field_base_alter(&$pipe, $data, $export) {
  if (!empty($data)) {
    foreach ($data as $field_name) {
      // $info = field_info_field($field_name);
      $field = _workflow_info_field($field_name);
      if ($field['type'] == 'workflow') {

        // $field['settings']['wid'] can be numeric or named.
        $workflow = workflow_load_single($field['settings']['wid']);
        $pipe['Workflow'][] = $workflow->name;
      }
    }
  }
}

/**
 * Implements hook_features_rebuild().
 */
function workflow_features_rebuild($module) {
  // workflow_features_revert($module);
  // $entity_type = 'Workflow';

  // Do not delete the previous workflow. It will break the installation due to
  // the new $wid that is created.
  // return entity_features_get_controller($entity_type)->revert($module);

  // $info = entity_get_info($entity_type);
  // if (in_array('EntityAPIControllerInterface', class_implements($info['controller class']))) {
  //   return entity_get_controller($entity_type)->import($export);
  // }
}

/**
 * CRUD style helper functions below.
 */

/**
 * Translates a role string to RIDs for importing.
 *
 * @param string $role_string
 *   A string of roles or fake 'author' role.
 *
 * @return array
 *   An array of RIDs.
 */
function _workflow_roles_to_rids($role_string) {
  // Get all roles, including 'author'.
  $permission = NULL;
  $roles = workflow_get_roles($permission);

  $rid_array = array();
  foreach (explode(',', $role_string) as $role_name) {
    if ($role_name === WORKFLOW_FEATURES_AUTHOR_NAME) {
      $rid_array[WORKFLOW_ROLE_AUTHOR_RID] = WORKFLOW_ROLE_AUTHOR_RID;
    }
    elseif ($role_name && in_array($role_name, $roles)) {
      $rid = array_search($role_name, $roles);
      $rid_array[$rid] = $rid;
    }
  }
  return $rid_array;
}

/**
 * Translates a string of rids to role names for exporting.
 *
 * @param array $rid_array
 *   An array of rids or fake 'author' role.
 *
 * @return string
 *   A string of role names separated by commas.
 */
function _workflow_rids_to_roles(array $rid_array) {
  // Get all roles, including 'author'.
  $permission = NULL;
  $roles = workflow_get_roles($permission);
  // There may be a role named 'author', so make 'author' distinct.
  $roles[WORKFLOW_ROLE_AUTHOR_RID] = WORKFLOW_FEATURES_AUTHOR_NAME;

  // Translate RIDs to rolenames.
  $return = implode(',', array_intersect_key($roles, array_flip($rid_array)));
  return trim($return, ',');
}
