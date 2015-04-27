<?php

/**
 * @file
 * Tokens hooks for Workflow Node module.
 *
 * These tokens are deprecated in favour of 'chained' tokens in main module.
 */

/**
 * Implements hook_token_info().
 */
function workflownode_token_info() {
  $types['workflow'] = array(
    'name' => t('Workflow'),
    'description' => t('Tokens related to workflows.'),
    'needs-data' => 'node',
  );

  // 'plain_tokens' indicates a 'chained_tokens' counterpart.
  $plain_tokens = array();
  $plain_tokens['workflow-name'] = array(
    'name' => t('Workflow name'),
    'description' => t('Name of workflow applied to this node.'),
  );
  $plain_tokens['workflow-current-state-name'] = array(
    'name' => t('Current state name'),
    'description' => t('Current state of content.'),
  );
  $plain_tokens['workflow-old-state-name'] = array(
    'name' => t('Old state name'),
    'description' => t('Old state of content.'),
  );
  $plain_tokens['workflow-current-state-updating-user-name'] = array(
    'name' => t('Username of last state changer'),
    'description' => t('Username of last state changer.'),
  );
  $plain_tokens['workflow-current-state-updating-user-link'] = array(
    'name' => t('Username link of last state changer'),
    'description' => t('Themed Username of last state changer.'),
  );
  $plain_tokens['workflow-current-state-updating-user-uid'] = array(
    'name' => t('Uid of last state changer'),
    'description' => t('uid of last state changer.'),
  );
  $plain_tokens['workflow-current-state-updating-user-mail'] = array(
    'name' => t('Email of last state changer'),
    'description' => t('email of last state changer.'),
  );
  $plain_tokens['workflow-current-state-updating-user-mail'] = array(
    'name' => t('Email link of last state changer'),
    'description' => t('email link of last state changer.'),
  );
  $plain_tokens['workflow-current-state-log-entry'] = array(
    'name' => t('Last workflow comment log'),
    'description' => t('Last workflow comment log.'),
  );
  $plain_tokens['workflow-current-state-date-iso'] = array(
    'name' => t('Current state date (ISO)'),
    'description' => t('Date of last state change (ISO).'),
  );
  $plain_tokens['workflow-current-state-date-tstamp'] = array(
    'name' => t('Current state date (timestamp)'),
    'description' => t('Date of last state change (timestamp).'),
  );
  $plain_tokens['workflow-current-state-date-formatted'] = array(
    'name' => t('Current state date (formatted by site default)'),
    'description' => t('Date of last state change (formatted by site default).'),
  );

  // Add support for custom date formats. (see token.tokens.inc)
  // @todo Remove when http://drupal.org/node/1173706 is fixed.
  $date_format_types = system_get_date_types();
  foreach ($date_format_types as $date_type => $date_info) {
    $plain_tokens['workflow-current-state-date-' . $date_type] = array(
      'name' => t('Current state date (using @format format)', array('@format' => $date_info['title'])),
      'description' => t("A date in '@type' format. (%date)", array('@type' => $date_type, '%date' => format_date(REQUEST_TIME, $date_type))),
      'module' => 'workflow',
    );
  }

  return array(
    'types' => $types,
    'tokens' => array(
      'workflow' => $plain_tokens,
      'node' => $plain_tokens,
    ),
  );
}

/**
 * Implements hook_tokens().
 *
 * This is still buggy for Workflow Field:
 * - we do not know upfront which field to read.
 * - some evaluations may not be correct.
 */
