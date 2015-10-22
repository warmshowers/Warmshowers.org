<?php
/**
 * @file
 * ws_unauth_front_page_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ws_unauth_front_page_feature_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function ws_unauth_front_page_feature_image_default_styles() {
  $styles = array();

  // Exported image style: anon_frontpage_slideshow.
  $styles['anon_frontpage_slideshow'] = array(
    'label' => 'Anon Frontpage Slideshow',
    'effects' => array(
      19 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 940,
          'height' => 448,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
