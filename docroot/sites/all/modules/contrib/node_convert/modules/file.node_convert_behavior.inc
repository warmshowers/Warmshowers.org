<?php

/**
 * @file
 * File behavior.
 *
 * Allows moving files and images to new folders specified in field settings.
 */

/**
 * Implementation of node_convert_change().
 */
function file_node_convert_change($data, $op) {
  if ($op == 'insert') {
    if (
      !empty($data['hook_options']['no_fields_flag'])
      && isset($data['hook_options']['move_files'])
      && $data['hook_options']['move_files'] == 1
    ) {
      $node = clone($data['node']);
      $destination_type = $data['dest_node_type'];
      $destination_fields = $data['destination_fields'];
      $field_info = field_info_fields();
      $destination_instances = field_info_instances('node', $destination_type);

      // Default supported field types are file and image. We allow altering
      // the list of supported field types, in case there are any other fields
      // that have the same structure of settings to get the file directory
      // URL scheme. If the settings structure differs, and you still want to
      // implement the behavior, your best best it to unset this file behavior
      // using hook_node_convert_behavior_alter(), and adding your own behavior.
      $file_types = array('file', 'image');
      drupal_alter('file_node_convert_types', $file_types);

      // Set the node type to the destination type, so that field_language, and
      // field_get_items return the correct language code.
      $node->type = $destination_type;

      // Clear the field language cache, so any new fields appear.
      drupal_static_reset('field_language');

      // Go through each field, and check if it's a file-like field, and it has
      // a non-empty file directory.
      foreach ($destination_fields as $i => $destination_field) {
        if (
          $destination_field != 'discard'
          && isset($field_info[$destination_field]['type'])
          && in_array($field_info[$destination_field]['type'], $file_types)
          && isset($destination_instances[$destination_field]['settings']['file_directory'])
          && !empty($destination_instances[$destination_field]['settings']['file_directory'])
        ) {
          // Get the field values.
          $files = field_get_items('node', $node, $destination_field);

          // Get the destination file directory, and stream wrapper scheme.
          $folder = $destination_instances[$destination_field]['settings']['file_directory'];
          $scheme = $field_info[$destination_field]['settings']['uri_scheme'];
          $new_path = $scheme . '://' . $folder;

          if (is_array($files)) {
            // Create the new path if it does not exist.
            file_prepare_directory($new_path, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);

            // Move the files to the new directory.
            foreach ($files as $file) {
              $file = (object) $file;

              // Don't allow overwriting files if they are already present in
              // the destination folder.
              $file_move_flags = FILE_EXISTS_ERROR;
              file_move($file, $new_path, $file_move_flags);
            }
          }
        }
      }
    }
  }
  elseif ($op == 'options') {
    $form = array();

    $form['move_files'] = array(
      '#type' => 'checkbox',
      '#title' => t('Move field files to new folder paths'),
      '#description' => t('Move files to the new paths specified by the field instances. E.g if the folder path for field_image is a/b, and you convert it to field_image_2 with path foo/bar,
the existing node image will be moved to foo/bar/filename.jpg.<br>
<br><br>Note 1: If you check this, all files for all fields will be moved to the new folders specified in each respective field instance setting.
<br><br>Note 2: If the file already exists in the destination folder, it will not be overwritten, so no data loss occurs.
<br><br>Note 3: If you use <em>Media</em> widget to re-use already uploaded files from some original location, they will also be affected, and moved to the new folders, which might <b>NOT</b> be always desired.
<br><br>Note 4: Currently supported field types for this operation are: File and Image.
'),
      '#default_value' => FALSE,
    );

    return $form;
  }

  return NULL;
}
