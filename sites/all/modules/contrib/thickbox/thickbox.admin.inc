<?php
// $Id: thickbox.admin.inc,v 1.1.2.11 2009/08/20 05:14:55 frjo Exp $

/**
 * @file
 * Administrative page callbacks for the thickbox module.
 */

/**
 * General configuration form for controlling the thickbox behaviour.
 */
function thickbox_admin_settings() {
  if (module_exists('image')) {
    $form['thickbox_imagemodule'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image module options')
    );
    $form['thickbox_imagemodule']['thickbox_auto'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable for image nodes'),
      '#default_value' => variable_get('thickbox_auto', 0),
      '#description' => t('Automatically activate Thickbox for all image nodes (requires the image module).'),
    );
    $options = array();
    $sizes = image_get_sizes();
    foreach ($sizes as $label => $size) {
      $options[$label] = $size['label'];
    }
    $form['thickbox_imagemodule']['thickbox_derivative'] = array(
      '#type' => 'select',
      '#title' => t('Image derivative'),
      '#options' => $options,
      '#default_value' => variable_get('thickbox_derivative', 'preview'),
      '#description' => t('Select which image derivative will be loaded.'),
    );
  }

  if (module_exists('imagefield')) {
    $form['thickbox_imagefield'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image field options (CCK)')
    );
    $form['thickbox_imagefield']['thickbox_imagefield_gallery'] = array(
      '#type' => 'radios',
      '#title' => t('Image field gallery'),
      '#default_value' => variable_get('thickbox_imagefield_gallery', 1),
      '#options' => array(0 => t('Per page gallery'), 1 => t('Per post gallery'), 2 => t('Per field gallery'), 3 => t('No gallery')),
      '#description' => t('Should the gallery be images within a single field, a single post (default) or all images on the page. The last option disables galleries.'),
    );
    if (module_exists('imagecache')) {
      $presets_options = array(0 => t('Original image (no preset)'));
      foreach (imagecache_presets() as $preset) {
        $presets_options[$preset['presetname']] = $preset['presetname'];
      }
      $form['thickbox_imagefield']['thickbox_imagecache_preset'] = array(
        '#type' => 'select',
        '#title' => t('Imagecache preset'),
        '#options' => $presets_options,
        '#default_value' => variable_get('thickbox_imagecache_preset', 0),
        '#description' => t('Select which Imagecache preset to use for viewing images in the thickbox.'),
      );
    }
  }

  $form['thickbox_login_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login settings')
  );
  $form['thickbox_login_settings']['thickbox_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable for login links'),
    '#default_value' => variable_get('thickbox_login', 0),
    '#description' => t('Automatically activate Thickbox for links to user/login.'),
  );
  $form['thickbox_login_settings']['thickbox_login_form'] = array(
    '#type' => 'select',
    '#title' => t('Login form to use'),
    '#options' => array('user_login' => t('User login'), 'user_login_block' => t('User login block'), 'custom' => t('Custom')),
    '#default_value' => variable_get('thickbox_login_form', 'user_login'),
    '#description' => t('Select which form to use for login. If "custom", specify below.'),
  );
  $form['thickbox_login_settings']['thickbox_login_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom login form'),
    '#default_value' => variable_get('thickbox_login_custom', ''),
    '#description' => t('Specify the form here if you have set the login form to "custom" above.'),
  );

  $form['thickbox_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['thickbox_advanced_settings']['thickbox_ie_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not include special CSS for Internet Explorer'),
    '#default_value' => variable_get('thickbox_ie_css', 0),
    '#description' => t('If you have added the Thickbox IE CSS to you themes IE CSS file or simply do not need it you can check this box.'),
  );
  $form['thickbox_advanced_settings']['thickbox_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Deactivate Thickbox on specific pages'),
    '#default_value' => variable_get('thickbox_pages', "admin*\nimg_assist*\nnode/add/*\nnode/*/edit"),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  return system_settings_form($form);
}
