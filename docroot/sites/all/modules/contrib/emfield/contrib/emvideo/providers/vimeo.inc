<?php
// $Id: vimeo.inc,v 1.1.2.12 2010/07/19 16:48:58 aaron Exp $

/**
 * @file
 *   Provide support for the Vimeo provider to the emfield.module.
 */

define('EMVIDEO_VIMEO_MAIN_URL', 'http://www.vimeo.com/');
define('EMVIDEO_VIMEO_API_INFO', 'http://vimeo.com/api');
define('EMVIDEO_VIMEO_COLOR_DEFAULT', '#01AAEA');

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
function emvideo_vimeo_info() {
  $features = array(
    array(t('Custom player color'), t('Yes'), t("You may customize the player's skin by choosing your own color.")),
    array(t('Thumbnails'), t('Yes'), t('You may select the size of thumbnail to request from Vimeo.')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'vimeo',
    'name' => t('Vimeo'),
    'url' => EMVIDEO_VIMEO_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">Vimeo</a>. You can learn more about its <a href="@api" target="_blank">API</a> here. (You no longer need a key for any current supported features.)', array('@provider' => EMVIDEO_VIMEO_MAIN_URL, '@api' => EMVIDEO_VIMEO_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['vimeo'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_vimeo_settings() {
  $form['vimeo']['color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player color'),
    '#description' => t('If allowed, this color, in hexidecimal form (#RRGGBB), will be used to change the skin of the Vimeo player.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vimeo']['color']['emvideo_vimeo_color_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override player color'),
    '#default_value' => variable_get('emvideo_vimeo_color_override', FALSE),
  );
  $form['vimeo']['color']['emvideo_vimeo_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Color'),
    '#default_value' => variable_get('emvideo_vimeo_color', EMVIDEO_VIMEO_COLOR_DEFAULT),
  );
  $form['vimeo']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['vimeo']['player_options']['emvideo_vimeo_on_screen_info'] = array(
    '#type' => 'checkboxes',
    '#title' => t('On-screen info'),
    '#default_value' => variable_get('emvideo_vimeo_on_screen_info', array('portrait', 'title', 'byline')),
  '#options' => array(
      'portrait' => t("Show video author's portrait"),
      'title' => t('Show video title'),
      'byline' => t('Show byline'),
    ),
    '#description' => t('Provide additional video information on the Vimeo player.'),
  );
  $form['vimeo']['player_options']['emvideo_vimeo_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
  '#default_value' => variable_get('emvideo_vimeo_full_screen', 1),
  '#description' => t('Allow users to view video using the entire computer screen.'),
  );
  $form['vimeo']['emvideo_vimeo_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo API Key'),
    '#default_value' => variable_get('emvideo_vimeo_api_key', ''),
  );
  $form['vimeo']['emvideo_vimeo_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo API Shared Secret'),
    '#default_value' => variable_get('emvideo_vimeo_api_secret', ''),
  );
  $form['vimeo']['emvideo_vimeo_thumb_size'] = array(
    '#type' => 'select',
    '#title' => t('Vimeo Thumbnail Size'),
    '#options' => array('96' => '96', '100' => '100', '160' => '160', '200' => '200', '460' => '460'),
    '#default_value' => variable_get('emvideo_vimeo_thumb_size', '160'),
  );
  return $form;
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
function emvideo_vimeo_extract($embed = '') {
  // http://vimeo.com/123456
  // http://www.vimeo.com/123456
  // http://vimeo.com/channels/hd#6912147
  //<object width="400" height="270"><param name="allowfullscreen" value="true" /><param name="allowscriptaccess" value="always" /><param name="movie" value="http://vimeo.com/moogaloop.swf?clip_id=4106135&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" /><embed src="http://vimeo.com/moogaloop.swf?clip_id=4106135&amp;server=vimeo.com&amp;show_title=1&amp;show_byline=1&amp;show_portrait=0&amp;color=&amp;fullscreen=1" type="application/x-shockwave-flash" allowfullscreen="true" allowscriptaccess="always" width="400" height="270"></embed></object><br /><a href="http://vimeo.com/4106135">Two Knives One Motive</a> from <a href="http://vimeo.com/x351">Tyler Capehart</a> on <a href="http://vimeo.com">Vimeo</a>.
  return array(
    '@vimeo\.com/moogaloop\.swf\?clip_id\=([^\"\&]+)@i',
    '@vimeo\.com/[^\"\&\d]*([^\"\&]+)@i',
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
function emvideo_vimeo_embedded_link($video_code) {
  return 'http://www.vimeo.com/'. $video_code;
}

function emvideo_vimeo_convert_color($color = NULL) {
  if ($color{0} == '#') {
    return substr($color, 1);
  }
  return $color;
}

/**
 * the embedded flash displaying the Vimeo video
 */
function theme_emvideo_vimeo_flash($embed, $width, $height, $autoplay) {
  $output = '';
  if ($embed) {
    $fullscreen = variable_get('emvideo_vimeo_full_screen', 1);
    $on_screen_info = variable_get('emvideo_vimeo_on_screen_info', array('portrait', 'title', 'byline'));
    $show_portrait = $on_screen_info['portrait'] ? 1 : 0;
    $show_title = $on_screen_info['title'] ? 1 : 0;
    $show_byline = $on_screen_info['byline'] ? 1 : 0;
    if (variable_get('emvideo_vimeo_color_override', FALSE)) {
      $color = emvideo_vimeo_convert_color(variable_get('emvideo_vimeo_color', EMVIDEO_VIMEO_COLOR_DEFAULT));
    }
    $output = '<object type="application/x-shockwave-flash" width="'. $width .'" height="'. $height .'" data="http://www.vimeo.com/moogaloop.swf?clip_id='. $embed .'&amp;server=www.vimeo.com&amp;fullscreen='. $fullscreen .'&amp;show_title='. $show_title .'&amp;show_byline='. $show_byline .'&amp;show_portrait='. $show_portrait .'&amp;color='. $color .'&autoplay='. $autoplay .'">';
    $output .= '<param name="quality" value="best" />';
    $output .= '<param name="wmode" value="transparent" />';
    $output .= '<param name="allowfullscreen" value="'. ($fullscreen ? 'true' : 'false') .'" />';
    $output .= '<param name="scale" value="showAll" />';
    $output .= '<param name="movie" value="http://www.vimeo.com/moogaloop.swf?clip_id='. $embed .'&amp;server=www.vimeo.com&amp;fullscreen='. $fullscreen .'&amp;show_title='. $show_title .'&amp;show_byline='. $show_byline .'&amp;show_portrait='. $show_portrait .'&amp;color='. $color .'&autoplay='. $autoplay .'" /></object>';
  }
  return $output;
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
function emvideo_vimeo_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_vimeo_flash', $embed, $width, $height, $autoplay);
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
function emvideo_vimeo_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_vimeo_flash', $embed, $width, $height, $autoplay);
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
function emvideo_vimeo_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $xml = emfield_request_xml('vimeo', 'http://vimeo.com/api/oembed.xml?url=http%3A//vimeo.com/'. $item['value'], array(), TRUE, FALSE, $item['value']);
  return $xml['OEMBED']['THUMBNAIL_URL'][0];
}

function emvideo_vimeo_emfield_subtheme() {
  return array(
    'emvideo_vimeo_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/vimeo.inc',
    ),
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_vimeo_content_generate() {
  return array(
    'http://vimeo.com/1408463',
    'http://vimeo.com/1040532',
    'http://vimeo.com/709081',
    'http://vimeo.com/5565562',
  );
}

