<?php

/**
 * @file
 * File resource.
 */

/**
 * THERE SHOULD BE NO UPDATE!!!
 * Drupal doesn't allow updating or replacing a file in the files table.
 * If you need to, create a new file and remove the old file.
 */
function _file_resource_definition() {
  return array(
    'file' => array(
      'operations' => array(
        'create' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/file_resource'),
          'help' => 'Creates a file',
          'callback' => '_file_resource_create',
          'access callback' => '_file_resource_access',
          'access arguments' => array('create'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'file',
              'type' => 'array',
              'description' => t('An array representing a file.'),
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'retrieve' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/file_resource'),
          'help' => 'Retrieves a file',
          'callback' => '_file_resource_retrieve',
          'access callback' => '_file_resource_access',
          'access arguments' => array('view'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'fid',
              'type' => 'int',
              'description' => 'The fid of the file to retrieve.',
              'source' => array('path' => '0'),
              'optional' => FALSE,
            ),
            array(
              'name'         => 'file_contents',
              'type'         => 'int',
              'description'  => t('To return file contents or not.'),
              'source'       => array('param' => 'file_contents'),
              'optional'     => TRUE,
              'default value' => TRUE,
            ),
          ),
        ),
        'delete' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/file_resource'),
          'help' => 'Deletes a file',
          'callback' => '_file_resource_delete',
          'access callback' => '_file_resource_access',
          'access arguments' => array('delete'),
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'cid',
              'type' => 'int',
              'description' => 'The id of the file to delete',
              'source' => array('path' => '0'),
              'optional' => FALSE,
            ),
          ),
        ),
        'index' => array(
          'file' => array('type' => 'inc', 'module' => 'services', 'name' => 'resources/file_resource'),
          'callback' => '_file_resource_index',
          'args' => array(
            array(
              'name' => 'page',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'The zero-based index of the page to get, defaults to 0.',
              'default value' => 0,
              'source' => array('param' => 'page'),
            ),
            array(
              'name' => 'fields',
              'optional' => TRUE,
              'type' => 'string',
              'description' => 'The fields to get.',
              'default value' => '*',
              'source' => array('param' => 'fields'),
            ),
            array(
              'name' => 'parameters',
              'optional' => TRUE,
              'type' => 'array',
              'description' => 'Parameters',
              'default value' => array(),
              'source' => array('param' => 'parameters'),
            ),
            array(
              'name' => 'pagesize',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'Number of records to get per page.',
              'default value' => variable_get('services_file_index_page_size', 20),
              'source' => array('param' => 'pagesize'),
            ),
          ),
          'access callback' => '_file_resource_access',
          'access arguments' => array('index'),
          'access arguments append' => TRUE,
        ),
      ),
    ),
  );
}

/**
 * Return an array of optionally paged fids baed on a set of criteria.
 *
 * An example request might look like
 *
 * http://domain/endpoint/file?fields=fid,filename&parameters[fid]=7&parameters[uid]=1
 *
 * This would return an array of objects with only fid and filename defined, where
 * fid = 7 and uid = 1.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 * @param $fields
 *   The fields you want returned.
 * @param $parameters
 *   An array containing fields and values used to build a sql WHERE clause
 *   indicating items to retrieve.
 * @param $page_size
 *   Integer number of items to be returned.
 * @return
 *   An array of file objects.
 *
 * @see _node_resource_index()
 */
function _file_resource_index($page, $fields, $parameters, $page_size) {

  $query = services_resource_build_index_query('files', 'f.timestamp', $page, $fields, $parameters, 'f', 'fid', $page_size, 'file');
  // Put together array of matching nodes to return.
  $results = array();
  while ($comments = db_fetch_object($query)) {
    $results[] = $comments;
  }

  // Put together array of matching files to return.
  return services_resource_build_index_list($results, 'files', 'fid');
}

/**
 * Adds a new file and returns the fid.
 *
 * @param $file
 *   An array as representing the file.
 * @return
 *   Unique identifier for the file (fid) or errors if there was a problem.
 */
