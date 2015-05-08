<?php
/**
 * @file
 * Variable module hook implementations
 */

/**
 * Implements hook_variable_info().
 */
function variable_file_variable_info($options) {
  $variables = array();
  if (module_exists('variable_example')) {
    // File and image upload.
    $variables['variable_example_upload_file'] = array(
      'type'              => 'file_upload',
      'title'             => t('Example file upload', array(), $options),
      'description'       => t('Upload my config file.', array(), $options),
      'upload_location'   => 'public://myuploaddir',
      'group'             => 'variable_example',
    );
    $variables['variable_example_upload_image'] = array(
      'type'              => 'image_upload',
      'title'             => t('Example image upload', array(), $options),
      'description'       => t('Upload my config file.', array(), $options),
      'upload_location'   => 'public://myuploaddir',
      'group'             => 'variable_example',
    );
    $variables['variable_example_managed_file'] = array(
      'type'              => 'file_managed',
      'title'             => t('Example file managed', array(), $options),
      'description'       => t('Upload my config file.', array(), $options),
      'upload_location'   => 'public://myuploaddir',
      'group'             => 'variable_example',
    );
    $variables['variable_example_managed_image'] = array(
      'type'              => 'image_managed',
      'title'             => t('Example image managed', array(), $options),
      'description'       => t('Upload my config file.', array(), $options),
      'upload_location'   => 'public://myuploaddir',
      'group'             => 'variable_example',
    );
  }
  return $variables;
}

/**
 * Implements hook_variable_type_info().
 */
function variable_file_variable_type_info() {
  // File path, manual upload.
  $type['file_upload'] = array(
    'title'          => t('File upload'),
    'element'        => array('#type' => 'fieldset'), //#theme' => 'variable_file_upload'),
    'element callback' => 'variable_file_form_element_file_upload',
    'format callback' => 'variable_file_format_file',
    'validate callback' => 'variable_file_form_file_upload_validate',
    'submit callback' => 'variable_file_form_file_upload_submit',
    'token'          => TRUE,
    'upload location' => 'public://',
    'validate size' => file_upload_max_size(),
  );
  // Image path, manual upload.
  $type['image_upload'] = array(
    'title' => t('Image upload'),
    'format callback' => 'variable_file_format_image',
    'type' => 'file_upload',
    'validate image' => TRUE,
    'validate extensions' => 'gif png jpg jpeg',
    // Default image style for formatting.
    'image style' => 'thumbnail',
  );
  // File object
  $type['file_managed'] = array(
    'title'          => t('File upload (managed file)'),
    'element'        => array('#type' => 'managed_file'),
    'element callback' => 'variable_file_form_element_file_managed',
    'format callback' => 'variable_file_format_file',
    'submit callback' => 'variable_file_form_file_managed_submit',
    'file managed' => TRUE,
    'upload location' => 'public://',
    'validate size' => file_upload_max_size(),
  );
  $type['image_managed'] = array(
    'title' => t('Image upload'),
    'type' => 'file_managed',
    'format callback' => 'variable_file_format_image',
    'validate image' => TRUE,
    'validate extensions' => 'gif png jpg jpeg',
    'image style' => 'thumbnail',
  );
  return $type;
}

/**
 * Build upload file variable.
 */
function variable_file_form_element_file_upload($variable, $options) {
  // Spawn into a fieldset.
  $variable += array('element' => array(), 'title' => '', 'description' => '');
  $element = $variable['element'] + array(
    '#type' => 'fieldset',
    '#title' => $variable['title'],
    '#description' => $variable['description'],
  );
  $value = variable_get_value($variable, $options);
  $element[$variable['name']] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#default_value' => $value,
  );
  $element[$variable['name'] . '__upload'] = array(
    '#title' => t('Upload'),
    '#type' => 'file',
  );
  // Format value if any.
  if (!empty($value) && file_exists($value)) {
    $element[$variable['name'] . '__display'] = variable_file_form_element_file_display($variable, $options);
  }

  $element = _variable_file_form_element($variable, $options, $element);
  $element[$variable['name'] . '__upload']['#description'] = theme('file_upload_help', array('upload_validators' => $element['#upload_validators']));
  return $element;
}

/**
 * Display file information element.
 */
function variable_file_form_element_file_display($variable, $options) {
  $value = variable_get_value($variable, $options);
  $file = _variable_file_image_object($value);
  $element = array(
    '#variable' => $variable,
    '#tree' => TRUE,
    '#theme' => 'file_widget',
    '#file' => $file,
  );
  // Reuse some managed file functions avoiding notices.
  $element['fid']['#value'] = -1;
  $element['filename']['#markup'] = theme('file_link', array('file' => $file));//variable_format_value($variable, $options);
  if ($variable['type'] == 'image_upload') {
    $element['#process'][] = 'variable_file_form_image_process';
  }
  return $element;
}

