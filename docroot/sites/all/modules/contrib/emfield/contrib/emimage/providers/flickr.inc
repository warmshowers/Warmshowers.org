<?php
// $Id: flickr.inc,v 1.1.2.11 2010/07/19 15:57:58 aaron Exp $

/**
 * @file
 * This include processes flickr.com image files for use by emfield.module.
 */

define('EMIMAGE_FLICKR_MAIN_URL', 'http://www.flickr.com/');
define('EMIMAGE_FLICKR_API_INFO', 'http://flickr.com/services/api');
define('EMIMAGE_FLICKR_API_APPLICATION_URL', 'http://www.flickr.com/services/api/keys');
define('EMIMAGE_FLICKR_REST_ENDPOINT', 'http://api.flickr.com/services/rest/');

define('EMIMAGE_FLICKR_DATA_VERSION', 1);

/**
 * hook emimage_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */

function emimage_flickr_info() {
  $name = t('Flickr');
  $features = array(
    array(t('Thumbnails'), t('Yes'), ''),
    array(t('Import photosets'), t('Yes'), t('If you have the Embedded Media Import module activated, you may allow @name photosets to be imported into content.', array('@name' => $name))),
  );
  return array(
    'provider' => 'flickr',
    'name' => t('Flickr'),
    'url' => EMIMAGE_FLICKR_MAIN_URL,
    'settings_description' => t('These settings specifically affect images displayed from <a href="@flickr" target="_blank">Flickr</a>.', array('@flickr' => EMIMAGE_FLICKR_MAIN_URL)),
    'supported_features' => $features,
    'import_sets_word' => t('photosets'),
  );
}

/**
 *  This allows flickr photosets to be imported into nodes
 */
function emimage_flickr_import($url, $limit = 0, $page = 0) {
  $codes = array();
  // http://www.flickr.com/photos/nikkiana/sets/72157601948647678/
  if (preg_match('@flickr\.com/photos/([^/]*)/([^/]*)/([^/]*)/@i', $url, $matches)) {
    $page++;  // flickr starts current page at 1
    $codes['#matches'] = $matches;
    $args = array('photoset_id' => $matches[3]);
    if ($limit) {
      $args['per_page'] = $limit;
    }
    $args['page'] = $page;
    $xml = emimage_flickr_request('flickr.photosets.getPhotos', $args);
    $codes['#pages'] = $xml['photoset']['pages'];
    $codes['#page'] = $xml['photoset']['page'] - 1;
    $codes['#total'] = $xml['photoset']['total'];
    $codes['#per_page'] = $xml['photoset']['per_page'];
    $codes['#set'] = array();
    foreach ($xml['photoset']['photo'] as $photo) {
      $data = emimage_flickr_data(NULL, array('value' => $photo['id']));
      $codes['#set'][] = array(
        '#code' => $photo['id'],
        '#title' => $photo['title'],
        '#link' => emimage_flickr_embedded_link($photo['id'], $xml['photoset']['owner']),
        '#thumb' => emimage_flickr_image_url($photo['id'], 100, 100, NULL, NULL, NULL),
        '#body' => $data['description'],
        '#tags' => $data['tags'],
//         '#body' => emimage_flickr_description($photo['id']),
//         '#tags' => emimage_flickr_tags($photo['id']),
      );
    }
/*
    $data['owner'] = $xml['photo']['owner']['nsid'];
    $data['title'] = $xml['photo']['title']['_content'];*/
  }
  return $codes;
}

/**
 * hook emimage_PROVIDER_settings
 * this should return a subform to be added to the emimage_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['flickr'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emimage_flickr_settings() {
  $form['flickr']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr API'),
    '#description' => t('You will first need to apply for an API Developer Key from the <a href="@flickr" target="_blank">Flickr Developer Profile page</a>.', array('@flickr' => EMIMAGE_FLICKR_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('emimage_flickr_api_key', '') != ''),
  );
  $form['flickr']['api']['emimage_flickr_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API Key'),
    '#default_value' => variable_get('emimage_flickr_api_key', ''),
    '#description' => t('Please enter your Flickr Developer Key here.'),
  );
  $form['flickr']['api']['emimage_flickr_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API Secret'),
    '#default_value' => variable_get('emimage_flickr_api_secret', ''),
    '#description' => t('If you have a secret for the Flickr API, enter it here.'),
  );
  return $form;
}

/**
 * this is a wrapper for emimage_request_xml that includes flickr's api key
 */
