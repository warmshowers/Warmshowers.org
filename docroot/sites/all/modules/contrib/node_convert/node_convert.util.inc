<?php

/**
 * @file
 * API and Utility Functions.
 */

/**
 * Converts a node to another content type.
 *
 * @param $nid
 *   The nid of the node to be converted.
 * @param $destination_node_type
 *   A string containing the destination node type of the node.
 * @param $source_fields
 *   An array containing the source field names.
 * @param $destination_fields
 *   An array containing the destination field names.
 * @param $no_fields_flag
 *   A boolean containing if there are source fields that have to be converted.
 * @param $hook_options
 *   An array containing values used by the hook_node_convert_change functions.
 * @return bool Returns the $nid.
 */
function node_convert_node_convert($nid, $destination_node_type, $source_fields, $destination_fields, $no_fields_flag, $hook_options = NULL) {
  $node = node_load($nid);
  if ($node == FALSE) {
    return FALSE;
  }

  // Change the node type in the DB
  db_update('node')->fields(array('type' => $destination_node_type))->condition('nid', $nid)->execute();

  // If there are fields that can be converted
  if ($no_fields_flag == FALSE) {

    // Conversion process for each field
    $re_save_body_field = FALSE;

    // Use node revisions to extract all field revision in node_convert_field_convert
    $node_revisions = node_revision_list($node);

    foreach ($source_fields as $key => $field) {
      $replaced_body = node_convert_field_convert($node, $field, $destination_fields[$key], $destination_node_type, $node_revisions);
      if ($replaced_body == REPLACE_BODY) {
        $re_save_body_field = TRUE;
      }
    }
    // If something was appended to the body, or replaced the body, we update body field.
    if ($re_save_body_field == TRUE) {
      $field_body = field_info_fields();
      $field_body = $field_body['body'];
      $field_ids = array($field_body['id'] => $field_body['id']);
      module_invoke($field_body['storage']['module'], 'field_storage_write', 'node', $node, FIELD_STORAGE_UPDATE, $field_ids);
    }
  }
  // We collate data to send to the hook implementations
  $data = array(
    'node' => $node,
    'dest_node_type' => $destination_node_type,
  );
  if (!empty($hook_options)) {
    $data['hook_options'] = $hook_options;
  }

  // We make sure that all custom node modules do their changes at the appropriate steps
  node_convert_invoke_all('node_convert_change', $data, 'insert');
  node_convert_invoke_all('node_convert_change', $data, 'delete');

  // TODO Check what to do about token cache clearing.
  /* if (module_exists('token')) {
    token_get_values('global', NULL, TRUE);
  }*/
  cache_clear_all('field:node:' . $node->nid, 'cache_field');
  $converted_node = node_load($nid, NULL, TRUE);
  $converted_node->type = $destination_node_type;
  try {
    module_invoke_all('node_convert_presave', $converted_node, $hook_options);
    node_save($converted_node);

    if (module_exists('rules')) {
      rules_invoke_event('node_convert_converted_node', $converted_node);
    }

  }
  catch (Exception $e) {
    drupal_set_message(t('Caught exception: @exception', array('@exception' => $e->getMessage())), 'error');
    $nid = FALSE;
  }

  return $nid;
}

/**
 * Transfers information from source_field to dest_field.
 *
 * @param $node
 *   The node object to be converted.
 * @param $source_field
 *   A string containing the name of the source field which contains to be transferred information.
 * @param $destination_field
 *   A string containing the name of the destination field where the information should be stored.
 * @param string $destination_node_type
 *   A string containing the destination node type of the node.
 * @param array $node_revisions
 * @return string Returns REPLACE_BODY if something replaced the body field, or was appended to it.
 */
