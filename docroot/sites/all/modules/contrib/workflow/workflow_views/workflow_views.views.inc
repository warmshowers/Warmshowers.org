<?php

/**
 * @file
 * Provide views data and handler information for workflow.module.
 *
 * Includes the 'workflow_node' and 'workflow_state' tables, but generates a
 * pseudo-table for each separate workflow that's been set up.
 *
 * @defgroup views_workflow_module workflow.module handlers
 */

/**
 * Implements hook_views_data_alter().
 *
 * N.B. Keep the following functions aligned when changing properties:
 * - workflow_tokens()
 * - workflow_entity_property_info_alter()
 * - workflow_views_views_data_alter()
 */
function workflow_views_views_data_alter(&$data) {
  // In version 7.x-1.2, the following groups were defined in
  // workflow_views_views_data. Now, in 7.x-2.x the Transitions have become
  // entities, and are already declared by EntityDefaultViewsController.
  // So, the overrides have now moved to _alter, to avoid Notices.

  // WorkflowTransition data.
  $data['workflow_transitions']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_transitions']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['workflow_transitions']['target_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_transitions']['target_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';

  // WorkflowTransition data.
  $data['workflow_node_history']['old_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_node_history']['old_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['workflow_node_history']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_node_history']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['workflow_node_history']['uid']['field']['handler'] = 'workflow_views_handler_field_username';
  $data['workflow_node_history']['uid']['filter']['handler'] = 'views_handler_filter_user_name';
  $data['workflow_node_history']['uid']['relationship'] = array(
    'handler' => 'views_handler_relationship',
    'base' => 'users',
    'base field' => 'uid',
    'label' => t('User'),
  );
  $data['workflow_node_history']['comment']['field']['handler'] = 'views_handler_field_xss';
  $data['workflow_node_history']['comment']['filter']['handler'] = 'views_handler_filter_string';
  $data['workflow_node_history']['stamp']['field']['handler'] = 'views_handler_field_date';
  $data['workflow_node_history']['stamp']['filter']['handler'] = 'views_handler_filter_date';
  $data['workflow_node_history']['stamp']['sort']['handler'] = 'views_handler_sort_date';
  $data['workflow_node_history']['hid']['filter']['numeric'] = TRUE;
  // This group's help and title are moved from hook_views_data to _alter(), to avoid Notices.
  $data['workflow_node_history']['old_sid']['title'] = t('Old state name');
  $data['workflow_node_history']['old_sid']['help'] = t('The title of the previous state of the transition.');
  $data['workflow_node_history']['sid']['title'] = t('New state name');
  $data['workflow_node_history']['sid']['help'] = t('The title of the new state in the transition. (For the latest transition, this is the current state.)');
  $data['workflow_node_history']['uid']['title'] = t('User');
  $data['workflow_node_history']['uid']['help'] = t('The user who triggered the transition.');
  $data['workflow_node_history']['comment']['title'] = t('Comment');
  $data['workflow_node_history']['comment']['help'] = t('The comment describing why the the transition was done.');
  $data['workflow_node_history']['stamp']['title'] = t('Time');
  $data['workflow_node_history']['stamp']['help'] = t('The time of the transition.');
  // This group's help and title are moved from hook_views_data to _alter(), to avoid Notices.
  $data['workflow_node_history']['table']['group'] = t('Workflow history');
  // The join will be filled explicitely further below.
  $data['workflow_node_history']['table']['join'] = array();
  // This Edit comment handler is copied from workflow_extensions.
  $data['workflow_node_history']['edit_comment'] = array(
    'title' => t('Edit comment'),
    'help' => t('Provide a link to edit the workflow log comment.'),
    'field' => array(
      'handler' => 'workflow_views_handler_field_comment_link_edit',
    ),
  );

  // WorkflowCurrentTransition data.
  $data['workflow_node_current'] = $data['workflow_node_history'];
  $data['workflow_node_current']['table']['group'] = t('Workflow current');
  // The join will be filled explicitely further below.
  $data['workflow_node_current']['table']['join'] = array();

  // WorkflowScheduledTransition data.
  $data['workflow_scheduled_transition']['table']['group'] = t('Workflow scheduled');
  // The join will be filled explicitely further below.
  $data['workflow_scheduled_transition']['table']['join'] = array();
  $data['workflow_scheduled_transition']['old_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_scheduled_transition']['old_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['workflow_scheduled_transition']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['workflow_scheduled_transition']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['workflow_scheduled_transition']['uid']['field']['handler'] = 'workflow_views_handler_field_username';
  $data['workflow_scheduled_transition']['uid']['filter']['handler'] = 'views_handler_filter_user_name';
  $data['workflow_scheduled_transition']['uid']['relationship'] = array(
    'handler' => 'views_handler_relationship',
    'base' => 'users',
    'base field' => 'uid',
    'label' => t('Scheduling user'),
  );
  $data['workflow_scheduled_transition']['comment']['field']['handler'] = 'views_handler_field_xss';
  $data['workflow_scheduled_transition']['comment']['filter']['handler'] = 'views_handler_filter_string';
  $data['workflow_scheduled_transition']['scheduled']['field']['handler'] = 'views_handler_field_date';
  $data['workflow_scheduled_transition']['scheduled']['filter']['handler'] = 'views_handler_filter_date';
  $data['workflow_scheduled_transition']['scheduled']['sort']['handler'] = 'views_handler_sort_date';
  // UX data.
  $data['workflow_scheduled_transition']['old_sid']['title'] = t('Current state Name');
  $data['workflow_scheduled_transition']['old_sid']['help'] = t('The current state Name that the node is in.');
  $data['workflow_scheduled_transition']['sid']['title'] = t('Scheduled state Name');
  $data['workflow_scheduled_transition']['sid']['help'] = t('The state Name, the node is scheduled for.');
  $data['workflow_scheduled_transition']['uid']['title'] = t('User');
  $data['workflow_scheduled_transition']['uid']['help'] = t('The user who scheduled the transition.');
  $data['workflow_scheduled_transition']['comment']['title'] = t('Comment');
  $data['workflow_scheduled_transition']['comment']['help'] = t('A comment describing the scheduled transition.');
  // Scheduled transitions do not have a stamp, but are scheduled.
  $data['workflow_scheduled_transition']['scheduled']['title'] = t('Scheduled time');
  $data['workflow_scheduled_transition']['scheduled']['help'] = t('The time the transition will be executed.');

  // Add a relationship for each entity type.
  foreach (entity_get_info() as $entity_type => $info) {
    if (isset($info['base table']) && isset($data[$info['base table']]['table'])) {
      $base_table = $info['base table'];
      $entity_id_key = $info['entity keys']['id'];
      $revision_id_key = $info['entity keys']['revision'];

      // @todo: these joins need extra join on revision_id,field_name,language.
      $data['workflow_node_history']['table']['join'][$base_table] = array(
          'table' => 'workflow_node_history',
          'field' => 'nid',
          'left_table' => $base_table,
          'left_field' => $entity_id_key,
          'extra' => array(
            array(
              'field' => 'entity_type',
              'value' => $entity_type,
              'table' => 'workflow_node_history',
            ),
          ),
        );
      $data['workflow_scheduled_transition']['table']['join'][$base_table] = array(
        'table' => 'workflow_scheduled_transition',
        'field' => 'nid',
        'left_table' => $base_table,
        'left_field' => $entity_id_key,
        'extra' => array(
          array(
            'field' => 'entity_type',
            'value' => $entity_type,
            'table' => 'workflow_scheduled_transition',
          ),
        ),
      );
      $data['workflow_node_current']['table']['join'][$base_table] = array(
        'table' => 'workflow_node_history',
        'field' => 'hid',
        'left_table' => $base_table,
        'left_field' => $entity_id_key,
        // Do not use the default 'views_join', but add a subquery.
        'handler' => 'views_join_subquery',
        // @todo: add "and   entity_type = {$entity_type}".
        'left_query' => " SELECT max(hid) FROM {workflow_node_history}
                            where nid = {$base_table}.{$entity_id_key}
                            and   sid != old_sid ",
        );
    }
  }

  // Workflow node.
  // Only use when 'Workflow Node' is active. Use separate title.
  if (module_exists('workflownode')) {
    $data['workflow_node'] = $data['workflow_node_history'];
    $data['workflow_node']['table']['group'] = t('Workflow node');
    // This join is only possible for nodes, not other entity types.
    $data['workflow_node']['table']['join'] = array(
      'node' => array(
        'field' => 'nid',
        'left_table' => 'node',
        'left_field' => 'nid',
      ),
    );
    // Workflow Node does not have these fields.
    unset($data['workflow_node']['hid']);
    unset($data['workflow_node']['entity_type']);
    unset($data['workflow_node']['field_name']);
    unset($data['workflow_node']['delta']);
    unset($data['workflow_node']['language']);
    unset($data['workflow_node']['old_sid']);
    $data['workflow_node']['sid']['title'] = t('Current state ID');
    $data['workflow_node']['sid']['help'] = t('The current workflow state ID that the node is in.');
    $data['workflow_node']['comment']['title'] = t('Scheduled comment');
    $data['workflow_node']['comment']['help'] = t('A comment describing why the node was scheduled for state transition.');
    $data['workflow_node']['uid']['title'] = t('Last changing user');
    $data['workflow_node']['uid']['help'] = t('The user who triggered the transition.');
    $data['workflow_node']['stamp']['title'] = t('Current time');
    $data['workflow_node']['stamp']['help'] = t('The time at which the node moved to the current state.');

    // The following 'uid' handlers are different between 'workflow node' and
    // the other ones. At the moment of rewriting in 2013: I'm not sure what's best,
    // and I don't want to break anything, so I'll just leave this untouched.
    unset($data['workflow_node']['uid']['argument']);
    unset($data['workflow_node']['uid']['filter']);
    $data['workflow_node']['uid'] += array(
      'filter' => array(
        'handler' => 'views_handler_filter_user_name',
        'numeric' => TRUE,
        'name table' => 'workflow_node',
        'name field' => 'uid',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
        'click sortable' => TRUE,
        'name table' => 'workflow_node',
        'name field' => 'uid',
      ),
    );
  }

  // Workflow states.
  // To support Workflow Field, do not use workflow_node.
  if (module_exists('workflownode')) {
    $data['workflow_states']['table']['group'] = t('Workflow states (deprecated)');
    $data['workflow_states']['table']['join'] = array(
      // This join is only possible for nodes, not other entity types.
      'node' => array(
        'field' => 'sid',
        'left_table' => 'workflow_node',
        'left_field' => 'sid',
      ),
    );
    $data['workflow_states']['weight'] = array(
      'title' => t('State weight'),
      'help' => t('The weight of the current workflow state that the node is in.'),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
    $data['workflow_states']['state'] = array(
      'title' => t('Current state name'),
      'help' => t('The readable name of the workflow state that the node is in. (Less efficient, use only when click-sorting by state name.)'),
      'field' => array(
        'handler' => 'workflow_views_handler_field_state',
        'click sortable' => TRUE,
      ),
    );
  }

  $data['entity_WorkflowConfigTransition']['target_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowConfigTransition']['target_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['entity_WorkflowConfigTransition']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowConfigTransition']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';

  $data['entity_WorkflowTransition']['old_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowTransition']['old_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['entity_WorkflowTransition']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowTransition']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['entity_WorkflowTransition']['uid']['field']['handler'] = 'workflow_views_handler_field_username';
  $data['entity_WorkflowTransition']['uid']['filter']['handler'] = 'views_handler_filter_user_name';
  $data['entity_WorkflowTransition']['comment']['field']['handler'] = 'views_handler_field_xss';
  $data['entity_WorkflowTransition']['comment']['filter']['handler'] = 'views_handler_filter_string';
  $data['entity_WorkflowTransition']['stamp']['field']['handler'] = 'views_handler_field_date';
  $data['entity_WorkflowTransition']['stamp']['filter']['handler'] = 'views_handler_filter_date';
  $data['entity_WorkflowTransition']['stamp']['sort']['handler'] = 'views_handler_sort_date';

  $data['entity_WorkflowScheduledTransition']['old_sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowScheduledTransition']['old_sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['entity_WorkflowScheduledTransition']['sid']['field']['handler'] = 'workflow_views_handler_field_sid';
  $data['entity_WorkflowScheduledTransition']['sid']['filter']['handler'] = 'workflow_views_handler_filter_sid';
  $data['entity_WorkflowScheduledTransition']['uid']['field']['handler'] = 'workflow_views_handler_field_username';
  $data['entity_WorkflowScheduledTransition']['uid']['filter']['handler'] = 'views_handler_filter_user_name';
  $data['entity_WorkflowScheduledTransition']['comment']['field']['handler'] = 'views_handler_field_xss';
  $data['entity_WorkflowScheduledTransition']['comment']['filter']['handler'] = 'views_handler_filter_string';
  $data['entity_WorkflowScheduledTransition']['scheduled']['field']['handler'] = 'views_handler_field_date';
  $data['entity_WorkflowScheduledTransition']['scheduled']['filter']['handler'] = 'views_handler_filter_date';
  $data['entity_WorkflowScheduledTransition']['scheduled']['sort']['handler'] = 'views_handler_sort_date';
}

/**
 * Implements hook_views_data().
 */
function workflow_views_views_data() {

  // A 'workflow link'.
  $data['node']['workflow_node']['moved to'] = array('views_entity_node', 'workflow_node');
  $data['views_entity_node']['workflow_node'] = array(
    'field' => array(
      'title' => t('Workflow link'),
      'help' => t('Provide a link to the workflow history tab to change the state of the content.'),
      'handler' => 'workflow_views_handler_field_node_link_workflow',
    ),
  );

  return $data;
}