/**
 * Build upload file variable.
 */
function variable_file_form_element_file_managed($variable, $options) {
  $element = variable_form_element_default($variable, $options);

  if ($variable['type'] == 'image_managed' && $value = variable_get_value($variable)) {
    $element['#process'][] = 'file_managed_file_process';
    $element['#process'][] = 'variable_file_form_image_process';
  }
  $element = _variable_file_form_element($variable, $options, $element);
  $element['#description'] = theme('file_upload_help', array('description' => $element['#description'], 'upload_validators' => $element['#upload_validators']));
  return $element;
}

/**
 * Build upload file element.
 */
function _variable_file_form_element($variable, $options, $element = array()) {
  if (isset($variable['validate extensions'])) {
    $element['#upload_validators']['file_validate_extensions'] = array($variable['validate extensions']);
  }
  if (isset($variable['validate size'])) {
    $element['#upload_validators']['file_validate_size'] = array(parse_size($variable['validate size']));
  }
  $element['#upload_location'] = $variable['upload location'];

  // Only for images.
  if (!empty($variable['validate image'])) {
    $element['#upload_validators']['file_validate_is_image'] = array();
    if (isset($variable['image resolution'])) {
      $element['#upload_validators']['file_validate_image_resolution'] = array($variable['image resolution']);
    }
  }

  return $element;
}

/**
 * Variable format callback. Generic file.
 */
function variable_file_format_file($variable, $options = array()) {
  if ($path = _variable_file_get_path($variable)) {
    $url = file_create_url($path);
    return l($url, $url);
  }
  else {
    return t('No file');
  }
}

/**
 * Unmanaged file, validate callback.
 */
function variable_file_form_file_upload_validate($variable, $options, $element, $form, &$form_state) {
  // Upload and validate unmanaged files.
  $name = $variable['name'];

  $validators = $element['#upload_validators'];
  // Check for a new uploaded file.
  $file = file_save_upload($name . '__upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values'][$name . '__upload'] = $file;
    }
    else {
      // File upload failed.
      return t('The file could not be uploaded.');
    }
  }
}

/**
 * Managed file, submit callback.
 */
function variable_file_form_file_upload_submit($variable, $options, $form, &$form_state) {
  $name = $variable['name'];
  // Unmanaged files, keep only file path
  if ($uploaded = $form_state['values'][$name . '__upload']) {
    $filename = file_unmanaged_copy($uploaded->uri, $variable['upload location']);
    $form_state['values'][$name] = $filename;
    unset($form_state['values'][$name . '__upload']);
  }
}

/**
 * Managed file, submit callback.
 */
function variable_file_form_file_managed_submit($variable, $options, $form, &$form_state) {
  global $user;

  $name = $variable['name'];
  $value = variable_get_value($variable);
  // Managed files, more complex handling.
  if ($uploaded = $form_state['values'][$name]) {
    $file = file_load($uploaded);
    if ($file && !$file->status) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'variable_file', 'variable_file', $user->uid);
      // Remove old file if existing.
      if ($value) {
        _variable_file_upload_remove_file($name);
      }
    }
  }
}

/**
 * Variable format callback. Image.
 */
function variable_file_format_image($variable, $options = array()) {
  if ($path = _variable_file_get_path($variable)) {
    $style_name = $variable['image style'];
    $style_path = image_style_path($style_name, $path);
    // Check the image style is created, create if not.
    if (!file_exists($style_path)) {
      $style_path = image_style_url($style_name, $path);
    }
    $variables = image_get_info($style_path);
    $variables['path'] = $style_path;
    return theme('image', $variables);
  }
  else {
    return t('No image');
  }
}

/**
 * Get file path.
 */
function _variable_file_get_path($variable) {
  if (!empty($variable['value'])) {
    if (!empty($variable['file managed'])) {
      $file = file_load($variable['value']);
      return $file ? $file->uri : '';
    }
    else {
      return $variable['value'];
    }
  }
}

/**
 * Convert simple filepath into pseudo image object.
 */
function _variable_file_image_object($path) {
  $file = new Stdclass();
  $file->fid = 0;
  $file->uri = $path;
  $file->filename = basename($path);
  if (is_file($path)) {
    $file->filesize = filesize($path);
    $file->filemime = file_get_mimetype($path);
  }
  return $file;
}

/**
 * Remove old file with element key.
 *
 * @param string $name
 *   An associative string of variable name.
 */
function _variable_file_upload_remove_file($name) {
  $fid = variable_get_value($name);
  if ($fid && $file = file_load($fid)) {
    file_usage_delete($file, 'variable_file', 'variable_file', 1);
    file_delete($file);
  }
}