function node_convert_field_convert(&$node, $source_field, $destination_field, $destination_node_type = '', $node_revisions = array()) {
  $field_info_source = field_info_fields(); // Get source field information
  $field_info_source = $field_info_source[$source_field];
  $db_info_source = $field_info_source['storage']; // Get DB specific source field information

  if ($destination_field == 'discard') {
    // Delete node info in the separate field table
    node_convert_invoke_field_storage_delete($field_info_source, $db_info_source, $node);
    return NULL;
  }

  $field_info_destination = array();
  $db_info_destination = array();
  if (!in_array($destination_field, array('discard', APPEND_TO_BODY, REPLACE_BODY))) {
    $field_info_destination = field_info_fields($destination_field); // Get destination field information
    $field_info_destination = $field_info_destination[$destination_field]; // Get destination field information
    $db_info_destination = $field_info_destination['storage']; // Get DB specific destination field information
  }

  // We save each field value from the DB for transfer. (this only applies to the current revision of the field)
  $source_values = field_get_items('node', $node, $source_field);

  if (count($node_revisions) > 1 && !in_array($destination_field, array(APPEND_TO_BODY, REPLACE_BODY))) {
    // Get all field revisions for current node
    $field_revision_values = array();
    $field_revision_source_table = current(array_keys($db_info_source['details']['sql']['FIELD_LOAD_REVISION']));
    $field_revision_destination_table = current(array_keys($db_info_destination['details']['sql']['FIELD_LOAD_REVISION']));

    $source_columns = array('entity_type', 'entity_id', 'revision_id', 'bundle', 'delta', 'language');
    foreach ($field_info_source['columns'] as $column => $attributes) {
      $source_columns[] = _field_sql_storage_columnname($source_field, $column);
    }

    $revision_query = db_select($field_revision_source_table, 'r', array('fetch' => PDO::FETCH_ASSOC))
      ->condition('entity_type', 'node')
      ->condition('bundle', $node->type)
      ->condition('entity_id', $node->nid)
      ->condition('revision_id', $node->vid, '<>')
      ->fields('r', $source_columns)->execute();

    // Change the bundle to the destination type of the node
    foreach ($revision_query as $row) {
      $row['bundle'] = $destination_node_type;
      $field_revision_values[] = $row;
    }

    // Remove all field revisions for current field in DB
    node_convert_invoke_field_storage_delete($field_info_source, $db_info_source, $node);

    // Reinsert the field revisions in the destination field revision table
    $query = db_insert($field_revision_destination_table);
    $columns = array('entity_type', 'entity_id', 'revision_id', 'bundle', 'delta', 'language');
    foreach ($field_info_destination['columns'] as $column => $attributes) {
      $columns[] = _field_sql_storage_columnname($destination_field, $column);
    }
    $query->fields($columns);
    foreach ($field_revision_values as $row) {
      $query->values(array_values($row));
    }
    $query->execute();

  }
  else {
    // After getting the source field values, we delete the values stored in the DB (this deletes values for all field revisions)
    node_convert_invoke_field_storage_delete($field_info_source, $db_info_source, $node);
  }

  // The source field value should be appended to the body or replaced.
  if ($destination_field == APPEND_TO_BODY || $destination_field == REPLACE_BODY) {
    static $node_body = '';
    //static $node_teaser = '';

    // We try to get the node body from a static variable, which means we did some body manipulations, otherwise load it.
    if (empty($node_body)) {
      $node_body_field = field_get_items('node', $node, 'body');
      $node_body = $node_body_field[0]['value'];
      //$node_teaser = $node_body_field[0]['summary'];
    }

    // Double check we have values in the field.
    if (is_array($source_values)) {
      // Get the field value.
      $field_value = node_convert_format_field_value($node, $field_info_source, TRUE);

      if ($destination_field == APPEND_TO_BODY) {
        $node_body = $node_body . "\n" . $field_value;
        //$node_teaser = $node_teaser . "\n" . $field_value['value'];
      }
      elseif ($destination_field == REPLACE_BODY) {
        $node_body = $field_value;
        //$node_teaser = $field_value['value'];
      }
      $lang_code = field_language('node', $node, $source_field);
      $node->body[$lang_code][0]['value'] = $node_body;
      //$node->body[$lang_code][0]['summary'] = $node_teaser;
    }

    return REPLACE_BODY;
  }

  // We put each field value back into the DB
  // To do it we first get the id of the field, then we find its language code from the source value
  // We add $source_values into the node object, and invoke field_storage write
  $field_ids = array($field_info_destination['id'] => $field_info_destination['id']);
  $lang_code = field_language('node', $node, $source_field);

  // Make sure that we actually have values in the source field
  if ($source_values !== FALSE) {
    $node->{$destination_field}[$lang_code] = $source_values;
  }
  else {
    $node->{$destination_field} = array();
  }

  // Give possibility to fields to pre-process their data
  // (e.g. Link module transforms attribute array into a serialized array before insertion)
  field_attach_presave('node', $node);
  // For some reason link_field_presave doesn't exist anymore, so we have to call it the processing function used inside manually.
  if ($field_info_destination['type'] == 'link_field') {
    $instances = field_info_instances('node', $destination_node_type);
    link_field_update('node', $node, $field_info_destination, $instances[$destination_field], $lang_code, $node->{$destination_field}[$lang_code]);
  }
  try {
    module_invoke($db_info_destination['module'], 'field_storage_write', 'node', $node, FIELD_STORAGE_INSERT, $field_ids);
  } catch (Exception $e) {
    drupal_set_message(t('Caught exception: @exception', array('@exception' => $e->getMessage())));
  }
  return NULL;
}

