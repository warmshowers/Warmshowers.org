<?php

/**
 * @file
 * Tokens hooks for Workflow module.
 *
 * To every entity type, a default Workflow token tree is added. To support
 * multiple tokens per entity bundle, an extra token tree 'Workflows' is
 * created.
 *
 * How to test?
 * - Enable module 'Token'; use page admin/help/token;
 * - Enable module 'Token example'; use page examples/token;
 * - Enable module Automatic Entity Label, set a label, and save entity.
 */

/**
 * Implements drupal_alter('token_field_info', $info) from token.tokens.inc.
 *
 * It would be nice if this would work! But it doesn't...
 */
/*
// function workflow_token_field_info_alter(&$info) {
//   $workflow_field_info = _workflow_info_fields();
//   foreach($workflow_field_info as $field_name => $field_info) {
//     if (array_key_exists($field_name, $info)) {
//       $info[$field_name]['type'] = 'WorkflowLastTransition';
//       $info[$field_name]['module'] = 'workflow';
//     }
//   }
// }
 */

/**
 * Adds a subtree to each WorkflowField.
 *
 * ATM we only generate tokens for the last transition of a field.
 */
function workflow_token_info_alter(&$data) {

  foreach ($data['tokens'] as $object => &$tokens) {
    // Add a token for scheduling, in 'seconds ago' format.
    if ($object == 'date' && !isset($tokens['seconds'])) {
      $tokens['seconds'] = array(
        'name'  => 'Seconds-since',
        'description' => "A date in 'seconds ago' format (<i>604800</i>). Use it for easy scheduling workflow transitions.",
        'module' => 'workflow',
      );
    }

    // High-jack the fields (they do not have sub-tokens, yet).
    foreach ($tokens as &$token) {
      // Caveat: the following algorithm is just a guess.
      if (isset($token['module']) && $token['module'] == 'token') {
        if (isset($token['description']) && 0 == substr_compare($token['description'], 'Workflow', 0, 8)) {
          $token['type'] = 'WorkflowTransition';
          $token['module'] = 'workflow';
        }
      }
    }
  }
}

/**
 * Implements hook_token_info().
 *
 * Adds tokens and token types, for Field, Workflow, State and Transition,
 * using the names of the entities from Workflow module.
 * Lots of tokens are already defined via entity_properties.
 * D7: a dependency on entity_token exists.
 *
 * @see workflow_entity_property_info_alter()
 */
function workflow_token_info() {

  if (!module_exists('entity_token')) {
    return array();
  }

  /*
   * Token types.
   */
  $types['WorkflowField'] = array(
    'name' => t('Workflows'),
    'description' => t('Tokens related to workflows.'),
    'needs-data' => 'entity',
  );
  $types['WorkflowTransition'] = array(
    'name' => t('Workflow Transition'),
    'description' => t('Tokens related to workflow transitions.'),
    // 'needs-data' => 'entity',
    'needs-data' => 'WorkflowField',
  );
  $types['WorkflowState'] = array(
    'name' => t('Workflow State'),
    'description' => t('Tokens related to workflow state.'),
    'needs-data' => 'WorkflowTransition',
  );
  $types['Workflow'] = array(
    'name' => t('Workflow'),
    'description' => t('Tokens related to workflows.'),
    'needs-data' => 'WorkflowTransition',
  );

  /*
   * Chained tokens for nodes.
   */
  $last_transition = array(
    'name' => t('Workflow last transition'),
    'description' => t('Last workflow state transition of content.'),
    'type' => 'WorkflowTransition',
    'module' => 'workflow',
  );

  // Add a token tree to each core entity type. This allows easy reference
  // in the majority of cases.
  $workflow_field['last-transition'] = $last_transition;
  $entity['last-transition'] = $last_transition;
  $user['last-transition'] = $last_transition;
  $node['last-transition'] = $last_transition;
  $term['last-transition'] = $last_transition;

  $return = array(
    'types' => $types,
    'tokens' => array(
      // 'entity' => $entity, // #2272121
      'user' => $user,
      'node' => $node,
      'term' => $term,
      'WorkflowField' => $workflow_field,
      // 'WorkflowTransition' => $workflow_transition,
      // 'WorkflowState' => $workflow_state,
      // 'Workflow' => $workflow,
    ),
  );

  return $return;
}

/**
 * Implements hook_tokens().
 *
 * N.B. Keep the following functions aligned when changing properties:
 * - workflow_tokens()
 * - workflow_entity_property_info_alter()
 * - workflow_views_views_data_alter()
 */
