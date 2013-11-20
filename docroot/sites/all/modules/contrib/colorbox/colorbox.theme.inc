<?php

/**
 * @file
 * Colorbox theme functions.
 */


/**
 * Handler for Colorbox display of imagecache + imagefield CCK fields.
 *
 * @param $element
 *   The CCK field element.
 * @return
 *   HTML output for displaying the image and link.
 */
function theme_colorbox_formatter_imagefield($element) {
  if (!module_exists('imagecache') || !module_exists('imagefield')) {
    return;
  }

  if (!empty($element['#item']['fid'])) {
    $item = $element['#item'];

    if (is_string($item['data'])) {
      $item['data'] = unserialize($item['data']);
    }
    if (!isset($item['filepath'])) {
      $file = field_file_load($item['fid']);
      $item['filepath'] = $file['filepath'];
    }
    // If the title is empty use description, alt or the node title in that order.
    if (empty($item['data']['title'])) {
      if (!empty($item['data']['description'])) {
        $item['data']['title'] = $item['data']['description'];
      }
      elseif (!empty($item['data']['alt'])) {
        $item['data']['title'] = $item['data']['alt'];
      }
      elseif (is_numeric($item['nid']) && $node = node_load($item['nid'])) {
        $item['data']['title'] = $node->title;
      }
    }

    // Shorten the title for the example styles or when title shortening is active.
    $style = variable_get('colorbox_style', 'default');
    $trim_length = variable_get('colorbox_title_trim_length', 75);
    if (((strpos($style, 'colorbox/example') !== FALSE) || variable_get('colorbox_title_trim', 0)) && (drupal_strlen($item['data']['title']) > $trim_length)) {
      $item['data']['title'] = drupal_substr($item['data']['title'], 0, $trim_length - 5) .'...';
    }

    // Build the gallery id.
    $nid = $item['nid'] ? $item['nid'] : 'nid';
    switch (variable_get('colorbox_imagefield_gallery', 1)) {
      case 0:
        $gallery_id = 'all';
        break;
      case 1:
        $gallery_id = $nid;
        break;
      case 2:
        $gallery_id = $nid .'-'. $element['#field_name'];
        break;
      case 3:
        $gallery_id = $nid .'-'. $item['fid'];
        break;
    }

    list($presetname, $modulename) = explode('__', $element['#formatter'], 2);
    if ($preset = imagecache_preset_by_name($presetname)) {
      return theme('colorbox_imagefield', $presetname, $item['filepath'], $item['data']['alt'], $item['data']['title'], $gallery_id, $element['#field_name']);
    }
  }
}

/**
 * Implementation of theme_colorbox_imagefield().
 *
 * @param $presetname
 *   presetname of the derivative you wish to generate a tag for.
 * @param $path
 *   path to the original image you wish to create a derivative image tag for.
 * @param $alt
 *   img tag alternate text
 * @param $title
 *   img tag title text
 * @param $gid
 *   gallery id
 * @param attributes
 *   optional drupal attributes array. If attributes is set, the default imagecache classes
 *   will not be set automatically, you must do this manually.
 */
function theme_colorbox_imagefield($presetname, $path, $alt = '', $title = '', $gid = '', $field_name = '', $attributes = NULL) {
  if (!empty($path)) {
    $image = theme('imagecache', $presetname, $path, $alt, $title, $attributes);
    if ($colorbox_presetname = variable_get('colorbox_imagecache_preset', 0)) {
      $link_path = imagecache_create_url($colorbox_presetname, $path);
    }
    else {
      $link_path = file_create_url($path);
    }
    $class = 'colorbox imagefield imagefield-imagelink imagefield-'. $field_name;

    return l($image, $link_path, array('html' => TRUE, 'attributes' => array('title' => $title, 'class' => $class, 'rel' => 'gallery-'. $gid)));
  }
}

/**
 * Preprocess variables for the colorbox-insert-image.tpl.php file.
 */
function template_preprocess_colorbox_insert_image(&$vars) {
  $vars['presetname'] = $vars['item']['presetname'];
  $vars['image_path'] = imagecache_create_url($vars['presetname'], $vars['item']['filepath']);
  $vars['insert_class'] = $vars['widget']['insert_class'];

  if ($colorbox_presetname = variable_get('colorbox_imagecache_preset', 0)) {
    $vars['link_path'] = imagecache_create_url($colorbox_presetname, $vars['item']['filepath']);
  }
  else {
    $vars['link_path'] = file_create_url($vars['item']['filepath']);
  }

  $vars['gallery_id'] = '';
  switch (variable_get('colorbox_insert_gallery', 0)) {
    case 0:
    case 1:
    case 2:
      $vars['gallery_id'] = 'all';
      break;
    case 3:
      $vars['gallery_id'] = 'insert-'. $vars['item']['fid'];
      break;
  }
}
