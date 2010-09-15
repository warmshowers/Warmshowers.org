<?php
// $Id: photobucket.inc,v 1.1.2.7 2010/07/19 15:57:58 aaron Exp $

/**
 * @file
 * This include processes photobucket.com image files for use by emfield.module.
 */

define('EMIMAGE_PHOTOBUCKET_MAIN_URL', 'http://www.photobucket.com/');
define('EMIMAGE_PHOTOBUCKET_DATA_VERSION', 1);

/**
 * hook emimage_PROVIDER_info
 * this returns information relevant to a specific 3rd party image provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */

function emimage_photobucket_info() {
  $features = array();
  return array(
    'provider' => 'photobucket',
    'name' => t('Photobucket'),
    'url' => EMIMAGE_PHOTOBUCKET_MAIN_URL,
    'settings_description' => t('These settings specifically affect images displayed from <a href="@photobucket" target="_blank">Photobucket</a>.', array('@photobucket' => EMIMAGE_PHOTOBUCKET_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emimage_photobucket_data_version() {
  return EMIMAGE_PHOTOBUCKET_DATA_VERSION;
}

function emimage_photobucket_data($field, $item) {
  $data = array();

  if (preg_match('![si]([^/.:@]+)\.photobucket\.com/albums/([^/]+)/([^/]+)/(\?action=view&current=)?(.+)$!i', $item['embed'], $matches)) {
    $data = array(
      'server' => $matches[1],
      'album' => $matches[2],
      'owner' => $matches[3],
      'file' => $matches[5],
    );
    $data['title'] = emimage_photobucket_image_title($data['file'], $data);
    $data['emimage_data_version'] = EMIMAGE_PHOTOBUCKET_DATA_VERSION;
  }
  return $data;
}

function emimage_photobucket_extract($embed = '') {
  // http://s201.photobucket.com/albums/aa274/layoutqueenie/?action=view&current=baileys_in_gardens.jpg
  // http://i201.photobucket.com/albums/aa274/layoutqueenie/baileys_in_gardens.jpg
  if (preg_match('![si]([^/.:@]+)\.photobucket\.com/albums/([^/]+)/([^/]+)/(\?action=view&current=)?(.+)$!i', $embed, $matches)) {
    return $matches[5];
  }
  return array();
}

/**
 * hook emimage_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content to watch
 *  @return
 *    a string containing the URL to view the image at the original provider's site
 */
function emimage_photobucket_embedded_link($code, $data) {
  return "http://s{$data['server']}.photobucket.com/albums/{$data['album']}/{$data['owner']}/?action=view&current={$code}";
}

/**
 *  implement emimage_PROVIDER_image_url
 *
 *  @param $code
 *    the code of the image
 *  @param $data
 *    any stored data for the image, which may already have the title
 *  @return
 *    the url directly to the image to display
 */
function emimage_photobucket_image_url($code, $data) {
  if (func_num_args() == 7) {
    $arg = func_get_arg(5);
    $code = &$arg['data']['file'];
    $data = &$arg['data'];
  }

  return "http://i{$data['server']}.photobucket.com/albums/{$data['album']}/{$data['owner']}/{$code}";
}

/**
 *  implement emimage_PROVIDER_image_title
 *
 *  @param $code
 *    the code of the image
 *  @param $data
 *    any stored data for the image, which may already have the title
 *  @return
 *    the title as the 3rd party provider knows it, if accessible to us. otherwise, ''
 */
function emimage_photobucket_image_title($code, $data) {
  if ($data['title']) {
    return $data['title'];
  }
  $url = emimage_photobucket_embedded_link($code, $data);
  return _emimage_photobucket_scrape_image_title($url);
}

/**
 * Visit the image URL and scrape the image title from HTML.
 *
 * @param String $url
 *   Image URL.
 * @return String
 *   Image title.
 */
function _emimage_photobucket_scrape_image_title($url) {
  static $title;
  if (isset($title[$url])) {
    return $title[$url];
  }

  $rs = drupal_http_request($url);
  $html = $rs->data;
  return $title[$url] = preg_match('@<span id="photoTitle">(.+?)</span>@is', $html, $matches)? $matches[1] : '';
}

function emimage_photobucket_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return emimage_photobucket_image_url($item['value'], $item['data']);
}

