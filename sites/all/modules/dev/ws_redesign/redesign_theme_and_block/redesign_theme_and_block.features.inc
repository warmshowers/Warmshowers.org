<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function redesign_theme_and_block_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function redesign_theme_and_block_imagecache_default_presets() {
  $items = array(
    '400w' => array(
      'presetname' => '400w',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '400',
            'height' => '',
            'upscale' => 1,
          ),
        ),
      ),
    ),
    '800w' => array(
      'presetname' => '800w',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '800',
            'height' => '',
          ),
        ),
      ),
    ),
    'Center_Content' => array(
      'presetname' => 'Center_Content',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '482',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'rightcolumn' => array(
      'presetname' => 'rightcolumn',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '235',
            'height' => '',
            'upscale' => 1,
          ),
        ),
      ),
    ),
    'thumbnail' => array(
      'presetname' => 'thumbnail',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '150',
            'height' => '150',
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
function redesign_theme_and_block_views_api() {
  return array(
    'api' => '2',
  );
}