function workflow_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $sanitize = !empty($options['sanitize']);
  $langcode = isset($options['language']) ? $options['language']->language : NULL;

  // The 'node' tokens have already been replaced with 'entity'.
  // Skip for easier debugging.
  // @todo: is this always the case, or only if Entity Tokens is enabled?
  if ($type == 'node' || $type == 'user' || $type == 'term') {
    return $replacements;
  }

  if ($type == 'date' && !empty($data['date'])) {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'seconds':
          // This is our custom date token in 'seconds ago' format.
          $seconds = REQUEST_TIME - $data['date'];
          $replacements[$original] = $seconds;
          // Avoid reporcessing in the remainder of this function.
          break;
      }
    }
    return $replacements;
  }
  elseif ($type == 'entity' && !empty($data['entity'])) {
    // new, chained tokens, as of version 7.x-2.3.
    $entity = $data['entity'];
    $entity_type = $data['entity_type'];
    // $token_type = $data['token_type'];

    foreach ($tokens as $name => $original) {
      if (FALSE !== strpos($name, ':')) {
        // This is a chained property (contains ':').
      }
      elseif (isset($data['workflow_token_type'])) {
        // This part taken from entity_token_tokens().
        $wrapper = entity_metadata_wrapper($data['workflow_token_type'], $data[$data['workflow_token_type']]);
        $property_name = str_replace('-', '_', $name);
        try {
          if ($name == 'workflow' ||
              $name == 'states' ||
              $name == 'transitions') {
            $replacement = _workflow_token_get_token($wrapper->$property_name, $options);
          }
          else {
            $replacement = _entity_token_get_token($wrapper->$property_name, $options);
          }
          if (isset($replacement)) {
            $replacements[$original] = $replacement;
          }
        }
        catch (EntityMetadataWrapperException $e) {
          // dpm('token not found: ' . $name);
          // If tokens for not existing values are requested, just do nothing.
        }
      }
    }

    // If this is a Last Transition, get subtokens for it.
    if ($sub_tokens = token_find_with_prefix($tokens, 'last-transition')) {
      // Get the workflow tokens from the transition of this entity.
      $transition = _workflow_tokens_get_transition($entity_type, $entity, NULL);
      $sub_data = array(
        'WorkflowTransition' => $transition,
        'workflow_token_type' => 'WorkflowTransition',
      );
      $sub_data += $data;
      $replacements += token_generate('entity', $sub_tokens, $sub_data, $options);
    }

    // If this is a WorkflowField, get subtokens for it.
    $name_parts = explode(':', $name, 2);
    $field_name = reset($name_parts);
    if ($field_name != 'created' && isset($entity->{$field_name}) && $sub_tokens = token_find_with_prefix($tokens, $field_name)) {
      $transition = _workflow_tokens_get_transition($entity_type, $entity, $field_name);
      $sub_data = array(
        'WorkflowTransition' => $transition,
        'workflow_token_type' => 'WorkflowTransition',
      );
      $sub_data += $data;
      $replacements += token_generate('entity', $sub_tokens, $sub_data, $options);
    }

    if (isset($data['WorkflowTransition'])) {
      $transition = $data['WorkflowTransition'];
      $field_name = $transition->field_name;

      if ($sub_tokens = token_find_with_prefix($tokens, 'Workflow')) {
        $sub_data = array(
          'Workflow' => $transition->getWorkflow(),
          'workflow_token_type' => 'Workflow',
        );
        $sub_data += $data;
        $replacements += token_generate('entity', $sub_tokens, $sub_data, $options);
      }

      // Unify to old-state, new-state.
      // Do not use underscores, or workflow_tokens will not work!
      if ($sub_tokens = token_find_with_prefix($tokens, 'old-state')) {
        $sub_data = array(
          'WorkflowState' => $transition->getOldState(),
          'workflow_token_type' => 'WorkflowState',
        );
        $sub_data += $data;
        $replacements += token_generate('entity', $sub_tokens, $sub_data, $options);
      }
      if ($sub_tokens = token_find_with_prefix($tokens, 'new-state')) {
        $sub_data = array(
          'WorkflowState' => $transition->getNewState(),
          'workflow_token_type' => 'WorkflowState',
        );
        $sub_data += $data;
        $replacements += token_generate('entity', $sub_tokens, $sub_data, $options);
      }
      if ($sub_tokens = token_find_with_prefix($tokens, 'user')) {
        if (isset($data['WorkflowTransition'])) {
          $sub_entity = $data['WorkflowTransition'];
        }
        $sub_data = array(
          'entity_type' => 'user',
          'user' => user_load($sub_entity->uid),
          'token_type' => 'user',
        );
        $replacements += token_generate('user', $sub_tokens, $sub_data, $options);
      }
      if ($sub_tokens = token_find_with_prefix($tokens, 'created')) {
        $sub_data = array(
          'entity_type' => 'date',
          'date' => $data['WorkflowTransition']->stamp,
          'token_type' => 'date',
        );
        $replacements += token_generate('date', $sub_tokens, $sub_data, $options);
      }
    }
  }
  return $replacements;
}

