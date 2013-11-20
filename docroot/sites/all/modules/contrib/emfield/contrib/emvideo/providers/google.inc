<?php
// $Id: google.inc,v 1.1.2.14 2009/10/29 13:29:51 aaron Exp $

/**
 * @file
 *   This include processes Google.com media files for use by emfield.module
 */

define('EMVIDEO_GOOGLE_MAIN_URL', 'http://video.google.com/');
define('EMVIDEO_GOOGLE_XML', 'http://video.google.com/videofeed');
define('EMVIDEO_GOOGLE_DOMAIN_DEFAULT', 'com');
define('EMVIDEO_GOOGLE_DATA_VERSION', 1);

function emvideo_google_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), ''),
    array(t('Duration'), t('Yes'), ''),
  );
  return array(
    'provider' => 'google',
    'name' => t('Google'),
    'url' => EMVIDEO_GOOGLE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@google" target="_blank">Google</a>.', array('@google' => EMVIDEO_GOOGLE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emvideo_google_settings() {
  $form = array();
  $form['emvideo_google_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Google video domain'),
    '#default_value' => variable_get('emvideo_google_domain', EMVIDEO_GOOGLE_DOMAIN_DEFAULT),
    '#description' => t('Google hosts their videos on various servers throughout the world. By default, videos from Google will be displayed from the United States, at their google.<b><em>com</em></b> servers. If you wish to display the video from another Google server, please enter the domain here, without the initial dot. For instance, you might enter <b><em>it</em></b> for the Italian Google servers at google.it, or <b><em>ca</em></b> for their Canadian servers.'),
  );
  return $form;
}

function emvideo_google_extract($embed) {
  return array(
    '@http://video\.google(?:\.[a-z]{2,4})+/videoplay(?:\?|%3F)docid(?:=|%3D)([^\&]*)\&@i',
    '@http://video\.google(?:\.[a-z]{2,4})+/videoplay(?:\?|%3F)docid(?:=|%3D)([^\&]*)\&@i',
    '@http://video\.google(?:\.[a-z]{2,4})+/videoplay\?docid=(.*)@i',
    '@http://video\.google(?:\.[a-z]{2,4})+/googleplayer\.swf\?docId=(-?\d*)@i',
    '@http://video\.google(?:\.[a-z]{2,4})+/url\?docid=([^\&]*)\&@i',
  );
}

function emvideo_google_request($embed, $cacheable = TRUE) {
  $args = array('docid' => $embed);
  return module_invoke('emfield', 'request_xml', 'google', EMVIDEO_GOOGLE_XML, $args, $cacheable);
}

function emvideo_google_embedded_link($video_code) {
  return 'http://video.google.'. variable_get('emvideo_google_domain', EMVIDEO_GOOGLE_DOMAIN_DEFAULT) .'/videoplay?docid='. $video_code;
}