function node_convert_invoke_field_storage_delete($field_info_source, $db_info_source, $node) {
  $field_id = $field_info_source['id'];
  module_invoke($db_info_source['module'], 'field_storage_delete', 'node', $node, array($field_id => $field_id));
}

/**
 * Displays error messages if any occurred, otherwise the success message.
 *
 * @param $result
 *   The result value of the node conversion. Possible values
 *   - FALSE  Displays an error message.
 *   - Any other  Displays success message.
 * @param $params
 *   An array containing message parameters. Possible values
 *   - display_success  If TRUE, the success message will be displayed, otherwise no message is displayed.
 *   Default is TRUE.
 */
function node_convert_messages($result, $params = array()) {
  $params += array('display_success' => TRUE);

  $message_arguments = array('@nid' => $params['nid']);
  if ($result == FALSE) {
    $message = "Conversion failed for node nid @nid.";

    watchdog(NODE_CONVERT_WATCHDOG, $message, $message_arguments, WATCHDOG_ERROR);
    drupal_set_message(t($message, $message_arguments), 'error');
  }
  elseif ($params['display_success'] == TRUE) {
    $message = 'Node @nid has been converted successfully.';

    drupal_set_message(t($message, $message_arguments));
    watchdog(NODE_CONVERT_WATCHDOG, $message, $message_arguments, WATCHDOG_INFO);
  }

}

/**
 * Returns a string containing the value of the $field from the $node object.
 *
 * @param $node
 *   A $node object
 * @param $field
 *   The field who's value to get.
 * @param bool $no_labels Returns values only.
 * @return string A string containing the value of the $field from the $node object.
 */