function _file_resource_create($file) {
  $file = _services_arg_value($file, 'file');

  global $user;

  // If the file data is empty then bail.
  if (!isset($file['file'])) {
    return services_error('No file data received.');
  }

  // Check whether we have filename or filepath for the file.
  if ((!isset($file['filename']) || empty($file['filename']))
    && (!isset($file['filepath']) || empty($file['filepath']))) {
    return services_error('No filename and no filepath specified or they are both empty.');
  }

  // Ensure we create new file.
  $file['fid'] = NULL;

  // Build the list of non-munged extensions.
  // @todo: this should not be here. we need to figure out the right place.
  // @todo: also isn't that repeated variable get a waste? I mean, I guess it
  //        is cached but still it is pretty ugly.
  $extensions = '';
  foreach ($user->roles as $rid => $name) {
    $extensions .= ' '. variable_get("upload_extensions_$rid",
    variable_get('upload_extensions_default', 'jpg jpeg gif png txt html doc xls pdf ppt pps odt ods odp'));
  }

  // If filename has not been specified extract it from filepath.
  if (!isset($file['filename']) || empty($file['filename'])) {
    $file['filename'] = trim(basename($file['filepath']), '.');
  }

  // If no filepath specified use standard directory.
  if (!isset($file['filepath']) || empty($file['filepath'])) {
    $file['filepath'] = file_directory_path() . '/' . $file['filename'];
  }

  // Get the directory name for the location of the file:
  $dir = dirname($file['filepath']);
  // Build the destination folder tree if it doesn't already exists.
  // @see http://drupal.org/node/180970
  $dir_array = explode('/', $dir);
  $file_check_directory_array = array();
  foreach ($dir_array as $dir_element) {
    $file_check_directory_array[] = $dir_element;
    $dir_path_element = implode('/', $file_check_directory_array);
    file_check_directory($dir_path_element, FILE_CREATE_DIRECTORY);
  }
  if (!file_check_directory($dir, FILE_CREATE_DIRECTORY)) {
    return services_error("Could not create destination directory for file. " . $dir);
  }
  // Update file data as necessary
  $file['filepath'] = file_destination(file_create_path($file['filepath']), FILE_EXISTS_RENAME);
  $file['filename'] = file_munge_filename(trim(basename($file['filepath']), '.'), $extensions, TRUE);
  $file['filemime'] = file_get_mimetype($file['filename']);
  $destination = file_destination(file_create_path($file['filepath']), FILE_EXISTS_RENAME);

  // Rename potentially executable files, to help prevent exploits.
  if (preg_match('/\.(php|pl|py|cgi|asp|js)$/i', $file['filename']) && (drupal_substr($file['filename'], -4) != '.txt')) {
    $file['filemime'] = 'text/plain';
    $file['filepath'] .= '.txt';
    $file['filename'] .= '.txt';
    // As the file may be named example.php.txt, we need to munge again to
    // convert to example.php_.txt, then create the correct destination.
    $file['filename'] = file_munge_filename($file['filename'], $extensions, TRUE);
    $destination = file_destination(file_create_path($file['filepath']), FILE_EXISTS_RENAME);
  }

  if (!$filename = file_save_data(base64_decode($file['file']), $destination)) {
    return services_error("Could not write file to destination");
  }
  $file['filepath'] = $destination;

  // Set the file user id.
  $file['uid'] = $user->uid;

  // Update the timestamp to the current time, otherwise the file could
  // get deleted on the next cron run if its status is set to 0.
  $file['timestamp'] = time();

  // If we made it this far it's safe to record this file in the database.
  drupal_write_record('files', $file);

  return array(
    'fid' => $file['fid'],
    'uri' => services_resource_uri(array('file', $file['fid'])),
  );
}

/**
 * Get a given file
 *
 * @param $fid
 *   Number. File ID
 * @return
 *   The file
 */
function _file_resource_retrieve($fid, $file_contents) {
  if ($file = db_fetch_array(db_query('SELECT * FROM {files} WHERE fid = %d', $fid))) {
    if ($file_contents) {
      $absolute_file_path = getcwd() . '/' . file_create_path($file['filepath']);
      $binaryfile = fopen($absolute_file_path, 'rb');
      if ($binaryfile === FALSE) {
        services_error(t('Cannot open file with ID %fid.', array('%fid' => $fid)));
      }
      $file['file'] = base64_encode(fread($binaryfile, filesize($file['filepath'])));
      fclose($binaryfile);
    }
    $file['uri'] = services_resource_uri(array('file', $file['fid']));
    return $file;
  }
  return services_error(t('There is no file with the given ID.'));
}

/**
 * Generates an array of base64 encoded files attached to a node.
 *
 * @param $nid
 *   Number. Node ID
 * @return
 *   Array. A list of all files from the given node
 */
function _file_resource_load_node_files($nid, $file_include_contents) {
  $node = node_load($nid);
  if (!isset($node->files)) {
    return services_error(t('There are no files on given node.'));
  }

  $return = array();
  foreach ($node->files as $file) {
    // Do not return files that are not listed.
    if (!$file->list) {
      continue;
    }
    $return[$file->fid] = array(
      'filename'  => $file->filename,
      'uid'       => $file->uid,
      'filemime'  => $file->filemime,
      'filesize'  => $file->filesize,
      'status'    => $file->status,
      'timestamp' => $file->timestamp
    );
    // If to add content of the file.
    if ($file_include_contents) {
      $filepath = file_create_path($file->filepath);
      $binaryfile = fopen($filepath, 'rb');
      $return[$file->fid]['file'] = base64_encode(fread($binaryfile, filesize($filepath)));
      fclose($binaryfile);
    }

  }
  return $return;
}

/**
 * Delete a file.
 *
 * @param $fid
 *   Unique identifier of the file to delete.
 * @return bool
 *   Whether or not the delete was successful.
 */
function _file_resource_delete($fid) {
  // Fetch details of the file.
  $file = db_fetch_array(db_query('SELECT * FROM {files} WHERE fid = %d', $fid));
  // Remove data from {files} table.
  db_query('DELETE FROM {files} WHERE fid = %d', $fid);
  // Remove file physically.
  return file_delete($file['filepath']);
}

/**
 * Access check callback for file controllers.
 */
function _file_resource_access($op = 'view', $args = array()) {
  // Adds backwards compatability with regression fixed in #1083242
  $args[0] = _services_access_value($args[0], 'file');
  global $user;
  if (user_access('administer files')) {
    return TRUE;
  }

  if ($op == 'create') {
    $file = (object)$args[0];
  }
  else {
    $file = db_fetch_object(db_query('SELECT * FROM {files} WHERE fid = %d', $args[0]));
  }

  switch ($op) {
    case 'view':
    case 'index':
      if (user_access('get any binary files')) {
        return TRUE;
      }
      return $file->uid == $user->uid && user_access('get own binary files');
      break;
    case 'create':
    case 'delete':
      return $file->uid == $user->uid && user_access('save file information');
      break;
  }

  return FALSE;
}

function _file_resource_node_access($op = 'view', $args = array()) {
  global $user;
  if (user_access('get any binary files')) {
    return TRUE;
  }
  elseif ($node = node_load($args[0])) {
    return $node->uid == $user->uid && user_access('get own binary files');
  }
  return FALSE;
}
