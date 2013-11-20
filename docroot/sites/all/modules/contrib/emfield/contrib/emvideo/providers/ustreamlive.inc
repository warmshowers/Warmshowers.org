<?php
// $Id: ustreamlive.inc,v 1.1.2.4 2010/07/19 16:11:47 aaron Exp $

define('EMVIDEO_USTREAMLIVE_MAIN_URL', 'http://www.ustream.tv/');
define('EMVIDEO_USTREAMLIVE_API_INFO', 'http://developer.ustream.tv/');
define('EMVIDEO_USTREAMLIVE_API_APPLICATION_URL', 'http://developer.ustream.tv/apps/register');
define('EMVIDEO_USTREAMLIVE_DATA_VERSION', 1);

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
function emvideo_ustreamlive_info() {
  $name = t('UStream.TV (Live Streams)');
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'ustream',
    'name' => $name,
    'url' => EMVIDEO_USTREAMLIVE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !ustream. You can learn more about its !api here.', array('!ustream' => l($name, EMVIDEO_USTREAMLIVE_MAIN_URL, array('target' => '_blank')), '!api' => l(t('API'), EMVIDEO_USTREAMLIVE_API_INFO, array('target' => '_blank')))),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['ustreamlive'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_ustreamlive_settings() {
  $form['ustreamlive']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('UStream.TV API'),
    '#description' => t('The API is required for thumbnails and other features.You will first need to apply for an API Developer Key from the !ustreamlive.', array('!ustreamlive' => l(t('Ustream Application Registration page'), EMVIDEO_USTREAMLIVE_API_APPLICATION_URL, array('target' => '_blank')))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['ustreamlive']['api']['emvideo_ustreamlive_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('UStream.TV API Key'),
    '#default_value' => variable_get('emvideo_ustreamlive_api_key', ''),
    '#description' => t('Please enter your UStream.TV API Key here.'),
  );
  return $form;
}

/**
 * this is a wrapper for emvideo_request_xml that includes ustreamlive's api key
 */
function emvideo_ustreamlive_request($code = '', $cached = TRUE) {
  $key = trim(variable_get('emvideo_ustreamlive_api_key', ''));
  $url = "http://api.ustreamlive.tv/php/video/$code/getInfo?key=$key";

  return module_invoke('emfield', 'request_xml', 'ustreamlive', $url, array(), $cached, FALSE, $code, TRUE);
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_ustreamlive_data_version() {
  return EMVIDEO_USTREAMLIVE_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_ustreamlive_data($field, $item) {
  $data = emvideo_ustreamlive_request($item['value']);
  $data['ustreamlive_api_version'] = $data['emvideo_data_version'] = EMVIDEO_USTREAMLIVE_DATA_VERSION;;
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
function emvideo_ustreamlive_extract($embed = '') {
  // <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="400" height="320" id="utv654382"><param name="flashvars" value="viewcount=false&amp;autoplay=false&amp;brand=embed"/><param name="allowfullscreen" value="true"/><param name="allowscriptaccess" value="always"/><param name="movie" value="http://www.ustream.tv/flash/live/240437"/><embed flashvars="viewcount=false&amp;autoplay=false&amp;brand=embed" width="400" height="320" allowfullscreen="true" allowscriptaccess="always" id="utv654382" name="utv_n_741085" src="http://www.ustream.tv/flash/live/240437" type="application/x-shockwave-flash" /></object><a href="http://www.ustream.tv/" style="padding:2px 0px 4px;width:400px;background:#FFFFFF;display:block;color:#000000;font-weight:normal;font-size:10px;text-decoration:underline;text-align:center;" target="_blank">Stream videos at Ustream</a>
  return array(
    '@ustream\.tv/flash/live/([^"\&\?]+)@i',
//     '@ustreamlive\.tv/recorded/([^"\&\?]+)@i',
  );
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_code
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_ustreamlive_embedded_link($video_code) {
  return 'http://www.ustreamlive.tv/flash/live/' . $video_code;
}

/**
 * the embedded flash displaying the ustreamlive video
 */
function theme_emvideo_ustreamlive_flash($embed, $width, $height, $autoplay, $options = array()) {
  static $counter;
  if ($embed) {
    $counter++;
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $autoplay;
    $autoplay_value = $autoplay ? 'true' : 'false';
    $id = 'utv654382';

    $output .= <<<USTREAM
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="$width" height="$height" id="$id"><param name="flashvars" value="viewcount=false&amp;autoplay=$autoplay_value&amp;brand=embed"/><param name="allowfullscreen" value="true"/><param name="allowscriptaccess" value="always"/><param name="movie" value="http://www.ustream.tv/flash/live/$embed"/><embed flashvars="viewcount=false&amp;autoplay=$autoplay_value&amp;brand=embed" width="$width" height="$height" allowfullscreen="true" allowscriptaccess="always" id="$id" name="utv_n_741085" src="http://www.ustream.tv/flash/live/$embed" type="application/x-shockwave-flash" /></object>
USTREAM;
//     $output .= "<object type='application/x-shockwave-flash' height='$height' width='$width' data='http://ustreamlive.tv/flash/video/$embed' id='ustreamlive-video-$counter' ><param name='movie' value='http://www.ustreamlive.tv/flash/video/$embed' /><param name='allowScriptAccess' value='always' /><param name='quality' value='best' /><param name='scale' value='noScale' /><param name='wmode' value='transparent' /><param name='FlashVars' value='autoplay=$autoplay_value' /><param name='allowfullscreen' value='true' /></object>";
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
function emvideo_ustreamlive_thumbnail($field, $item, $formatter, $node, $width, $height, $options = array()) {
  $ustreamlive_id = $item['value'];
  // old code to grab thumbnail via api
//   $request = emvideo_ustreamlive_request('ustreamlive.videos.get_details', array('video_id' => $ustreamlive_id));
//   $tn = $request['THUMBNAIL_URL'][0];

  // if we have a large thumbnail size, then get the larger version available.
//   if ($width > 130 || $height > 97) {
//     $tn = "http://img.ustreamlive.tv/vi/$ustreamlive_id/0.jpg";
//   } else {
//     // ustreamlive offers 3 thumbnails. select one randomly.
//     $rand = rand(0, 2) + 1;
//     $tn = "http://img.ustreamlive.tv/vi/$ustreamlive_id/$rand.jpg";
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
function emvideo_ustreamlive_video($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
// print_r($item);
  $output = theme('emvideo_ustreamlive_flash', $embed, $width, $height, $autoplay, $options);
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
function emvideo_ustreamlive_preview($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $output = theme('emvideo_ustreamlive_flash', $embed, $width, $height, $autoplay, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_ustreamlive_emfield_subtheme() {
    return array(
        'emvideo_ustreamlive_flash'  => array(
            'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/ustreamlive.inc'
        )
    );
}
