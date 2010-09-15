<?php
// $Id: myspace.inc,v 1.1.2.13 2010/07/19 16:37:46 aaron Exp $

/**
 * @file
 *   This include processes myspace.com media files for use by emfield.module.
 */

define('EMVIDEO_MYSPACE_MAIN_URL', 'http://vids.myspace.com/');
define('EMVIDEO_MYSPACE_API_URL', 'http://developer.myspace.com/community/');
define('EMVIDEO_MYSPACE_DATA_VERSION', 1);

function emvideo_myspace_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), ''),
    array(t('Duration'), t('Yes'), ''),
  );
  return array(
    'provider' => 'myspace',
    'name' => t('MySpace'),
    'url' => EMVIDEO_MYSPACE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@provider" target="_blank">MySpace</a>.', array('@provider' => EMVIDEO_MYSPACE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_myspace_settings() {
  $form = array();
  return $form;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_myspace_data_version() {
  return EMVIDEO_MYSPACE_DATA_VERSION;
}

/**
 *  hook emvideo_PROVIDER_data
 *
 *  Provides an array to be serialised and made available with $item elsewhere.
 *
 *  This data can be used to store any extraneous information available
 *  specifically to the 8tracks provider.
 */
function emvideo_myspace_data($field, $item) {
  // Initialize the data array.
  $data = array();
  // We are using oEmbed to retrieve a standard set of data from the provider.
  // You should change the URL as specified by the 8tracks provider.
  // If the 8tracks provider does not support oEmbed, you must remove this
  // section entirely, or rewrite it to use their API.
  // See http://oembed.com/ for for information.
  $data = emfield_request_xml('Myspace', 'http://mediaservices.myspace.com/services/rss.ashx?type=video&videoID=' . $item['value'], array(), TRUE, FALSE, $item['value']);

  // Create some version control. Thus if we make changes to the data array
  // down the road, we can respect older content. If allowed by Embedded Media
  // Field, any older content will automatically update this array as needed.
  // In any case, you should account for the version if you increment it.
  $data['emvideo_data_version'] = $data['emvideo_myspace_version'] = EMVIDEO_MYSPACE_DATA_VERSION;

  $data['emvideo_myspace_thumbnail'] = $data['ITEM']['MEDIA:THUMBNAIL'][1]['URL'];
  // This stores a URL to the video's thumbnail.

  return $data;
}

/**
 *  Implements emvideo_PROVIDER_extract().
 */
function emvideo_myspace_extract($url) {

 return array(
    '@src="myspace\.com/index.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspace\.com/index\.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspace\.com/index\.cfm\?fuseaction=vids\.individual&videoid=([^&"]+)@i',
    '@src="myspacetv\.com/index.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)"@i',
    '@myspacetv\.com/index\.cfm\?fuseaction=vids\.individual&amp;videoid=([^&"]+)@i',
    '@myspacetv\.com/index\.cfm\?fuseaction=vids\.individual&videoid=([^&"]+)@i',
  );
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the audio code.
 *  @return
 *    A string containing the URL to view the video at the original provider's site.
 */
function emvideo_myspace_embedded_link($video_code) {
  return 'http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid='. $video_code;
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_myspace_duration($item) {
  if (!isset($item['data']['emvideo_myspace_version'])) {
    $item['data'] = emvideo_myspace_data(NULL, $item);
  }
  return isset($item['data']['ITEM']['MEDIA:CONTENT'][1]['DURATION']) ? $item['data']['ITEM']['MEDIA:CONTENT'][1]['DURATION'] : 0;
}

function theme_emvideo_myspace_flash($embed, $width, $height) {
  static $count = 0;
  if ($embed) {
    $embed = check_plain($embed);
    $width = (integer) $width;
    $height = (integer) $height;
    $output .= '<object height="'. $height .'px" width="'. $width .'px" >
      <param name="allowFullScreen" value="true"/>
      <param name="wmode" value="transparent"/>
      <param name="movie" value="http://mediaservices.myspace.com/services/media/embed.aspx/m='. $embed .',t=1,mt=video" />
      <embed src="http://mediaservices.myspace.com/services/media/embed.aspx/m='. $embed .',t=1,mt=video" width="'. $width .'" height="'. $height .'" allowFullScreen="true" type="application/x-shockwave-flash" wmode="transparent"></embed></object>';
  }
  $count++;
  return $output;
}

function emvideo_myspace_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $item['data']['emvideo_myspace_thumbnail'] ;
}

function emvideo_myspace_video($embed, $width, $height, $field, $item, $node) {
  $output = theme('emvideo_myspace_flash', $embed, $width, $height);
  return $output;
}

function emvideo_myspace_preview($embed, $width, $height, $field, $item, $node) {
  $output = theme('emvideo_myspace_flash', $embed, $width, $height);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_myspace_emfield_subtheme() {
  return array(
    'emvideo_myspace_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/myspace.inc'
    )
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_myspace_content_generate() {
  return array(
    'http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid=62472581&searchid=ea1f646c-39df-4963-b93d-f916b019f1a1',
    'http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid=62472739&searchid=ea1f646c-39df-4963-b93d-f916b019f1a1',
    'http://vids.myspace.com/index.cfm?fuseaction=vids.individual&videoid=62473325&searchid=ea1f646c-39df-4963-b93d-f916b019f1a1',
  );
}
