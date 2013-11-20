<?php
// $Id: lastfm.inc,v 1.1.2.7 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *   Provide support for Last.fm to the emfield.module.
 */

define('EMVIDEO_LASTFM_MAIN_URL', 'http://www.lastfm.com/');

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
function emvideo_lastfm_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'lastfm',
    'name' => t('Last.fm'),
    'url' => EMVIDEO_LASTFM_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@lastfm" target="_blank">Last.fm</a>.', array('@lastfm' => EMVIDEO_LASTFM_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['lastfm'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_lastfm_settings() {
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
function emvideo_lastfm_extract($embed = '') {
  // http://www.last.fm/music/The+Shins/+videos/2794412
  // <object width="340" height="289" id="player" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,0,0" align="middle"> <param name="movie" value="http://cdn.last.fm/videoplayer/33/VideoPlayer.swf" /> <param name="menu" value="false" /> <param name="quality" value="high" /> <param name="bgcolor" value="#000000" /> <param name="allowFullScreen" value="true" /> <param name="flashvars" value="embed=true&creator=The+Shins&title=Phantom+Limb&uniqueName=2794412&albumArt=http://cdn.last.fm/coverart/130x130/3243014.jpg&album=Wincing+the+Night+Away&duration=&image=http://userserve-ak.last.fm/serve/image:320/2794412.jpg&FSSupport=true" /> <embed src="http://cdn.last.fm/videoplayer/33/VideoPlayer.swf" menu="false" quality="high" bgcolor="#000000" width="340" height="289" name="player" align="middle" allowFullScreen="true" flashvars="embed=true&creator=The+Shins&title=Phantom+Limb&uniqueName=2794412&albumArt=http://cdn.last.fm/coverart/130x130/3243014.jpg&album=Wincing+the+Night+Away&duration=&image=http://userserve-ak.last.fm/serve/image:320/2794412.jpg&FSSupport=true" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" /> </object>

  // thumb:
  // http://userserve-ak.last.fm/serve/image:160/2794412.jpg

  // src="http://www.lastfm.com/v/nvbQQnvxXDk"
  // http://lastfm.com/watch?v=nvbQQnvxXDk
  // http://www.lastfm.com/watch?v=YzFCA-xUc8w&feature=dir
  if (preg_match('@cdn.last.fm@i', $embed, $matches)) {
    if (preg_match('@uniqueName=([0-9]+)@i', $embed, $matches)) {
      return $matches[1];
    }
  }
  else if (preg_match('@last\.fm@i', $embed, $matches)) {
    if (preg_match('@([0-9]+)@i', $embed, $matches)) {
      return $matches[1];
    }
  }
  return array(
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
function emvideo_lastfm_embedded_link($video_code) {
  return '';
}

/**
 * the embedded flash displaying the lastfm video
 */
function theme_emvideo_lastfm_flash($embed, $width, $height, $autoplay) {
  static $count;
  if ($embed) {
    // set css id count
    $count++;
    $output .= '<object width="'. $width .'" height="'. $height .'" id="emfield-videocck-player-lastfm-'. $count .'" classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=9,0,0,0" align="middle"> <param name="movie" value="http://cdn.last.fm/videoplayer/33/VideoPlayer.swf" /> <param name="menu" value="false" /> <param name="quality" value="high" /> <param name="bgcolor" value="#000000" /> <param name="allowFullScreen" value="true" /> <param name="flashvars" value="embed=true&amp;creator=&amp;title=&amp;uniqueName='. $embed .'&amp;albumArt=&amp;album=&amp;duration=&amp;image=http://userserve-ak.last.fm/serve/image:320/'. $embed .'.jpg&amp;FSSupport=true" /> <embed src="http://cdn.last.fm/videoplayer/33/VideoPlayer.swf" menu="false" quality="high" bgcolor="#000000" width="'. $width .'" height="'. $height .'" name="player" align="middle" allowFullScreen="true" flashvars="embed=true&amp;creator=&amp;title=&amp;uniqueName='. $embed .'&amp;albumArt=&amp;album=&amp;duration=&amp;image=http://userserve-ak.last.fm/serve/image:320/'. $embed .'.jpg&amp;FSSupport=true" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" /> </object>';
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
function emvideo_lastfm_thumbnail($field, $item, $formatter, $node, $width, $height) {
  $lastfm_id = $item['value'];

  // if we have a large thumbnail size, then get the larger version available.
  if ($width >= 320) {
    $tn = "http://userserve-ak.last.fm/serve/image:320/$lastfm_id.jpg";
  }
  else {
    $tn = "http://userserve-ak.last.fm/serve/image:160/$lastfm_id.jpg";
  }
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
function emvideo_lastfm_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_lastfm_flash', $embed, $width, $height, $autoplay);
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
function emvideo_lastfm_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_lastfm_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_lastfm_emfield_subtheme() {
  return array(
    'emvideo_lastfm_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/lastfm.inc'
    )
  );
}