function theme_emvideo_google_flash($embed, $width, $height, $autoplay) {
  if ($embed) {
    $autoplay = $autoplay ? '&amp;autoPlay=true' : '';
    // this will be executed by not Internet Explorer browsers
    $output = '<!--[if !IE]> <-->
  <object type="application/x-shockwave-flash" width="'. $width .'" height="'. $height .'"
  data="http://video.google.'. variable_get('emvideo_google_domain', EMVIDEO_GOOGLE_DOMAIN_DEFAULT) .'/googleplayer.swf?docId='. check_plain($embed) . $autoplay .'">
  <!--> <![endif]-->'."\n";

    // this will be executed by Internet Explorer
    $output .= '<!--[if IE]>
  <object type="application/x-shockwave-flash" width="'. $width .'" height="'. $height .'"
  classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
  codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,40,0">
  <![endif]-->'."\n";

    // params will be passed to both IE or not IE browsers
    $output .= '<param name="movie" value="http://video.google.'. variable_get('emvideo_google_domain', EMVIDEO_GOOGLE_DOMAIN_DEFAULT) .'/googleplayer.swf?docId='. check_plain($embed) . $autoplay .'" />'."\n";
    // following a list of params simply copied from old embed tag params. I don't know if this are really needed.
    $output .= '<param name="quality" value="best" />
    <param name="bgcolor" value="#ffffff" />
    <param name="allowScriptAccess" value="sameDomain" />
    <param name="scale" value="noScale" />
    <param name="wmode" value="transparent" />
    <param name="salign" value="TL" />
    <param name="FlashVars" value="playerMode=embedded" />
    <p>'. t('Your browser is not able to display this multimedia content.') .'</p>
  </object>';
  }
  return $output;
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_google_data_version() {
  return EMVIDEO_GOOGLE_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_google_data($field, $item) {
  $data = array();

  // Create some 'field' version control.
  $data['emvideo_data_version'] = EMVIDEO_GOOGLE_DATA_VERSION;

  $rss = emvideo_google_request($item['value']);

  if (
    is_array($rss['ITEM']) &&
    is_array($rss['ITEM']['MEDIA:GROUP']) &&
    is_array($rss['ITEM']['MEDIA:GROUP']['MEDIA:CONTENT']) &&
    is_array($rss['ITEM']['MEDIA:GROUP']['MEDIA:CONTENT'][1])
  ) {
    $video = $rss['ITEM']['MEDIA:GROUP']['MEDIA:CONTENT'][1];
    $data['filepath'] = $video['URL'];
    $data['filemime'] = $video['TYPE'];
    $data['medium'] = $video['MEDIUM'];
    $data['expression'] = $video['EXPRESSION'];
    $data['duration'] = $video['DURATION'];
    $data['width'] = $video['WIDTH'];
    $data['height'] = $video['HEIGHT'];
  }

  if (is_array($rss['MEDIA:GROUP'])) {
    if (is_array($rss['MEDIA:GROUP']['MEDIA:THUMBNAIL']) && is_array($rss['MEDIA:GROUP']['MEDIA:THUMBNAIL'][1])) {
      $thumbnail = $rss['MEDIA:GROUP']['MEDIA:THUMBNAIL'][1];
      $data['thumbnail']['filepath'] = $thumbnail['URL'];
      $data['thumbnail']['width'] = $thumbnail['WIDTH'];
      $data['thumbnail']['height'] = $thumbnail['HEIGHT'];
    }
    if (is_array($rss['MEDIA:GROUP']['MEDIA:PLAYER']) && is_array($rss['MEDIA:GROUP']['MEDIA:PLAYER'][1])) {
      $data['player']['filepath'] = $rss['MEDIA:GROUP']['MEDIA:PLAYER'][1]['URL'];
    }
  }

  if ($data['thumbnail']['filepath'] == '' && is_array($rss['ITEM'])) {
    // for whatever reason the thumbnail failed try the old method
    // we'll parse it from the description, where it's repeated.
    $desc = $rss['ITEM']['DESCRIPTION'][0];
    $regex = '@<img src="([^"]*)"@';
    if (preg_match($regex, $desc, $matches)) {
      $data['thumbnail']['filepath'] = $matches[1];
    }
  }

  // google rss doesn't actually have <embeded> media, just MRSS, and no size
  // so not actually really really necessary or really useful but for completeness
  $play = 'http://video.google.'. variable_get('emvideo_google_domain', EMVIDEO_GOOGLE_DOMAIN_DEFAULT) .'/googleplayer.swf?docId='. check_plain($item['embed']);
  $response = emfield_request_header('google', $play, FALSE, FALSE);

  if ($response->code == 200) {
    $data['filesize'] = $response->headers['Content-Length'];
  }

  return $data;
}

/**
 * hook emvideo_PROVIDER_duration($item)
 * Returns the duration of the video in seconds.
 *  @param $item
 *    The video item itself, which needs the $data array.
 *  @return
 *    The duration of the video in seconds.
 */
function emvideo_google_duration($item) {
  if (!isset($item['data']['emvideo_data_version'])) {
    $item['data'] = emvideo_google_data(NULL, $item);
  }
  return isset($item['data']['duration']) ? $item['data']['duration'] : 0;
}

function emvideo_google_rss($item, $teaser) {
  if ($item['value']) {
    if (isset($item['data']['emvideo_data_version'])) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_google_data(NULL, $item);
    }

    return $data;
  }
}

function emvideo_google_thumbnail($field, $item, $formatter, $node, $width, $height) {
  if (isset($item['data']['emvideo_data_version'])) {
    $data = $item['data'];
  }
  else {
    $data = emvideo_google_data($field, $item);
  }

  return $data['thumbnail']['filepath'];

  return NULL;
}

function emvideo_google_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_google_flash', $embed, $width, $height, $autoplay);
  return $output;
}

function emvideo_google_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_google_flash', $embed, $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_google_emfield_subtheme() {
  return array(
    'emvideo_google_flash' => array(
      'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
      'file' => 'providers/google.inc'
    )
  );
}

/**
 *  Implement hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_google_content_generate() {
  return array(
    'http://video.google.com/videoplay?docid=-3401332494586334627&ei=EzXBSr7ZDILkrALR8tTJBg&q=drupal&hl=en&emb=1',
    'http://video.google.com/videoplay?docid=3803572165737229287&ei=kDXBSobXK4jUqQK6zcHqBg&q=drupal&hl=en&emb=1',
    'http://video.google.com/videoplay?docid=3883642707114931305&ei=kDXBSobXK4jUqQK6zcHqBg&q=drupal&hl=en&emb=1',
    'http://video.google.com/videoplay?docid=-6482735792871677056&ei=5DXBSvOEBIyQqALuyvXXBg&q=drupal&hl=en&emb=1',
  );
}