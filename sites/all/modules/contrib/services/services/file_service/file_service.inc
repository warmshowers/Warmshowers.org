<?php
// $Id: file_service.inc,v 1.1.2.7.2.3 2009/12/05 01:42:38 heyrocker Exp $

/**
 * @file
 *  Link general file functionalities to services module.
 */

/**
 * Get all elements from a given file
 *
 * @param $fid
 *   Number. File ID
 * @return
 *   Array. All elements for a given file
 */
function file_service_get($fid) {
  if ($file = db_fetch_array(db_query('SELECT * FROM {files} WHERE fid = %d', $fid))) {
    $filepath = file_create_path($file['filepath']);
    $binaryfile = fopen($filepath, 'rb');
    $file['file'] = base64_encode(fread($binaryfile, filesize($filepath)));
    fclose($binaryfile);
    return $file;
  }
  else {
    return services_error(t('There is no file with the given ID.'));
  }
}


/**
 * Check if the user has permission to save file information.
 *
 * Will probably want to revisit this down the road to make it
 * somewhat tougher, esp if we get to the point where we're saving
 * actual files (see below.)
 */
function file_service_save_access() {
  return user_access('save file information');
}

/**
 * Save file information.
 *
 * This is basically a copy of file_save_upload() from includes/file.inc
 * without the upload-specific requirements. In D7 with File API this will
 * be a lot easier to deal with.
 *
 * @param $file
 *   Array representing the file information
 */
function file_service_save($file) {
  global $user;
  $file = (object) $file;

  // If the file data is empty then bail
  if (!isset($file->file)) {
    return FALSE;
  }

  // If the submitted file is an update, then set the update parameter for
  // drupal_write_record(), indicating such. Otherwise we can just pass the
  // object in and it will be treated as an insert.
  $update = array();
  if (!empty($file->fid)) {
    $update = 'fid';
  }

  // Build the list of non-munged extensions.
  // @todo: this should not be here. we need to figure out the right place.
  // @todo: also isn't that repeated variable get a waste? I mean, I guess it
  //        is cached but still it is pretty ugly.
  $extensions = '';
  foreach ($user->roles as $rid => $name) {
    $extensions .= ' '. variable_get("upload_extensions_$rid",
    variable_get('upload_extensions_default', 'jpg jpeg gif png txt html doc xls pdf ppt pps odt ods odp'));
  }

  // Get the directory name for the location of the file:
  $dir = dirname($file->filepath);
  // Build the destination folder tree if it doesn't already exists.
  if (!file_check_directory($dir, FILE_CREATE_DIRECTORY)) {
    return services_error("Could not create destination directory for file.");
  }

  // Update file object as necessary
  $file->filepath = file_destination(file_create_path($file->filepath), FILE_EXISTS_RENAME);
  $file->filename = file_munge_filename(trim(basename($file->filepath), '.'), $extensions, TRUE);
  $file->filemime = file_get_mimetype($file->filename);

  // Rename potentially executable files, to help prevent exploits.
  if (preg_match('/\.(php|pl|py|cgi|asp|js)$/i', $file->filename) && (substr($file->filename, -4) != '.txt')) {
    $file->filemime = 'text/plain';
    $file->filepath .= '.txt';
    $file->filename .= '.txt';
  }

  // If the destination is not provided, or is not writable, error our
  if (empty($file->filepath) || file_check_path($file->filepath) === FALSE) {
    return services_error("Destintion directory does not exist or is not writeable.");
  }

  //The filepath that ends up in the node must contain the filename
  $file->filepath .= '/'. $file->filename;

  // Write the file
  if (!file_save_data(base64_decode($file->file), $file->filepath)) {
    return services_error("Could not write file to destination");
  }

  // If we made it this far it's safe to record this file in the database.
  drupal_write_record('files', $file, $update);

  // hook_file_insert() requires an object
  if (empty($update)) {
    foreach (module_implements('file_insert') as $module) {
      $function =  $module .'_file_insert';
      $function($file);
    }
  }

  // Return the fid
  return $file->fid;
}

/**
 * Check if the user has permission to get a given file
 */
function file_service_get_access($fid) {
  global $user;
  if (user_access('get any binary files')) {
    return TRUE;
  }
  elseif ($file = db_fetch_array(db_query('SELECT * FROM {files} WHERE fid = %d', $fid))) {
    return $file['uid'] == $user->uid && user_access('get own binary files');
  }
}

/**
 * Generates an array of base64 encoded files attached to a node
 *
 * @param $nid
 *   Number. Node ID
 * @return
 *   Array. A list of all files from the given node
 */
function file_service_get_node_files($nid) {
  $node = node_load($nid);
  if (isset($node->files)) {
    $files = array();
    foreach ($node->files as $file) {
      // Rebuild the files array so it only contains files we know we're allowed to list
      if ($file->list) {
        $files[] = $file;
      }
    }
    if (count($files) > 0) {
      $send = array();
      foreach ($files as $file) {
        $file = array_shift($files);
        $filepath = file_create_path($file->filepath);
        $binaryfile = fopen($filepath, 'rb');
        $send[$file->fid] = array(
          'file'      => base64_encode(fread($binaryfile, filesize($filepath))),
          'filename'  => $file->filename,
          'uid'       => $file->uid,
          'filemime'  => $file->filemime,
          'filesize'  => $file->filesize,
          'status'    => $file->status,
          'timestamp' => $file->timestamp
        );
        fclose($binaryfile);
      }
    }
    return $send;
  }
  else {
    return services_error(t('There are no files on given node.'));
  }
}

/**
 * Check if the user has permission to get all files from a given node.
 */
function file_service_get_node_files_access($nid) {
  global $user;
  if (user_access('get any binary files')) {
    return TRUE;
  }
  elseif ($node = node_load($nid)) {
    return $node->uid == $user->uid && user_access('get own binary files');
  }
}
