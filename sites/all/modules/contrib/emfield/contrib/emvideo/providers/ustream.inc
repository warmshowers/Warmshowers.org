<?php
// $Id: ustream.inc,v 1.1.2.4 2010/07/19 16:11:47 aaron Exp $

define('EMVIDEO_USTREAM_MAIN_URL', 'http://www.ustream.tv/');
define('EMVIDEO_USTREAM_API_INFO', 'http://developer.ustream.tv/');
define('EMVIDEO_USTREAM_API_APPLICATION_URL', 'http://developer.ustream.tv/apps/register');
define('EMVIDEO_USTREAM_DATA_VERSION', 1);

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emvideo_ustream_info() {
  $name = t('UStream.TV');
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'ustream',
    'name' => $name,
    'url' => EMVIDEO_USTREAM_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !ustream. You can learn more about its !api here.', array('!ustream' => l($name, EMVIDEO_USTREAM_MAIN_URL, array('target' => '_blank')), '!api' => l(t('API'), EMVIDEO_USTREAM_API_INFO, array('target' => '_blank')))),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['ustream'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_ustream_settings() {
  $form['ustream']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('UStream.TV API'),
    '#description' => t('The API is required for thumbnails and other features.You will first need to apply for an API Developer Key from the !ustream.', array('!ustream' => l(t('Ustream Application Registration page'), EMVIDEO_USTREAM_API_APPLICATION_URL, array('target' => '_blank')))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['ustream']['api']['emvideo_ustream_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('UStream.TV API Key'),
    '#default_value' => variable_get('emvideo_ustream_api_key', ''),
    '#description' => t('Please enter your UStream.TV API Key here.'),
  );
  return $form;
}

/**
 * this is a wrapper for emvideo_request_xml that includes ustream's api key
 */
function emvideo_ustream_request($code = '', $cached = TRUE) {
  $key = trim(variable_get('emvideo_ustream_api_key', ''));
  $url = "http://api.ustream.tv/php/video/$code/getInfo?key=$key";

  return module_invoke('emfield', 'request_xml', 'ustream', $url, array(), $cached, FALSE, $code, TRUE);
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_ustream_data_version() {
  return EMVIDEO_USTREAM_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_ustream_data($field, $item) {
  $data = emvideo_ustream_request($item['value']);
  $data['ustream_api_version'] = $data['emvideo_data_version'] = EMVIDEO_USTREAM_DATA_VERSION;
  return $data;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_ustream_extract($embed = '') {
  // http://www.ustream.tv/recorded/570233
  return array(
    '@ustream\.tv/recorded/([^"\&\?]+)@i',
  );
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_ustream_duration($item) {
  if (!isset($item['data']['ustream_api_version'])) {
    $item['data'] = emvideo_ustream_data(NULL, $item);
  }
  return isset($item['data']['results']['lengthInSecond']) ? intval($item['data']['results']['lengthInSecond']) : 0;
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_ustream_embedded_link($video_code) {
  return 'http://www.ustream.tv/recorded/' . $video_code;
}

/**
 * the embedded flash displaying the ustream video
 */
function theme_emvideo_ustream_flash($embed, $width, $height, $autoplay, $options = array()) {
  static $counter;
  if ($embed) {
    $counter++;
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
    $autoplay_value = $autoplay ? 'true' : 'false';

    $output .= "<object type='application/x-shockwave-flash' height='$height' width='$width' data='http://ustream.tv/flash/video/$embed' id='ustream-video-$counter' ><param name='movie' value='http://www.ustream.tv/flash/video/$embed' /><param name='allowScriptAccess' value='always' /><param name='quality' value='best' /><param name='scale' value='noScale' /><param name='wmode' value='transparent' /><param name='FlashVars' value='autoplay=$autoplay_value' /><param name='allowfullscreen' value='true' /></object>";
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_ustream_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  $ustream_id = $item['value'];
  // old code to grab thumbnail via api
//   $request = emvideo_ustream_request('ustream.videos.get_details', array('video_id' => $ustream_id));
//   $tn = $request['THUMBNAIL_URL'][0];

  // if we have a large thumbnail size, then get the larger version available.
//   if ($width > 130 || $height > 97) {
//     $tn = "http://img.ustream.tv/vi/$ustream_id/0.jpg";
//   } else {
//     // ustream offers 3 thumbnails. select one randomly.
//     $rand = rand(0, 2) + 1;
//     $tn = "http://img.ustream.tv/vi/$ustream_id/$rand.jpg";
//   }
  return $tn;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_ustream_video($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
// print_r($item);
  $output = theme('emvideo_ustream_flash', $embed, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_ustream_preview($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $output = theme('emvideo_ustream_flash', $embed, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_ustream_emfield_subtheme() {
    return array(
        'emvideo_ustream_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/ustream.inc'
        )
    );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_ustream_content_generate() {
  return array(
    'http://www.ustream.tv/recorded/1448530',
    'http://www.ustream.tv/recorded/1448057',
    'http://www.ustream.tv/recorded/1726952',
  );
}
