<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function redesign_views_imagecache_default_presets() {
  $items = array(
    'photo_album_block' => array(
      'presetname' => 'photo_album_block',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '200',
            'height' => '200',
          ),
        ),
      ),
    ),
    'profile_tiny' => array(
      'presetname' => 'profile_tiny',
      'actions' => 
      array (
        0 => array (
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array (
            'width' => '30',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function redesign_views_views_api() {
  return array(
    'api' => '2',
  );
}