function workflownode_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();
  $sanitize = !empty($options['sanitize']);
  $langcode = !empty($options['language']->language) ? $options['language']->language : LANGUAGE_NONE;
  $date = REQUEST_TIME;

  if (($type == 'node' || $type == 'workflow') && !empty($data['node'])  && !empty($data['node']->nid)) {
    $node = $data['node'];
    $entity_id = $node->nid;
    $entity_type = 'node'; // @todo: support other entity types in workflow_tokens().
    $entity_bundle = $node->type;
    // We do not know which field we want to use. Just pick one.
    $field_name = ''; // For workflow_node, multiple workflow per node_type are not supported.

    if ($workflow = workflow_get_workflows_by_type($entity_bundle, $entity_type)) {
      // Get a list of all possible states for returning state names.
      $states = workflow_state_load_multiple();

      $last_history = workflow_transition_load_single($entity_type, $entity_id, '');

      if (isset($node->workflow) && !isset($node->workflow_stamp)) {
        // Workflow Node mode.
        // The node is being submitted but the form data has not been saved to the database yet,
        // so we set the token values from the workflow form fields.
        $uid = $node->uid;
        $account = user_load($uid);
        $new_sid = $node->workflow;
        $old_sid = isset($last_history->old_sid) ? $last_history->old_sid : $workflow->getCreationSid();
        $user_name = ($uid && isset($node->name)) ? $node->name : variable_get('anonymous', 'Anonymous');
        $mail = ($uid && isset($node->user_mail)) ? $node->user_mail : '';
        $comment = isset($node->workflow_comment) ? $node->workflow_comment : '';
      }
      else {
        // Workflow Node/Workflow Field mode.
        if (empty($last_history)) {
          // If the node has no workflow history, the node is being inserted
          // and will soon be transitioned to the first valid state.
          $uid = $node->uid;
          $account = user_load($uid);
          $old_sid = $workflow->getCreationSid();
          $new_sid = $workflow->getFirstSid($entity_type, $node, '', $account, FALSE);
          if (!$new_sid) {
            // Some users may not be allowed to cause transitions, so there
            // will be no choices. What to choose?
            $new_sid = workflow_node_current_state($node, $entity_type, $field_name);
          }
          $user_name = ($uid && isset($node->name)) ? $node->name : variable_get('anonymous', 'Anonymous');
          $mail = ($uid && isset($node->user_mail)) ? $node->user_mail : '';
          $comment = isset($node->workflow_comment) ? $node->workflow_comment : '';
        }
        else {
          // We now know the correct field_name.
          $field_name = $last_history->field_name; // @todo: support multiple Workflow fields per entity in workflow_tokens().

          // Sometimes there is no workflow set (edit?).
          // Is a transition in progress?
          $current_sid = workflow_node_current_state($node, $entity_type, $field_name);
          if ($current_sid != $last_history->new_sid) {
            $new_sid = $current_sid;
            $old_sid = $last_history->new_sid;
            $date = REQUEST_TIME; // $node->workflow_stamp;
            $uid = $node->uid;
          }
          else {
            // Not a transition.
            $new_sid = $last_history->new_sid;
            $old_sid = $last_history->old_sid;
            $date = $last_history->stamp;
            $uid = $last_history->uid;
          }

          $account = user_load($uid);
          // Default to the most recent transition data in the workflow history table.
          $user_name = $account->uid ? $account->name : variable_get('anonymous', 'Anonymous');
          $mail = $account->uid ? $account->mail : '';
          $comment = $last_history->comment;
        }
      }

      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'workflow-name':
            $replacements[$original] = $sanitize ? check_plain($workflow->label()) : $workflow->label();
            break;

          case 'workflow-current-state-name':
            $replacements[$original] = $sanitize ? check_plain($states[$new_sid]->getName()) : $states[$new_sid]->getName();
            break;

          case 'workflow-old-state-name':
            $old_state_name = (!empty($states[$old_sid])) ? $states[$old_sid]->getName() : 'Deleted state';
            $replacements[$original] = $sanitize ? check_plain($old_state_name) : $old_state_name;
            break;

          case 'workflow-current-state-updating-user-name':
            $name = format_username($account);
            $replacements[$original] = $sanitize ? check_plain($name) : $name;
            break;

          case 'workflow-current-state-updating-user-link':
            $replacements[$original] = theme('username', array('account' => $account));
            break;

          case 'workflow-current-state-updating-user-uid':
            // User IDs are integers only and do not need sanitization.
            $replacements[$original] = $uid;
            break;

          case 'workflow-current-state-updating-user-mail':
            $replacements[$original] = $sanitize ? check_plain($account->mail) : $account->mail;
            break;

          case 'workflow-current-state-updating-user-mailto':
            $replacements[$original] = '<a href="mailto:' . check_plain($account->mail) . '">' . check_plain($user_name) . '</a>';
            break;

          case 'workflow-current-state-log-entry':
            $replacements[$original] = $sanitize ? check_markup($comment, filter_default_format(), $langcode) : $comment;
            break;

          case 'workflow-current-state-date-iso':
            $replacements[$original] = format_date($date, 'custom', 'c', NULL, $langcode);
            break;

          case 'workflow-current-state-date-tstamp':
            $replacements[$original] = $sanitize ? check_plain($date) : $date;
            break;

          case 'workflow-current-state-date-formatted':
            $replacements[$original] = format_date($date, 'medium', '', NULL, $langcode);
            break;

          default:
            // Add support for custom date formats. (see token.tokens.inc)
            // @todo Remove when http://drupal.org/node/1173706 is fixed.
            // @todo It seems the custom date formats work normally in D7.22 with chained tokens.
            $date_format_types = system_get_date_types();
            foreach ($date_format_types as $date_type => $date_info) {
              if ($name == 'workflow-current-state-date-' . $date_type) {
                $replacements[$original] = format_date($date, $date_type, '', NULL, $langcode);
              }
            }
            break;
        }
      }
    }
  }

  return $replacements;
}
