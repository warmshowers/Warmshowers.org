<?php

/**
 * @file
 * Workflow_access records are a **faux-exportable** component.
 */

/**
 * Helper to find the roles needs by an export.
 *
 * @return array
 *   A workflow_name-indexed hash of non-default roles in workflow_access for
 *   that workflow.
 */
function _workflow_access_get_affected_roles() {
  $sql = <<<SQL
SELECT DISTINCT
  w.name AS wname,
  r.name AS rname
FROM {workflow_access} wa
  INNER JOIN {workflow_states} ws ON wa.sid = ws.sid
  INNER JOIN {workflows} w ON ws.wid = w.wid
  INNER JOIN {role} r ON wa.rid = r.rid
SQL;
  $roles = array();
  foreach (db_query($sql) as $row) {
    $roles[$row->wname][$row->rname] = $row->rname;
  }
  return $roles;
}

/**
 * Implements hook_features_export_options().
 *
 * Provide a list of all workflows as selectable Feature components.
 *
 * Generates the options to choose from.
 * Using $wid as key. Result is used by hook_features_export().
 */
function workflow_access_features_export_options() {
  $workflows = array();
  foreach (workflow_load_multiple() as $workflow) {
    $name = $workflow->getName();
    $workflows[$name] = t('workflow access configuration for: @label', array('@label' => $name));
  }
  return $workflows;
}

/**
 * Implements hook_features_export().
 */
function workflow_access_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['workflow_access'] = 'workflow_access';
  $export['dependencies']['workflow'] = 'workflow';

  $roles = _workflow_access_get_affected_roles();

  foreach ($data as $workflow_name) {
    if ($workflow = workflow_load_by_name($workflow_name)) {

      $export['features']['workflow_access'][$workflow_name] = $workflow_name;
      $export['features']['workflow'][$workflow_name] = $workflow_name;

      // Access configuration for a workflow needs that workflow to exist.
      $pipe['workflow'][$workflow_name] = $workflow_name;

      // And it needs the roles to which rights are granted.
      if (isset($roles[$workflow_name])) {
        foreach ($roles[$workflow_name] as $role_name) {
          $pipe['user_role'][$role_name] = $role_name;
        }
      }
    }
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 *
 * Instruct Features which php code to generate, including the code-ified
 * workflow access records we want to export from db into code.
 */
function workflow_access_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $workflows = array();';

  // For each selected workflow, fetch all related workflow access records
  // we want to put into code.
  foreach ($data as $workflow_name) {
    $workflow = workflow_load_by_name($workflow_name);
    if ($workflow) {
      $states = $workflow->getStates($all = TRUE);
      if ($states) {
        $code[] = "\n  \$workflows['$workflow_name'] = array();";
        foreach ($states as $state) {
          $access_records = workflow_access_get_features_workflow_access_by_sid($state->sid);
          if (!empty($access_records)) {
            $state_name = $state->getName();
            $code[] = "  \$workflows['$workflow_name']['$state_name'] = array();";
            foreach ($access_records as $record) {
              $rname = $record->rname;
              unset($record->rname);
              $code[] = "  \$workflows['$workflow_name']['$state_name']['$rname'] = " . features_var_export($record, '  ') . ';';
            }
          }
        }
      }
    }
  }

  $code[] = "\n  return \$workflows;";
  $code = implode("\n", $code);
  return array('workflow_access_features_default_settings' => $code);
}


/**
 * Implements hook_features_rebuild().
 *
 * Instruct Features to insert our records (that were exported into code)
 * into the workflow_access table.
 */
function workflow_access_features_rebuild($module) {
  $workflow_access_records = module_invoke($module, 'workflow_access_features_default_settings');
  // Retrieve the workflow IDs.
  $wids = array();
  foreach ($workflow_access_records as $workflow_name => $state) {
    $workflow = workflow_load_by_name($workflow_name);
    $wids[$workflow_name] = $workflow->wid;
  }

  foreach ($wids as $workflow_name => $wid) {
    $states = $workflow->getStates($all = TRUE);
    foreach ($states as $state) {
      // Remove all workflow access records for states belonging to this
      // workflow. We don't want lingering entries - we only want the ones we're
      // about to insert.
      workflow_access_delete_workflow_access_by_sid($state->sid);
    }
  }

  // Insert our workflow access records. They look like
  // workflow_name[state_name][role_name] => array(grant_name => 0|1, ...)
  foreach ($workflow_access_records as $workflow_name => $states) {
    foreach ($states as $state_name => $records) {
      if ($state = workflow_state_load_by_name($state_name, $wids[$workflow_name])) {
        foreach ($records as $rname => $record) {
          $record['sid'] = ($state) ? $state->sid : $state->sid;
          if ($rname == WORKFLOW_FEATURES_AUTHOR_NAME) {
            $record['rid'] = WORKFLOW_ROLE_AUTHOR_RID;
          }
          else {
            $role = user_role_load_by_name($rname);
            $record['rid'] = $role->rid;
          }
          workflow_access_insert_workflow_access_by_sid($record);
        }
      }
    }
  }
}

/**
 * Implements hook_features_revert().
 */
function workflow_access_features_revert($module) {
  workflow_access_features_rebuild($module);
}

/**
 * Get workflow_access object like below by state id.
 *
 * Array(
 *   'rname' => 'authenticated user',
 *   'grant_view' => 1,
 *   'grant_update' => 0,
 *   'grant_delete' => 0,
 * );
 *
 * State ID and Workflow ID are not returned because they are implicit for a
 * given sid.
 */
function workflow_access_get_features_workflow_access_by_sid($sid) {
  // Get all workflow access rules for a sid, where wa.rid is either a valid
  // role or -1, which stands for the author.
  $sql = <<<SQL
SELECT
  r.name as rname,
  wa.grant_view, wa.grant_update, wa.grant_delete
FROM {workflow_access} wa
  LEFT JOIN {role} r ON wa.rid = r.rid
WHERE
  wa.sid = :sid AND (wa.rid = r.rid OR wa.rid = :rid)
SQL;
  $results = db_query($sql, array(':sid' => $sid, ':rid' => WORKFLOW_ROLE_AUTHOR_RID));
  $records = $results->fetchAll();
  foreach ($records as $record) {
    if (empty($record->rname)) {
      $record->rname = WORKFLOW_FEATURES_AUTHOR_NAME;
    }
  }
  return $records;
}