function node_convert_format_field_value($node, $field, $no_labels = FALSE) {
  /* @var $field_values Array */
  $field_values = field_get_items('node', $node, $field['field_name']);
  $options = array();

  if ($field['type'] == 'text') {
    $options = array('singular_label' => 'Text', 'plural_label' => 'Texts', 'columns' => array('safe_value'));
  }
  elseif ($field['type'] == 'text_with_summary') {
    $options = array('singular_label' => 'Text w/Summary', 'plural_label' => 'Texts w/Summary', 'columns' => array('safe_value'));
  }
  elseif ($field['type'] == 'image') {
    $options = array('singular_label' => 'File ID', 'plural_label' => 'File IDs', 'columns' => array('fid', 'Title' => 'title', 'URI' => 'uri'));
  }
  elseif ($field['type'] == 'link_field') {
    $options = array('singular_label' => 'Link', 'plural_label' => 'Links', 'columns' => array('url', 'title'));
  }
  elseif ($field['type'] == "email") {
    $options = array('singular_label' => 'Email', 'plural_label' => 'Emails', 'columns' => array('email'));
  }
  elseif ($field['type'] == 'file') {
    $options = array('singular_label' => 'File ID', 'plural_label' => 'File IDs', 'columns' => array('fid', 'URI' => 'uri'));
  }
  elseif ($field['type'] == 'taxonomy_term_reference') {
    $options = array('singular_label' => 'Term ID', 'plural_label' => 'Term IDs', 'columns' => array('tid'));
  }
  elseif ($field['type'] == 'node_reference') {
    $options = array('singular_label' => 'Node ID', 'plural_label' => 'Node IDs', 'columns' => array('nid'));
  }
  elseif ($field['type'] == 'user_reference') {
    $options = array('singular_label' => 'User ID', 'plural_label' => 'User IDs', 'columns' => array('uid'));
  }
  elseif ($field['type'] == 'entityreference') {
    $options = array('singular_label' => 'Entity ID', 'plural_label' => 'Entity IDs', 'columns' => array('target_id'),
      'prefix_label' => 'Entity Type: ' . $field['settings']['target_type']);
  }
  elseif ($field['type'] == 'date') {
    $options = array('singular_label' => 'Date', 'plural_label' => 'Dates', 'columns' => array('value', 'timezone'));
  }
  elseif ($field['type'] == 'datestamp') {
    $options = array('singular_label' => 'Date-stamp', 'plural_label' => 'Date-stamps', 'columns' => array('value', 'timezone'));
  }
  elseif ($field['type'] == 'datetime') {
    $options = array('singular_label' => 'Date-time', 'plural_label' => 'Date-time', 'columns' => array('value', 'timezone'));
  }
  elseif ($field['type'] == 'number_decimal') {
    $options = array('singular_label' => 'Decimal', 'plural_label' => 'Decimals', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'number_float') {
    $options = array('singular_label' => 'Float', 'plural_label' => 'Floats', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'number_integer') {
    $options = array('singular_label' => 'Integer', 'plural_label' => 'Integers', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'list_boolean') {
    $options = array('singular_label' => 'List Boolean', 'plural_label' => 'List Booleans', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'list_float') {
    $options = array('singular_label' => 'List Float', 'plural_label' => 'List Floats', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'list_integer') {
    $options = array('singular_label' => 'List Integer', 'plural_label' => 'List Integers', 'columns' => array('value'));
  }
  elseif ($field['type'] == 'list_text') {
    $options = array('singular_label' => 'List Text', 'plural_label' => 'List Texts', 'columns' => array('value'));
  }
  elseif (isset($field_values[0]['value'])) {
    $options['columns'] = array('value');
    //$value = t("Unknown field format, can't display field value.");
  }
  else {
    $unknown_format = TRUE;
  }

  // It's a known format, show the value.
  if (!isset($unknown_format)) {
    $options += array('no_labels' => $no_labels);
    $value = node_convert_format_field_value_helper($field_values, $options);
  }
  else {
    // Format is not known, and we should show the error message.
    if (!$no_labels) {
      $value = t("Unknown field format, can't display field value.");
    }
    // Format unknown, don't show anything.
    else {
      $value = '';
    }
  }
  return $value;
}

/**
 * Format a field's values.
 *
 * @param $field_values
 * @param array $options
 * @internal param $singular_label
 * @internal param $plural_label
 * @internal param $columns
 * @internal param string $prefix_label
 * @internal param $field
 * @return string
 */
function node_convert_format_field_value_helper($field_values, $options = array()) {
  $output = '';

  $prefix_label = isset($options['prefix_label']) ? $options['prefix_label'] : '';
  $singular_label = isset($options['singular_label']) ? $options['singular_label'] : '';
  $plural_label = isset($options['plural_label']) ? $options['plural_label'] : '';
  $columns = isset($options['columns']) ? $options['columns'] : array();
  $no_labels = isset($options['no_labels']) ? $options['no_labels'] : array();

  if (empty($field_values)) {
    if (!$no_labels) {
      $output = 'NULL';
    }
    return $output;
  }

  // Prefix a label.
  if (!empty($prefix_label) && !$no_labels) {
    $output .= $prefix_label . ' ; ';
  }

  // Add the label depending on number of field values.
  $count = count($field_values);
  $label = format_plural($count, $singular_label, $plural_label, array());

  $items = array();
  foreach ($field_values as $item) {
    $column_values = array();

    foreach ($columns as $column_key => $column) {
      $column_label = '';
      if (!is_numeric($column_key) && !empty($item[$column]) && !$no_labels) {
        $column_label = $column_key . ': ';
      }
      $column_values[] = $column_label . $item[$column];
    }
    $items[] = implode(' ', $column_values);

  }

  $final_label = '';
  if (!$no_labels) {
    $final_label = $label . ': ';
  }
  $output .= $final_label . implode(', ', $items);
  return $output;
}