/**
 * Gets the token replacement by correctly obeying the options.
 *
 * Taken from _entity_token_get_token().
 */
function _workflow_token_get_token($wrapper, $options) {

  // if ($wrapper->value() === NULL) {
  // // Do not provide a replacement if there is no value.
  // return NULL;
  // }

  if (empty($options['sanitize'])) {
    // When we don't need sanitized tokens decode already sanitizied texts.
    $options['decode'] = TRUE;
  }
  $langcode = isset($options['language']) ? $options['language']->language : LANGUAGE_NONE;

  // If there is a label for a property, e.g., defined by an options list or an
  // entity label, make use of it.
  if ($label = $wrapper->label()) {
    return empty($options['sanitize']) ? $label : check_plain(t($label, array(), array('langcode' => $langcode)));
  }

  switch ($wrapper->type()) {
    case 'Workflow':
    case 'WorkflowState':
    case 'WorkflowTransition':
      return $wrapper->value();
  }

  // Care for outputing list values.
  if ($wrapper instanceof EntityListWrapper) {
    $output = array();
    foreach ($wrapper as $item) {
      $output[] = _workflow_token_get_token($item, $options);
    }
    return implode(', ', $output);
  }
  // Else we do not have a good string to output, e.g., for struct values. Just
  // output the string representation of the wrapper.
  return (string) $wrapper;
}

/**
 * Helper function to get the Transition.
 *
 * If the node is being created or updated (using the NUMERIC id),
 * do not read from db, since the field widget has not been processed yet.
 * @todo: solve this with module weight?
 */
function _workflow_tokens_get_transition($entity_type, $entity, $field_name) {
  global $user;
  $transitions = array();

  list($entity_id, , $entity_bundle) = entity_extract_ids($entity_type, $entity);

  $langcode = _workflow_metadata_workflow_get_properties($entity, array(), 'langcode', $entity_type, $field_name);

  if (!empty($entity_id) && !isset($entity->original)) {
    $transition = workflow_transition_load_single($entity_type, $entity_id, $field_name);
    return $transition;
  }

  // Get transition data from online-data.
  // Create dummy transitions, just to set $node->workflow_transitions[].
  foreach (_workflow_info_fields($entity, $entity_type, $entity_bundle) as $found_name => $field) {
    // If $field_name = NULL, any workflow_field/node is OK.
    if ($field_name <> NULL && $found_name != $field_name) {
      continue;
    }

    $old_sid = FALSE;
    $new_sid = $found_name ? _workflow_get_sid_by_items($entity->{$found_name}[$langcode]) : $entity->workflow_sid;

    if (!isset($entity_id)) {
      // Creating an entity.
      // When creating a node, and only 1 valid sid is available, then the
      // widget is not shown. This generates a problem, since the new/old
      // sid isn't yet loaded in the Entity.
      if ($new_state = workflow_state_load_single($new_sid)) {
        $workflow = $new_state->getWorkflow();
        $old_sid = $workflow->getCreationSid();
      }
      else {
        // $field['settings']['wid'] can be numeric or named.
        $workflow = workflow_load_single($field['settings']['wid']);
        $old_sid = $workflow->getCreationSid();
        $new_sid = $workflow->getFirstSid($entity_type, $entity, $field_name, $user, FALSE);
      }
    }
    elseif (isset($entity->original)) {
      if ($field_name && !isset($entity->original->{$found_name}[$langcode])) {
        // When updating a node, that did not have a workflow attached before.
        // (Happens when you add workflows to existing Entity types.)
        $old_sid = workflow_node_previous_state($entity, $entity_type, $found_name);
      }
      elseif ($field_name) {
        // Updating an entity.
        $old_sid = _workflow_get_sid_by_items($entity->original->{$found_name}[$langcode]);
      }
      else {
        $old_sid = workflow_node_previous_state($entity, $entity_type, $found_name);
      }
    }

    $transition = new WorkflowTransition();
    $transition->setValues($entity_type, $entity, $field_name, $old_sid, $new_sid, $user->uid, REQUEST_TIME, '');

    // Store the transition, so it can be easily fetched later on.
    // Store in an array, to prepare for multiple workflow_fields per entity.
    // This is a.o. used in hook_entity_update to trigger 'transition post'.
    $transitions[$field_name] = $transition;
  }

  $transition = ($field_name && isset($transitions[$field_name])) ? $transitions[$field_name] : reset($transitions);
  return $transition;
}
