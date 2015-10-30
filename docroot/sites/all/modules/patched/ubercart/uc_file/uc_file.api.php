<?php

/**
 * @file
 * Hooks provided by the File Downloads module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Gives clearance to a user to download a file.
 *
 * By default the uc_file module can implement 3 restrictions on downloads: by
 * number of IP addresses downloaded from, by number of downloads, and by a set
 * expiration date. Developers wishing to add further restrictions can do so by
 * implementing this hook. After the 3 aforementioned restrictions are checked,
 * the uc_file module will check for implementations of this hook.
 *
 * @param $user
 *   The drupal user object that has requested the download
 * @param $file_download
 *   The file download object as defined as a row from the uc_file_users table
 *   that grants the user the download
 *
 * @return
 *   TRUE or FALSE depending on whether the user is to be permitted download of
 *   the requested files. When a implementation returns FALSE it should set an
 *   error message in Drupal using drupal_set_message() to inform customers of
 *   what is going on.
 */
function hook_uc_download_authorize($user, $file_download) {
  if (!$user->status) {
    drupal_set_message(t("This account has been banned and can't download files anymore."), 'error');
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * Performs actions on file products.
 *
 * The uc_file module comes with a file manager (found at Administer » Store
 * administration » Products » View file downloads) that provides some basic
 * functionality: deletion of multiple files and directories, and upload of
 * single files (those looking to upload multiple files should just directly
 * upload them to their file download directory then visit the file manager
 * which automatically updates new files found in its directory). Developers
 * that need to create more advanced actions with this file manager can do so
 * by using this hook.
 *
 * @param $op
 *   The operation being taken by the hook, possible ops defined below.
 *   - info: Called before the uc_file module builds its list of possible file
 *     actions. This op is used to define new actions that will be placed in
 *     the file action select box.
 *   - insert: Called after uc_file discovers a new file in the file download
 *     directory.
 *   - form: When any defined file action is selected and submitted to the form
 *     this function is called to render the next form. Because this is called
 *     whenever a module-defined file action is selected, the variable
 *     $args['action'] can be used to define a new form or append to an existing
 *     form.
 *   - upload: After a file has been uploaded, via the file manager's built in
 *     file upload function, and moved to the file download directory this op
 *     can perform any remaining operations it needs to perform on the file
 *     before its placed into the uc_files table.
 *   - upload_validate: This op is called to validate the uploaded file that
 *     was uploaded via the file manager's built in file upload function. At
 *     this point, the file has been uploaded to PHP's temporary directory.
 *     Files passing this upload validate function will be moved into the file
 *     downloads directory.
 *   - validate: This op is called to validate the file action form.
 *   - submit: This op is called to submit the file action form.
 * @param $args
 *   A keyed array of values that varies depending on the op being performed,
 *   possible values defined below.
 *   - info: None.
 *   - insert:
 *     - file_object: The file object of the newly discovered file.
 *   - form:
 *     - action: The file action being performed as defined by the key in the
 *       array sent by hook_uc_file_action($op = 'info').
 *     - file_ids: The file ids (as defined in the uc_files table) of the
 *       selected files to perform the action on.
 *   - upload:
 *     - file_object: The file object of the file moved into file downloads
 *       directory.
 *     - form_id: The form_id variable of the form_submit function.
 *     - form_values: The form_values variable of the form_submit function.
 *   - upload_validate:
 *     - file_object: The file object of the file that has been uploaded into
 *       PHP's temporary upload directory.
 *     - form_id: The form_id variable of the form_validate function.
 *     - form_values: The form_values variable of the form_validate function.
 *   - validate:
 *     - form_id: The form_id variable of the form_validate function.
 *     - form_values: The form_values variable of the form_validate function.
 *   - submit:
 *     - form_id: The form_id variable of the form_submit function.
 *     - form_values: The form_values variable of the form_submit function.
 *
 * @return
 *   The return value of hook depends on the op being performed, possible return
 *   values defined below:
 *   - info: The associative array of possible actions to perform. The keys are
 *     unique strings that defines the actions to perform. The values are the
 *     text to be displayed in the file action select box.
 *   - insert: None.
 *   - form: This op should return an array of drupal form elements as defined
 *     by the drupal form API.
 *   - upload: None.
 *   - upload_validate: None.
 *   - validate: None.
 *   - submit: None.
 */
function hook_uc_file_action($op, $args) {
  switch ($op) {
    case 'info':
      return array('uc_image_watermark_add_mark' => 'Add Watermark');
    case 'insert':
      // Automatically adds watermarks to any new files that are uploaded to
      // the file download directory.
      _add_watermark($args['file_object']->uri);
    break;
    case 'form':
      if ($args['action'] == 'uc_image_watermark_add_mark') {
        $form['watermark_text'] = array(
          '#type' => 'textfield',
          '#title' => t('Watermark text'),
        );
        $form['actions'] = array('#type' => 'actions');
        $form['actions']['submit_watermark'] = array(
          '#type' => 'submit',
          '#value' => t('Add watermark'),
        );
      }
    return $form;
    case 'upload':
      _add_watermark($args['file_object']->uri);
      break;
    case 'upload_validate':
      // Given a file path, function checks if file is valid JPEG.
      if (!_check_image($args['file_object']->uri)) {
        form_set_error('upload', t('Uploaded file is not a valid JPEG'));
      }
    break;
    case 'validate':
      if ($args['form_values']['action'] == 'uc_image_watermark_add_mark') {
        if (empty($args['form_values']['watermark_text'])) {
          form_set_error('watermar_text', t('Must fill in text'));
        }
      }
    break;
    case 'submit':
      if ($args['form_values']['action'] == 'uc_image_watermark_add_mark') {
        foreach ($args['form_values']['file_ids'] as $file_id) {
          $filename = db_query("SELECT filename FROM {uc_files} WHERE fid = :fid", array(':fid' => $file_id))->fetchField();
          // Function adds watermark to image.
          _add_watermark($filename);
        }
      }
    break;
  }
}

/**
 * Makes changes to a file before it is downloaded by the customer.
 *
 * Stores, either for customization, copy protection or other reasons, might
 * want to send customized downloads to customers. This hook will allow this
 * to happen.  Before a file is opened to be transferred to a customer, this
 * hook will be called to make any altercations to the file that will be used
 * to transfer the download to the customer. This, in effect, will allow a
 * developer to create a new, personalized, file that will get transferred to
 * a customer.
 *
 * @param $file_user
 *   The file_user object (i.e. an object containing a row from the
 *   uc_file_users table) that corresponds with the user download being
 *   accessed.
 * @param $ip
 *   The IP address from which the customer is downloading the file.
 * @param $fid
 *   The file id of the file being transferred.
 * @param $file
 *   The file path of the file to be transferred.
 *
 * @return
 *   The path of the new file to transfer to customer.
 */
function hook_uc_file_transfer_alter($file_user, $ip, $fid, $file) {
  // For large files this might be too memory intensive.
  $file_data = file_get_contents($file) . " [insert personalized data]";
  $new_file = tempnam(file_directory_temp(), 'tmp');
  file_put_contents($new_file, $file_data);
  return $new_file;
}

/**
 * @} End of "addtogroup hooks".
 */
