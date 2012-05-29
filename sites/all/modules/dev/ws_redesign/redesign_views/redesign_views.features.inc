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
            'width' => '240',
            'height' => '240',
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
