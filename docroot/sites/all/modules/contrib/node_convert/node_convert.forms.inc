<?php

/**
 * @file
 * Form page callbacks for the node_convert module.
 */

function node_convert_conversion_form($form, $form_state, $node) {
  $form = array();

  /* Setting the steps */
  if (!isset($form_state['values']['step'])) {
    $op = 'choose_destination_type';
  }
  elseif ($form_state['values']['step'] == 'choose_destination_type') {
    $op = 'choose_destination_fields';
  }
  $form['step'] = array(
    '#type' => 'value',
    '#value' => $op,
  );
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  /* Form steps */
  if ($op == 'choose_destination_type') {
    $type = node_type_get_name($node);
    // Remember current node type, used in theme_ function
    $form['current_type'] = array(
      '#markup' => $type,
    );
    // Get available content types
    $types = node_convert_return_access_node_types('to');
    if ($types != FALSE) {
      $key = array_search($form['current_type']['#markup'], $types);
      // Delete the current content type from the list
      if ($key !== FALSE) {
        unset($types[$key]);
      }
      $options = $types;
      // Populate the select with possible content types
      $form['destination_type'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t("To what content type should this node be converted"),
      );
    }
    else {
      // Just used as a message, not sure if it's the best implementation
      $form['destination_type'] = array(
        '#markup' => t("You don't have access to any node types."),
      );
    }
  }
  elseif ($op == 'choose_destination_fields') {
    // $source_fields = content_types($node->type);
    // Get source type fields
    $source_fields = field_info_instances('node', $node->type);
    $fields_info = field_info_fields();
    // In case there are no fields, just convert the node type
    if (count($source_fields) == 0) {
      $no_fields = TRUE;
    }
    // Otherwise
    else {
      $no_fields = FALSE;
      // Get destination type fields
      $dest_fields = field_info_instances('node', $form_state['storage']['destination_type']);
      $i = 0;
      foreach ($source_fields as $source_field_name => $source_field) {
        $i++;
        $options = array();
        $options['discard'] = 'discard';
        $options[APPEND_TO_BODY] = t('Append to body');
        $options[REPLACE_BODY] = t('Replace body');

        // Insert destination type fields into $options that are of the same type as the source.
        foreach ($dest_fields as $dest_field_name => $dest_value) {
          if ($fields_info[$source_field_name]['type'] == $fields_info[$dest_field_name]['type']) {
            $options[$dest_value['field_name']] = $dest_value['field_name'];
          }
        }
        // Remember the source fields to be converted
        $form['source_field_' . $i] = array(
          '#type' => 'value',
          '#value' => $source_field['field_name'],
        );

        $form['container_' . $i] = array(
          '#type' => 'container',
          '#suffix' => '<br />'
        );

        // The select populated with possible destination fields for each source field
        // If the destination node type has the same field as the source node type, the default value is set to it.
        $form['container_' . $i]['dest_field_' . $i] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => $source_field['field_name'],
          '#title' => check_plain($source_field['field_name']) . " " . t("should be inserted into"),
        );

        // Print the current value of the source field
        $temp_value = node_convert_format_field_value($node, $fields_info[$source_field_name]);
        $form['container_' . $i]['current_field_value_' . $i] = array(
          '#type' => 'item',
          '#markup' => '<div>' . t("Current value is:") . " <b>" . $temp_value . '</b></div>',
        );
      }
      $form['number_of_fields'] = array(
        '#type' => 'value',
        '#value' => $i,
      );
    }
    $form['no_fields'] = array(
      '#type' => 'value',
      '#value' => $no_fields,
    );

    $hook_options = node_convert_invoke_all('node_convert_change', array('dest_node_type' => $form_state['storage']['destination_type']), 'options');
    if (!empty($hook_options)) {
      $form['hook_options'] = $hook_options;
      array_unshift($form['hook_options'], array('#value' => '<br /><strong>' . t("Also the following parameters are available:") . '</strong>'));
      $form['hook_options']['#tree'] = TRUE;
    }
  }

  if ($op != 'choose_destination_fields' && isset($types) && $types != FALSE) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Next"),
    );
  }
  elseif ($op == 'choose_destination_fields') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t("Convert"),
      '#weight' => 100,
    );
  }

  return $form;
}

function node_convert_conversion_form_validate($form, &$form_state) {
  if ($form_state['values']['step'] == 'choose_destination_fields') {
    node_convert_invoke_all('node_convert_change', array('dest_node_type' => $form_state['storage']['destination_type'], 'form_state' => $form_state), 'options validate');
  }
}

function node_convert_conversion_form_submit($form, &$form_state) {
  // Remember the destination type
  if ($form_state['values']['step'] == 'choose_destination_type') {
    $form_state['rebuild'] = TRUE;
    $form_state['storage']['destination_type'] = $form_state['values']['destination_type'];
  }
  elseif ($form_state['values']['step'] == 'choose_destination_fields') {
    // Information needed in the convert process: nid, vid, source type, destination type
    $dest_node_type = $form_state['storage']['destination_type'];
    $node = $form_state['values']['node'];
    $nid = $node->nid;
    $no_fields = $form_state['values']['no_fields'];
    $number_of_fields = $form_state['values']['number_of_fields'];

    // If there are fields that can to be converted
    if ($no_fields == FALSE) {
      for ($i = 1; $i <= $number_of_fields; $i++) {
        $source_fields[] = $form_state['values']['source_field_' . $i]; //  Source fields
        $dest_fields[] = $form_state['values']['dest_field_' . $i]; // Destination fields
      }
    }
    if (!empty($form['hook_options'])) {
      $hook_options = $form_state['values']['hook_options'];
    }
    else {
      $hook_options = NULL;
    }
    $result = node_convert_node_convert($nid, $dest_node_type, $source_fields, $dest_fields, $no_fields, $hook_options);
    // We display errors if any, or the default success message.
    node_convert_messages($result, array('nid' => $nid));
    // We clear the storage so redirect works
    $form_state['storage'] = array();
    $form_state['redirect'] = "node/" . $nid;
  }
}