function emimage_flickr_request($method, $args = array(), $cached = TRUE) {
  // display an error if we don't have an api key yet
  emimage_flickr_error_check();

  $args['api_key'] = trim(variable_get('emimage_flickr_api_key', ''));
  if ($secret = trim(variable_get('emimage_flickr_api_secret', ''))) {
    $args['secret'] = md5($secret . $arghash);
  }
  $args['method'] = $method;
  $args['format'] = 'php_serial';

  $xml = module_invoke('emfield', 'request_xml', 'flickr', EMIMAGE_FLICKR_REST_ENDPOINT, $args, $cached, FALSE, FALSE, TRUE);
  return $xml;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emimage_flickr_data_version() {
  return EMIMAGE_FLICKR_DATA_VERSION;
}

function emimage_flickr_data($field, $item) {
  $data = array();

  // use the page id, since we'll have that in most cases (except in embed pastes, which gets parsed during extraction)
  // we use this to get an rss feed w/ all the info for the video. interesting reading ;)
  $xml = emimage_flickr_request('flickr.photos.getInfo', array('photo_id' => $item['value']));

  $data['owner'] = $xml['photo']['owner']['nsid'];
  $data['title'] = $xml['photo']['title']['_content'];
  $data['description'] = $xml['photo']['description']['_content'];
  $data['tags'] = array();
  if (is_array($xml['photo']['tags']['tag'])) {
    foreach ($xml['photo']['tags']['tag'] as $tag) {
      $data['tags'][] = $tag['raw'];
    }
  }
  // Store the dimensions of the largest image so we can scale it correctly at display-time
  $width = 0 ;
  $height = 0;
  $xml = emimage_flickr_request('flickr.photos.getSizes', array('photo_id' => $item['value']));
  foreach( $xml["sizes"]["size"] as $size){
    // Ignore the Square size, as that doesn't give us the true aspect ratio
    if ($size["label"] != "Square") {
      if ( (int)$size["width"] > $width ) {
        $width = (int)$size["width"];
        $height = (int)$size["height"];
      }
    }
  }
  if ($width > 0) {
    $data['width'] = $width ;
    $data['height'] = $height ;
  }

  $data['emimage_data_version'] = EMIMAGE_FLICKR_DATA_VERSION;
  return $data;
}

/**
 *  This will log an error if we don't have a key yet. In addition, if the user is an admin, we'll display an error.
 */
function emimage_flickr_error_check() {
  static $checked;
  if (!$checked && (variable_get('emimage_flickr_api_key', '') == '')) {
    global $user;
    $error = t('You do not yet have a Flickr API key set. You will need to <a href="@apply" target="_blank">apply for a Flickr API key</a> and enter your key at the <a href="@settings">settings administration page</a> before Flickr images may be displayed.', array('@apply' => EMIMAGE_FLICKR_API_APPLICATION_URL, '@settings' => url('admin/content/emfield')));
    if (user_access('administer site configuration')) {
      drupal_set_message($error, 'error');
    }
    watchdog('Embedded Media Field', '!error', array('!error' => $error));
  }
  $checked = TRUE;
}

function emimage_flickr_extract($embed = '') {
  // http://flickr.com/photos/96898796@N00/194727976/
  return array('@flickr\.com/photos/[^/]*/(\d+)@i');
}

/**
 * hook emimage_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emimage_flickr_embedded_link($code, $data = array()) {
  if ($data['owner']) {
    $owner = $data['owner'];
  }
  else {
    $xml = emimage_flickr_request('flickr.photos.getInfo', array('photo_id' => $code));
    $owner = $xml['photo']['owner']['nsid'];
  }
  return 'http://www.flickr.com/photos/'. $owner .'/'. $code;
}

/**
 *  Implements emimage_PROVIDER_image_url.
 *
 *  @param $code
 *    The provider code of the image.
 *  @param $width
 *  @param $height
 *    The dimensions of the photo to display.
 *  @return
 *    The url directly to the image to display.
 */
function emimage_flickr_image_url($code, $width, $height, $formatter = NULL, $field = NULL, $item = NULL, $node = NULL) {
  if ($code) {
    $size = _emimage_flickr_guess_size($width, $height);
    $getsize = emimage_flickr_request('flickr.photos.getSizes', array('photo_id' => $code));

    // For small images, there may not be the sizes 0..4 that
    // _emimage_flickr_guess_size assumes, so:
    $size = min( $size, (count($getsize['sizes']['size'])-1) );

    $url = $getsize['sizes']['size'][$size]['source'];
  }
  return $url;
}

/**
 *  Implements emimage_PROVIDER_image_title.
 *
 *  @param $code
 *    The provider code of the image.
 *  @param $data
 *    Any stored data for the image, which may already have the title.
 *  @return
 *    The title as the 3rd party provider knows it, if accessible to us. otherwise, ''.
 */
function emimage_flickr_image_title($code, $data) {
  if ($data['title']) {
    return $data['title'];
  }
  $photo = emimage_flickr_request('flickr.photos.getInfo', array('photo_id' => $code));
  return $photo['photo']['title']['_content'] ? $photo['photo']['title']['_content'] : '';
}

/**
 *  Helper function for emimage_flickr_image_url.
 *  This will return the appropriate array key for the image size we wish.
 */
function _emimage_flickr_guess_size($width, $height) {
  $max = max($width, $height);

  if ($max) {
    foreach (array('0' => 75, '1' => 100, '2' => 240, '3' => 500, '4' => 1024) as $size => $value) {
      if ($max <= $value) {
        return $size;
      }
    }
  }

  // If we don't have width or height set, then get the original size.
  return '5';
}

function emimage_flickr_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return emimage_flickr_image_url($item['value'], $width, $height, $formatter, $field, $item, $node);
}
